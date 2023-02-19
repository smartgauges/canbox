#include <string.h>

#include "hw.h"
#include "hw_can.h"
#include "car.h"

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

static void lr2_2007my_ms_6c_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.selector = STATE_UNDEF;
		return;
	}

	switch (msg[7] & 0x7) {

		case 0:
			carstate.selector = e_selector_p;
			break;
		case 1:
			carstate.selector = e_selector_r;
			break;
		case 2:
			carstate.selector = e_selector_n;
			break;
		case 3:
			carstate.selector = e_selector_d;
			break;
	}
}

static void lr2_2007my_ms_7e_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		//hdlc_put_debug("7e timeout");

		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;
		carstate.taho = 0;

		return;
	}

	//hdlc_put_debug("7e rcv");

	/**
	 * 0x003b8x key insert
	 * 0x003bax start
	 * 0x003b9x run
	 */

	if (msg[2] & 0x80)
		carstate.acc = 1;
	else
		carstate.acc = 0;

	if ((msg[2] & 0xa0) == 0xa0)
		carstate.ign = 1;
	else
		carstate.ign = 0;

	if ((msg[2] & 0x98) == 0x98) {

		carstate.engine = 1;
		carstate.ign = 1;
	}
	else
		carstate.engine = 0;

	if (msg[6] & 0x80)
		carstate.taho = (((uint16_t)msg[3]) << 8 | msg[4]) & 0xfff;
	else
		carstate.taho = 0;
}

static void lr2_2007my_ms_fd_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.speed = 0;
		return;
	}

	carstate.speed = (((uint16_t)msg[6]) << 8 | msg[7]) & 0xffff;
	carstate.speed /= 100;
}

static void lr2_2007my_ms_110_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.park_lights = STATE_UNDEF;
		carstate.park_break = STATE_UNDEF;
		return;
	}

	carstate.park_break = (msg[1] & 0x08) ? 1 : 0;
	carstate.park_lights = (msg[0] & 0x20) ? 0 : 1;
}

static void lr2_2007my_ms_188_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.radar.state = e_radar_off;
		return;
	}

	/**
	 * 0x70 both on, 0x60 - rear on
	 * 1.f8 - state, 1.07 - front/rear:3 - rear, 5 - front, 1 - both?
	 * 2.xx - rear
	 * 3.xx
	 * 4.xx
	 * 5.xx
	 * 6.xx
	 * 7.xx - front
	 */
	uint32_t f = ((uint32_t)msg[5] << 16) | ((uint32_t)msg[6] << 8) | msg[7];
	uint8_t f0 = (f >> 15) & 0x1f;
	uint8_t f1 = (f >> 10) & 0x1f;
	uint8_t f2 = (f >> 5) & 0x1f;
	uint8_t f3 = f & 0x1f;

	carstate.radar.state = ((0x70 == (msg[0] & 0xf0)) || (0x60 == (msg[0] & 0xf0))) ? e_radar_on : e_radar_off;

	carstate.radar.fl = scale(f0, 0, 0x0f, 0, 99);
	carstate.radar.flm = scale(f1, 0, 0x0f, 0, 99);
	carstate.radar.frm = scale(f2, 0, 0x0f, 0, 99);
	carstate.radar.fr = scale(f3, 0, 0x0f, 0, 99);

	uint32_t r = ((uint32_t)msg[2] << 16) | ((uint32_t)msg[3] << 8) | msg[4];
	uint8_t r0 = (r >> 15) & 0x1f;
	uint8_t r1 = (r >> 10) & 0x1f;
	uint8_t r2 = (r >> 5) & 0x1f;
	uint8_t r3 = r & 0x1f;

	carstate.radar.rl = scale(r0, 0, 0x0f, 0, 99);
	carstate.radar.rlm = scale(r1, 0, 0x0f, 0, 99);
	carstate.radar.rrm = scale(r2, 0, 0x0f, 0, 99);
	carstate.radar.rr = scale(r3, 0, 0x0f, 0, 99);
}

static void lr2_2007my_ms_2a0_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.illum = STATE_UNDEF;
		return;
	}

	carstate.illum = scale(msg[1] & 0x0f, 0, 0x0f, 0, 100);
}

static void lr2_2007my_ms_405_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.vin[0] = STATE_UNDEF;
		return;
	}

	if (msg[0] == 0x10) {

		memcpy(carstate.vin, msg + 5, 3);
	}
	else if (msg[0] == 0x11) {

		memcpy(carstate.vin + 3, msg + 1, 7);
	}
	else if (msg[0] == 0x12) {

		memcpy(carstate.vin + 10, msg + 1, 7);
	}

	//SALFA2BB7AH151382
	//SAL
	//405#100000000053414C
	//FA2BB7A
	//405#1146413242423741
	//H151382
	//405#1248313531333832
}

static struct msg_desc_t lr2_2007my_ms[] =
{
	{ 0x06c, 30, 0, 0, lr2_2007my_ms_6c_handler },
	{ 0x07e, 50, 0, 0, lr2_2007my_ms_7e_handler },
	{ 0x0fd, 50, 0, 0, lr2_2007my_ms_fd_handler },
	{ 0x110, 60, 0, 0, lr2_2007my_ms_110_handler },
	{ 0x188, 70, 0, 0, lr2_2007my_ms_188_handler },
	{ 0x2a0, 115, 0, 0, lr2_2007my_ms_2a0_handler },
	{ 0x405, 500, 0, 0, lr2_2007my_ms_405_handler },
};

