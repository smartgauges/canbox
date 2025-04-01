#include <string.h>
#include <stdbool.h>
#include "car.h"
#include "hw_can.h"
#include "utils.h"  // Make sure utils.h includes the scale function

// Peugeot 407 CAN message handlers (using data from fazerxlo/simulator)

// --- Helper Function (Inline for Efficiency) ---
static inline uint16_t get_be16(const uint8_t *buf) {
    return ((uint16_t)buf[0] << 8) | buf[1];
}

static inline int16_t get_be16_signed(const uint8_t *buf) {
    return (int16_t)get_be16(buf);
}

static inline uint32_t get_be32(const uint8_t *buf) {
    return ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
}

static inline uint32_t get_be24(const uint8_t *buf) {
    return ((uint32_t)buf[0] << 16) | ((uint32_t)buf[1] << 8) | buf[2];
}

// --- Defines based on PSACANBridge analysis for 0x0B6 ---
#define ID_0x0B6_RPM_BYTE_MSB       5
#define ID_0x0B6_RPM_BYTE_LSB       6 // Note: PSACANBridge used get_be16(&data[5]), implying data[5]=MSB, data[6]=LSB
#define ID_0x0B6_COOLANT_BYTE       5 // From YAML, matches 0x0F6 coolant byte in PSACANBridge
#define ID_0x0B6_COOLANT_INVALID    0xFF // Assume FF is invalid like oil temp
#define ID_0x0B6_COOLANT_COLD_RAW   0x00 // Value seen in logs when likely cold

static void peugeot_407_ms_0B6_engine_status_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states derived from this message on timeout
        carstate.taho = 0; // Reset RPM
        carstate.engine = 0; // Assume engine off if message stops
        // Reset coolant temp to a safe/invalid value. Use the same logic as 0x161.
        carstate.engine_temp = -48;
        return;
    }

    // --- Decode Engine RPM ---
    // Using Big Endian read (Byte 5 = MSB, Byte 6 = LSB) and divide by 4
    uint16_t rpm_raw = get_be16(&msg[ID_0x0B6_RPM_BYTE_MSB]); // Reads msg[5] and msg[6]
    carstate.taho = rpm_raw / 4;

    // Update engine running state based on RPM
    // Threshold might need adjustment based on idle speed. 500 is common.
    if (carstate.taho > 400) { // Engine considered running if RPM > ~400
        carstate.engine = 1;
    } else {
        carstate.engine = 0;
    }

    // --- Decode Engine Coolant Temperature ---
    // Using formula from YAML: (RAW * 0.75) - 48.0
    uint8_t coolant_raw = msg[ID_0x0B6_COOLANT_BYTE]; // Using Byte 5 as per YAML

    // Check for potentially invalid readings (like 0xFF or the 0x00 seen in logs)
    if (coolant_raw == ID_0x0B6_COOLANT_INVALID || coolant_raw == ID_0x0B6_COOLANT_COLD_RAW) {
         // Keep previous value or set to invalid marker? Let's use -48 (min possible)
         // Only reset fully if it hasn't received a valid value yet
        if (carstate.engine_temp == 0) { // Check if still initial value
             carstate.engine_temp = -48;
         }
         // Otherwise, hold the last known good temperature during brief invalid readings
    } else {
        carstate.engine_temp = (int16_t)(((float)coolant_raw * 0.75f) - 48.0f);
    }

    // We are ignoring Speed, Ignition, and Illumination from this message ID based on analysis.
}


// --- Defines based on verified 0x36 log data ---
#define ID_0x036_IGN_STATE_BYTE         4
#define ID_0x036_IGN_STATE_MASK         0x03 // Bits 0-1 seem sufficient based on values 0,1,2,3
#define ID_0x036_IGN_STATE_OFF          0x00
#define ID_0x036_IGN_STATE_ON           0x01
#define ID_0x036_IGN_STATE_CRANKING     0x02 // State during engine start
#define ID_0x036_IGN_STATE_ACC          0x03

#define ID_0x036_LIGHT_BYTE             3
#define ID_0x036_LIGHT_ENABLE_MASK      0x20 // Bit 5: Dashboard lighting enabled
#define ID_0x036_LIGHT_BRIGHTNESS_MASK  0x0F // Bits 0-3: Brightness level (0-15)

// Structure to hold decoded state from 0x036 for clarity elsewhere
typedef struct {
    uint8_t raw_ign_state;
    bool lights_enabled;
    uint8_t brightness;
} state_0x036_t;

static void peugeot_407_ms_036_ign_light_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.ign = STATE_UNDEF;
        carstate.acc = STATE_UNDEF;
        carstate.illum = 0; // Brightness level
        return;
    }

    // Decode current message
    state_0x036_t current_state;
    current_state.raw_ign_state = msg[ID_0x036_IGN_STATE_BYTE] & ID_0x036_IGN_STATE_MASK;
    current_state.lights_enabled = (msg[ID_0x036_LIGHT_BYTE] & ID_0x036_LIGHT_ENABLE_MASK);
    current_state.brightness = msg[ID_0x036_LIGHT_BYTE] & ID_0x036_LIGHT_BRIGHTNESS_MASK;

    // --- Update Ignition/ACC State ---
    switch (current_state.raw_ign_state) {
        case ID_0x036_IGN_STATE_ON:
        case ID_0x036_IGN_STATE_CRANKING: // Treat cranking as IGN ON for HU power
            carstate.ign = 1;
            carstate.acc = 1;
            break;
        case ID_0x036_IGN_STATE_ACC:
            carstate.ign = STATE_UNDEF;
            carstate.acc = 1;
            break;
        case ID_0x036_IGN_STATE_OFF:
        default: // Treat unknown states as OFF
            carstate.ign = STATE_UNDEF;
            carstate.acc = STATE_UNDEF;
            break;
    }

    // --- Update Illumination State ---
    // Store the brightness level (0-15)
    carstate.illum = current_state.brightness;
}



