// src/protocol/raiserzc.c

#include "protocol/raiserzc.h"
#include "canbox.h"     // Access to common canbox functions if needed (e.g. debug print)
#include "car.h"        // Access to car_get_* functions
#include "conf.h"       // Access to conf_get_* for potential config checks
#include "utils.h"      // Access to scale() ? (Or implement locally if needed)
#include "hw_usart.h"   // Needed for hw_usart_write

#include <string.h>     // For memcpy, memset
#include <stdio.h>      // For snprintf (debugging command process)

// --- RZC Protocol Constants ---
#define RZC_HEADER 0xFD
#define RZC_MAX_DATA_LEN 30 // Define a reasonable max data payload size
#define RZC_MAX_FRAME_LEN (1 + 1 + 1 + RZC_MAX_DATA_LEN + 1) // HD+LEN+TYPE+DATA+CS

// --- RZC DataType Definitions (Slave -> Host) ---
#define RZC_DTYPE_SLAVE_CONTROL     0x01 // Backlight etc. (Low priority)
#define RZC_DTYPE_BUTTON_CMD        0x02 // Buttons (Steering Wheel, Panel)
#define RZC_DTYPE_AC_INFO           0x21 // AC Info (14款408 specific?)
#define RZC_DTYPE_WHEEL_ANGLE       0x29 // Steering Wheel Angle
#define RZC_DTYPE_RADAR_ALL         0x30 // All-around Radar
#define RZC_DTYPE_RADAR_REVERSE     0x32 // Reverse Radar only
#define RZC_DTYPE_TRIP_PAGE0        0x33 // Trip Computer Inst + Range
#define RZC_DTYPE_TRIP_PAGE1        0x34 // Trip Computer Trip 1
#define RZC_DTYPE_TRIP_PAGE2        0x35 // Trip Computer Trip 2
#define RZC_DTYPE_OUTSIDE_TEMP      0x36 // Outside Temperature
#define RZC_DTYPE_ALERT_RECORDS     0x37 // Stored Alerts (Low priority)
#define RZC_DTYPE_VEHICLE_STATUS    0x38 // Doors, Lights, Settings Status
#define RZC_DTYPE_FUNCTION_STATUS   0x39 // Model-specific status (Low priority)
#define RZC_DTYPE_DIAGNOSTIC_INFO   0x3A // Diagnostics (Low priority)
#define RZC_DTYPE_TRIP_CLEAR_STATUS 0x3D // Trip Clear Status (Low priority)
#define RZC_DTYPE_MEM_SPEED         0x3B // Memorized Speed (Low priority)
#define RZC_DTYPE_CRUISE_LIMIT      0x3D // Cruise/Limit Settings (Low priority)
#define RZC_DTYPE_CRUISE_POPUP      0x3F // Cruise/Limit Popup (Low priority)
#define RZC_DTYPE_VERSION_INFO      0x7F // CANbox Version

// --- RZC DataType Definitions (Host -> Slave) ---
#define RZC_DTYPE_VEHICLE_SET       0x80 // Vehicle Parameter Setting
#define RZC_DTYPE_TRIP_SET          0x82 // Trip Computer Setting
#define RZC_DTYPE_REQ_ALERTS        0x85 // Request Alert Records
#define RZC_DTYPE_REQ_FUNC_STATUS   0x86 // Request Function Status
#define RZC_DTYPE_REQ_DIAG          0x87 // Request Diagnostic Info
#define RZC_DTYPE_SET_MEM_SPEED     0x88 // Set Memorized Speed
#define RZC_DTYPE_SET_CRUISE_LIMIT  0x89 // Set Cruise/Limit Speed
#define RZC_DTYPE_AC_SET            0x8A // AC Setting Command
#define RZC_DTYPE_REQ_DISPLAY       0x8F // Request Display Info
#define RZC_DTYPE_SET_CRUISE_INSTR  0x99 // Cruise/Limit Instrument Setting
#define RZC_DTYPE_TIME_SET          0xA6 // Time Setting Command

// --- Internal Helper Functions ---

/**
 * @brief Calculates the RZC protocol checksum.
 * @param data_type The DataType byte.
 * @param length The Length byte (DataType + Data size).
 * @param data Pointer to the Data payload buffer.
 * @param data_size Size of the Data payload.
 * @return The calculated 8-bit checksum.
 */
