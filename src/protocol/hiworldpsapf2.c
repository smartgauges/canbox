#include "protocol/hiworldpsapf2.h"
#include "canbox.h" // For access to snd_canbox_hiworld_msg, main loop state etc.
#include "car.h"    // For car_get_* functions
#include "conf.h"   // For potentially checking car type if needed later
#include "utils.h"  // For scale() and snd_canbox_hiworld_msg()
#include "hw_usart.h" // For hw_usart_write (used in cmd_process debug output)

#include <string.h> // For memcpy
#include <stdio.h>  // For snprintf in cmd_process debug output

// --- Internal Helper Functions ---

// Helper to map temperature to Hiworld protocol value
// Based on Table in Section 4.2 ComID 0x31
static uint8_t map_temperature_to_hiworld(uint8_t car_temp_val) {
    // car_temp_val is assumed to be Celsius * 1 or similar direct value
    // Hiworld uses 0x01=14.0, 0x02=14.5 ... 0x20=30.0, 0x00=LO, 0xFE=HI
    // TODO: This mapping needs careful verification based on actual car_get_air_*_temp output
    if (car_temp_val < 14) return 0x00; // LO
    if (car_temp_val > 30) return 0xFE; // HI

    // Simple linear mapping for now (may need adjustment for .5 degree steps)
    // Map 14-30 C to 1-32 (0x01 - 0x20)
    // Each step in Hiworld is 0.5 C
    uint8_t hiworld_val = (uint8_t)((car_temp_val - 14.0) * 2.0 + 1.0);

    // Crude handling for .5 degree (needs better logic if car_temp_val includes .5)
     if (car_temp_val * 10 % 10 >= 5) { // Check if there's a .5 component
         hiworld_val = (uint8_t)(((car_temp_val - 14.0) * 2.0) + 1.5); // Adjust for .5 step
     } else {
         hiworld_val = (uint8_t)((car_temp_val - 14.0) * 2.0 + 1.0);
     }

     if (hiworld_val == 0) hiworld_val = 1; // Ensure LO maps correctly if temp is exactly 14.0
     if (hiworld_val > 0x20) hiworld_val = 0x20; // Clamp at 30.0 C

    return hiworld_val;
}


// --- Protocol Implementation Functions ---

// ComID 0x11: Basic Vehicle Information (Doors, Status, Wheel, Temp)
static void canbox_hiworld_psa_basic_info_process(void) {
    uint8_t data[6] = {0};

    // ... (Door decoding remains the same) ...
     if (car_get_door_fl()) data[0] |= 0x80;
    if (car_get_door_fr()) data[0] |= 0x40;
    if (car_get_door_rl()) data[0] |= 0x20;
    if (car_get_door_rr()) data[0] |= 0x10;
    if (car_get_tailgate()) data[0] |= 0x08;
    if (car_get_bonnet()) data[0] |= 0x04;


    // Data 1: Status
    if (car_get_acc()) data[1] |= 0x80; // Bit 7: ACC Status

    // Use the reliable 'lights enabled' flag from the 0x036 state
    if (car_get_illum()) data[1] |= 0x40; // Bit 6: Illumination Status

    if (get_rear_delay_state()) data[1] |= 0x20; // Bit 5: Reverse Status
    if (car_get_park_break()) data[1] |= 0x10; // Bit 4: Parking Brake Status
    if (car_get_ds_belt()) data[1] |= 0x08;    // Bit 3: Seat Belt Status
    struct radar_t radar;
    car_get_radar(&radar);
    if (radar.state != e_radar_off && radar.state != e_radar_undef) data[1] |= 0x04; // Bit 2: Original Radar Status
    // Bits 1-0 reserved

    // ... (Steering Wheel Angle, Temp decoding remains the same) ...
    int8_t wheel_angle = 0;
    car_get_wheel(&wheel_angle);
    data[2] = (uint8_t)wheel_angle; // Still needs scaling verification for Hiworld

    int16_t temp = car_get_temp();
    if (temp < -40 || temp > 87) {
        data[3] = 0xFF;
    } else {
        data[3] = (uint8_t)(temp + 40);
    }
    data[4] = 0x00;
    data[5] = 0x00;


    snd_canbox_hiworld_msg(0x11, data, sizeof(data));
}