// --- Defines based on verified 0x0F6 log data ---
#define ID_0x0F6_BYTE0                  0
#define ID_0x0F6_REVERSE_MASK           0x08 // VERIFY THIS BIT! (Log shows bit 3, docs say bit 2)
#define ID_0x0F6_TURN_LEFT_MASK         0x01 // Needs verification with logs
#define ID_0x0F6_TURN_RIGHT_MASK        0x02 // Needs verification with logs
#define ID_0x0F6_IGNITION_ON_MASK       0x80 // Bit 7 confirms IGN is ON

#define ID_0x0F6_AMBIENT_TEMP_BYTE      3

static void peugeot_407_ms_0F6_status_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states derived *only* from this message if they timeout
        carstate.selector = STATE_UNDEF;
        // Keep carstate.selector based on its primary source message if it exists
        carstate.temp = -40; // Reset ambient temp to minimum possible
        // Reset turn signal indicators if you add them later
        return;
    }

    // --- Decode Reverse Gear Status (VERIFY MASK!) ---
    if (msg[ID_0x0F6_BYTE0] & ID_0x0F6_REVERSE_MASK) {
        carstate.selector = e_selector_r;
    } else {
        if (carstate.selector == e_selector_r) {
            carstate.selector = e_selector_p; // Or undef/last known non-reverse state
        }
    }

    // --- Decode Ambient Temperature ---
    // Using formula from PSACANBridge: (RAW * 0.5) - 40.0
    uint8_t ambient_temp_raw = msg[ID_0x0F6_AMBIENT_TEMP_BYTE];
    if (ambient_temp_raw != 0xFF) { // Check for invalid value
       // Cast to float for calculation, then back to int16_t for storage
       carstate.temp = (int16_t)(((float)ambient_temp_raw * 0.5f) - 40.0f);
    } else {
       carstate.temp = -40; // Or some other indicator of invalid data
    }


    // --- TODO: Decode Turn Signals (when logs are available) ---
    // uint8_t left_ts = (msg[ID_0x0F6_BYTE0] & ID_0x0F6_TURN_LEFT_MASK);
    // uint8_t right_ts = (msg[ID_0x0F6_BYTE0] & ID_0x0F6_TURN_RIGHT_MASK);
    // Update carstate or trigger callbacks/protocol messages as needed

    // We don't update carstate.ign here, assuming 0x036 is the primary source.
    // We ignore coolant temp and odometer from this message based on analysis.
}

// --- Defines based on PSACANBridge code for 0x128 ---
#define ID_0x128_LIGHT_BYTE         5
#define ID_0x128_PARK_LIGHT_MASK    0x80 // Bit 7: Sidelights/Parking Lights
#define ID_0x128_NEAR_LIGHT_MASK    0x40 // Bit 6: Low Beam/Near Lights
// Add other light masks if needed later:
// #define ID_0x128_HIGH_BEAM_MASK     0x20
// #define ID_0x128_FRONT_FOG_MASK     0x10
// #define ID_0x128_REAR_FOG_MASK      0x08
// #define ID_0x128_TURN_RIGHT_MASK    0x04
// #define ID_0x128_TURN_LEFT_MASK     0x02

#define ID_0x128_STATUS_BYTE        7
#define ID_0x128_SEATBELT_MASK      0x80 // Bit 7: Driver Seatbelt Warning Light (1 = Warning/Unfastened?)
#define ID_0x128_PARK_BRAKE_MASK    0x40 // Bit 6: Parking Brake Light ON

#define ID_0x128_FUEL_BYTE          4
#define ID_0x128_LOW_FUEL_MASK      0x01 // Bit 0: Low Fuel Warning Light

// This handler primarily updates light statuses and potentially warning light statuses.
// Prefer dedicated messages for primary state of Park Brake if available.
static void peugeot_407_ms_128_lights_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states derived *reliably* from this message
        carstate.park_lights = 0; // Assume OFF
        carstate.near_lights = 0; // Assume OFF
        // Optionally reset warning light states if this is the primary source
        // carstate.park_break_light_on = 0; // If storing light status separately
        // carstate.ds_belt_warning_on = 0; // If storing light status separately
        return;
    }

    // --- Decode Light Status ---
    carstate.park_lights = (msg[ID_0x128_LIGHT_BYTE] & ID_0x128_PARK_LIGHT_MASK) ? 1 : 0;
    carstate.near_lights = (msg[ID_0x128_LIGHT_BYTE] & ID_0x128_NEAR_LIGHT_MASK) ? 1 : 0;
    // TODO: Decode other lights if needed for advanced features

    // --- Decode Status Indicators (Warning Lights) ---
    // These reflect the *light on the dashboard*, not necessarily the underlying state.
    // It's usually better to get park brake state from a dedicated sensor message if possible.
    // Update carstate.park_break ONLY if this is confirmed as the primary source.
    // For now, let's assume a dedicated message OR 0x131 provides primary park brake state.
    // bool park_brake_light_on = (msg[ID_0x128_STATUS_BYTE] & ID_0x128_PARK_BRAKE_MASK);

    // Update carstate.ds_belt based on the warning light. Assume 1=Unfastened.
    // Verify if 0 means fastened or just light off.
    carstate.ds_belt = (msg[ID_0x128_STATUS_BYTE] & ID_0x128_SEATBELT_MASK) ? 1 : 0;

    // --- Decode Low Fuel Warning Light ---
    // This reflects the dashboard light. Use the calculated percentage for actual state.
    // bool low_fuel_light_on = (msg[ID_0x128_FUEL_BYTE] & ID_0x128_LOW_FUEL_MASK);
    // You could store this in a separate carstate variable if needed by the HU protocol.
    // carstate.low_fuel_warning_light = low_fuel_light_on;

}


