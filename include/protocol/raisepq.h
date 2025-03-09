#ifndef PROTOCOL_RAISEPQ_H
#define PROTOCOL_RAISEPQ_H

#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

void canbox_raise_vw_radar_process(uint8_t fmax[4], uint8_t rmax[4]);
void canbox_raise_vw_wheel_process(uint8_t type, int16_t min, int16_t max);
void canbox_raise_vw_door_process(void);
void canbox_raise_vw_vehicle_info(void);
void canbox_raise_vw_ac_process(void);
void canbox_raise_vw_cmd_process(uint8_t ch);
void canbox_raisepq_park_process(uint8_t fmax[4], uint8_t rmax[4]);

extern void canbox_inc_volume(uint8_t val); // Button command prototypes
extern void canbox_dec_volume(uint8_t val);
extern void canbox_prev(void);

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_RAISEPQ_H