// ComID 0x12: Detailed Vehicle Information (VIN, Speed)
// NOTE: This is combined with basic info in the main process call for efficiency
static void canbox_hiworld_psa_detailed_info_process(void) {
    uint8_t data[8] = {0}; // LEN=0x09 -> 8 DATA bytes

    // Data 0-5: Last 6 digits of VIN (Indices 11-16 from carstate.vin)
    uint8_t vin_buf[18];
    uint8_t vin_len = car_get_vin(vin_buf);
    if (vin_len >= 17) { // Ensure we have enough VIN digits
        memcpy(data, vin_buf + 11, 6); // Copy VIN[11] to VIN[16]
    }
     else if (vin_len >= 9) { // Fallback: copy last available 6 digits?
         memcpy(data, vin_buf + vin_len - 6, 6); // Need verification
     }
     else {
        // Handle cases where VIN is too short or unavailable ("na")
        memset(data, 0x20, 6); // Fill with spaces or '?' or 0x00
     }


    // Data 6-7: Vehicle Speed (km/h * 100)
    uint16_t speed = car_get_speed();
    uint16_t speed_scaled = speed * 100;
    data[6] = speed_scaled & 0xFF;        // Low Byte
    data[7] = (speed_scaled >> 8) & 0xFF; // High Byte

    snd_canbox_hiworld_msg(0x12, data, sizeof(data));
}


// Helper to map RAW PSA temp byte (0x00=LO, 0x01=14, ..., 0x1E=30, 0x1F=HI, 0xFF=Inv)
// to Hiworld Temp Byte (0x00=LO, 0x01=14,... 0x20=30, 0xFE=HI, 0xFF=Inv/Sync)
#define ID_0x1D0_TEMP_RAW_INVALID   0xFF
#define ID_0x1D0_TEMP_RAW_LO        0x00
#define ID_0x1D0_TEMP_RAW_HI        0x1F // Value corresponding to "HI"
#define STATE_UNDEF 0xff

static uint8_t map_psa_raw_temp_to_hiworld(uint8_t psa_raw_temp) {
    if (psa_raw_temp == STATE_UNDEF || psa_raw_temp == ID_0x1D0_TEMP_RAW_INVALID) return 0xFF; // Invalid/Sync
    if (psa_raw_temp == ID_0x1D0_TEMP_RAW_LO) return 0x00; // LO
    if (psa_raw_temp == ID_0x1D0_TEMP_RAW_HI) return 0xFE; // HI
    // Map 0x01 (14.0) to 0x1E (30.0) -> Hiworld 0x01 to 0x20 (32 steps)
    // PSA Raw range 1..30 maps to Hiworld 1..32
     if (psa_raw_temp >= 1 && psa_raw_temp <= 30) {
         // PSA Raw 0x01 (14.0C) -> Hiworld 0x01
         // PSA Raw 0x02 (14.5C) -> Hiworld 0x02
         // ...
         // PSA Raw 0x1E (30.0C) -> Hiworld 0x20 (32 decimal)
         return psa_raw_temp; // Direct mapping seems correct for the numeric range 1-30 (0x01-0x1E) -> 1-32 (0x01-0x20)
                              // Need to verify if PSA raw 0x1F maps to Hiworld 0xFE
     }
     // Fallback for unexpected PSA raw values
     return 0xFF;
}