static void lr2_2013my_ms_10_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.wheel = 0;
		return;
	}

#if 0
	//0 - 30 - 59
	uint8_t sangle = scale(carstate.state.wheel, -127, 127, 0, 59);

	if (carstate.state.wheel >= 0)
		msg[6] = 29 - sangle;
	else
		msg[6] = 29 + sangle;
#endif


	//lr2 wheelbase = 2660, radius 5700, width 1600
	//atan(wheelbase / (radius - width)) = angle
	//max 0x2900

	//0 - 30 - 59
	uint8_t angle = msg[6];
	carstate.wheel = scale(angle, 0, 59, -100, 100);
}

static void lr2_2013my_ms_b8_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;
		carstate.engine = STATE_UNDEF;
		carstate.illum = STATE_UNDEF;

		return;
	}

	carstate.acc = ((msg[0] & 0x80) && (msg[1] & 0x04)) ? 1 : 0;
	carstate.ign = (msg[1] & 0x02) ? 1 : 0;
	carstate.engine = ((msg[0] & 0x80) && (msg[1] & 0xe7)) ? 1 : 0;
	carstate.illum = scale(msg[3], 0, 0xff, 0, 100);
}

static void lr2_2013my_ms_4a6_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.radar.state = e_radar_off;
		return;
	}

	/**
	 * 0x04 on
	 * 1.f8 - state, 1.07 - front/rear:1 - rear, 2 - front
	 * 2.xx - front
	 * 3.xx
	 * 4.xx
	 * 5.xx
	 * 6.xx
	 * 7.xx - rear
	 */
	uint32_t f = ((uint32_t)msg[2] << 16) | ((uint32_t)msg[3] << 8) | msg[4];
	uint8_t f0 = (f >> 15) & 0x1f;
	uint8_t f1 = (f >> 10) & 0x1f;
	uint8_t f2 = (f >> 5) & 0x1f;
	uint8_t f3 = f & 0x1f;

	carstate.radar.state = (0x04 == (msg[0] & 0x0f)) ? e_radar_on : e_radar_off;

	carstate.radar.fl = scale(f0, 0, 0x0f, 0, 99);
	carstate.radar.flm = scale(f1, 0, 0x0f, 0, 99);
	carstate.radar.frm = scale(f2, 0, 0x0f, 0, 99);
	carstate.radar.fr = scale(f3, 0, 0x0f, 0, 99);

	uint32_t r = ((uint32_t)msg[5] << 16) | ((uint32_t)msg[6] << 8) | msg[7];
	uint8_t r0 = (r >> 15) & 0x1f;
	uint8_t r1 = (r >> 10) & 0x1f;
	uint8_t r2 = (r >> 5) & 0x1f;
	uint8_t r3 = r & 0x1f;

	carstate.radar.rl = scale(r0, 0, 0x0f, 0, 99);
	carstate.radar.rlm = scale(r1, 0, 0x0f, 0, 99);
	carstate.radar.rrm = scale(r2, 0, 0x0f, 0, 99);
	carstate.radar.rr = scale(r3, 0, 0x0f, 0, 99);

#if 0
	//on/off
	msg[0] = (carstate.radar[0] == 0x70) ? 0x05 : 0x00;

	//front
	if ((carstate.radar[1] & 0x7) == 0x5)
		msg[1] = (carstate.radar[1] & 0xf8) | 0x2;
	//rear
	else if ((carstate.radar[1] & 0x7) == 0x3)
		msg[1] = (carstate.radar[1] & 0xf8) | 0x1;
	else
		msg[1] = 0;
#endif
}

struct msg_desc_t lr2_2013my_ms[] =
{
	{ 0x10, 50, 0, 0, lr2_2013my_ms_10_handler },
	{ 0xb8, 60, 0, 0, lr2_2013my_ms_b8_handler },
	{ 0x4a6, 90, 0, 0, lr2_2013my_ms_4a6_handler },
	{ 0x405, 500, 0, 0, lr2_2007my_ms_405_handler },
};

//mscan 2510020
//hscan 1a2402a
static void xc90_2007my_ms_wheel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.wheel = 0;
		return;
	}

	uint8_t angle = msg[6] & 0x3f;
	uint8_t wheel = scale(angle, 0, 0x3f, 0, 100);

	if (msg[5] & 0x04)
		carstate.wheel = wheel;
	else
		carstate.wheel = -wheel;
}

static void xc90_2007my_ms_gear_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.selector = STATE_UNDEF;
		return;
	}

	switch ((msg[6] >> 4) & 0x07) {

		case 1:
			carstate.selector = e_selector_p;
			break;
		case 2:
			carstate.selector = e_selector_r;
			break;
		case 3:
			carstate.selector = e_selector_n;
			break;
		case 4:
			carstate.selector = e_selector_d;
			break;

		default:
			carstate.selector = e_selector_p;
			break;
	}
}

static void xc90_2007my_ms_lsm1_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.illum = STATE_UNDEF;

		return;
	}

	carstate.illum = scale(msg[2], 0, 0xff, 0, 100);
}

static void xc90_2007my_ms_lsm0_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.park_lights = STATE_UNDEF;
		carstate.near_lights = STATE_UNDEF;
		return;
	}

	carstate.park_lights = msg[3] & 0x04 ? 1 : 0;
	carstate.near_lights = msg[3] & 0x08 ? 1 : 0;
}

