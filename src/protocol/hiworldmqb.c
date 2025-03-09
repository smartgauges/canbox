#include "protocol/hiworldmqb.h"
#include "canbox.h" // For snd_canbox_hiworld_msg, canbox_hiworld_checksum, scale, radar_t, car_get_radar, get_rear_delay_state
#include "car.h"   // For car_get_wheel, car_get_radar, car_get_selector, car_get_door_fl, car_get_door_fr, car_get_door_rl, car_get_door_rr, car_get_tailgate, car_get_bonnet
#include "conf.h"  // For conf_get_canbox
#include "utils.h"

#include <string.h> // For memcpy

// --- Helper Functions (Assume these are in canbox.c or a shared utility file) ---
// In this example, we assume `scale`, `canbox_hiworld_checksum`, and `snd_canbox_hiworld_msg` are defined elsewhere and accessible.
// static float scale(...) { ... }  // Assuming scale is defined elsewhere
// static uint8_t canbox_hiworld_checksum(...) { ... } // Assuming canbox_hiworld_checksum is defined elsewhere
// static void snd_canbox_hiworld_msg(...) { ... }    // Assuming snd_canbox_hiworld_msg is defined elsewhere
// --- End Helper Functions ---


// --- Protocol Implementations for HiWorld VW (MQB) ---

void canbox_hiworld_vw_mqb_radar_process(uint8_t fmax[4], uint8_t rmax[4]) { // Radar process for HiWorld VW MQB
    uint8_t pmax = (e_selector_r == car_get_selector()) ? 165 : 250;
    uint8_t pstart = (e_selector_r == car_get_selector()) ? 1 : 5;

    struct radar_t radar;
    car_get_radar(&radar);
    if (radar.state == e_radar_undef) return;

    uint8_t park_is_on = (e_radar_on == radar.state) ? 1 : 0;

    if (park_is_on) {
        uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

        data[0] = pmax + pstart - scale(radar.rl, 0, 99, 0, pmax);
        data[1] = pmax + pstart - scale(radar.rlm, 0, 99, 0, pmax);
        data[2] = pmax + pstart - scale(radar.rrm, 0, 99, 0, pmax);
        data[3] = pmax + pstart - scale(radar.rr, 0, 99, 0, pmax);

        data[4] = pmax + pstart - scale(radar.fr, 0, 99, 0, pmax);
        data[5] = pmax + pstart - scale(radar.frm, 0, 99, 0, pmax);
        data[6] = pmax + pstart - scale(radar.flm, 0, 99, 0, pmax);
        data[7] = pmax + pstart - scale(radar.fl, 0, 99, 0, pmax);

        snd_canbox_hiworld_msg(0x41, data, sizeof(data));
    }
}

void canbox_hiworld_vw_mqb_wheel_process(uint8_t type, int16_t min, int16_t max) { // Wheel process for HiWorld VW MQB
    if (!get_rear_delay_state()) return; // Assuming get_rear_delay_state is available

    int8_t wheel = 0;
    // No wheel angle data is sent in HiWorld MQB protocol according to the original canbox.c, so we don't use wheel here.
    // The wheel angle data in HiWorld MQB is likely sent in the radar message (canbox_hiworld_vw_mqb_radar_process).
    (void) type; (void) min; (void) max; // Suppress unused parameter warnings

    // int16_t sangle = scale(wheel, -100, 100, min, max); // Not used in HiWorld MQB wheel process
    // uint8_t wbuf[] = { sangle, sangle >> 8 };
    // snd_canbox_hiworld_msg(type, wbuf, sizeof(wbuf)); // Not sending wheel angle separately for HiWorld MQB
}

void canbox_hiworld_vw_mqb_door_process(void) { // Door process for HiWorld VW MQB
    uint8_t fl_door = car_get_door_fl();
    uint8_t fr_door = car_get_door_fr();
    uint8_t rl_door = car_get_door_rl();
    uint8_t rr_door = car_get_door_rr();
    uint8_t tailgate = car_get_tailgate();
    uint8_t bonnet = car_get_bonnet();

    uint8_t state = 0;

    if (bonnet) state |= 0x4;
    if (tailgate) state |= 0x8;
    if (rr_door) state |= 0x10;
    if (rl_door) state |= 0x20;
    if (fr_door) state |= 0x40;
    if (fl_door) state |= 0x80;

    uint8_t data[] = { 0x00, 0x00, state, 0x00, 0x00, 0x00, 0x00 };
    snd_canbox_hiworld_msg(0x12, data, sizeof(data));
}


// Reuse button commands from RaiseVW (PQ) protocol as they are likely similar
extern void canbox_inc_volume(uint8_t val); // Assuming these are declared in canbox.h or a common header
extern void canbox_dec_volume(uint8_t val);
extern void canbox_prev(void);
extern void canbox_next(void);
extern void canbox_mode(void);
extern void canbox_cont(void);
extern void canbox_mici(void);


void canbox_hiworld_vw_mqb_cmd_process(uint8_t ch) { // Command process for HiWorld VW MQB (if needed)
    (void)ch; // Placeholder - adapt if HiWorld MQB protocol has specific commands
}

void canbox_hiworld_vw_mqb_park_process(uint8_t fmax[4], uint8_t rmax[4]) { // Park process for HiWorld VW MQB
    canbox_hiworld_vw_mqb_radar_process(fmax, rmax); // Reuse radar process as park process
}


const protocol_ops_t hiworld_vw_mqb_protocol_ops = {
    .radar_process = canbox_hiworld_vw_mqb_radar_process,
    .wheel_process = canbox_hiworld_vw_mqb_wheel_process,
    .door_process = canbox_hiworld_vw_mqb_door_process,
    .vehicle_info_process = NULL, // No specific vehicle info process implemented in original canbox.c for HiWorld MQB example
    .ac_process = NULL,          // No AC process implemented in original canbox.c for HiWorld MQB example
    .inc_volume = canbox_inc_volume, // Reuse button commands from RaiseVW
    .dec_volume = canbox_dec_volume,
    .prev = canbox_prev,
    .next = canbox_next,
    .mode = canbox_mode,
    .cont = canbox_cont,
    .mici = canbox_mici,
    .cmd_process = canbox_hiworld_vw_mqb_cmd_process,
    .park_process = canbox_hiworld_vw_mqb_park_process,
};
