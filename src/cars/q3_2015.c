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