static void xc90_2007my_ms_rem_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.fl_door = STATE_UNDEF;
		carstate.fr_door = STATE_UNDEF;
		carstate.rl_door = STATE_UNDEF;
		carstate.rr_door = STATE_UNDEF;
		carstate.bonnet = STATE_UNDEF;
		carstate.tailgate = STATE_UNDEF;

		carstate.radar.state = e_radar_off;
		return;
	}

	carstate.fl_door = (msg[5] & 0x02) ? 1 : 0;
	carstate.fr_door = (msg[5] & 0x04) ? 1 : 0;
	carstate.rl_door = (msg[5] & 0x08) ? 1 : 0;
	carstate.rr_door = (msg[5] & 0x10) ? 1 : 0;
	carstate.bonnet = (msg[5] & 0x41) ? 1 : 0;
	carstate.tailgate = (msg[5] & 0x20) ? 1 : 0;

	uint8_t v = (msg[3] >> 3) & 0x1f;
	v = scale(v, 0x00, 0x1f, 0, 99);

	//uint8_t who = msg[3] & 0x7;

	uint8_t on = (msg[2] & 0x01) ? 0x1 : 0x0;
	if (e_selector_r != car_get_selector())
		on = 0x0;

	carstate.radar.state = on ? e_radar_on : e_radar_off;
	carstate.radar.fl = v;
	carstate.radar.flm = v;
	carstate.radar.frm = v;
	carstate.radar.fr = v;
	carstate.radar.rl = v;
	carstate.radar.rlm = v;
	carstate.radar.rrm = v;
	carstate.radar.rr = v;
}

static void xc90_2007my_ms_swm_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		key_state.key_volume = STATE_UNDEF;
		key_state.key_mode = STATE_UNDEF;
		key_state.key_prev = STATE_UNDEF;
		key_state.key_next = STATE_UNDEF;

		return;
	}

#if 0
	//up
	if (!(msg[7] & 0x08)) {

		if ((key_state.key_volume != 1) && key_state.key_cb && key_state.key_cb->inc_volume)
			key_state.key_cb->inc_volume(1);

		key_state.key_volume = 1;
	}
	//down
	else if (!(msg[7] & 0x04)) {

		if ((key_state.key_volume != 0) && key_state.key_cb && key_state.key_cb->dec_volume)
			key_state.key_cb->dec_volume(1);

		key_state.key_volume = 0;
	}
	else
		key_state.key_volume = STATE_UNDEF;
#endif

	//PREV
	uint8_t key_prev = msg[7] & 0x01;
	//1->0 short release
	if ((key_state.key_prev == 1) && (key_prev == 0) && key_state.key_cb && key_state.key_cb->prev)
		key_state.key_cb->prev();
	key_state.key_prev = key_prev;

	//NEXT
	uint8_t key_next = (msg[7] >> 1) & 0x01;
	//1->0 short release
	if ((key_state.key_next == 1) && (key_next == 0) && key_state.key_cb && key_state.key_cb->next)
		key_state.key_cb->next();
	key_state.key_next = key_next;
}

static void xc90_2007my_ms_acc_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;

		return;
	}

	if (msg[1] & 0x40)
		carstate.acc = 1;
	else
		carstate.acc = 0;

	if (msg[1] & 0x20)
		carstate.ign = 1;
	else
		carstate.ign = 0;
}

struct msg_desc_t xc90_2007my_ms[] =
{
	{ 0x0217ffc, 20, 0, 0, xc90_2007my_ms_lsm0_handler },
	{ 0x131726c, 25, 0, 0, xc90_2007my_ms_swm_handler },
	{ 0x12173be, 45, 0, 0, xc90_2007my_ms_rem_handler },
	{ 0x2510020, 80, 0, 0, xc90_2007my_ms_wheel_handler },
	{ 0x2803008, 60, 0, 0, xc90_2007my_ms_lsm1_handler },
	{ 0x3200428, 90, 0, 0, xc90_2007my_ms_gear_handler },
	{ 0x2006428, 120, 0, 0, xc90_2007my_ms_acc_handler },
};

static void skoda_fabia_ms_wheel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.wheel = 0;
		return;
	}

	carstate.wheel = (((uint16_t)msg[2]) << 8 | msg[1]) & 0xffff;
}

static void skoda_fabia_ms_gear_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.selector = STATE_UNDEF;
		carstate.speed = 0;
		return;
	}

	if (msg[0] & 0x02)
		carstate.selector = e_selector_r;
	else
		carstate.selector = STATE_UNDEF;

	carstate.speed = (((uint16_t)msg[2]) << 8 | msg[1]) & 0xffff;
	carstate.speed /= 2;
}

static void skoda_fabia_ms_illum_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.illum = STATE_UNDEF;

		return;
	}

	carstate.illum = (msg[0] & 0x64) ? 100 : 0;
}

static void skoda_fabia_ms_park_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.park_break = STATE_UNDEF;
		return;
	}

	carstate.park_break = (msg[0] & 0x20) ? 1 : 0;
}

