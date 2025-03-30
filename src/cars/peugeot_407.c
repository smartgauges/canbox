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
        carstate.ign = 0;
        carstate.acc = 0;
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
            carstate.ign = 0;
            carstate.acc = 1;
            break;
        case ID_0x036_IGN_STATE_OFF:
        default: // Treat unknown states as OFF
            carstate.ign = 0;
            carstate.acc = 0;
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

static void peugeot_407_ms_0F6_handler(const uint8_t * msg, struct msg_desc_t * desc)
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


static void peugeot_407_ms_engine_status_0B6_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.engine_temp = -48; // Minimum possible temperature.  Use a clearly invalid value.
        return;
    }

    // --- Coolant Temperature (Byte 5) ---
    // Factor: 0.75, Offset: -48, Units: °C
    carstate.engine_temp = (int16_t)((float)msg[5] * 0.75 - 48.0);
}

static void peugeot_407_ms_speed_odometer_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.speed = 0;
        carstate.odometer = 0;
        return;
    }

     // Vehicle Speed.  Factor 0.01,  Units: km/h
     carstate.speed = (uint16_t)((float)get_be16(&msg[0]) * 0.01);

     // Odometer. Factor 1, Units: km
     carstate.odometer = ((uint32_t)msg[1] << 16) | ((uint32_t)msg[2] << 8) | msg[3]; // Assuming bytes 1, 2, 3. Check the correct order!
}

static void peugeot_407_ms_doors_fuel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.fuel_lvl = 0;
        carstate.fl_door = STATE_UNDEF;
        carstate.fr_door = STATE_UNDEF;
        carstate.rl_door = STATE_UNDEF;
        carstate.rr_door = STATE_UNDEF;
        return;
    }

    // Fuel Level.  No scaling needed according to YAML.
    carstate.fuel_lvl = msg[0];
	carstate.fl_door = (msg[1] & 0x01) ? 1 : 0; // Example: Bit 0 of Byte 1 for FL door
    carstate.fr_door = (msg[1] & 0x02) ? 1 : 0; // Example: Bit 1 of Byte 1 for FR door
    carstate.rl_door = (msg[1] & 0x04) ? 1 : 0; // Example: Bit 2 of Byte 1 for RL door
    carstate.rr_door = (msg[1] & 0x08) ? 1 : 0; // Example: Bit 3 of Byte 1 for RR door
}


static void peugeot_407_ms_temp_battery_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.temp = 0; // Outside Temperature
        carstate.voltage = 0; // Battery Voltage
        return;
    }
    carstate.temp = (int16_t)((float)msg[0] * 0.1 - 40.0);

    // Battery voltage. Factor: 0.1, Units: V
    carstate.voltage = (uint16_t)((float)msg[1] * 0.1 * 100); //To keep the same format of voltage as original code.

}


static void peugeot_407_ms_1E1_handler(const uint8_t *msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        // Reset any relevant states for this message on timeout.
        // For now, do nothing, as it's all unknown signals,
        // but if you determine some critical state is in here, reset it.
        return;
    }

    // This handler is for the message with unknown signals.
    // As you reverse engineer those signals, you'll add the appropriate decoding logic.
    // For example:
    //  if (msg[0] & 0x01) {  // Example: IF bit 0 of byte 0 means something...
    //      carstate.some_variable = msg[1]; //  ...then set some_variable to the value of byte 1.
    //  }
}


static void peugeot_407_ms_1E5_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
     if (is_timeout(desc)) {
        return;
    }
}

static void peugeot_407_ms_28C_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
        carstate.speed = 0;
        carstate.odometer = 0;
        return;
    }
    carstate.speed = (uint16_t)((float)get_be16(&msg[0]) * 0.01);

    carstate.odometer = ((uint32_t)msg[1] << 16) | ((uint32_t)msg[2] << 8) | msg[3];
}

static void peugeot_407_ms_165_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        return;
    }
}

static void peugeot_407_ms_126_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        return;
    }
}

