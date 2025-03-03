#include <string.h>

#include "hw.h"
#include "hw_can.h"
#include "car.h"

#define USE_LR2_2007MY
#define USE_LR2_2013MY
#define USE_XC90_2007MY
#define USE_SKODA_FABIA
#define USE_Q3_2015
#define USE_TOYOTA_PREMIO_26X

static float scale(float value, float in_min, float in_max, float out_min, float out_max)
{
	return (((value - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min;
}

#define STATE_UNDEF 0xff
typedef struct car_state_t
{
	enum e_car_t car;
	uint8_t vin[18];
	uint8_t acc;
	uint8_t ign;
	uint8_t engine;
	uint32_t taho;
	uint32_t speed;
	//in percentages 0% : 100%
	uint8_t illum;
	uint8_t selector;
	struct radar_t radar;
	//in percentages -100% : 100%
	int8_t wheel;

	uint8_t fl_door;
	uint8_t fr_door;
	uint8_t rl_door;
	uint8_t rr_door;
	uint8_t bonnet;
	uint8_t tailgate;

	uint8_t park_lights;
	uint8_t near_lights;
	uint8_t park_break;
	uint8_t low_washer;
	//driver seat belt
	uint8_t ds_belt;

	uint32_t odometer;
	uint32_t voltage;
	uint8_t low_voltage;
	uint32_t temp;
	uint8_t fuel_lvl;
	uint8_t low_fuel_lvl;
} car_state_t;

static car_state_t carstate =
{
	.car = e_car_nums,
	.vin = { STATE_UNDEF },
	.acc = STATE_UNDEF,
	.ign = STATE_UNDEF,
	.engine = STATE_UNDEF,
	.taho = 0,
	.speed = 0,
	.illum = STATE_UNDEF,
	.selector = STATE_UNDEF,
	.radar = { .state = STATE_UNDEF, },
	.wheel = 0,

	.fl_door = STATE_UNDEF,
	.fr_door = STATE_UNDEF,
	.rl_door = STATE_UNDEF,
	.rr_door = STATE_UNDEF,
	.bonnet = STATE_UNDEF,
	.tailgate = STATE_UNDEF,

	.park_lights = STATE_UNDEF,
	.near_lights = STATE_UNDEF,
	.park_break = STATE_UNDEF,
	.low_washer = STATE_UNDEF,
	.ds_belt = STATE_UNDEF,

	.odometer = 0,
	.voltage = 0,
	.low_voltage = STATE_UNDEF,
	.temp = 0,
	.fuel_lvl = 0,
	.low_fuel_lvl = STATE_UNDEF,
};

typedef struct car_air_state_t
{
	uint8_t ac;
	uint8_t ac_max;
	uint8_t recycling;
	uint8_t recycling_max;
	uint8_t recycling_min;
	uint8_t dual;
	uint8_t rear;
	uint8_t rear_lock;
	uint8_t aqs;

	uint8_t wind;
	uint8_t middle;
	uint8_t floor;

	uint8_t powerfull;
	uint8_t fanspeed; // 0 - 7
	uint8_t l_temp;	  // 15 - 32
	uint8_t r_temp;   // 15 - 32

	uint8_t l_seat; // 0 - 3
	uint8_t r_seat; // 0 - 3
} car_air_state_t;

static car_air_state_t car_air_state =
{
	.ac = STATE_UNDEF,
	.ac_max = STATE_UNDEF,
	.recycling = STATE_UNDEF,
	.recycling_max = STATE_UNDEF,
	.recycling_min = STATE_UNDEF,
	.dual = STATE_UNDEF,
	.rear = STATE_UNDEF,
	.rear_lock = STATE_UNDEF,
	.aqs = STATE_UNDEF,
	.wind = STATE_UNDEF,
	.middle = STATE_UNDEF,
	.floor = STATE_UNDEF,
	.powerfull = STATE_UNDEF,
	.fanspeed = STATE_UNDEF,
	.l_temp = STATE_UNDEF,
	.r_temp = STATE_UNDEF,
	.l_seat = STATE_UNDEF,
	.r_seat = STATE_UNDEF,
};

typedef struct key_state_t
{
	uint8_t key_volume;
	uint8_t key_prev;
	uint8_t key_next;
	uint8_t key_mode;
	uint8_t key_cont;
	uint8_t key_navi;
	uint8_t key_mici;

	struct key_cb_t * key_cb;
} key_state_t;

static struct key_state_t key_state =
{
	.key_volume = STATE_UNDEF,
	.key_prev = STATE_UNDEF,
	.key_next = STATE_UNDEF,
	.key_mode = STATE_UNDEF,
	.key_cont = STATE_UNDEF,
	.key_navi = STATE_UNDEF,
	.key_mici = STATE_UNDEF,
	.key_cb = 0,
};

struct msg_desc_t;
typedef struct msg_desc_t
{
	uint32_t id;
	uint16_t period;
	uint16_t tick;
	uint32_t num;
	void (*in_handler)(const uint8_t * msg, struct msg_desc_t * desc);
} msg_desc_t;

uint8_t is_timeout(struct msg_desc_t * desc)
{
	if (desc->tick >= (2 * desc->period)) {

		desc->tick = 2 * desc->period;
		return 1;
	}

	return 0;
}

static void anymsg_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	(void)msg;

	if (is_timeout(desc)) {

		carstate.acc = 0;
		carstate.ign = 0;
		return;
	}

	carstate.acc = 1;
	carstate.ign = 1;
}

struct msg_desc_t anymsg_desc[] =
{
	{ 0x0, 100, 0, 0, anymsg_handler },
};

#ifdef USE_LR2_2007MY
#include "cars/lr2_2007my.c"
#endif

#ifdef USE_LR2_2013MY
#include "cars/lr2_2013my.c"
#endif

#ifdef USE_XC90_2007MY
#include "cars/xc90_2007my.c"
#endif

#ifdef USE_SKODA_FABIA
#include "cars/skoda_fabia.c"
#endif

#ifdef USE_Q3_2015
#include "cars/q3_2015.c"
#endif

#ifdef USE_TOYOTA_PREMIO_26X
#include "cars/toyota_premio_26x.c"
#endif

enum e_car_t car_get_next_car(void)
{
	int car = carstate.car;

	car++;
	if (car >= e_car_nums)
		car = e_car_anymsg;

	return car;
}

static void in_process(struct can_t * can, uint8_t ticks, struct msg_desc_t * msg_desc, uint8_t desc_num)
{
	uint8_t msgs_num = hw_can_get_msg_nums(can);
	uint32_t all_packs = 0;
	for (uint8_t i = 0; i < msgs_num; i++) {

		struct msg_can_t msg;
		if (!hw_can_get_msg(can, &msg, i))
			continue;

		all_packs += msg.num;

		for (uint32_t j = 0; j < desc_num; j++) {

			struct msg_desc_t * desc = &msg_desc[j];

			//special purpose - any activity on the bus
			if (0 == desc->id) {

				if (desc->in_handler) {

					//last msg
					if (i == (msgs_num - 1)) {

						if (all_packs == desc->num)
							desc->tick += ticks;
						else
							desc->tick = 0;

						desc->num = all_packs;

						desc->in_handler(msg.data, desc);
					}
				}
			}
			else if (msg.id == desc->id) {

				if (desc->in_handler) {

					//no new packs, increase timeout
					if (msg.num == desc->num)
						desc->tick += ticks;
					else
						desc->tick = 0;

					desc->num = msg.num;

					desc->in_handler(msg.data, desc);
				}

				break;
			}
		}
	}
}

void car_init(enum e_car_t car, struct key_cb_t * cb)
{
	carstate.car = car;

	carstate.vin[0] = STATE_UNDEF,
	carstate.acc = STATE_UNDEF,
	carstate.ign = STATE_UNDEF,
	carstate.engine = STATE_UNDEF,
	carstate.taho = 0,
	carstate.speed = 0,
	carstate.illum = STATE_UNDEF,
	carstate.selector = STATE_UNDEF,
	carstate.radar.state = STATE_UNDEF,
	carstate.wheel = 0,

	carstate.park_lights = STATE_UNDEF,
	carstate.near_lights = STATE_UNDEF,
	carstate.park_break = STATE_UNDEF,

	key_state.key_cb = cb;

	e_speed_t speed = e_speed_125;
	if ((car == e_car_skoda_fabia) || (car == e_car_q3_2015))
		speed = e_speed_100;
	else if (car == e_car_toyota_premio_26x)
		speed = e_speed_500;
	hw_can_set_speed(hw_can_get_mscan(), speed);
}

enum e_car_t car_get_car(void)
{
	return carstate.car;
}

void car_process(uint8_t ticks)
{
	struct can_t * can = hw_can_get_mscan();

	switch (carstate.car) {

		case e_car_anymsg:
			in_process(can, ticks, anymsg_desc, sizeof(anymsg_desc)/sizeof(anymsg_desc[0]));
			break;
		case e_car_lr2_2007my:
#ifdef USE_LR2_2007MY
			in_process(can, ticks, lr2_2007my_ms, sizeof(lr2_2007my_ms)/sizeof(lr2_2007my_ms[0]));
#endif
			break;
		case e_car_lr2_2013my:
#ifdef USE_LR2_2013MY
			in_process(can, ticks, lr2_2013my_ms, sizeof(lr2_2013my_ms)/sizeof(lr2_2013my_ms[0]));
#endif
			break;
		case e_car_xc90_2007my:
#ifdef USE_XC90_2007MY
			in_process(can, ticks, xc90_2007my_ms, sizeof(xc90_2007my_ms)/sizeof(xc90_2007my_ms[0]));
#endif
			break;
		case e_car_skoda_fabia:
#ifdef USE_SKODA_FABIA
			in_process(can, ticks, skoda_fabia_ms, sizeof(skoda_fabia_ms)/sizeof(skoda_fabia_ms[0]));
#endif
			break;
		case e_car_q3_2015:
#ifdef USE_Q3_2015
			in_process(can, ticks, q3_2015_ms, sizeof(q3_2015_ms)/sizeof(q3_2015_ms[0]));
			break;
#endif
		case e_car_toyota_premio_26x:
#ifdef USE_TOYOTA_PREMIO_26X
			in_process(can, ticks, toyota_premio_26x_ms, sizeof(toyota_premio_26x_ms)/sizeof(toyota_premio_26x_ms[0]));
#endif
			break;
		default:
			break;
	}
}

uint8_t car_get_acc(void)
{
	if (carstate.acc == STATE_UNDEF)
		return 0;

	return carstate.acc;
}

uint8_t car_get_ign(void)
{
	if (carstate.ign == STATE_UNDEF)
		return 0;

	return carstate.ign;
}

uint8_t car_get_engine(void)
{
	if (carstate.engine == STATE_UNDEF)
		return 0;

	return carstate.engine;
}

uint8_t car_get_illum(void)
{
	if (carstate.illum == STATE_UNDEF)
		return 0;

	return carstate.illum;
}

uint8_t car_get_park_lights(void)
{
	if (carstate.park_lights == STATE_UNDEF)
		return 0;

	return carstate.park_lights;
}

uint8_t car_get_near_lights(void)
{
	if (carstate.near_lights == STATE_UNDEF)
		return 0;

	return carstate.near_lights;
}

void car_get_radar(struct radar_t * r)
{
	memcpy(r, &carstate.radar, sizeof(radar_t));
}

uint8_t car_get_wheel(int8_t * wheel)
{
	*wheel = carstate.wheel;

	return 8;
}

uint8_t car_get_vin(uint8_t * buf)
{
	memset(buf, 0x0, 18);
	if (carstate.vin[0] == STATE_UNDEF) {

		buf[0] = 'n';
		buf[1] = 'a';
		return 2;
	}

	memcpy(buf, carstate.vin, 17);
	return 17;
}

uint32_t car_get_odometer(void)
{
	return carstate.odometer;
}

enum e_selector_t car_get_selector(void)
{
	return carstate.selector;
}

uint8_t car_get_door_fl(void)
{
	if (carstate.fl_door == STATE_UNDEF)
		return 0;

	return carstate.fl_door;
}

uint8_t car_get_door_fr(void)
{
	if (carstate.fr_door == STATE_UNDEF)
		return 0;

	return carstate.fr_door;
}

uint8_t car_get_door_rl(void)
{
	if (carstate.rl_door == STATE_UNDEF)
		return 0;

	return carstate.rl_door;
}

uint8_t car_get_door_rr(void)
{
	if (carstate.rr_door == STATE_UNDEF)
		return 0;

	return carstate.rr_door;
}

uint8_t car_get_bonnet(void)
{
	if (carstate.bonnet == STATE_UNDEF)
		return 0;

	return carstate.bonnet;
}

uint8_t car_get_tailgate(void)
{
	if (carstate.tailgate == STATE_UNDEF)
		return 0;

	return carstate.tailgate;
}

uint8_t car_get_park_break(void)
{
	if (carstate.park_break == STATE_UNDEF)
		return 0;

	return carstate.park_break;
}

uint8_t car_get_low_washer(void)
{
	if (carstate.low_washer == STATE_UNDEF)
		return 0;

	return carstate.low_washer;
}

uint8_t car_get_ds_belt(void)
{
	if (carstate.ds_belt == STATE_UNDEF)
		return 0;

	return carstate.ds_belt;
}

uint16_t car_get_taho(void)
{
	return carstate.taho;
}

uint16_t car_get_speed(void)
{
	return carstate.speed;
}

uint32_t car_get_voltage(void)
{
	return carstate.voltage;
}

uint32_t car_get_temp(void)
{
	return carstate.temp;
}

uint8_t car_get_fuel_level(void)
{
	return carstate.fuel_lvl;
}

uint8_t car_get_low_voltage(void)
{
	if (carstate.low_voltage == STATE_UNDEF)
		return 0;

	return carstate.low_voltage;
}

uint8_t car_get_low_fuel_level(void)
{
	if (carstate.low_fuel_lvl == STATE_UNDEF)
		return 0;

	return carstate.low_fuel_lvl;
}

uint8_t car_get_air_ac(void)
{
	if (car_air_state.ac == STATE_UNDEF)
		return 0;

	return car_air_state.ac;
}

uint8_t car_get_air_ac_max(void)
{
	if (car_air_state.ac_max == STATE_UNDEF)
		return 0;

	return car_air_state.ac_max;
}

uint8_t car_get_air_recycling(void)
{
	if (car_air_state.recycling == STATE_UNDEF)
		return 0;

	return car_air_state.recycling;
}

uint8_t car_get_air_recycling_max(void)
{
	if (car_air_state.recycling_max == STATE_UNDEF)
		return 0;

	return car_air_state.recycling_max;
}

uint8_t car_get_air_recycling_min(void)
{
	if (car_air_state.recycling_min == STATE_UNDEF)
		return 0;

	return car_air_state.recycling_min;
}

uint8_t car_get_air_dual(void)
{
	if (car_air_state.dual == STATE_UNDEF)
		return 0;

	return car_air_state.dual;
}

uint8_t car_get_air_rear(void)
{
	if (car_air_state.rear == STATE_UNDEF)
		return 0;

	return car_air_state.rear;
}

uint8_t car_get_air_rear_lock(void)
{
	if (car_air_state.rear_lock == STATE_UNDEF)
		return 0;

	return car_air_state.rear_lock;
}

uint8_t car_get_air_aqs(void)
{
	if (car_air_state.aqs == STATE_UNDEF)
		return 0;

	return car_air_state.aqs;
}

uint8_t car_get_air_wind(void)
{
	if (car_air_state.wind == STATE_UNDEF)
		return 0;

	return car_air_state.wind;
}

uint8_t car_get_air_middle(void)
{
	if (car_air_state.middle == STATE_UNDEF)
		return 0;

	return car_air_state.middle;
}

uint8_t car_get_air_floor(void)
{
	if (car_air_state.floor == STATE_UNDEF)
		return 0;

	return car_air_state.floor;
}

uint8_t car_get_air_powerfull(void)
{
	if (car_air_state.powerfull == STATE_UNDEF)
		return 0;

	return car_air_state.powerfull;
}

uint8_t car_get_air_fanspeed(void)
{
	if (car_air_state.fanspeed == STATE_UNDEF)
		return 0;

	return car_air_state.fanspeed;
}

uint8_t car_get_air_l_temp(void)
{
	if (car_air_state.l_temp == STATE_UNDEF)
		return 0;

	return car_air_state.l_temp;
}

uint8_t car_get_air_r_temp(void)
{
	if (car_air_state.r_temp == STATE_UNDEF)
		return 0;

	return car_air_state.r_temp;
}

uint8_t car_get_air_l_seat(void)
{
	if (car_air_state.l_seat == STATE_UNDEF)
		return 0;

	return car_air_state.l_seat;
}

uint8_t car_get_air_r_seat(void)
{
	if (car_air_state.r_seat == STATE_UNDEF)
		return 0;

	return car_air_state.r_seat;
}