static void skoda_fabia_ms_rem_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.fl_door = STATE_UNDEF;
		carstate.fr_door = STATE_UNDEF;
		carstate.rl_door = STATE_UNDEF;
		carstate.rr_door = STATE_UNDEF;
		carstate.bonnet = STATE_UNDEF;
		carstate.tailgate = STATE_UNDEF;

		carstate.radar.state = e_radar_off;
		return;
	}

	carstate.fl_door = (msg[0] & 0x01) ? 1 : 0;
	carstate.fr_door = (msg[0] & 0x02) ? 1 : 0;
	carstate.rl_door = (msg[0] & 0x04) ? 1 : 0;
	carstate.rr_door = (msg[0] & 0x08) ? 1 : 0;
	carstate.bonnet = (msg[1] & 0x08) ? 1 : 0;
	carstate.tailgate = (msg[1] & 0x02) ? 1 : 0;

	uint8_t v = (msg[3] >> 3) & 0x1f;
	v = scale(v, 0x00, 0x1f, 0, 99);

	uint8_t on = (msg[2] & 0x01) ? 0x1 : 0x0;
	if (e_selector_r != car_get_selector())
		on = 0x0;

	carstate.radar.state = on ? e_radar_on : e_radar_off;
	carstate.radar.fl = v;
	carstate.radar.flm = v;
	carstate.radar.frm = v;
	carstate.radar.fr = v;
	carstate.radar.rl = v;
	carstate.radar.rlm = v;
	carstate.radar.rrm = v;
	carstate.radar.rr = v;
}

static void skoda_fabia_ms_swm_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		key_state.key_volume = STATE_UNDEF;
		key_state.key_mode = STATE_UNDEF;
		key_state.key_prev = STATE_UNDEF;
		key_state.key_next = STATE_UNDEF;

		return;
	}

#if 0
	//up
	if (!(msg[7] & 0x08)) {

		if ((key_state.key_volume != 1) && key_state.key_cb && key_state.key_cb->inc_volume)
			key_state.key_cb->inc_volume(1);

		key_state.key_volume = 1;
	}
	//down
	else if (!(msg[7] & 0x04)) {

		if ((key_state.key_volume != 0) && key_state.key_cb && key_state.key_cb->dec_volume)
			key_state.key_cb->dec_volume(1);

		key_state.key_volume = 0;
	}
	else
		key_state.key_volume = STATE_UNDEF;
#endif

	//PREV
	uint8_t key_prev = msg[7] & 0x01;
	//1->0 short release
	if ((key_state.key_prev == 1) && (key_prev == 0) && key_state.key_cb && key_state.key_cb->prev)
		key_state.key_cb->prev();
	key_state.key_prev = key_prev;

	//NEXT
	uint8_t key_next = (msg[7] >> 1) & 0x01;
	//1->0 short release
	if ((key_state.key_next == 1) && (key_next == 0) && key_state.key_cb && key_state.key_cb->next)
		key_state.key_cb->next();
	key_state.key_next = key_next;
}

static void skoda_fabia_ms_acc_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (msg[0] == 0x01)
		carstate.acc = 1;

	if (msg[0] == 0x00)
		carstate.acc = 0;

	else
		if (is_timeout(desc)) {

			carstate.ign = STATE_UNDEF;

			return;
		}

	carstate.ign = (msg[0] & 0x07) ? 1 : 0;
}

static void skoda_fabia_ms_aircon_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		car_air_state.ac = STATE_UNDEF;
		return;
	}

	car_air_state.ac = (msg[0] & 0x10) ? 1 : 0;
}

static void skoda_fabia_ms_taho_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
    if (is_timeout(desc)) {

		carstate.taho = 0;

		return;
	}

	carstate.taho = (((uint16_t)msg[2]) << 8 | msg[1]) & 0xffff;
	carstate.taho /= 4;
}

struct msg_desc_t skoda_fabia_ms[] =
{
	{ 0x635, 100, 0, 0, skoda_fabia_ms_illum_handler },
	{ 0x131726c, 25, 0, 0, skoda_fabia_ms_swm_handler },
	{ 0x371, 200, 0, 0, skoda_fabia_ms_rem_handler },
	{ 0x2510020, 80, 0, 0, skoda_fabia_ms_wheel_handler },
	{ 0x621, 200, 0, 0, skoda_fabia_ms_park_handler },
	{ 0x351, 100, 0, 0, skoda_fabia_ms_gear_handler },
	{ 0x271, 100, 0, 0, skoda_fabia_ms_acc_handler },
	{ 0x3e1, 200, 0, 0, skoda_fabia_ms_aircon_handler },
	{ 0x353, 100, 0, 0, skoda_fabia_ms_taho_handler },
};

// CAN COMFORT
static void q3_2015_ms_2c3_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;
		return;
	}

	/*
	   0001 0000 - 0x2C3 : 10 00 00 00 00 00 00 00 - no Key
	   0000 0001 - 0x2C3 : 01 FF FF FF FF FF FF FF - Key inserted, IGN off
	   0000 0111 - 0x2C3 : 07 FF FF FF FF FF FF FF - Ign on
	   0111 1011 - 0x2C3 : 0B FF FF FF FF FF FF FF - Starter
	*/

	if (msg[0] & 0x01)
		carstate.acc = 1;
	else
		carstate.acc = 0;

	if ((msg[0] & 0x02) == 0x02)
		carstate.ign = 1;
	else
		carstate.ign = 0;
}

