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
	uint8_t illum;
	uint8_t selector;
	uint8_t radar[8];
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
	.radar = { STATE_UNDEF, },
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
};

typedef struct key_state_t
{
	uint8_t key_volume;
	uint8_t key_prev;
	uint8_t key_next;
	uint8_t key_mode;

	struct key_cb_t * key_cb;
} key_state_t;

static struct key_state_t key_state =
{
	.key_volume = STATE_UNDEF,
	.key_prev = STATE_UNDEF,
	.key_next = STATE_UNDEF,
	.key_mode = STATE_UNDEF,
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
		carstate.taho = STATE_UNDEF;

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

		carstate.speed = STATE_UNDEF;
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

		carstate.radar[0] = STATE_UNDEF;
		return;
	}

	/**
	 * 0.70 - on/off
	 * 1.f8 - state, 1.07 - front/rear:3 - rear, 5 - front, 1 - both?
	 * 2.xx - rear
	 * 3.xx
	 * 4.xx
	 * 5.xx
	 * 6.xx
	 * 7.xx - front
	 */
	memcpy(carstate.radar, msg, 8);
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

		carstate.radar[0] = STATE_UNDEF;
		return;
	}

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

	carstate.radar[0] = msg[0];
	carstate.radar[1] = msg[1];
	carstate.radar[2] = msg[5];
	carstate.radar[3] = msg[6];
	carstate.radar[4] = msg[7];
	carstate.radar[5] = msg[2];
	carstate.radar[6] = msg[3];
	carstate.radar[7] = msg[4];
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

		carstate.radar[0] = STATE_UNDEF;
		return;
	}

	carstate.fl_door = (msg[5] & 0x02) ? 1 : 0;
	carstate.fr_door = (msg[5] & 0x04) ? 1 : 0;
	carstate.rl_door = (msg[5] & 0x08) ? 1 : 0;
	carstate.rr_door = (msg[5] & 0x10) ? 1 : 0;
	carstate.bonnet = (msg[5] & 0x41) ? 1 : 0;
	carstate.tailgate = (msg[5] & 0x20) ? 1 : 0;

	uint32_t v = (msg[3] >> 3) & 0x1f;
	uint8_t who = msg[3] & 0x7;
	uint32_t rear = (v << 15) | (v << 10) | (v << 5) | v;
	uint32_t front = 0;

	uint8_t on = (msg[2] & 0x01) ? 0x1 : 0x0;
	if (e_selector_r != car_get_selector())
		on = 0x0;

	carstate.radar[0] = on ? 0x70 : 0x0;
	carstate.radar[1] = (v << 3) | who;
	carstate.radar[2] = rear >> 16;
	carstate.radar[3] = rear >> 8;
	carstate.radar[4] = rear;
	carstate.radar[5] = front >> 16;
	carstate.radar[6] = front >> 8;
	carstate.radar[7] = front;
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
	carstate.radar[0] = STATE_UNDEF,
	carstate.wheel = 0,

	carstate.park_lights = STATE_UNDEF,
	carstate.near_lights = STATE_UNDEF,
	carstate.park_break = STATE_UNDEF,

	key_state.key_cb = cb;
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

uint8_t car_get_radar(uint8_t * buf)
{
	memset(buf, 0x0, 8);
	if (carstate.radar[0] == STATE_UNDEF)
		return 0;

	memcpy(buf, carstate.radar, 8);
	return 8;
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