// --- Defines based on PSACANBridge code for 0x21F ---
#define ID_0x21F_VOL_BYTE       3
#define ID_0x21F_VOL_UP_MASK    0x08 // Bit 3
#define ID_0x21F_VOL_DOWN_MASK  0x04 // Bit 2

#define ID_0x21F_SEEK_BYTE      7
#define ID_0x21F_SEEK_UP_MASK   0x80 // Bit 7 (Next)
#define ID_0x21F_SEEK_DOWN_MASK 0x40 // Bit 6 (Previous)

#define ID_0x21F_SRC_BYTE       1
#define ID_0x21F_SRC_MASK       0x02 // Bit 1 (Mode/Source)

// TODO: Define masks/logic for other buttons if identified (e.g., Phone/Cont, Mici/VR)
// They might be on this ID or another one (like 0x1B0 from RCZ).

static void peugeot_407_ms_21F_swc_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset previous key states if message stops
        key_state.key_volume = STATE_UNDEF;
        key_state.key_prev = 0; // Assume released on timeout
        key_state.key_next = 0;
        key_state.key_mode = 0;
        // Reset others if added
        return;
    }

    // --- Volume Handling ---
    uint8_t current_vol_up = (msg[ID_0x21F_VOL_BYTE] & ID_0x21F_VOL_UP_MASK);
    uint8_t current_vol_down = (msg[ID_0x21F_VOL_BYTE] & ID_0x21F_VOL_DOWN_MASK);

    // Check Volume Up press (transition from not pressed to pressed)
    if (current_vol_up && (key_state.key_volume != 1)) {
        if (key_state.key_cb && key_state.key_cb->inc_volume) {
            key_state.key_cb->inc_volume(1); // Argument '1' might indicate steps, adjust if needed
        }
        key_state.key_volume = 1; // Store state as 'Up pressed'
    }
    // Check Volume Down press
    else if (current_vol_down && (key_state.key_volume != 0)) {
        if (key_state.key_cb && key_state.key_cb->dec_volume) {
            key_state.key_cb->dec_volume(1);
        }
        key_state.key_volume = 0; // Store state as 'Down pressed'
    }
    // Check for release (neither button pressed)
    else if (!current_vol_up && !current_vol_down && key_state.key_volume != STATE_UNDEF) {
        key_state.key_volume = STATE_UNDEF; // Reset state to released/unknown
    }

    // --- Seek/Track Handling ---
    uint8_t current_seek_down = (msg[ID_0x21F_SEEK_BYTE] & ID_0x21F_SEEK_DOWN_MASK);
    uint8_t current_seek_up = (msg[ID_0x21F_SEEK_BYTE] & ID_0x21F_SEEK_UP_MASK);

    // Check Previous Track press (transition from 0 to 1)
    if (current_seek_down && !key_state.key_prev) {
         if (key_state.key_cb && key_state.key_cb->prev) {
            key_state.key_cb->prev();
        }
    }
    key_state.key_prev = current_seek_down ? 1 : 0; // Update previous state

    // Check Next Track press (transition from 0 to 1)
    if (current_seek_up && !key_state.key_next) {
         if (key_state.key_cb && key_state.key_cb->next) {
            key_state.key_cb->next();
        }
    }
    key_state.key_next = current_seek_up ? 1 : 0; // Update previous state

    // --- Source/Mode Button Handling ---
    uint8_t current_mode = (msg[ID_0x21F_SRC_BYTE] & ID_0x21F_SRC_MASK);
    // Check Mode press (transition from 0 to 1)
    if (current_mode && !key_state.key_mode) {
        if (key_state.key_cb && key_state.key_cb->mode) {
            key_state.key_cb->mode();
        }
    }
    key_state.key_mode = current_mode ? 1 : 0; // Update previous state


    // --- TODO: Handle Scroll Wheel (Byte 0) ---
    // Needs analysis of how the value changes. Is it a delta? Absolute?
    // Map to appropriate key_cb functions if needed (e.g., could potentially map to vol up/down or list scroll).

    // --- TODO: Handle other buttons (Phone/Cont, Mici/VR) ---
    // Determine their CAN ID and data bits via sniffing and add logic here or in a separate handler.
}

// --- Handler Functions ---