static void q3_2015_ms_65F_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.vin[0] = STATE_UNDEF;
		return;
	}

	// WAUZZZ8U4GR060231

	//                      W  A  U
	// 65F # 00 FF FF FF FF 57 41 55
	//          Z  Z  Z  8  U  4  G
	// 65F # 01 5A 5A 5A 38 55 34 47
	//          R  0  6  0  2  3  1
	// 65F # 02 52 30 36 30 32 33 31

	//    6  |     1
	// 00.36.7c.c2.01.57.41.55
	// 01.5a.5a.5a.38.55.34.47
	// 02.52.30.36.30.32.33.31

	if (msg[0] == 0x00)
		memcpy(carstate.vin, msg + 5, 3);
	else if (msg[0] == 0x01)
		memcpy(carstate.vin + 3, msg + 1, 7);
	else if (msg[0] == 0x02)
		memcpy(carstate.vin + 10, msg + 1, 7);
}

static void q3_2015_ms_65D_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.odometer = STATE_UNDEF;
		return;
	}

	uint8_t odo[3] = { (msg[3] & 0x0F), msg[2], msg[1] };
	uint32_t value = 0;

	for (int i = 0; i < 3; i++) {
		value = (value << 8) + (odo[i] & 0xFF);
	}

	carstate.odometer = value;
}

static void q3_2015_ms_571_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.voltage = STATE_UNDEF;
		return;
	}

	// 0x571 : a6 00 00 00 00 00 00 00
	// 5 + (0.05 * 0xa6) = 13.3
	carstate.voltage = 5 + (0.05 * msg[0]);
}

static void q3_2015_ms_470_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.fl_door = STATE_UNDEF;
		carstate.fr_door = STATE_UNDEF;
		carstate.rl_door = STATE_UNDEF;
		carstate.rr_door = STATE_UNDEF;
		carstate.bonnet = STATE_UNDEF;
		carstate.tailgate = STATE_UNDEF;

		return;
	}

	// 0x470 : 00 00 24 16 20 00 00 00

	carstate.fl_door  = (msg[1] & 0x01) ? 1 : 0;
	carstate.fr_door  = (msg[1] & 0x02) ? 1 : 0;
	carstate.rl_door  = (msg[1] & 0x04) ? 1 : 0;
	carstate.rr_door  = (msg[1] & 0x08) ? 1 : 0;
	carstate.bonnet   = (msg[1] & 0x10) ? 1 : 0;
	carstate.tailgate = (msg[1] & 0x20) ? 1 : 0; // 60 or 20?
}

static void q3_2015_ms_359_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.selector = STATE_UNDEF;
		return;
	}

	// Gear selector 0x359 : 80 01 00 00 00 6b 08 XX

	// 88 10001000 P  136	1000XXXX 8
	// 70 01110000 R  112	0111XXXX 7
	// 60 01100000 N  96	0110XXXX 6
	// 50 01010000 D  80	0101XXXX 5
	// C0 11000000 S  192	1100XXXX 0A
	// E0 11100000 M  224	1110XXXX 0B
	// A0 10100000 M+ 160	1010XXXX 0C
	// B0 10110000 M- 176	1011XXXX 0E


	switch ((msg[7] >> 4) & 0x0f) {

		case 0x08:
			carstate.selector = e_selector_p;
			break;
		case 0x07:
			carstate.selector = e_selector_r;
			break;
		case 0x06:
			carstate.selector = e_selector_n;
			break;
		case 0x05:
			carstate.selector = e_selector_d;
			break;
		case 0x0a: // Manual +
			carstate.selector = e_selector_m_p;
			break;
		case 0x0b: // Мanual -
			carstate.selector = e_selector_m_m;
			break;
		case 0x0c:
			carstate.selector = e_selector_s;
			break;
		case 0x0e: // Manual
			carstate.selector = e_selector_m;
			break;
		default:
			carstate.selector = e_selector_p;
			break;
	}

	// 0x359 : 80 01 00 00 00 6b 08 88  // 0 km\h

	carstate.speed =  ((msg[2] * 256) + msg[1]) / 100; // Km\h
	// carstate.speed =  ((msg[2] * 256) + msg[1]) / 322; // Mil
	// carstate.speed =  ((msg[2] * 256) + msg[1]) / 192; // ?
}

static void q3_2015_ms_5BF_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		key_state.key_next = STATE_UNDEF;
		key_state.key_navi = STATE_UNDEF;
		key_state.key_cont = STATE_UNDEF;
		key_state.key_mici = STATE_UNDEF;
		return;
	}

	// 0x5BF : 00 00 00 11 FF FF FF FF all keys release
	// 0x5BF : 21 00 06 11 FF FF FF FF next track
	// 0x5BF : 1B 00 04 11 FF FF FF FF I-Nav
	// 0x5BF : 19 00 06 11 FF FF FF FF voice
	// 0x5BF : 02 00 01 11 FF FF FF FF right
	// 0x5BF : 03 00 04 11 FF FF FF FF left
	// 0x5BF : 20 00 00 11 FF FF FF FF center(mute)
	// 0x5BF : 12 00 01 11 FF FF FF FF volume+ вверх
	// 0x5BF : 12 00 0F 11 FF FF FF FF volume-
	// 0x5BF : 07 00 06 11 FF FF FF FF center(menu)
	// 0x5BF : 06 00 01 11 FF FF FF FF up(menu)
	// 0x5BF : 06 00 0F 11 FF FF FF FF down(menu)
	// 0x5BF : 01 00 04 11 FF FF FF FF menu

	uint8_t key_next = 0;
	uint8_t key_navi = 0;
	uint8_t key_cont = 0;
	uint8_t key_mici = 0;

	if (msg[0] == 0x21) {

		if ((msg[2] & 0x06) == 0x06)
			key_cont = 1;
		else
			key_next = 1;
	}

	if (msg[0] == 0x1B)
		key_navi = 1;

	if (msg[0] == 0x19)
		key_mici = 1;


	if ((key_state.key_cont == 1) && (key_cont == 0) && key_state.key_cb && key_state.key_cb->cont) key_state.key_cb->cont();

	if ((key_state.key_next == 1) && (key_next == 0) && key_state.key_cb && key_state.key_cb->next) key_state.key_cb->next();

	if ((key_state.key_navi == 1) && (key_navi == 0) && key_state.key_cb && key_state.key_cb->navi)	key_state.key_cb->navi();

	if ((key_state.key_mici == 1) && (key_mici == 0) && key_state.key_cb && key_state.key_cb->mici)	key_state.key_cb->mici();

	key_state.key_navi = key_navi;
	key_state.key_cont = key_cont;
	key_state.key_next = key_next;
	key_state.key_mici = key_mici;
}