static uint8_t raise_rcz_checksum(uint8_t data_type, uint8_t length, const uint8_t *data, uint8_t data_size) {
    uint8_t sum = 0;
    sum += length;
    sum += data_type;
    for (uint8_t i = 0; i < data_size; i++) {
        sum += data[i];
    }
    return sum;
}

/**
 * @brief Sends a correctly formatted RZC protocol message over UART.
 * @param data_type The DataType byte.
 * @param msg Pointer to the Data payload buffer.
 * @param size Size of the Data payload (n).
 */
static void snd_raise_rcz_msg(uint8_t data_type, const uint8_t *msg, uint8_t size) {
    if (size > RZC_MAX_DATA_LEN) {
        // Handle error: data payload too large
        return;
    }

    uint8_t frame[RZC_MAX_FRAME_LEN];
    uint8_t length = 1 + size; // Length = DataType byte + Data bytes

    frame[0] = RZC_HEADER;       // Header
    frame[1] = length;           // Length
    frame[2] = data_type;        // DataType

    if (msg && size > 0) {
        memcpy(&frame[3], msg, size); // Copy Data payload
    }

    // Calculate Checksum (covers Length, DataType, and Data)
    frame[3 + size] = raise_rcz_checksum(data_type, length, msg, size);

    // Send the complete frame
    hw_usart_write(hw_usart_get(), frame, 3 + size + 1); // HD+LEN+TYPE+DATA+CS
}

// Helper to map temperature (°C) to RZC format (Bit 7=sign, Bits 6-0=value)
static uint8_t map_temp_to_rzc(int16_t temp_c) {
    uint8_t sign_bit = 0;
    uint8_t value = 0;

    if (temp_c < 0) {
        sign_bit = 0x80;
        value = (uint8_t)(-temp_c);
    } else {
        value = (uint8_t)temp_c;
    }

    // Clamp value to 7 bits (0-127)
    if (value > 127) {
        value = 127;
    }

    return sign_bit | (value & 0x7F);
}

// Helper to map radar distance (0-7) to RZC radar distance (0-5)
static uint8_t map_radar_dist_to_rzc(uint8_t car_dist_0_7) {
    // RZC uses 0=Closest(5 bars), 1=4 bars, ..., 4=1 bar, 5=Inactive
    // CAR uses 0=Closest, ..., 6=Far, 7=Inactive
    if (car_dist_0_7 >= 7) return 0x05; // Inactive/Farthest
    if (car_dist_0_7 == 0) return 0x00; // Closest
    if (car_dist_0_7 == 1) return 0x01;
    if (car_dist_0_7 == 2) return 0x01; // Map 1&2 to 4 bars
    if (car_dist_0_7 == 3) return 0x02; // 3 bars
    if (car_dist_0_7 == 4) return 0x03; // 2 bars
    if (car_dist_0_7 == 5) return 0x04; // 1 bar
    if (car_dist_0_7 == 6) return 0x04; // Map 5&6 to 1 bar

    return 0x05; // Default to inactive
}

// --- Protocol Implementation Functions ---

// DataType 0x38: Vehicle Status (Doors, Lights, Settings subset)
// This seems like the most comprehensive single status message.
static void raise_rcz_vehicle_status_process(void) {
    uint8_t data[6] = {0}; // Spec shows up to Data5 used

    // Data0 (Doors)
    if (car_get_door_fl()) data[0] |= 0x80;
    if (car_get_door_fr()) data[0] |= 0x40;
    if (car_get_door_rl()) data[0] |= 0x20;
    if (car_get_door_rr()) data[0] |= 0x10;
    if (car_get_tailgate()) data[0] |= 0x08;
    // Bonnet bit not explicitly mentioned for 0x38 Data0 in spec, unlike Hiworld 0x11

    // Data1 (Settings/Status 1) - Map what we have
    // Bit 7: Rear Wiper -> Ignore for now
    // Bit 6: Auto Park Brake -> Ignore
    // Bit 4: Auto Door Lock -> Ignore
    // Bit 3: Park Assist System -> Use radar state? 1=Enabled
    struct radar_t radar; car_get_radar(&radar);
    if (radar.state != e_radar_off && radar.state != e_radar_undef) data[1] |= 0x08;
    // Bit 2: Central Lock -> Ignore
    // Bits 1-0: Current Trip Page -> Ignore (handled by separate messages)

    // Data2 (Settings/Status 2)
    // Bit 7: Daytime Lights -> Ignore
    // Bits 6-5: Headlight Delay -> Ignore
    // Bit 0: Adaptive Headlights -> Ignore

    // Data3 (Settings/Status 3)
    // Bits 7-5: Ambiance Light -> Ignore (Use main illum)
    // Bit 3: Park Sensor Disable -> Ignore (Assume enabled if sending radar)
    if (get_rear_delay_state()) data[3] |= 0x04; // Bit 2: Reverse Status
    if (car_get_park_break()) data[3] |= 0x02;   // Bit 1: P Gear / Handbrake Status (1=ON)
    if (car_get_park_lights()) data[3] |= 0x01;  // Bit 0: Parking Light Status (1=ON)

    // Data4 (Settings/Status 4)
    // Bits 7-6: Follow Me Home -> Ignore
    // Bits 5-4: Welcome Light -> Ignore
    // Bits 2-1: Audio EQ/Ambiance -> Ignore (Handled by HU)
    // Bit 0: Fuel Unit Setting -> Ignore (Assumed by HU)

    // Data5 (Settings/Status 5)
    // Bit 7: Blind Spot -> Ignore
    // Bit 6: Start/Stop -> Ignore
    // Bit 5: Welcome Function -> Ignore
    // Bit 4: Driver Door Unlock Only -> Ignore
    // Bits 3-0: Language -> Ignore

    snd_raise_rcz_msg(RZC_DTYPE_VEHICLE_STATUS, data, sizeof(data));
}