static void canbox_hiworld_psa_ac_process(void) {
    uint8_t data[7] = {0}; // LEN=0x08 -> 7 DATA bytes

    // --- Data 0 ---
    uint8_t ac_on_state = car_get_air_ac(); // Use the value from car_air_state
    if (ac_on_state) data[0] |= 0x80; // Bit 7: AC System ON/OFF (Based on AC compressor)
    if (car_get_air_ac_max()) data[0] |= 0x40; // Bit 6: AC MAX Mode
    if (car_get_air_recycling()) data[0] |= 0x20; // Bit 5: Recirculation
    if (car_get_air_dual()) data[0] |= 0x10; // Bit 4: Dual Zone Mode
    // Bit 3: AUTO Mode - Assuming ON if AC is ON and Fan > 0
    if (ac_on_state && car_get_air_fanspeed() > 0) data[0] |= 0x08;
    if (car_get_air_rear()) data[0] |= 0x04; // Bit 2: Rear Defrost
    // Bit 1 Reserved
    if (ac_on_state) data[0] |= 0x01; // Bit 0: AC Compressor Status

    // --- Data 1 ---
    if (car_get_air_aqs()) data[1] |= 0x80; // Bit 7: Auto Recirculation (AQS)
    // Bit 6 Reserved
    uint8_t fanspeed = car_get_air_fanspeed(); // Get 0-7 value
    if(fanspeed > 7) fanspeed = 7; // Clamp
    data[1] |= (fanspeed & 0x07) << 3; // Bits 5-3: Fan Speed
    if (car_get_air_floor()) data[1] |= 0x04; // Bit 2: Airflow Floor
    if (car_get_air_middle()) data[1] |= 0x02; // Bit 1: Airflow Face/Mid
    if (car_get_air_wind()) data[1] |= 0x01; // Bit 0: Airflow Windshield

    // --- Data 2 & 3: Temperatures ---
    data[2] = map_psa_raw_temp_to_hiworld(car_get_air_l_temp()); // Map stored PSA raw value
    data[3] = map_psa_raw_temp_to_hiworld(car_get_air_r_temp()); // Map stored PSA raw value

    // Data 4-6: Reserved
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;

    snd_canbox_hiworld_msg(0x31, data, sizeof(data));
}

// Helper to map radar distance (0-7 from car.c) to Hiworld (1-255)
// Using the inverted scale logic: 1=closest, 255=farthest/inactive
static uint8_t map_radar_distance_hiworld(uint8_t car_distance_0_7) {
    // Ensure input is within expected range
    if (car_distance_0_7 > 7) {
        car_distance_0_7 = 7;
    }

    if (car_distance_0_7 == 7) {
        return 0xFF; // 7 maps to farthest/inactive
    } else {
        // Map 0 (closest) -> 1
        // Map 6 (far) -> 250 (approx, using linear scaling)
        // Inverted scale: Higher value means further away.
        // Scale 0-6 linearly onto 1-250
        // Using float for intermediate calculation for better scaling
        float scaled = scale((float)car_distance_0_7, 0.0f, 6.0f, 1.0f, 250.0f);
        // Invert the scale for Hiworld (1=closest, 250=far)
        return (uint8_t)(251.0f - scaled);
        // Example:
        // Input 0 -> scale=1.0 -> result = 250
        // Input 3 -> scale=125.5 -> result = 126
        // Input 6 -> scale=250.0 -> result = 1
    }
     // Fallback/error case - should not happen if input is 0-7
    // return 0xFF;
}


// ComID 0x41: Radar (Parking Sensor) Information
static void canbox_hiworld_psa_radar_process(uint8_t fmax[4], uint8_t rmax[4]) {
    (void)fmax; // Not used by Hiworld PSA radar format
    (void)rmax;

    struct radar_t radar;
    car_get_radar(&radar); // Get the latest radar state

    // Don't send if the radar state is undefined (e.g., after startup before first message)
    if (radar.state == e_radar_undef) {
        return;
    }

    uint8_t data[12]; // LEN=0x0D -> 12 DATA bytes.

    // Map distances (0=closest, 7=farthest -> 1=closest, 255=farthest)
    data[0] = map_radar_distance_hiworld(radar.rl);
    data[1] = map_radar_distance_hiworld(radar.rlm);
    data[2] = map_radar_distance_hiworld(radar.rrm);
    data[3] = map_radar_distance_hiworld(radar.rr);
    data[4] = map_radar_distance_hiworld(radar.fr);
    data[5] = map_radar_distance_hiworld(radar.frm);
    data[6] = map_radar_distance_hiworld(radar.flm);
    data[7] = map_radar_distance_hiworld(radar.fl);

    // Bytes 8-10 are reserved
    data[8] = 0x00;
    data[9] = 0x00;
    data[10] = 0x00;

    // Byte 11: Parking System Status
    switch (radar.state) {
        case e_radar_off:
            data[11] = 0x00; // OFF
            // Ensure all distances show inactive when system is off
            memset(data, 0xFF, 8); // Set first 8 bytes (distances) to 0xFF
            break;
        case e_radar_on_rear:
            data[11] = 0x10; // Rear ON
             // Optionally clear front sensors if only rear is active? Test HU behavior.
             // data[4]=0xFF; data[5]=0xFF; data[6]=0xFF; data[7]=0xFF;
            break;
        case e_radar_on_front:
            data[11] = 0x20; // Front ON
             // Optionally clear rear sensors if only front is active? Test HU behavior.
             // data[0]=0xFF; data[1]=0xFF; data[2]=0xFF; data[3]=0xFF;
            break;
        case e_radar_on: // Both implicitly
            data[11] = 0x30; // Front & Rear ON
            break;
        // case e_radar_undef: // Handled above, don't send anything
        default: // Should not happen
             data[11] = 0x00;
             memset(data, 0xFF, 8);
             break;
    }

    // Send the message regardless of ON/OFF state to ensure display clears correctly
    snd_canbox_hiworld_msg(0x41, data, sizeof(data));
}