static void q3_2015_ms_635_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.illum = STATE_UNDEF;
		return;
	}

	carstate.illum = scale(msg[1], 0x00, 0x63, 0, 100);
}

static void q3_2015_ms_3c3_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.wheel = 0;
		return;
	}

	// 0x3c3 : e0 81 00 00 80 40 f6 5
	// 0x3C3 : 9F 2B 00 00 80 00 60 3F - extreme left position
	// 0x3C3 : AA AB 00 00 80 40 9A 65 - extreme right position
	// 0x3C3 : 00 00 00 00 80 C0 6F 32 - middle position
	// 0x3C3 : 0B 80 00 00 80 20 92 51 - slightly right position
	// 0x3C3 : 0B 00 00 00 80 60 7C 91 - slightly left position


	// 0x3C3 - Steering status
	// Byte 0 - Steering angle HiByte
	// Byte 1 - Steering angle LoByte
	// Byte 2 - Steering torque HiByte
	// Byte 3 - Steering torque LoByte

	// Angle in degrees = ((Byte1*256)+Byte0)/91? (91 seemed to come out correctly on my microcontroller with integer variables)
	// Any ideas on toque divider? I'm assuming the data is in Nm.


	uint8_t angle = msg[1] & 0x7f;
	uint8_t wheel = scale(angle, 0, 0x44, 0, 100);

	if (msg[1] & 0x80) {
		// turn right
		carstate.wheel = wheel;
	} else {
		// turn left
		carstate.wheel = -wheel;
	}
}

static void q3_2015_ms_35b_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.taho = STATE_UNDEF;
		carstate.engine = STATE_UNDEF;
		return;
	}

	// 0x35B : 08 CC 0A BA 03 19 0E 80 // 91,5 degrees 691 RPM

	carstate.taho = ((msg[2] * 256) + msg[1]) / 4; // RPM

	if (carstate.taho > 500)
		carstate.engine = 1;
	else
		carstate.engine = 0;

	carstate.temp = (msg[3] - 64) * 0.75; // coolant
}

static void q3_2015_ms_621_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.park_break = STATE_UNDEF;
		carstate.low_fuel_lvl = STATE_UNDEF;
		carstate.low_washer = STATE_UNDEF;
		return;
	}

	// 0x621 : 20 48 10 17 01 00 00 00
	//	byte0:xx?xxxxx - park break
	//	byte0:xxxxx?xx - low washer
	//	byte3:x??????? - fuel level
	//	?xxxxxxx - low fuel level

	carstate.fuel_lvl = msg[3] & 0x7F; // ? fuel level

	if ((msg[0] & 0x20) == 0x20)
		carstate.park_break = 1;
	else
		carstate.park_break = 0;


	if ((msg[0] & 0x04) == 0x04)
		carstate.low_washer = 1;
	else
		carstate.low_washer = 0;

	if ((msg[3] & 0x80) == 0x80)
		carstate.low_fuel_lvl = 1;
	else
		carstate.low_fuel_lvl = 0;
}

static void q3_2015_ms_3E1_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		car_air_state.fanspeed = STATE_UNDEF;
		car_air_state.ac = STATE_UNDEF;
		car_air_state.rear = STATE_UNDEF;
		return;
	}

	/*
0x3E1 : 20 66 24 02 XX 00 00 01
0000 1111 - 0F - FAN 1
0001 1011 - 1B - FAN 2
0010 1011 - 2B - FAN 3
0011 1100 - 3C - FAN 4
0101 0111 - 57 - FAN 5
0111 0010 - 72 - FAN 6
1000 1101 - 8D - FAN 8
1010 1000 - A8 - FAN 9
1100 0011 - C3 - FAN 10
1101 1110 - DE - FAN 11
1111 1001 - F9 - POWERFULL

0x3E1 : 20 66 1F 02 0F 00 XX 02

0000 0010 - 02 - AC ON
0000 0000 - 00 - AC OFF

0x3E1 : 20 66 1F 02 00 00 04 03 - ac on

0x3E1 : 20 68 20 02 0F 00 00 XX

00 - turbo
01 - normal
02 - normal
03 - off

0x3E1 : 2X 68 1F 02 0F 00 00 02
X - rear window heating on
*/

	car_air_state.fanspeed = (msg[4] >> 4) & 0x0F;
	car_air_state.ac = (msg[6] >> 1) & 0x01;
	car_air_state.rear = (msg[0] >> 3) & 0x01;
}