// DataType 0x29: Steering Wheel Angle
static void raise_rcz_wheel_process(uint8_t type, int16_t min, int16_t max) {
    (void)type; // RZC uses fixed DataType 0x29

    int8_t wheel_percent = 0;
    if (!car_get_wheel(&wheel_percent)) return; // Only send if valid data exists

    // Scale -100..+100 percent to RZC's -5450..+5450 range
    // Note: RZC spec says <0 is RIGHT, >0 is LEFT. Our carstate is <0 LEFT, >0 RIGHT. Need to invert.
    int16_t rcz_angle = (int16_t)scale((float)(-wheel_percent), -100.0f, 100.0f, (float)min, (float)max);

    uint8_t data[2];
    data[0] = rcz_angle & 0xFF;        // Low Byte
    data[1] = (rcz_angle >> 8) & 0xFF; // High Byte

    snd_raise_rcz_msg(RZC_DTYPE_WHEEL_ANGLE, data, sizeof(data));
}


// DataType 0x32: Reverse Radar Info (Using this as primary for simplicity)
static void raise_rcz_radar_process(uint8_t fmax[4], uint8_t rmax[4]) {
    (void)fmax; // Not used in this protocol format
    (void)rmax;

    struct radar_t radar;
    car_get_radar(&radar);

    uint8_t data[7] = {0}; // LEN = 0x0A -> 7 DATA bytes for 0x32

    // Data0: Radar Status
    if (radar.state == e_radar_off || radar.state == e_radar_undef) {
        data[0] = 0x03; // Disabled
        // Set all distances to inactive when disabled
        memset(&data[1], 0x05, 6); // Fill Distances (Data1-6) with '5' (Inactive)
    } else {
        data[0] = 0x02; // Enabled and Display Info
        // Map distances (0-7 -> 0-5)
        data[1] = map_radar_dist_to_rzc(radar.rl);  // Rear Left
        data[2] = map_radar_dist_to_rzc(radar.rlm); // Rear Middle (Use RLM for single middle)
        data[3] = map_radar_dist_to_rzc(radar.rr);  // Rear Right
        data[4] = map_radar_dist_to_rzc(radar.fl);  // Front Left
        data[5] = map_radar_dist_to_rzc(radar.flm); // Front Middle (Use FLM for single middle)
        data[6] = map_radar_dist_to_rzc(radar.fr);  // Front Right
    }

    snd_raise_rcz_msg(RZC_DTYPE_RADAR_REVERSE, data, sizeof(data));
    // Note: Sending 0x30 (All-around) might be needed if HU expects it, structure is different.
}


// DataType 0x36: Outside Temperature
static void raise_rcz_temperature_process() {
    int16_t temp_c = car_get_temp();
    uint8_t data[1];
    data[0] = map_temp_to_rzc(temp_c);
    snd_raise_rcz_msg(RZC_DTYPE_OUTSIDE_TEMP, data, sizeof(data));
}

