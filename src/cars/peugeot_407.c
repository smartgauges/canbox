#include <string.h>
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


#define IGNITION_STATE_MASK       0x1  

#define DASHBOARD_LIGHTNING_MASK 0x20

#define BRIGHTNESS_MASK 0xF

static void peugeot_407_ms_036_ign_light_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.ign = 0;
        carstate.illum = 0;
        carstate.near_lights = 0;
        carstate.acc = 0;
        return;
    }

    carstate.ign = ((msg[4] && IGNITION_STATE_MASK) > 0);
    carstate.acc = carstate.ign;
   
    carstate.near_lights = ((msg[3] && DASHBOARD_LIGHTNING_MASK) > 0);

    carstate.illum = (msg[3] && BRIGHTNESS_MASK);
}


static void peugeot_407_ms_engine_status_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.engine_temp = 0;  // Reset to a default/safe value
        return;
    }

    // Engine Coolant Temperature. Factor: 0.75, Offset: -48
    carstate.engine_temp = (int16_t)((float)msg[5] * 0.75 - 48.0);

	// Engine State (Assuming bits 0-2 of byte 0, like your previous placeholder).
    uint8_t ignition_state = msg[0] & 0x07;
    if (ignition_state == 6) {
        carstate.ign = 1;
        carstate.acc = 1;  // Often, ACC is also on when IGN is on
    } else if (ignition_state == 2) {
        carstate.ign = 0;
        carstate.acc = 1;
    } else {
        carstate.ign = 0;
        carstate.acc = 0;
    }
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

static void peugeot_407_ms_161_handler(const uint8_t *msg, struct msg_desc_t *desc) {
    if (is_timeout(desc)) {
        carstate.oil_temp = 0;   // Or a suitable "invalid" value, maybe -40 to match the offset.
        carstate.fuel_lvl = 0; // Or a suitable "invalid" value.
        return;
    }

    // Oil Temperature: Byte 2, Offset: +40.  Matches PSAVanCanBridge.
    carstate.oil_temp = (int16_t)msg[2] + 40;

    // Fuel Level: Byte 3.  PSAVanCanBridge has a complex calculation involving
    // several bits and a "tank full" value. We'll start by just storing the
    // raw value and refine it later.
    carstate.fuel_lvl = msg[3];

    // The PSAVanCanBridge code suggests these calculations:
    //
     uint8_t fuel_lvl = (msg[3] >> 2) & 0x3F; // Get the 6 relevant bits.
     uint8_t max_fuel    = (msg[3] >> 1) & 0x7F;
     uint8_t fuel_percent = 0; // Or some other default/error value.
     if (max_fuel != 0) { // Avoid division by zero
         fuel_percent =  (uint8_t)(((uint32_t)fuel_lvl * 100) / max_fuel);
     }
    //
    // We will implement this LATER, once we've confirmed we can *receive* the
    // message at all.  First, store the raw value:
    carstate.fuel_lvl = msg[3];  // Store the raw byte for now.

    // The other bytes are, as of now, still unknown, but this structure is a *much*
    // better starting point.
}

static struct msg_desc_t peugeot_407_ms[] =
{
    { 0x36,    100, 0, 0, peugeot_407_ms_036_ign_light_handler }, // Add this line
    { 0x0B6,    100, 0, 0, peugeot_407_ms_engine_status_handler },
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
    { 0x161,    100, 0, 0, peugeot_407_ms_161_handler },
    // Add more message descriptors here as you identify them
};