static void q3_2015_ms_3E3_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		car_air_state.l_seat = STATE_UNDEF;
		car_air_state.r_seat = STATE_UNDEF;
		car_air_state.powerfull = STATE_UNDEF;
		return;
	}

	/*
0x3E3 : 00 00 97 XY 00 00 00 00

97 - temp ?

XY 36 - 0011 0110

Y - left seat heating
0000 - 0 - off
0110 - 6 - 3 leds
0100 - 4 - 2 leds
0010 - 2 - 1 led

X - right seat heating
0011 - 3 - 3 leads
0010 - 2 - 2 leads
0001 - 1 - 1 lead
0000 - 0 - off

0100 - 4 - turbo front window airheating
*/

	switch ((msg[3] >> 4) & 0x03) {
		case 0x01:
			car_air_state.r_seat = 1;
			break;
		case 0x02:
			car_air_state.r_seat = 2;
			break;
		case 0x03:
			car_air_state.r_seat = 3;
			break;
		case 0x00:
			car_air_state.r_seat = 0;
			break;
		default:
			car_air_state.r_seat = 0;
			break;
	}

	switch ((msg[3] >> 1) & 0x03) {
		case 0x01:
			car_air_state.l_seat = 1;
			break;
		case 0x02:
			car_air_state.l_seat = 2;
			break;
		case 0x03:
			car_air_state.l_seat = 3;
			break;
		case 0x00:
			car_air_state.l_seat = 0;
			break;
		default:
			car_air_state.l_seat = 0;
			break;
	}

	if ((msg[3] >> 6) & 0x01)
		car_air_state.powerfull = 1;
	else
		car_air_state.powerfull = 0;
}

static void q3_2015_ms_6DA_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {

		carstate.radar.state = e_radar_off;
		return;
	}

	// 0x6DA : 42 93 FF FF FF FF 00 00
	// 0x6DA : 42 92 FF FF FF FF 00 00

	// 0x6DA : XX YY AA BB CC DD 00 00
	// XX - 42 on 32 off

	// AA - left
	// BB - left middle
	// СС - right middle
	// DD - right

	// YY
	// Byte 2, bit 0: front/rear
	// 0 front
	// 1 rear

	// XX
	// Byte 1, bit 7: on/off
	// 0 off
	// 1 on

	/*
	 * // Extreme values   left  l-mid r-mid right
	 * uint8_t fmxd[4] = { 0x55, 0x77, 0x77, 0x55 }; // front max far
	 * uint8_t fmnd[4] = { 0x0F, 0x0C, 0x0C, 0x0F }; // front max near
	 * uint8_t rmxd[4] = { 0x55, 0x98, 0x98, 0x55 }; // rear max far
	 * uint8_t rmnd[4] = { 0x0F, 0x13, 0x13, 0x0F }; // rear max near
	 *
	 */

	if (msg[0] == 0x42) {

		carstate.radar.state = e_radar_on;

		if (msg[1] & 0x01) {
			// rear
			carstate.radar.rl = 99 - scale(msg[2], 0xf, 0x55, 0, 99);
			carstate.radar.rlm = 99 - scale(msg[3], 0x13, 0x98, 0, 99);
			carstate.radar.rrm = 99 - scale(msg[4], 0x13, 0x98, 0, 99);
			carstate.radar.rr = 99 - scale(msg[5], 0xf, 0x55, 0, 99);
		} else {
			// front
			carstate.radar.fl = 99 - scale(msg[2], 0xf, 0x55, 0, 99);
			carstate.radar.flm = 99 - scale(msg[3], 0xc, 0x77, 0, 99);
			carstate.radar.frm = 99 - scale(msg[4], 0xc, 0x77, 0, 99);
			carstate.radar.fr = 99 - scale(msg[5], 0xf, 0x55, 0, 99);
		}
	}
	else if (msg[0] == 0x32)
		carstate.radar.state = e_radar_off;
	else
		carstate.radar.state = e_radar_off;
}

static struct msg_desc_t q3_2015_ms[] =
{
	{ 0x2c3,  100, 0, 0, q3_2015_ms_2c3_handler }, // ACC
	{ 0x65F,  200, 0, 0, q3_2015_ms_65F_handler }, // VIN
	{ 0x65D, 1000, 0, 0, q3_2015_ms_65D_handler }, // Odometer
	{ 0x571,  600, 0, 0, q3_2015_ms_571_handler }, // Voltage
	{ 0x470,   50, 0, 0, q3_2015_ms_470_handler }, // Doors
	{ 0x359,  100, 0, 0, q3_2015_ms_359_handler }, // Gear selector
	{ 0x5BF,  100, 0, 0, q3_2015_ms_5BF_handler }, // Keys
	{ 0x635,  100, 0, 0, q3_2015_ms_635_handler }, // Illum
	{ 0x3c3,  100, 0, 0, q3_2015_ms_3c3_handler }, // Wheel
	{ 0x35b,  100, 0, 0, q3_2015_ms_35b_handler }, // Taho
	{ 0x621,  100, 0, 0, q3_2015_ms_621_handler }, // Break
	{ 0x6DA,   50, 0, 0, q3_2015_ms_6DA_handler }, // Parks
	{ 0x3E1,  500, 0, 0, q3_2015_ms_3E1_handler }, // AC
	{ 0x3E3,  500, 0, 0, q3_2015_ms_3E3_handler }, // Seat heating
};