static void peugeot_407_ms_128_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        return;
    }
}
static void peugeot_407_ms_0E1_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.radar.state = e_radar_off;
        // Reset individual sensor distances to a safe/default value (e.g., 0 or 255).
        carstate.radar.fl = 0;
        carstate.radar.flm = 0;
        carstate.radar.frm = 0;
        carstate.radar.fr = 0;
        carstate.radar.rl = 0;
        carstate.radar.rlm = 0;
        carstate.radar.rrm = 0;
        carstate.radar.rr = 0;
        return;
    }

	// Byte 0:  Initial value (seems to vary depending on ignition status).  Not critical for now.
    // Byte 1:  Beep period control and sound enable.  We don't decode this for *displaying* the data.
    // Bytes 2, 3, 4: Sensor distances.

	// --- Sensor Data Decoding (Based on PSACAN.md and simulator hints) ---

	// Byte 2: Rear Left, Rear Center, Rear Right
    carstate.radar.rl  = msg[2] >> 5;          // Bits 7-5: Rear Left (0-7, where 0 is closest, 7 is farthest/no object)
    carstate.radar.rlm = (msg[2] >> 2) & 0x07; // Bits 4-2: Rear Left Middle
    carstate.radar.rrm = (msg[2] << 1) & 0x06 | (msg[3] >> 7) & 0x01;  // Bits 1-0 of Byte 2 and Bit 7 of Byte 3
    carstate.radar.rr  = (msg[3] >> 4) & 0x07; // Bits 6-4: Rear Right

    // Bytes 3 and 4: Front Left, Front Center, Front Right
    carstate.radar.fl  = (msg[3] >> 1) & 0x07; // Bits 3-1: Front Left
    carstate.radar.flm = (msg[3] << 2) & 0x04 | (msg[4] >> 6) & 0x03;  // Bit 0 of byte 3 and bits 7-6 of byte 4
    carstate.radar.frm = (msg[4] >> 3) & 0x07; // Bits 5-3: Front Right Middle
    carstate.radar.fr  = msg[4] & 0x07;         // Bits 2-0: Front Right

    // Byte 5: 0x01 to show on display
    // --- Determine Overall Radar State ---
	// Combine logic from byte 1 with explicit check for 0x01 in byte 5 to improve robustness
    uint8_t park_active = (msg[5] == 0x01); // Check if parking assist is active

    if (park_active) {
        // Determine if it's front, rear, or both based on msg[1] (as per original logic)
        if ((msg[1] & 0x10) == 0x10) { // Bit 4 indicates front active
            carstate.radar.state = e_radar_on_front;  // Front only
        }
		else if ((msg[1] & 0x40) == 0x40)
		{
            carstate.radar.state = e_radar_on_rear; // Rear only
		}
        else if ((msg[1] & 0x20) == 0x20) {
             carstate.radar.state = e_radar_on; //both?
        }
		else {
            carstate.radar.state = e_radar_off;
        }
    } else {
        carstate.radar.state = e_radar_off; // No active sensors
    }
}

static void peugeot_407_ms_161_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.oil_temp = -40; // Use a clearly invalid value, consistent with offset.
        carstate.fuel_lvl = 0;  // Reset to a default/safe value.
        return;
    }

    // --- Oil Temperature (Byte 2) ---
    // Offset: +40, Units: °C
    carstate.oil_temp = (int16_t)msg[2] + 40;

    // --- Fuel Level (Byte 3) ---
    // This is more complex.  PSAVanCanBridge suggests a calculation involving
    // a "raw" fuel level and a "max fuel" value.  We'll extract both,
    // and you'll need to implement the final calculation later.
    uint8_t fuel_level_raw = msg[3]; // Store the *raw* byte 3 value.
    uint8_t fuel_level_max = (msg[3] >> 1) & 0x7F; // Maximum value for fuel
    uint8_t fuel_level = (msg[3] >> 2) & 0x3F; // Extract the 6 bits for fuel level

    // for now lets add a simple calculation
    carstate.fuel_lvl = fuel_level;
    if (fuel_level_max != 0){
        carstate.fuel_lvl = (uint8_t)(((uint32_t)fuel_level * 100) / fuel_level_max);
        if (carstate.fuel_lvl > 10)
            carstate.low_fuel_lvl = 0;
        else
            carstate.low_fuel_lvl = 1;
    }

    // --- Remaining Bytes ---
    // Bytes 0, 1, and 4-7 are currently unknown. You'll need to analyze
    // your CAN logs to determine their purpose.

}

static struct msg_desc_t peugeot_407_ms[] =
{
    { 0x36,    100, 0, 0, peugeot_407_ms_036_ign_light_handler },
    { 0x0F6,    100, 0, 0, peugeot_407_ms_0F6_handler }, 



    { 0x0B6,    100, 0, 0, peugeot_407_ms_engine_status_0B6_handler },
    { 0x161,    100, 0, 0, peugeot_407_ms_161_handler },

    { 0x336,   1000, 0, 0, peugeot_407_ms_vin_336_handler },
    { 0x3B6,   1000, 0, 0, peugeot_407_ms_vin_3B6_handler },
    { 0x2B6,   1000, 0, 0, peugeot_407_ms_vin_2B6_handler },



    { 0x14C,    100, 0, 0, peugeot_407_ms_speed_odometer_handler },
    { 0x131,    100, 0, 0, peugeot_407_ms_doors_fuel_handler },
    { 0x168,   1000, 0, 0, peugeot_407_ms_temp_battery_handler },
	{ 0x1E1,    100, 0, 0, peugeot_407_ms_1E1_handler },
	{ 0x1E5,    100, 0, 0, peugeot_407_ms_1E5_handler },
	{ 0x28C,    100, 0, 0, peugeot_407_ms_28C_handler },
    { 0x165,    100, 0, 0, peugeot_407_ms_165_handler },
    { 0x126,    100, 0, 0, peugeot_407_ms_126_handler },
    { 0x128,    100, 0, 0, peugeot_407_ms_128_handler },
    { 0x0E1,    100, 0, 0, peugeot_407_ms_0E1_handler },
    
    // Add more message descriptors here as you identify them
};
