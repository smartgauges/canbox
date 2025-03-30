#ifndef PROTOCOL_BMWNBEVO_H
#define PROTOCOL_BMWNBEVO_H

#include "interface.h"

#ifdef __cplusplus
extern "C" {
#endif

void canbox_od_bmw_nbt_evo_radar_process(uint8_t fmax[4], uint8_t rmax[4]);
void canbox_od_bmw_nbt_evo_wheel_process(uint8_t type, int16_t min, int16_t max);
void canbox_od_bmw_nbt_evo_door_process(void);
void canbox_od_bmw_nbt_evo_cmd_process(uint8_t ch);
void canbox_od_bmw_nbt_evo_park_process(uint8_t fmax[4], uint8_t rmax[4]);

extern void canbox_inc_volume(uint8_t val); // Button command prototypes
extern void canbox_dec_volume(uint8_t val);
extern void canbox_prev(void);
extern void canbox_next(void);

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_BMWNBEVO_H
