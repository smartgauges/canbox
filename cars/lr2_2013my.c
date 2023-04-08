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

static void lr2_2013my_ms_405_handler(const uint8_t * msg, struct msg_desc_t * desc)
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
}

struct msg_desc_t lr2_2013my_ms[] =
{
	{ 0x10, 50, 0, 0, lr2_2013my_ms_10_handler },
	{ 0xb8, 60, 0, 0, lr2_2013my_ms_b8_handler },
	{ 0x4a6, 90, 0, 0, lr2_2013my_ms_4a6_handler },
	{ 0x405, 500, 0, 0, lr2_2013my_ms_405_handler },
};