static void peugeot_407_ms_vin_336_handler(const uint8_t *msg, struct msg_desc_t *desc)
{
	if (is_timeout(desc)) {
		// Handle timeout (maybe set a flag indicating VIN not fully received)
        memset(carstate.vin, 0, sizeof(carstate.vin)); // Clear the VIN
        carstate.vin[0] = 'n'; //"na"
		carstate.vin[1] = 'a';
		return;
	}
	// First 3 chars of VIN
    memcpy(carstate.vin, msg, 3);

}

static void peugeot_407_ms_vin_3B6_handler(const uint8_t *msg, struct msg_desc_t *desc)
{
    if (is_timeout(desc)) {
       // Handle timeout
		memset(carstate.vin, 0, sizeof(carstate.vin)); // Clear the VIN
        carstate.vin[0] = 'n'; //"na"
		carstate.vin[1] = 'a';
		return;
	}

	// Chars 4-9 of VIN
    memcpy(carstate.vin + 3, msg, 6);
}

static void peugeot_407_ms_vin_2B6_handler(const uint8_t *msg, struct msg_desc_t *desc)
{
	if (is_timeout(desc)) {
		// Handle timeout
        memset(carstate.vin, 0, sizeof(carstate.vin)); // Clear the VIN
        carstate.vin[0] = 'n'; //"na"
		carstate.vin[1] = 'a';
		return;
	}

	// Last 8 chars of VIN
    memcpy(carstate.vin + 9, msg, 8);
    carstate.vin[17] = '\0'; // Null-terminate the VIN
}

// --- Defines based on PSACANBridge/YAML for 0x14C/0x28C ---
// Using 0x28C as the source based on log analysis
#define ID_0x28C_SPEED_BYTE_MSB     0
#define ID_0x28C_SPEED_BYTE_LSB     1
#define ID_0x28C_ODO_BYTE_1         1 // MSB of Odometer
#define ID_0x28C_ODO_BYTE_2         2 // Mid Byte
#define ID_0x28C_ODO_BYTE_3         3 // LSB of Odometer


// Handler for 0x28C (Primary source for Speed, potential Odometer)
static void peugeot_407_ms_28C_speed_odo_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states on timeout
        carstate.speed = 0;
        // Keep last known odometer? Or reset? Resetting might be confusing.
        // Let's keep the last known value unless it was never valid.
        // if (carstate.odometer == 0) carstate.odometer = 0; // Or some invalid marker
        return;
    }

    // --- Decode Vehicle Speed ---
    // Reads msg[0] (MSB) and msg[1] (LSB)
    uint16_t speed_raw = get_be16(&msg[ID_0x28C_SPEED_BYTE_MSB]);
    // Apply scaling factor 0.01
    carstate.speed = (uint16_t)((float)speed_raw * 0.01f);

    // --- Decode Odometer ---
    // Reads msg[1] (MSB), msg[2] (Mid), msg[3] (LSB)
    // Note: Requires confirmation with logs showing odometer changes.
    carstate.odometer = get_be24(&msg[ID_0x28C_ODO_BYTE_1]);
}

static void peugeot_407_ms_131_doors_fuel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.fl_door = STATE_UNDEF;
        carstate.fr_door = STATE_UNDEF;
        carstate.rl_door = STATE_UNDEF;
        carstate.rr_door = STATE_UNDEF;
        return;
    }

	carstate.fl_door = (msg[1] & 0x01) ? 1 : 0; // Example: Bit 0 of Byte 1 for FL door
    carstate.fr_door = (msg[1] & 0x02) ? 1 : 0; // Example: Bit 1 of Byte 1 for FR door
    carstate.rl_door = (msg[1] & 0x04) ? 1 : 0; // Example: Bit 2 of Byte 1 for RL door
    carstate.rr_door = (msg[1] & 0x08) ? 1 : 0; // Example: Bit 3 of Byte 1 for RR door
}


// --- Defines based on PSACANBridge analysis for 0x168 ---
#define ID_0x168_TEMP_BYTE          0
#define ID_0x168_VOLTAGE_BYTE       1
#define ID_0x168_INVALID_TEMP_RAW   0xFF // Common invalid value, check logs if different
#define ID_0x168_INVALID_VOLT_RAW   0xFF // Common invalid value

// Ambient temp often has a wider valid range than coolant/oil
#define AMBIENT_TEMP_MIN -40
#define AMBIENT_TEMP_MAX 87 // Based on Hiworld protocol range derived from +40 offset

// Voltage range check
#define VOLTAGE_MIN_V   5.0f // Minimum plausible voltage from formula (RAW=0)
#define VOLTAGE_MAX_V  17.75f // Maximum plausible voltage from formula (RAW=255)

