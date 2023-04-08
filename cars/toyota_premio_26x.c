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
	carstate.speed = ((((uint16_t)msg[5]) << 8 | msg[6]) + 50) / 100;
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
		carstate.ds_belt = STATE_UNDEF;
		return;
	}
	carstate.acc        = (msg[4] & 0x10) ? 1:0;
	carstate.ign 		= (msg[4] & 0x20) ? 1:0;
	carstate.park_break = (msg[7] & 0x10) ? 0:1;
	carstate.fl_door 	= (msg[5] & 0x20) ? 1:0;
	carstate.fr_door 	= (msg[5] & 0x10) ? 1:0;
	carstate.rl_door 	= (msg[5] & 0x08) ? 1:0;
	carstate.rr_door 	= (msg[5] & 0x04) ? 1:0;
	carstate.tailgate 	= (msg[5] & 0x01) ? 1:0;
	carstate.ds_belt    = (msg[7] & 0x40) ? 1:0;
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

	if ((msg[4] & 0xF0) == 0x80)
		carstate.selector = e_selector_p;
	else if ((msg[4] & 0xF0) == 0x40)
		carstate.selector = e_selector_r;
	else if ((msg[4] & 0xF0) == 0x20)
		carstate.selector = e_selector_n;
	else if ((msg[4] & 0xF0) == 0x00 && msg[5] == 0x40)
		carstate.selector = e_selector_d;
	else if ((msg[4] & 0xF0) == 0x00 && (msg[5] == 0x00 || msg[5] == 0x01))
		carstate.selector = e_selector_s;
	else
		carstate.selector = STATE_UNDEF;
}

static void toyota_premio_26x_ms_odometer(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.odometer = 0;
		return;
	}
	carstate.odometer = ((uint32_t)msg[5] << 16) | ((uint32_t)msg[6] << 8) | ((uint32_t)msg[7] << 0);
}

static void toyota_premio_26x_ms_tacho_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.taho = 0;
		return;
	}
	carstate.taho = ((uint32_t)msg[0] << 8) | ((uint32_t)msg[1] << 0);
	carstate.taho = ((carstate.taho * 3) + 2) / 4;
}

static void toyota_premio_26x_ms_temp_handler(const uint8_t * msg, struct msg_desc_t * desc)
{
	if (is_timeout(desc)) {
		carstate.temp = 0;
		return;
	}

	carstate.temp = (int32_t)((int8_t)msg[3]-0x30);
}

struct msg_desc_t toyota_premio_26x_ms[] =
{
	{ 0x025,  	 80, 0, 0, toyota_premio_26x_ms_wheel_handler },
	{ 0x0b4,	100, 0, 0, toyota_premio_26x_ms_speed_handler },
	{ 0x620,    200, 0, 0, toyota_premio_26x_ms_ign_brake_doors_handler },
	{ 0x622,   1000, 0, 0, toyota_premio_26x_ms_light_handler },
	{ 0x3b4,   1000, 0, 0, toyota_premio_26x_ms_drive_mode_handler},
	{ 0x611,   1000, 0, 0, toyota_premio_26x_ms_odometer},
	{ 0x2c4,    100, 0, 0, toyota_premio_26x_ms_tacho_handler},
	{ 0x3b0,   2000, 0, 0, toyota_premio_26x_ms_temp_handler},
};

