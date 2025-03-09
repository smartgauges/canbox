// IGNITION ON / ACC
// VIN number
// Odometer
// Voltage
// Doors
// Gear selector
// Keys pressed
// Illum
// Wheel
// Taho
// Break status
// Parking and parking sensors
// AC
// Seat heating
// TPMS
// RPM
// OIL temperature
// FUEL level
// Coolant temperature
// Seat belts
// RD4 radio status
// RD4 input selected
// RD4 CD playback status
// RD4 CD Charger status

#include <string.h>
#include "car.h"
#include "hw_can.h"
#include "utils.h"

// Placeholder implementation for Peugeot 407 car module

static void peugeot_407_ms_0b6_engine_status_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.engine = STATE_UNDEF;
        carstate.temp = 0; // Coolant Temperature
        return;
    }

    carstate.temp = msg[5]; // Placeholder: Byte 5 as coolant temperature (needs scaling/offset)

    // Example Ignition State (Placeholder - needs verification)
    // Assuming bits 0-2 of byte 0 indicate ignition state
    uint8_t ignition_state = msg[0] & 0x07;
    if (ignition_state == 6) { // Example: Value 6 represents "ON"
        carstate.ign = 1;
    } else if (ignition_state == 2) { // Example: Value 2 represents "ACC"
        carstate.ign = 0; // Or set to ACC state if you define one
        carstate.acc = 1;
    } else {
        carstate.ign = 0;
        carstate.acc = 0;
    }
}

static void peugeot_407_ms_14c_speed_odometer_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.speed = 0;
        carstate.odometer = 0;
        return;
    }

    carstate.speed = msg[0]; // Placeholder: Byte 0 as speed (needs scaling)

    // Example Odometer (Placeholder - needs verification and proper byte order)
    carstate.odometer = (uint32_t)msg[1] << 16 | (uint32_t)msg[2] << 8 | (uint32_t)msg[3];
}

static void peugeot_407_ms_131_doors_fuel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.fuel_lvl = 0;
        carstate.fl_door = STATE_UNDEF;
        carstate.fr_door = STATE_UNDEF;
        carstate.rl_door = STATE_UNDEF;
        carstate.rr_door = STATE_UNDEF;
        return;
    }

    carstate.fuel_lvl = msg[0]; // Placeholder: Byte 0 as fuel level (needs scaling)

    // Example Door Status (Placeholder - needs verification)
    carstate.fl_door = (msg[1] & 0x01) ? 1 : 0; // Example: Bit 0 of Byte 1 for FL door
    carstate.fr_door = (msg[1] & 0x02) ? 1 : 0; // Example: Bit 1 of Byte 1 for FR door
    carstate.rl_door = (msg[1] & 0x04) ? 1 : 0; // Example: Bit 2 of Byte 1 for RL door
    carstate.rr_door = (msg[1] & 0x08) ? 1 : 0; // Example: Bit 3 of Byte 1 for RR door
}

static void peugeot_407_ms_168_temp_battery_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {
        carstate.temp = 0; // Outside Temperature
        carstate.voltage = 0; // Battery Voltage
        return;
    }
    carstate.temp = (int32_t)msg[0] - 40; // Placeholder: Byte 0 as outside temperature with offset
    carstate.voltage = msg[1]; // Placeholder: Byte 1 as battery voltage (needs scaling)
}


static struct msg_desc_t peugeot_407_ms[] =
{
    { 0x0B6,    100, 0, 0, peugeot_407_ms_0b6_engine_status_handler },
    { 0x14C,    100, 0, 0, peugeot_407_ms_14c_speed_odometer_handler },
    { 0x131,    100, 0, 0, peugeot_407_ms_131_doors_fuel_handler },
    { 0x168,   1000, 0, 0, peugeot_407_ms_168_temp_battery_handler },
    // Add more message descriptors here as you identify them
};