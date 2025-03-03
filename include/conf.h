#ifndef CONF_H
#define CONF_H

#ifdef __cplusplus
extern "C"
{
#endif

enum e_car_t
{
	e_car_anymsg = 0,
	e_car_lr2_2007my,
	e_car_lr2_2013my,
	e_car_xc90_2007my,
	e_car_skoda_fabia,
	e_car_q3_2015,
	e_car_toyota_premio_26x,
	e_car_nums,
};

enum e_canbox_t
{
	e_cb_raise_vw_pq = 0,
	e_cb_raise_vw_mqb,
	e_cb_od_bmw_nbt_evo,
	e_cb_hiworld_vw_mqb,
	e_cb_nums,
};

#define MAX_REAR_DELAY 8000

void conf_read(void);
void conf_write(void);

enum e_car_t conf_get_car(void);
void conf_set_car(enum e_car_t car);
uint8_t conf_get_illum(void);
void conf_set_illum(uint8_t illum);
uint16_t conf_get_rear_delay(void);
void conf_set_rear_delay(uint16_t rear_delay);
enum e_canbox_t conf_get_canbox(void);
void conf_set_canbox(enum e_canbox_t cb);

uint16_t conf_get_idx(void);

#ifdef __cplusplus
}
#endif

#endif