static void peugeot_407_ms_168_temp_battery_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states on timeout
        carstate.temp = AMBIENT_TEMP_MIN; // Reset to min possible value
        carstate.voltage = 0; // Reset voltage
        // Reset low_voltage status if primarily determined here
        // carstate.low_voltage = STATE_UNDEF;
        return;
    }

    // --- Decode Outside Ambient Temperature ---
    // Using formula from PSACANBridge: (RAW * 0.5) - 40.0
    uint8_t temp_raw = msg[ID_0x168_TEMP_BYTE];
    if (temp_raw != ID_0x168_INVALID_TEMP_RAW) { // Check for invalid raw value
        // Calculate using float, then store as int16_t
        float temp_calculated = ((float)temp_raw * 0.5f) - 40.0f;
        // Optional: Add range check if needed
        // if (temp_calculated >= AMBIENT_TEMP_MIN && temp_calculated <= AMBIENT_TEMP_MAX) {
             carstate.temp = (int16_t)temp_calculated;
        // } else { // Handle out-of-range calculated value }
    } else {
        // Handle invalid raw reading - keep last known good value? Or set to min?
        // Let's keep the last valid value for temp unless it's still the initial state
        if (carstate.temp == 0) { // Or check against AMBIENT_TEMP_MIN if that's default invalid
            carstate.temp = AMBIENT_TEMP_MIN;
        }
    }


    // --- Decode Battery Voltage ---
    // Using formula from PSACANBridge: (RAW * 0.05) + 5.0
    uint8_t voltage_raw = msg[ID_0x168_VOLTAGE_BYTE];
    if (voltage_raw != ID_0x168_INVALID_VOLT_RAW) { // Check for invalid raw value
        float voltage_calculated = ((float)voltage_raw * 0.05f) + 5.0f;

        // Optional: Range check
        // if (voltage_calculated >= VOLTAGE_MIN_V && voltage_calculated <= VOLTAGE_MAX_V) {
            // Store voltage scaled * 100 (e.g., 12.5V stored as 1250)
            carstate.voltage = (uint32_t)(voltage_calculated * 100.0f);

            // Update low_voltage warning state (example threshold 11.8V)
            // if (voltage_calculated < 11.8f) {
            //     carstate.low_voltage = 1;
            // } else {
            //     carstate.low_voltage = 0;
            // }
        // } else { // Handle out-of-range calculated value }
    } else {
        // Handle invalid raw reading - keep last known value? Or set to 0?
         if (carstate.voltage == 0) { // If still initial value
             carstate.voltage = (uint32_t)(VOLTAGE_MIN_V * 100.0f); // Set to min plausible
         }
        // carstate.low_voltage = STATE_UNDEF; // Status unknown if reading invalid
    }
}


// --- Defines based on PSACANBridge code for 0x1D0 ---
#define ID_0x1D0_FAN_BYTE           5
#define ID_0x1D0_FAN_MASK           0x07 // Bits 2-0 for speed 0-7

#define ID_0x1D0_AIRFLOW_BYTE       4
#define ID_0x1D0_AIRFLOW_WIND_MASK  0x10 // Bit 4: Windshield
#define ID_0x1D0_AIRFLOW_MID_MASK   0x20 // Bit 5: Face/Middle
#define ID_0x1D0_AIRFLOW_FLOOR_MASK 0x40 // Bit 6: Floor // Corrected based on typical PSA mapping

#define ID_0x1D0_STATUS_BYTE        3
#define ID_0x1D0_RECIRC_MASK        0x80 // Bit 7: Recirculation
#define ID_0x1D0_AC_MASK            0x20 // Bit 5: AC Compressor Active
#define ID_0x1D0_AC_AUTO_MASK       0x08 // Bit 3: Auto mode (Needs verification if reliable)

#define ID_0x1D0_TEMP_L_BYTE        1 // Corrected: PSACANBridge psa.h shows Byte 1 for Left
#define ID_0x1D0_TEMP_R_BYTE        0 // Corrected: PSACANBridge psa.h shows Byte 0 for Right
#define ID_0x1D0_TEMP_RAW_INVALID   0xFF
#define ID_0x1D0_TEMP_RAW_LO        0x00
#define ID_0x1D0_TEMP_RAW_HI        0x1F // Value corresponding to "HI"

// Helper to decode temperature (matches Hiworld mapping closely, but returns float)
static float decode_psa_temp(uint8_t raw_temp) {
    if (raw_temp == ID_0x1D0_TEMP_RAW_INVALID) return -100.0f; // Indicate invalid
    if (raw_temp == ID_0x1D0_TEMP_RAW_LO) return 13.5f; // Represent LO as slightly below min numeric
    if (raw_temp == ID_0x1D0_TEMP_RAW_HI) return 30.5f; // Represent HI as slightly above max numeric
    // Map 0x01 (14.0) to 0x1E (30.0) - Assuming 0.5 deg steps
    return 14.0f + ((float)(raw_temp - 1) * 0.5f);
}