// --- Button Handler Wrappers (Translate car events to Hiworld ComID 0x21) ---

// Helper to send key press/release for ComID 0x21
static void send_hiworld_key(uint8_t key_code) {
    uint8_t data_press[] = {key_code, 0x01, 0x00};   // Data 0=Key Code, Data 1=Status (Pressed)
    uint8_t data_release[] = {key_code, 0x00, 0x00}; // Data 0=Key Code, Data 1=Status (Released)

    snd_canbox_hiworld_msg(0x21, data_press, sizeof(data_press));
    // Optional short delay between press and release if needed by HU
    // mdelay(20); // Requires a mdelay function
    snd_canbox_hiworld_msg(0x21, data_release, sizeof(data_release));
}

static void canbox_hiworld_psa_inc_volume(uint8_t val) {
    (void)val; // val usually not needed for single press
    send_hiworld_key(0x01); // Volume +
}

static void canbox_hiworld_psa_dec_volume(uint8_t val) {
    (void)val;
    send_hiworld_key(0x02); // Volume -
}

static void canbox_hiworld_psa_prev(void) {
    send_hiworld_key(0x04); // Prev Track / Seek Down
}

static void canbox_hiworld_psa_next(void) {
    send_hiworld_key(0x05); // Next Track / Seek Up
}

static void canbox_hiworld_psa_mode(void) {
    // Documentation maps multiple buttons to 'mode'. Using SRC for now.
    send_hiworld_key(0x06); // SRC (Source)
}

static void canbox_hiworld_psa_cont(void) {
    // Documentation maps TEL to 'cont'.
    send_hiworld_key(0x07); // TEL (Phone)
}

static void canbox_hiworld_psa_mici(void) {
    // Documentation maps VR to 'mici'.
    send_hiworld_key(0x08); // VR (Voice Recognition)
}

// --- Command Processing (from Head Unit) ---

// RX State machine structure (similar to Raise)
enum hiworld_rx_state {
    RX_HIWORLD_WAIT_START1,
    RX_HIWORLD_WAIT_START2,
    RX_HIWORLD_LEN,
    RX_HIWORLD_CMD,
    RX_HIWORLD_DATA,
    RX_HIWORLD_CRC
};

#define RX_HIWORLD_BUFFER_SIZE 32 // Adjust if needed