static void toyota_premio_26x_ms_wheel_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.wheel = 0;
		return;
	}

	int16_t angle = (int16_t)(((uint16_t)(msg[0] & 0x0F)) << 8 | msg[1]);
	angle = (angle < 2048) ? angle : (angle - 4096);

	carstate.wheel = scale(angle, -380, 380, -100, 100);
}

static void toyota_premio_26x_ms_speed_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.speed = 0;
		return;
	}
	carstate.speed = (((uint16_t)msg[5]) << 8 | msg[6]);
}

static void toyota_premio_26x_ms_ign_brake_doors_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.acc = STATE_UNDEF;
		carstate.ign = STATE_UNDEF;
		carstate.park_break = STATE_UNDEF;
		carstate.fl_door = STATE_UNDEF;
		carstate.fr_door = STATE_UNDEF;
		carstate.rl_door = STATE_UNDEF;
		carstate.rr_door = STATE_UNDEF;
		carstate.tailgate = STATE_UNDEF;
		return;
	}
	carstate.acc        = (msg[4] & 0x10) ? 1:0;
	carstate.ign 		= (msg[7] & 0x40) ? 1:0;
	carstate.park_break = (msg[7] & 0x10) ? 1:0;
	carstate.fl_door 	= (msg[5] & 0x20) ? 1:0;
	carstate.fr_door 	= (msg[5] & 0x10) ? 1:0;
	carstate.rl_door 	= (msg[5] & 0x08) ? 1:0;
	carstate.rr_door 	= (msg[5] & 0x04) ? 1:0;
	carstate.tailgate 	= (msg[5] & 0x01) ? 1:0;
}

static void toyota_premio_26x_ms_light_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.illum = STATE_UNDEF;
		carstate.near_lights = STATE_UNDEF;
		carstate.park_lights = STATE_UNDEF;
		return;
	}
	carstate.illum 			= (msg[3] & 0x10) ? 100:0;
	carstate.near_lights 	= (msg[3] & 0x20) ? 1:0;
	carstate.park_lights    = (msg[3] & 0x10) ? 1:0;
}

static void toyota_premio_26x_ms_drive_mode_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
	 	carstate.selector = STATE_UNDEF;
		return;
	}

	if((msg[4] & 0xF0) == 0x80)
		carstate.selector = e_selector_p;
	else if((msg[4] & 0xF0) == 0x40)
		carstate.selector = e_selector_r;
	else if((msg[4] & 0xF0) == 0x20)
		carstate.selector = e_selector_n;
	else if((msg[4] & 0xF0) == 0x00 && msg[5] == 0x40)
		carstate.selector = e_selector_d;
	else if((msg[4] & 0xF0) == 0x00 && (msg[5] == 0x00 || msg[5] == 0x01))
		carstate.selector = e_selector_s;
	else
		carstate.selector = STATE_UNDEF;
}


struct msg_desc_t toyota_premio_26x_ms[] =
{
	{ 0x025,  	 80, 0, 0, toyota_premio_26x_ms_wheel_handler },
	{ 0x0b4,	100, 0, 0, toyota_premio_26x_ms_speed_handler },
	{ 0x620,    200, 0, 0, toyota_premio_26x_ms_ign_brake_doors_handler },
	{ 0x622,   1000, 0, 0, toyota_premio_26x_ms_light_handler },
	{ 0x3b4,   1000, 0, 0, toyota_premio_26x_ms_drive_mode_handler},
};

static void in_process(struct can_t * can, uint8_t ticks, struct msg_desc_t * msg_desc, uint8_t desc_num)
{
	uint8_t msgs_num = hw_can_get_msg_nums(can);

	for (uint8_t i = 0; i < msgs_num; i++) {

		struct msg_can_t msg;
		if (!hw_can_get_msg(can, &msg, i))
			continue;

		for (uint32_t j = 0; j < desc_num; j++) {

			struct msg_desc_t * desc = &msg_desc[j];

			if (msg.id == desc->id) {

				if (desc->in_handler) {

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
	else if(car == e_car_toyota_premio_26x)
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

		case e_car_lr2_2007my:
			in_process(can, ticks, lr2_2007my_ms, sizeof(lr2_2007my_ms)/sizeof(lr2_2007my_ms[0]));
			break;
		case e_car_lr2_2013my:
			in_process(can, ticks, lr2_2013my_ms, sizeof(lr2_2013my_ms)/sizeof(lr2_2013my_ms[0]));
			break;
		case e_car_xc90_2007my:
			in_process(can, ticks, xc90_2007my_ms, sizeof(xc90_2007my_ms)/sizeof(xc90_2007my_ms[0]));
			break;
		case e_car_skoda_fabia:
			in_process(can, ticks, skoda_fabia_ms, sizeof(skoda_fabia_ms)/sizeof(skoda_fabia_ms[0]));
			break;
		case e_car_q3_2015:
			in_process(can, ticks, q3_2015_ms, sizeof(q3_2015_ms)/sizeof(q3_2015_ms[0]));
			break;
		case e_car_toyota_premio_26x:
			in_process(can, ticks, toyota_premio_26x_ms, sizeof(toyota_premio_26x_ms)/sizeof(toyota_premio_26x_ms[0]));
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