static void peugeot_407_ms_1D0_climate_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset climate states to default/OFF on timeout
        car_air_state.fanspeed = 0;
        car_air_state.wind = 0;
        car_air_state.middle = 0;
        car_air_state.floor = 0;
        car_air_state.ac = 0;
        car_air_state.recycling = 0;
        // Reset temps to a default state? Maybe keep last known? For now, set to 'invalid' indicator.
        car_air_state.l_temp = STATE_UNDEF; // Use STATE_UNDEF for temp invalidity
        car_air_state.r_temp = STATE_UNDEF;
        // Reset other air states if they are added later (dual, ac_max etc.)
        car_air_state.dual = STATE_UNDEF;
        car_air_state.ac_max = STATE_UNDEF;
        // ...
        return;
    }

    // --- Decode Fan Speed ---
    car_air_state.fanspeed = msg[ID_0x1D0_FAN_BYTE] & ID_0x1D0_FAN_MASK; // 0-7

    // --- Decode Airflow Direction ---
    car_air_state.wind   = (msg[ID_0x1D0_AIRFLOW_BYTE] & ID_0x1D0_AIRFLOW_WIND_MASK) ? 1 : 0;
    car_air_state.middle = (msg[ID_0x1D0_AIRFLOW_BYTE] & ID_0x1D0_AIRFLOW_MID_MASK)  ? 1 : 0;
    car_air_state.floor  = (msg[ID_0x1D0_AIRFLOW_BYTE] & ID_0x1D0_AIRFLOW_FLOOR_MASK) ? 1 : 0;

    // --- Decode Status Bits ---
    car_air_state.ac        = (msg[ID_0x1D0_STATUS_BYTE] & ID_0x1D0_AC_MASK)       ? 1 : 0;
    car_air_state.recycling = (msg[ID_0x1D0_STATUS_BYTE] & ID_0x1D0_RECIRC_MASK)   ? 1 : 0;
    // bool auto_mode     = (msg[ID_0x1D0_STATUS_BYTE] & ID_0x1D0_AC_AUTO_MASK); // Store if needed

    // --- Decode Temperatures ---
    uint8_t temp_l_raw = msg[ID_0x1D0_TEMP_L_BYTE];
    uint8_t temp_r_raw = msg[ID_0x1D0_TEMP_R_BYTE];

    float temp_l_c = decode_psa_temp(temp_l_raw);
    float temp_r_c = decode_psa_temp(temp_r_raw);

    // Store as uint8_t representing the raw value or mapped value for Hiworld?
    // Hiworld mapping (0=LO, 1=14.0,... 32=30.0, 33=HI) seems different from PSA raw.
    // Let's store the RAW PSA value for now, and handle mapping in hiworldpsa.c
     car_air_state.l_temp = (temp_l_raw == ID_0x1D0_TEMP_RAW_INVALID) ? STATE_UNDEF : temp_l_raw;
     car_air_state.r_temp = (temp_r_raw == ID_0x1D0_TEMP_RAW_INVALID) ? STATE_UNDEF : temp_r_raw;

    // Update other potentially missing car_air_state fields based on other messages or defaults if necessary
    // e.g., Dual mode might be from another message or inferred if L/R temps differ significantly.
    // For now, we only decode what's directly in 0x1D0 according to PSACANBridge.
}

// --- Defines based on PSACANBridge/PSACAN.md code for 0x0E1 ---
#define ID_0x0E1_DISPLAY_ACTIVE_BYTE    5
#define ID_0x0E1_DISPLAY_ACTIVE_MASK    0x01 // Bit 0

#define ID_0x0E1_ZONE_ACTIVE_BYTE       1
#define ID_0x0E1_FRONT_ACTIVE_MASK      0x10 // Bit 4
#define ID_0x0E1_REAR_ACTIVE_MASK       0x40 // Bit 6
// #define ID_0x0E1_BOTH_ACTIVE_MASK    0x20 // Bit 5 - Verify if needed

#define ID_0x0E1_REAR_SENSORS_BYTE_A    2
#define ID_0x0E1_REAR_SENSORS_BYTE_B    3
#define ID_0x0E1_FRONT_SENSORS_BYTE_A   3
#define ID_0x0E1_FRONT_SENSORS_BYTE_B   4

static void peugeot_407_ms_0E1_parktronic_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    // Define a default inactive state
    const radar_t inactive_radar = {
        .state = e_radar_off,
        .fl = 7, .flm = 7, .frm = 7, .fr = 7, // Use '7' for farthest/inactive
        .rl = 7, .rlm = 7, .rrm = 7, .rr = 7
    };

    if (is_timeout(desc)) {
        // Reset radar state on timeout
        memcpy(&carstate.radar, &inactive_radar, sizeof(radar_t));
        return;
    }

    // Check if the Parktronic display should be active
    bool display_active = (msg[ID_0x0E1_DISPLAY_ACTIVE_BYTE] & ID_0x0E1_DISPLAY_ACTIVE_MASK);

    if (!display_active) {
        // If display isn't active, ensure state is off and distances are inactive
        if (carstate.radar.state != e_radar_off) {
             memcpy(&carstate.radar, &inactive_radar, sizeof(radar_t));
        }
        return;
    }

    // --- Determine Active Zones ---
    bool front_active = (msg[ID_0x0E1_ZONE_ACTIVE_BYTE] & ID_0x0E1_FRONT_ACTIVE_MASK);
    bool rear_active = (msg[ID_0x0E1_ZONE_ACTIVE_BYTE] & ID_0x0E1_REAR_ACTIVE_MASK);

    if (front_active && rear_active) {
        carstate.radar.state = e_radar_on; // Both active
    } else if (front_active) {
        carstate.radar.state = e_radar_on_front;
    } else if (rear_active) {
        carstate.radar.state = e_radar_on_rear;
    } else {
        // If display is active but neither zone flag is set? Default to OFF or a specific state?
        // Let's assume OFF if no zone is explicitly active, even if display_active is true.
         memcpy(&carstate.radar, &inactive_radar, sizeof(radar_t));
         return; // Exit if no zones active
    }

    // --- Decode Sensor Distances (0=Closest, 7=Farthest) ---
    // Rear Sensors
    carstate.radar.rl  = (msg[ID_0x0E1_REAR_SENSORS_BYTE_A] >> 5) & 0x07;
    carstate.radar.rlm = (msg[ID_0x0E1_REAR_SENSORS_BYTE_A] >> 2) & 0x07;
    carstate.radar.rrm = ((msg[ID_0x0E1_REAR_SENSORS_BYTE_A] << 1) & 0x06) | ((msg[ID_0x0E1_REAR_SENSORS_BYTE_B] >> 7) & 0x01);
    carstate.radar.rr  = (msg[ID_0x0E1_REAR_SENSORS_BYTE_B] >> 4) & 0x07;

    // Front Sensors
    carstate.radar.fl  = (msg[ID_0x0E1_FRONT_SENSORS_BYTE_A] >> 1) & 0x07;
    carstate.radar.flm = ((msg[ID_0x0E1_FRONT_SENSORS_BYTE_A] << 2) & 0x04) | ((msg[ID_0x0E1_FRONT_SENSORS_BYTE_B] >> 6) & 0x03);
    carstate.radar.frm = (msg[ID_0x0E1_FRONT_SENSORS_BYTE_B] >> 3) & 0x07;
    carstate.radar.fr  = msg[ID_0x0E1_FRONT_SENSORS_BYTE_B] & 0x07;

    // Safety check: If only rear is active, potentially zero out front distances?
    // Or rely on the Hiworld mapping function to handle 0xFF correctly?
    // Let's keep raw values for now and let mapping handle it.
    if (carstate.radar.state == e_radar_on_rear) {
       carstate.radar.fl = 7; carstate.radar.flm = 7; carstate.radar.frm = 7; carstate.radar.fr = 7;
    } else if (carstate.radar.state == e_radar_on_front) {
       carstate.radar.rl = 7; carstate.radar.rlm = 7; carstate.radar.rrm = 7; carstate.radar.rr = 7;
    }
}

