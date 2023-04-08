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

