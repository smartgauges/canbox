// src/protocol_bmwnbtevo.c
#include "protocol/bmwnbtevo.h"
#include "canbox.h" // For snd_canbox_msg, canbox_checksum, scale, radar_t, car_get_radar, get_rear_delay_state
#include "car.h"   // For car_get_wheel, car_get_radar
#include "conf.h"  // For conf_get_canbox
#include "utils.h"

#include <string.h> // For memcpy

// --- Helper Functions (Assume these are in canbox.c or a shared utility file) ---
// In this example, we assume `scale`, `canbox_checksum`, and `snd_canbox_msg` are defined elsewhere and accessible.
// If not, you'd need to implement them here or include the file where they are defined.
// static float scale(...) { ... }  // Assuming scale is defined elsewhere
// static uint8_t canbox_checksum(...) { ... } // Assuming canbox_checksum is defined elsewhere
// static void snd_canbox_msg(...) { ... }    // Assuming snd_canbox_msg is defined elsewhere
// --- End Helper Functions ---


// --- Protocol Implementations for Audi BMW (NBT Evo) ---

void canbox_od_bmw_nbt_evo_radar_process(uint8_t fmax[4], uint8_t rmax[4]) { // Radar process for BMW NBT Evo
    struct radar_t radar;
    car_get_radar(&radar);
    if (radar.state == e_radar_undef) return;

    uint8_t _park_is_on = (e_radar_on == radar.state) ? 1 : 0;
    static uint8_t park_is_on = 0;

    if (park_is_on != _park_is_on || park_is_on) {
        park_is_on = _park_is_on;
        uint8_t b[] = { 0x0, park_is_on ? 0x08 : 0x00 };
        snd_canbox_msg(0x24, b, sizeof(b));
    }

    if (!park_is_on) return;

    uint8_t fbuf[] = { 0x00, 0x00, 0x00, 0x00 };
    fbuf[0] = fmax[0] + 1 - scale(radar.fr, 0, 99, 0, fmax[0]);
    fbuf[1] = fmax[1] + 1 - scale(radar.frm, 0, 99, 0, fmax[1]);
    fbuf[2] = fmax[2] + 1 - scale(radar.flm, 0, 99, 0, fmax[2]);
    fbuf[3] = fmax[3] + 1 - scale(radar.fl, 0, 99, 0, fmax[3]);
    snd_canbox_msg(0x23, fbuf, sizeof(fbuf));

    uint8_t rbuf[] = { 0x00, 0x00, 0x00, 0x00 };
    rbuf[0] = rmax[0] + 1 - scale(radar.rl, 0, 99, 0, rmax[0]);
    rbuf[1] = rmax[1] + 1 - scale(radar.rlm, 0, 99, 0, rmax[1]);
    rbuf[2] = rmax[2] + 1 - scale(radar.rrm, 0, 99, 0, rmax[2]);
    rbuf[3] = rmax[3] + 1 - scale(radar.rr, 0, 99, 0, rmax[3]);
    snd_canbox_msg(0x22, rbuf, sizeof(rbuf));
}

void canbox_od_bmw_nbt_evo_wheel_process(uint8_t type, int16_t min, int16_t max) { // Wheel process for BMW NBT Evo
    if (!get_rear_delay_state()) return; // Assuming get_rear_delay_state is available

    int8_t wheel = 0;
    if (!car_get_wheel(&wheel)) return;

    int16_t sangle = scale(wheel, -100, 100, min, max);
    uint8_t wbuf[] = { sangle, sangle >> 8 };
    snd_canbox_msg(type, wbuf, sizeof(wbuf));
}

void canbox_od_bmw_nbt_evo_door_process(void) { // Door process for BMW NBT Evo
    // Door status for BMW NBT Evo might be different, adapt as needed based on your reverse engineering
    // For now, let's assume it's similar to RaiseVW MQB door process for demonstration
    uint8_t fl_door = car_get_door_fl(); // Assuming car_get_door_* functions are still used
    uint8_t fr_door = car_get_door_fr();
    uint8_t rl_door = car_get_door_rl();
    uint8_t rr_door = car_get_door_rr();
    uint8_t tailgate = car_get_tailgate();
    uint8_t bonnet = car_get_bonnet();

    uint8_t state = 0;

    if (bonnet) state |= 0x4;
    if (tailgate) state |= 0x8;
    if (rl_door) state |= 0x10;
    if (rr_door) state |= 0x20;
    if (fr_door) state |= 0x40;
    if (fl_door) state |= 0x80;

    snd_canbox_msg(0x24, &state, 1); // Message type might be different for BMW NBT Evo
}


// Reuse button commands from RaiseVW (PQ) protocol as they might be similar
// (or implement BMW NBT Evo specific button commands if needed)
extern void canbox_inc_volume(uint8_t val); // Assuming these are declared in canbox.h or a common header
extern void canbox_dec_volume(uint8_t val);
extern void canbox_prev(void);
extern void canbox_next(void);
extern void canbox_mode(void);
extern void canbox_cont(void);
extern void canbox_mici(void);


void canbox_od_bmw_nbt_evo_cmd_process(uint8_t ch) { // Command process for BMW NBT Evo (if needed)
    (void)ch; // Placeholder - adapt if BMW NBT Evo protocol has specific commands
}

void canbox_od_bmw_nbt_evo_park_process(uint8_t fmax[4], uint8_t rmax[4]) { // Park process for BMW NBT Evo
    canbox_od_bmw_nbt_evo_radar_process(fmax, rmax); // Reuse radar process as park process for BMW NBT Evo
}


const protocol_ops_t od_bmw_nbt_evo_protocol_ops = {
    .radar_process = canbox_od_bmw_nbt_evo_radar_process,
    .wheel_process = canbox_od_bmw_nbt_evo_wheel_process,
    .door_process = canbox_od_bmw_nbt_evo_door_process,
    .vehicle_info_process = NULL, // No specific vehicle info process implemented in original canbox.c for BMW NBT Evo example
    .ac_process = NULL,          // No AC process implemented in original canbox.c for BMW NBT Evo example
    .inc_volume = canbox_inc_volume, // Reuse button commands from RaiseVW
    .dec_volume = canbox_dec_volume,
    .prev = canbox_prev,
    .next = canbox_next,
    .mode = canbox_mode,
    .cont = canbox_cont,
    .mici = canbox_mici,
    .cmd_process = canbox_od_bmw_nbt_evo_cmd_process,
    .park_process = canbox_od_bmw_nbt_evo_park_process,
};