// --- Defines based on PSACANBridge code for 0x161 ---
#define ID_0x161_OIL_TEMP_BYTE      2
#define ID_0x161_FUEL_CALC_BYTE     3
#define ID_0x161_OIL_TEMP_INVALID   0xFF // Value observed indicating invalid/not ready temp

static void peugeot_407_ms_161_temp_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset states derived from this message on timeout
        carstate.oil_temp = -40; // Reset oil temp to its minimum possible value based on offset
        // Fuel level percentage is calculated here, so maybe set it to 0 or STATE_UNDEF on timeout?
        // Let's set to 0, assuming empty on timeout might be safer.
        carstate.fuel_lvl = 0;
        carstate.low_fuel_lvl = STATE_UNDEF; // Low fuel status unknown on timeout
        return;
    }

    // --- Decode Oil Temperature ---
    uint8_t oil_temp_raw = msg[ID_0x161_OIL_TEMP_BYTE];
    if (oil_temp_raw != ID_0x161_OIL_TEMP_INVALID) {
        // Apply offset: Temp = RAW + 40
        carstate.oil_temp = (int16_t)oil_temp_raw + 40;
    } else {
        // Keep previous value or set to a specific "invalid" marker?
        // Let's keep the previous valid value unless it's the initial state.
        if (carstate.oil_temp == 0) { // Check if it's still the initial value
             carstate.oil_temp = -40; // Set to minimum possible value if still initial
        }
        // If it had a valid value before, just keep it during brief invalid readings.
    }

    // --- Calculate Fuel Level Percentage ---
    uint8_t fuel_calc_byte = msg[ID_0x161_FUEL_CALC_BYTE];
    uint8_t fuel_level_bits = (fuel_calc_byte >> 2) & 0x3F; // Lower 6 bits = level
    uint8_t max_fuel_bits   = (fuel_calc_byte >> 1) & 0x7F; // 7 bits = max capacity representation

    if (max_fuel_bits != 0) {
        // Calculate percentage (using 32-bit intermediate to avoid overflow)
        uint8_t fuel_percent = (uint8_t)(((uint32_t)fuel_level_bits * 100) / max_fuel_bits);

        // Clamp the value to 0-100%
        if (fuel_percent > 100) {
            fuel_percent = 100;
        }
        carstate.fuel_lvl = fuel_percent;

        // Update low fuel level warning based on percentage
        // Threshold might need adjustment (e.g., 10% or 12%)
        if (carstate.fuel_lvl <= 10) { // Example: Low fuel below 10%
            carstate.low_fuel_lvl = 1;
        } else {
            carstate.low_fuel_lvl = 0;
        }
    } else {
        // Max fuel is 0, calculation invalid - maybe sensor not ready?
        // Keep previous value or set to 0? Let's set to 0.
        carstate.fuel_lvl = 0;
        carstate.low_fuel_lvl = STATE_UNDEF; // Status unknown if calc is invalid
    }
}

// --- Defines based on PSACAN.md/PSACANBridge for 0x221 ---
#define ID_0x221_CONS_BYTE_MSB       4
#define ID_0x221_CONS_BYTE_LSB       5
#define ID_0x221_RANGE_BYTE_MSB      2
#define ID_0x221_RANGE_BYTE_LSB      3
#define ID_0x221_INV_CONS_BYTE       7
#define ID_0x221_INV_CONS_MASK      0x01 // Bit 0
#define ID_0x221_INV_RANGE_BYTE      6
#define ID_0x221_INV_RANGE_MASK     0x01 // Bit 0

