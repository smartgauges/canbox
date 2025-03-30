#include "protocol/raisemqb.h"
#include "canbox.h"
#include "car.h"
#include "conf.h"
#include "utils.h"
#include <string.h>


// ... (Implement Raise MQB specific versions of functions, reusing helpers where possible) ...
// NOTE:  Implement canbox_raise_vw_mqb_radar_process, canbox_raise_vw_mqb_door_process, etc.
//        Adapt implementations from canbox.c, using the `snd_canbox_msg`, `scale`, etc. helpers.
//        Button commands (volume, prev, next, mode, cont, mici) can likely reuse the same implementations as RaisePQ.

void canbox_raise_vw_mqb_radar_process(uint8_t fmax[4], uint8_t rmax[4]) { // Example MQB Radar
    // ... (MQB Radar process implementation - adapted from canbox.c) ...
    uint8_t pmax = (e_selector_r == car_get_selector()) ? 165 : 250; // Example from original canbox.c
    uint8_t pstart = (e_selector_r == car_get_selector()) ? 1 : 5; // Example from original canbox.c
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
        snd_canbox_msg(0x41, data, sizeof(data)); // Assuming snd_canbox_msg is still used for MQB Raise, if not, adapt.
    }
}

void canbox_raise_vw_mqb_door_process(void) { // Example MQB Door
    // ... (MQB Door process implementation - adapted from canbox.c) ...
    uint8_t fl_door = car_get_door_fl();
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

    uint8_t data[] = { 0x00, 0x00, state, 0x00, 0x00, 0x00, 0x00 };
    snd_canbox_msg(0x12, data, sizeof(data)); // Assuming snd_canbox_msg is still used for MQB Raise, if not, adapt.
}

void canbox_raise_vw_mqb_wheel_process(uint8_t type, int16_t min, int16_t max) { // Example MQB Wheel
    if (!get_rear_delay_state()) return;
    int8_t wheel = 0;
    if (!car_get_wheel(&wheel)) return;
    int16_t sangle = scale(wheel, -100, 100, min, max);
    uint8_t wbuf[] = { sangle, sangle >> 8 };
    snd_canbox_msg(type, wbuf, sizeof(wbuf)); // Assuming snd_canbox_msg is still used for MQB Raise, if not, adapt.
}


const protocol_ops_t raise_vw_mqb_protocol_ops = {
    .radar_process = canbox_raise_vw_mqb_radar_process,
    .wheel_process = canbox_raise_vw_mqb_wheel_process,
    .door_process = canbox_raise_vw_mqb_door_process,
    .vehicle_info_process = NULL, // No specific vehicle info process for MQB example in original canbox.c
    .ac_process = NULL,          // No AC process for MQB example in original canbox.c
    .inc_volume = canbox_inc_volume, // Reusing from RaisePQ, likely the same for button commands
    .dec_volume = canbox_dec_volume,
    .prev = canbox_prev,
    .next = canbox_next,
    .mode = canbox_mode,
    .cont = canbox_cont,
    .mici = canbox_mici,
    .cmd_process = canbox_cmd_process,
    .park_process = canbox_raise_vw_mqb_radar_process, // Radar process used as park process for MQB Raise
};
