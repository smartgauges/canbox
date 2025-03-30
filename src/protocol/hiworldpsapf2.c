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

// Helper to map radar distance (0-99 or 0-7 from car.c) to Hiworld (1-255)
// Using the inverted scale logic: 1=closest, 255=farthest/inactive
static uint8_t map_radar_distance_hiworld(uint8_t car_distance) {
    // Assuming car_distance is 0-7 (based on peugeot_407.c handler)
    // If it's 0-99, the scaling needs adjustment.
    // 0 in car.c means closest, 7 means farthest/no object.
    if (car_distance == 7) return 0xFF; // Map "farthest/no object" to 0xFF
    // Map 0 (closest) to 1, and 6 (far) to a value less than 0xFF.
    // Linear mapping: map 0-6 range to 1-250 range (inverted)
    return 251 - (uint8_t)scale((float)car_distance, 0.0f, 6.0f, 1.0f, 250.0f);
}


// --- Protocol Implementation Functions ---

// ComID 0x11: Basic Vehicle Information (Doors, Status, Wheel, Temp)
static void canbox_hiworld_psa_basic_info_process(void) {
    uint8_t data[6] = {0}; // LEN=0x07 -> 6 DATA bytes

    // ... (Keep Door decoding for Data 0 as before) ...
    if (car_get_door_fl()) data[0] |= 0x80;
    if (car_get_door_fr()) data[0] |= 0x40;
    if (car_get_door_rl()) data[0] |= 0x20;
    if (car_get_door_rr()) data[0] |= 0x10;
    if (car_get_tailgate()) data[0] |= 0x08;
    if (car_get_bonnet()) data[0] |= 0x04;


    // Data 1: Status
    if (car_get_acc()) data[1] |= 0x80; // Use updated carstate.acc

    // Illumination Status Bit (Data 1, Bit 6)
    // Turn ON Hiworld bit if dashboard lights are considered enabled
    // Using the simple check: brightness level > 0
    uint8_t illum_brightness = car_get_illum(); // Get brightness level 0-15
    if (illum_brightness > 0) { // Turn on ILL bit if brightness > 0 (adjust threshold if needed)
         data[1] |= 0x40;
    }
    // More robust: Check the specific enable bit if you decode and store it separately

    if (get_rear_delay_state()) data[1] |= 0x20;
    if (car_get_park_break()) data[1] |= 0x10;
    if (car_get_ds_belt()) data[1] |= 0x08;
    struct radar_t radar;
    car_get_radar(&radar);
    if (radar.state != e_radar_off && radar.state != e_radar_undef) data[1] |= 0x04;

    // ... (Keep Steering Wheel Angle and Temp decoding for Data 2, 3 as before) ...
    int8_t wheel_angle = 0;
    car_get_wheel(&wheel_angle);
    data[2] = (uint8_t)wheel_angle;

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


// ComID 0x31: Air Conditioning Information
static void canbox_hiworld_psa_ac_process(void) {
    uint8_t data[7] = {0}; // LEN=0x08 -> 7 DATA bytes

    uint8_t ac_on = car_get_air_ac(); // Assuming 1 means compressor on / system active

    // Data 0
    if (ac_on) data[0] |= 0x80; // AC System ON/OFF (Using AC Compressor status)
    if (car_get_air_ac_max()) data[0] |= 0x40; // AC MAX Mode
    if (car_get_air_recycling()) data[0] |= 0x20; // Recirculation (Manual/Auto based on Data 1 Bit 7)
    if (car_get_air_dual()) data[0] |= 0x10; // Dual Zone Mode
    // Bit 3 AUTO Mode - How to determine this? Maybe if fanspeed > 0 and AC is on? Needs logic.
    // Let's assume if fan>0 and AC on, AUTO is conceptually active for the HU display
    if (ac_on && car_get_air_fanspeed() > 0) data[0] |= 0x08; // AUTO mode (Assumption)
    if (car_get_air_rear()) data[0] |= 0x04; // Rear Defrost
    // Bit 1 Reserved
    if (ac_on) data[0] |= 0x01; // AC Compressor Status

    // Data 1
    if (car_get_air_aqs()) data[1] |= 0x80; // Auto Recirculation (AQS)
    // Bit 6 Reserved
    uint8_t fanspeed = car_get_air_fanspeed(); // Assuming 0-7 range
    if (fanspeed > 7) fanspeed = 7;
    data[1] |= (fanspeed & 0x07) << 3; // Fan Speed (Bits 5-3)
    if (car_get_air_floor()) data[1] |= 0x04; // Airflow: Floor
    if (car_get_air_middle()) data[1] |= 0x02; // Airflow: Face/Mid
    if (car_get_air_wind()) data[1] |= 0x01; // Airflow: Windshield

    // Data 2: Left Temperature
    data[2] = map_temperature_to_hiworld(car_get_air_l_temp());

    // Data 3: Right Temperature
    data[3] = map_temperature_to_hiworld(car_get_air_r_temp());

    // Data 4-6: Reserved
    data[4] = 0x00;
    data[5] = 0x00;
    data[6] = 0x00;

    snd_canbox_hiworld_msg(0x31, data, sizeof(data));
}

// ComID 0x41: Radar (Parking Sensor) Information
static void canbox_hiworld_psa_radar_process(uint8_t fmax[4], uint8_t rmax[4]) {
    // fmax/rmax are not used in this protocol's radar message structure
    (void)fmax;
    (void)rmax;

    struct radar_t radar;
    car_get_radar(&radar);

    uint8_t data[12] = {0xFF}; // LEN=0x0D -> 12 DATA bytes. Default to 0xFF (inactive/far)

    // Map distances (0=closest, 7=farthest/off -> 1=closest, 255=farthest/off)
    data[0] = map_radar_distance_hiworld(radar.rl);
    data[1] = map_radar_distance_hiworld(radar.rlm);
    data[2] = map_radar_distance_hiworld(radar.rrm);
    data[3] = map_radar_distance_hiworld(radar.rr);
    data[4] = map_radar_distance_hiworld(radar.fr); // Order is FR, FRM, FLM, FL in doc
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
        case e_radar_undef:
            data[11] = 0x00; // OFF
            break;
        case e_radar_on_rear:
            data[11] = 0x10; // Rear ON
            break;
        case e_radar_on_front:
            data[11] = 0x20; // Front ON
            break;
        case e_radar_on: // Both implicitly
            data[11] = 0x30; // Front & Rear ON
            break;
        default:
             data[11] = 0x00; // OFF
             break;

    }

    // Only send if the system is not OFF (to avoid spamming 0xFFs unnecessarily?)
    // Or maybe always send to explicitly turn off the display? Let's send always for now.
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