static void peugeot_407_ms_221_trip_inst_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.inst_consumption_raw = 0;
        carstate.range_km = 0;
        return;
    }

    // --- Decode Instantaneous Consumption ---
    uint8_t  inst_consumption_invalid = (msg[ID_0x221_INV_CONS_BYTE] & ID_0x221_INV_CONS_MASK);
    if (!inst_consumption_invalid) {
        // Read raw Big Endian value. Scaling factor needs verification (e.g., /10 for L/100km?)
        carstate.inst_consumption_raw = get_be16(&msg[ID_0x221_CONS_BYTE_MSB]);
        // Example scaling (VERIFY THIS!): carstate.inst_consumption_L100km = (float)carstate.inst_consumption_raw / 10.0f;
    } else {
        carstate.inst_consumption_raw = 0xFFFF; // Indicate invalid with max value?
    }

    // --- Decode Range (DTE) ---
    uint8_t  range_invalid = (msg[ID_0x221_INV_RANGE_BYTE] & ID_0x221_INV_RANGE_MASK);
    if (!range_invalid) {
        // Read raw Big Endian value. Units likely km.
        carstate.range_km = get_be16(&msg[ID_0x221_RANGE_BYTE_MSB]);
    } else {
        carstate.range_km = 0xFFFF; // Indicate invalid
    }
}


// --- Confirmed Byte Indices (based on C4 B7 structure) ---
#define TRIP_AVG_SPEED_BYTE      0 // km/h
#define TRIP_DIST_BYTE_MSB       1 // km (High Byte)
#define TRIP_DIST_BYTE_LSB       2 // km (Low Byte)
#define TRIP_AVG_CONS_BYTE_MSB   3 // L/100km * 10 (High Byte) - Assuming Big Endian
#define TRIP_AVG_CONS_BYTE_LSB   4 // L/100km * 10 (Low Byte) - Assuming Big Endian

// Handler for 0x2A1 (Trip 1 Data)
static void peugeot_407_ms_2A1_trip1_handler(const uint8_t * msg, struct msg_desc_t * desc) {
    if (is_timeout(desc)) {
        // Reset Trip 1 values if message stops
        carstate.avg_speed1 = 0;
        carstate.avg_consumption1_raw = 0xFFFF; // Indicate invalid
        carstate.trip_distance1 = 0;
        return;
    }

    // Decode values based on confirmed structure
    carstate.avg_speed1 = msg[TRIP_AVG_SPEED_BYTE]; // km/h

    // Distance = Byte1 << 8 | Byte 2 (km)
    carstate.trip_distance1 = ((uint32_t)msg[TRIP_DIST_BYTE_MSB] << 8) | msg[TRIP_DIST_BYTE_LSB];

    // Average Consumption (Raw L/100km * 10) - Assuming Big Endian
    carstate.avg_consumption1_raw = ((uint16_t)msg[TRIP_AVG_CONS_BYTE_MSB] << 8) | msg[TRIP_AVG_CONS_BYTE_LSB];
}

// Handler for 0x261 (Trip 2 Data)
static void peugeot_407_ms_261_trip2_handler(const uint8_t * msg, struct msg_desc_t * desc) {
    if (is_timeout(desc)) {
        // Reset Trip 2 values if message stops
        carstate.avg_speed2 = 0;
        carstate.avg_consumption2_raw = 0xFFFF; // Indicate invalid
        carstate.trip_distance2 = 0;
        return;
    }

    // Decode values based on confirmed structure
    carstate.avg_speed2 = msg[TRIP_AVG_SPEED_BYTE]; // km/h

    // Distance = Byte1 << 8 | Byte 2 (km)
    carstate.trip_distance2 = ((uint32_t)msg[TRIP_DIST_BYTE_MSB] << 8) | msg[TRIP_DIST_BYTE_LSB];

    // Average Consumption (Raw L/100km * 10) - Assuming Big Endian
    carstate.avg_consumption2_raw = ((uint16_t)msg[TRIP_AVG_CONS_BYTE_MSB] << 8) | msg[TRIP_AVG_CONS_BYTE_LSB];
}


static struct msg_desc_t peugeot_407_ms[] =
{
    { 0x36,    100, 0, 0, peugeot_407_ms_036_ign_light_handler },
    { 0x0B6,    50, 0, 0, peugeot_407_ms_0B6_engine_status_handler },
    { 0x0F6,    100, 0, 0, peugeot_407_ms_0F6_status_handler }, 
    { 0x128,    100, 0, 0, peugeot_407_ms_128_lights_handler },
    { 0x21F,    100, 0, 0, peugeot_407_ms_21F_swc_handler },

    { 0x336,   1000, 0, 0, peugeot_407_ms_vin_336_handler },
    { 0x3B6,   1000, 0, 0, peugeot_407_ms_vin_3B6_handler },
    { 0x2B6,   1000, 0, 0, peugeot_407_ms_vin_2B6_handler },

    { 0x131,    100, 0, 0, peugeot_407_ms_131_doors_fuel_handler },
    { 0x0E1,    100, 0, 0, peugeot_407_ms_0E1_parktronic_handler },
    { 0x161,    100, 0, 0, peugeot_407_ms_161_temp_handler },


    { 0x28C,    100, 0, 0, peugeot_407_ms_28C_speed_odo_handler },
    { 0x168,   1000, 0, 0, peugeot_407_ms_168_temp_battery_handler },

    { 0x1D0,    100, 0, 0, peugeot_407_ms_1D0_climate_handler },

    { 0x221,    500, 0, 0, peugeot_407_ms_221_trip_inst_handler },
    { 0x2A1,    500, 0, 0, peugeot_407_ms_2A1_trip1_handler }, 
    { 0x261,    500, 0, 0, peugeot_407_ms_261_trip2_handler },
    
    // Add more message descriptors here as you identify them
};