static void canbox_hiworld_psa_cmd_process(uint8_t ch) {
    static enum hiworld_rx_state rx_state = RX_HIWORLD_WAIT_START1;
    static uint8_t rx_buffer[RX_HIWORLD_BUFFER_SIZE];
    static uint8_t rx_len = 0;
    static uint8_t rx_idx = 0;
    static uint8_t rx_cmd = 0;

    switch (rx_state) {
        case RX_HIWORLD_WAIT_START1:
            if (ch == 0x5A) {
                rx_state = RX_HIWORLD_WAIT_START2;
            }
            break;

        case RX_HIWORLD_WAIT_START2:
            if (ch == 0xA5) {
                rx_state = RX_HIWORLD_LEN;
            } else {
                rx_state = RX_HIWORLD_WAIT_START1; // Invalid sequence
            }
            break;

        case RX_HIWORLD_LEN:
            if (ch < 1 || ch > (RX_HIWORLD_BUFFER_SIZE - 3)) { // Min LEN=1 (ComID only), max check
                 rx_state = RX_HIWORLD_WAIT_START1; // Invalid length
                 break;
            }
            rx_len = ch;
            rx_buffer[0] = ch; // Store LEN for checksum calculation
            rx_idx = 1;
            rx_state = RX_HIWORLD_CMD;
            break;

        case RX_HIWORLD_CMD:
            rx_cmd = ch;
            rx_buffer[rx_idx++] = ch; // Store ComID
            if (rx_len == 1) { // No data bytes
                rx_state = RX_HIWORLD_CRC;
            } else {
                rx_state = RX_HIWORLD_DATA;
            }
            break;

        case RX_HIWORLD_DATA:
            rx_buffer[rx_idx++] = ch;
            if (rx_idx >= (rx_len + 1)) { // +1 because buffer stores LEN and ComID too
                 rx_state = RX_HIWORLD_CRC;
            }
            break;

        case RX_HIWORLD_CRC:
            {
                uint8_t calculated_checksum = canbox_hiworld_checksum(rx_buffer, rx_len + 1); // Checksum includes LEN, ComID, DATA
                uint8_t received_checksum = ch;
                uint8_t ack_buf[5];
                ack_buf[0] = 0x5A;
                ack_buf[1] = 0xA5;
                ack_buf[2] = 0x01; // LEN for ACK/NACK is 1
                ack_buf[3] = (calculated_checksum == received_checksum) ? 0xFF : 0xFE; // ACK or NACK
                ack_buf[4] = rx_cmd; // ComID being ACK/NACKed
                ack_buf[5] = canbox_hiworld_checksum(ack_buf + 2, 3); // Checksum for ACK/NACK frame

                hw_usart_write(hw_usart_get(), ack_buf, sizeof(ack_buf));

                if (calculated_checksum == received_checksum && ack_buf[3] == 0xFF) {
                    // --- Process Valid Command ---
                    // Example: Handle Vehicle Type Setting 0x24
                    if (rx_cmd == 0x24 && rx_len >= 3) { // Check LEN >= 1(ComID) + 2(Data)
                        uint8_t vehicle_id = rx_buffer[2]; // Data 0
                        uint8_t option_id = rx_buffer[3]; // Data 1
                        // TODO: Potentially use these IDs to adjust behavior if needed
                        // For now, just print debug info
                        char dbg_buf[64];
                        snprintf(dbg_buf, sizeof(dbg_buf), "\r\nHiworld: Received Vehicle Type %02X, Option %02X\r\n", vehicle_id, option_id);
                        hw_usart_write(hw_usart_get(), (uint8_t*)dbg_buf, strlen(dbg_buf));
                    }
                    // TODO: Add handlers for other relevant commands from Head Unit (0x1B, 0x3B, 0x7B, 0x7D, etc.)
                    else {
                        // Optional: Print unhandled commands for debugging
                         char dbg_buf[64];
                         snprintf(dbg_buf, sizeof(dbg_buf), "\r\nHiworld: Received Unhandled CMD %02X\r\n", rx_cmd);
                         hw_usart_write(hw_usart_get(), (uint8_t*)dbg_buf, strlen(dbg_buf));
                    }
                }
            }
            rx_state = RX_HIWORLD_WAIT_START1; // Reset state machine
            break;

        default:
            rx_state = RX_HIWORLD_WAIT_START1; // Should not happen
            break;
    }
}

// Park Process - Reuse Radar Process
static void canbox_hiworld_psa_park_process(uint8_t fmax[4], uint8_t rmax[4]) {
    canbox_hiworld_psa_radar_process(fmax, rmax);
}

// --- Protocol Operations Structure ---

const protocol_ops_t hiworld_psa_protocol_ops = {
    .radar_process = canbox_hiworld_psa_radar_process,
    .wheel_process = NULL, // Wheel angle sent in basic info message 0x11
    .door_process = canbox_hiworld_psa_basic_info_process, // Basic info sends doors, status, wheel, temp
    .vehicle_info_process = canbox_hiworld_psa_detailed_info_process, // Detailed info sends VIN, Speed
    .ac_process = canbox_hiworld_psa_ac_process,
    .inc_volume = canbox_hiworld_psa_inc_volume,
    .dec_volume = canbox_hiworld_psa_dec_volume,
    .prev = canbox_hiworld_psa_prev,
    .next = canbox_hiworld_psa_next,
    .mode = canbox_hiworld_psa_mode,
    .cont = canbox_hiworld_psa_cont,
    .mici = canbox_hiworld_psa_mici,
    .cmd_process = canbox_hiworld_psa_cmd_process,
    .park_process = canbox_hiworld_psa_park_process, // Use radar process for parking display
};