// DataType 0x33: Trip Page 0
static void raise_rcz_trip0_process() {
    uint8_t data[9] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; // Default invalid

    if (!carstate.inst_consumption_invalid) {
        // Scale and format instantaneous consumption (L/100km * 10?) -> VERIFY TARGET UNIT/SCALING
        uint16_t cons_scaled = carstate.inst_consumption_raw / 10; // Example scaling
        data[0] = (cons_scaled >> 8) & 0xFF;
        data[1] = cons_scaled & 0xFF;
    }
    if (!carstate.range_invalid) {
        // Format range (KM)
        uint16_t range_km = carstate.range_km;
        data[2] = (range_km >> 8) & 0xFF;
        data[3] = range_km & 0xFF;
    }
    // Data 4-5 (Set Destination) - Usually not sent by CANbox
    // Data 6-8 (Start/Stop Time) - Likely not applicable

    snd_raise_rcz_msg(RZC_DTYPE_TRIP_PAGE0, data, sizeof(data));
}
// Placeholder functions for Trip 1 & 2 (Implement similarly when CAN IDs/Data are known)
static void raise_rcz_trip1_process() {
     uint8_t data[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
     // TODO: Populate with carstate.avg_consumption1_raw, carstate.avg_speed1, carstate.trip_distance1
     // after verifying scaling and formatting for RZC protocol (Bytes 0-1 = AvgCons, 2-3=AvgSpeed, 4-5=Dist)
     snd_raise_rcz_msg(RZC_DTYPE_TRIP_PAGE1, data, sizeof(data));
}
static void raise_rcz_trip2_process() {
     uint8_t data[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
     // TODO: Populate with carstate.avg_consumption2_raw, carstate.avg_speed2, carstate.trip_distance2
     snd_raise_rcz_msg(RZC_DTYPE_TRIP_PAGE2, data, sizeof(data));
}

// Combine multiple sends into logical groups for canbox_process/park_process
static void raise_rcz_send_main_status() {
    raise_rcz_vehicle_status_process(); // Sends 0x38
    raise_rcz_temperature_process();    // Sends 0x36
}

static void raise_rcz_send_trip_info() {
    raise_rcz_trip0_process(); // Sends 0x33
    raise_rcz_trip1_process(); // Sends 0x34 (with placeholder data for now)
    raise_rcz_trip2_process(); // Sends 0x35 (with placeholder data for now)
}


// --- Button Handling ---

// Helper to send RZC 0x02 Button Command
static void send_raise_rcz_key(uint8_t rzc_key_code, uint8_t status) {
    uint8_t data[3] = {0};
    data[0] = rzc_key_code;
    data[1] = status; // 0x01 = Press, 0x00 = Release
    data[2] = 0x00;   // Reserved
    snd_raise_rcz_msg(RZC_DTYPE_BUTTON_CMD, data, sizeof(data));
}

// Mappings from generic callbacks to RZC key codes (verify these from the spec table!)
static void raise_rcz_inc_volume(uint8_t val) { (void)val; send_raise_rcz_key(0x14, 1); send_raise_rcz_key(0x14, 0); }
static void raise_rcz_dec_volume(uint8_t val) { (void)val; send_raise_rcz_key(0x15, 1); send_raise_rcz_key(0x15, 0); }
static void raise_rcz_prev(void) { send_raise_rcz_key(0x13, 1); send_raise_rcz_key(0x13, 0); } // Seek-
static void raise_rcz_next(void) { send_raise_rcz_key(0x12, 1); send_raise_rcz_key(0x12, 0); } // Seek+
static void raise_rcz_mode(void) { send_raise_rcz_key(0x11, 1); send_raise_rcz_key(0x11, 0); } // Source/Phone
static void raise_rcz_cont(void) { send_raise_rcz_key(0x30, 1); send_raise_rcz_key(0x30, 0); } // Tel On (Answer) - Map 'cont' here?
static void raise_rcz_mici(void) { send_raise_rcz_key(0x29, 1); send_raise_rcz_key(0x29, 0); } // Push To Talk (Voice)

// --- Command Processing (Host -> Slave) ---

// RX State machine structure for RZC protocol
enum rzc_rx_state {
    RX_RZC_WAIT_START,
    RX_RZC_LEN,
    RX_RZC_DTYPE,
    RX_RZC_DATA,
    RX_RZC_CS
};

#define RX_RZC_BUFFER_SIZE 32 // Max expected frame size

static void raise_rcz_cmd_process(uint8_t ch) {
    static enum rzc_rx_state rx_state = RX_RZC_WAIT_START;
    static uint8_t rx_buffer[RX_RZC_BUFFER_SIZE]; // Buffer to hold LEN, DTYPE, DATA
    static uint8_t rx_len = 0; // Expected LEN value from frame
    static uint8_t rx_idx = 0; // Current index into rx_buffer
    static uint8_t rx_dtype = 0;

    switch (rx_state) {
        case RX_RZC_WAIT_START:
            if (ch == RZC_HEADER) {
                rx_idx = 0; // Reset buffer index
                rx_state = RX_RZC_LEN;
            }
            break;

        case RX_RZC_LEN:
            if (ch >= 1 && ch < (RX_RZC_BUFFER_SIZE - 2)) { // Min LEN=1 (DTYPE only), check buffer bounds
                rx_len = ch;
                rx_buffer[rx_idx++] = ch; // Store LEN
                rx_state = RX_RZC_DTYPE;
            } else {
                rx_state = RX_RZC_WAIT_START; // Invalid length
            }
            break;

        case RX_RZC_DTYPE:
            rx_dtype = ch;
            rx_buffer[rx_idx++] = ch; // Store DTYPE
            if (rx_len == 1) { // Only DataType, no Data payload
                rx_state = RX_RZC_CS;
            } else {
                rx_state = RX_RZC_DATA;
            }
            break;

        case RX_RZC_DATA:
            rx_buffer[rx_idx++] = ch; // Store Data byte
            // Check if we have received all expected data bytes (LEN - 1)
            if (rx_idx >= (rx_len + 1)) { // +1 because buffer also holds LEN and DTYPE
                rx_state = RX_RZC_CS;
            }
            break;

        case RX_RZC_CS:
            {
                uint8_t received_checksum = ch;
                // Checksum includes LEN, DTYPE, and DATA (DATA size = rx_len - 1)
                uint8_t calculated_checksum = raise_rcz_checksum(rx_dtype, rx_len, &rx_buffer[2], rx_len - 1);

                if (calculated_checksum == received_checksum) {
                    // Checksum OK - Process command
                     #ifdef DEBUG_MSG
                     char dbg_buf[64];
                     snprintf(dbg_buf, sizeof(dbg_buf), "RZC RX OK: DType=0x%02X Len=%d\r\n", rx_dtype, rx_len);
                     hw_usart_write(hw_usart_get(), (uint8_t*)dbg_buf, strlen(dbg_buf));
                     #endif
                    // TODO: Add actual command handling based on rx_dtype
                    // e.g., if (rx_dtype == RZC_DTYPE_TIME_SET) { handle_time_set(&rx_buffer[2], rx_len-1); }
                } else {
                    // Checksum Failed
                     #ifdef DEBUG_MSG
                     char dbg_buf[64];
                     snprintf(dbg_buf, sizeof(dbg_buf), "RZC RX CS FAIL: DType=0x%02X Got=0x%02X Exp=0x%02X\r\n",
                              rx_dtype, received_checksum, calculated_checksum);
                     hw_usart_write(hw_usart_get(), (uint8_t*)dbg_buf, strlen(dbg_buf));
                     #endif
                }
            }
            rx_state = RX_RZC_WAIT_START; // Reset state machine
            break;

        default:
            rx_state = RX_RZC_WAIT_START; // Reset on error
            break;
    }
}


// --- Protocol Operations Structure Instance ---

const protocol_ops_t raise_rcz_protocol_ops = {
    .radar_process = raise_rcz_radar_process, // Use 0x32 for simplicity
    .wheel_process = raise_rcz_wheel_process, // Use 0x29
    .door_process = raise_rcz_vehicle_status_process, // Send 0x38 (includes doors)
    .vehicle_info_process = raise_rcz_send_trip_info, // Send 0x33, 0x34, 0x35
    .ac_process = NULL, // No dedicated AC message function defined yet (0x21 is model specific) - could add to vehicle_status or implement 0x21
    .inc_volume = raise_rcz_inc_volume,       // Map to 0x02 + key code
    .dec_volume = raise_rcz_dec_volume,       // Map to 0x02 + key code
    .prev = raise_rcz_prev,                   // Map to 0x02 + key code
    .next = raise_rcz_next,                   // Map to 0x02 + key code
    .mode = raise_rcz_mode,                   // Map to 0x02 + key code
    .cont = raise_rcz_cont,                   // Map to 0x02 + key code
    .mici = raise_rcz_mici,                   // Map to 0x02 + key code
    .cmd_process = raise_rcz_cmd_process,       // Handle incoming commands from HU
    .park_process = raise_rcz_radar_process,    // Reuse radar function for parking
};
