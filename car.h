#ifndef CAR_H
#define CAR_H

#include <inttypes.h>
#include "conf.h"

#ifdef __cplusplus
extern "C"
{
#endif

enum e_selector_t
{
	e_selector_p = 0,
	e_selector_r,
	e_selector_n,
	e_selector_d,
	e_selector_s,
	e_selector_m,
};

typedef struct key_cb_t
{
	void (*mode)(void);
	void (*inc_volume)(uint8_t val);
	void (*dec_volume)(uint8_t val);
	void (*prev)(void);
	void (*next)(void);
} key_cb_t;

void car_init(enum e_car_t car, struct key_cb_t * cb);
void car_process(uint8_t);

enum e_car_t car_get_car(void);
uint8_t car_get_acc(void);
uint8_t car_get_ign(void);
uint8_t car_get_engine(void);
uint8_t car_get_radar(uint8_t * buf);
uint8_t car_get_wheel(int8_t * wheel);
uint8_t car_get_park_lights(void);
uint8_t car_get_near_lights(void);
uint8_t car_get_illum(void);
uint8_t car_get_vin(uint8_t * buf);
enum e_selector_t car_get_selector(void);
uint16_t car_get_speed(void);
uint16_t car_get_taho(void);
uint8_t car_get_door_fl(void);
uint8_t car_get_door_fr(void);
uint8_t car_get_door_rl(void);
uint8_t car_get_door_rr(void);
uint8_t car_get_bonnet(void);
uint8_t car_get_park_break(void);
uint8_t car_get_washer(void);
uint8_t car_get_dsbelt(void);
uint8_t car_get_tailgate(void);
uint8_t car_get_ac(void);
uint8_t car_get_ac_recirc(void);
uint8_t car_get_ac_auto(void);
uint8_t car_get_ac_dual(void);
uint8_t car_get_ac_fdef(void);
uint8_t car_get_ac_rdef(void);

#ifdef __cplusplus
}
#endif

#endif

