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
	e_selector_m_p,
	e_selector_m_m,
};

enum e_radar_t
{
	e_radar_undef = 0,
	e_radar_off,
	e_radar_on,
	e_radar_on_front,
	e_radar_on_rear,
};

typedef struct radar_t
{
	uint8_t state;
	//front left
	uint8_t fl;
	//front left middle
	uint8_t flm;
	//front right middle
	uint8_t frm;
	//fron right
	uint8_t fr;
	//rear left
	uint8_t rl;
	//rear left middle
	uint8_t rlm;
	//rear right middle
	uint8_t rrm;
	//rear right
	uint8_t rr;
} radar_t;

typedef struct key_cb_t
{
	void (*mode)(void);
	void (*inc_volume)(uint8_t val);
	void (*dec_volume)(uint8_t val);
	void (*prev)(void);
	void (*next)(void);
	void (*navi)(void);
	void (*cont)(void);
	void (*mici)(void);
} key_cb_t;

void car_init(enum e_car_t car, struct key_cb_t * cb);
void car_process(uint8_t);

enum e_car_t car_get_car(void);
enum e_car_t car_get_next_car(void);

uint8_t car_get_acc(void);
uint8_t car_get_ign(void);
uint8_t car_get_engine(void);
void car_get_radar(struct radar_t * radar);
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
uint8_t car_get_tailgate(void);

uint8_t car_get_park_break(void);
uint8_t car_get_low_washer(void);
uint8_t car_get_ds_belt(void);

uint32_t car_get_odometer(void);
uint32_t car_get_voltage(void);
uint32_t car_get_temp(void);
uint8_t car_get_fuel_level(void);
uint8_t car_get_low_voltage(void);
uint8_t car_get_low_fuel_level(void);

uint8_t car_get_air_ac(void);
uint8_t car_get_air_ac_max(void);
uint8_t car_get_air_recycling(void);
uint8_t car_get_air_recycling_max(void);
uint8_t car_get_air_recycling_min(void);
uint8_t car_get_air_dual(void);
uint8_t car_get_air_rear(void);
uint8_t car_get_air_rear_lock(void);
uint8_t car_get_air_aqs(void);
uint8_t car_get_air_wind(void);
uint8_t car_get_air_middle(void);
uint8_t car_get_air_floor(void);
uint8_t car_get_air_powerfull(void);
uint8_t car_get_air_fanspeed(void);
uint8_t car_get_air_l_temp(void);
uint8_t car_get_air_r_temp(void);
uint8_t car_get_air_l_seat(void);
uint8_t car_get_air_r_seat(void);

#ifdef __cplusplus
}
#endif

#endif

