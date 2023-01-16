#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "hw_usart.h"
#include "conf.h"
#include "car.h"

static float scale(float value, float in_min, float in_max, float out_min, float out_max)
{
	return (((value - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min;
}

static uint8_t canbox_checksum(uint8_t * buf, uint8_t len)
{
	uint8_t sum = 0;
	for (uint8_t i = 0; i < len; i++)
		sum += buf[i];

	sum = sum ^ 0xff;

	return sum;
}

static uint8_t canbox_hiworld_checksum(uint8_t * buf, uint8_t len)
{
	uint8_t sum = 0;
	for (uint8_t i = 0; i < len; i++)
		sum += buf[i];

	sum = sum - 1;

	return sum;
}

static void snd_canbox_hiworld_msg(uint8_t type, uint8_t * msg, uint8_t size)
{
	uint8_t buf[5/*header header size type ... chksum*/ + size];
	buf[0] = 0x5a;
	buf[1] = 0xa5;
	buf[2] = size;
	buf[3] = type;
	memcpy(buf + 4, msg, size);
	buf[4 + size] = canbox_hiworld_checksum(buf + 2, size + 2);
	hw_usart_write(hw_usart_get(), buf, sizeof(buf));
}

static void snd_canbox_msg(uint8_t type, uint8_t * msg, uint8_t size)
{
	uint8_t buf[4/*header type size ... chksum*/ + size];
	buf[0] = 0x2e;
	buf[1] = type;
	buf[2] = size;
	memcpy(buf + 3, msg, size);
	buf[3 + size] = canbox_checksum(buf + 1, size + 2);
	hw_usart_write(hw_usart_get(), buf, sizeof(buf));
}

/*
 * +-------+
 * | RAISE |
 * +-------+
 */
void canbox_raise_vw_radar_process(uint8_t fmax[4], uint8_t rmax[4])
{
	struct radar_t radar;
	car_get_radar(&radar);
	if (radar.state == e_radar_undef)
		return;

	uint8_t _park_is_on = (e_radar_on == radar.state) ? 1 : 0;
	static uint8_t park_is_on = 0;

	if (park_is_on != _park_is_on || park_is_on) {

		park_is_on = _park_is_on;

		if (e_cb_raise_vw_pq == conf_get_canbox()) {

			uint8_t b = park_is_on ? 0x02 : 0x00;
			snd_canbox_msg(0x25, &b, sizeof(b));
		}
		else if (e_cb_od_bmw_nbt_evo == conf_get_canbox()) {

			uint8_t b[] = { 0x0, park_is_on ? 0x08 : 0x00 };
			snd_canbox_msg(0x24, b, sizeof(b));
		}
	}

	if (!park_is_on)
		return;

	uint8_t fbuf[] = { 0x00, 0x00, 0x00, 0x00 };
	fbuf[0] = fmax[0] + 1 - scale(radar.fr, 0, 99, 0, fmax[0]);
	fbuf[1] = fmax[1] + 1 - scale(radar.frm, 0, 99, 0, fmax[1]);
	fbuf[2] = fmax[2] + 1 - scale(radar.flm, 0, 99, 0, fmax[2]);
	fbuf[3] = fmax[3] + 1 - scale(radar.fl, 0, 99, 0, fmax[3]);
	snd_canbox_msg(0x23, fbuf, sizeof(fbuf));

	uint8_t rbuf[] = { 0x00, 0x00, 0x00, 0x00 };
	rbuf[0] = rmax[0] + 1 - scale(radar.rl, 0, 99, 0, rmax[0]);
	rbuf[1] = rmax[1] + 1 - scale(radar.rlm, 0, 99, 0, rmax[1]);
	rbuf[2] = rmax[2] + 1 - scale(radar.rrm, 0, 99, 0, rmax[2]);
	rbuf[3] = rmax[3] + 1 - scale(radar.rr, 0, 99, 0, rmax[3]);
	snd_canbox_msg(0x22, rbuf, sizeof(rbuf));
}

uint8_t get_rear_delay_state(void);
void canbox_raise_vw_wheel_process(uint8_t type, int16_t min, int16_t max)
{
	if (!get_rear_delay_state())
		return;

	int8_t wheel = 0;

	if (!car_get_wheel(&wheel))
		return;

	int16_t sangle = scale(wheel, -100, 100, min, max);
	uint8_t wbuf[] = { sangle, sangle >> 8 };
	snd_canbox_msg(type, wbuf, sizeof(wbuf));
}

void canbox_raise_vw_door_process(void)
{
	uint8_t fl_door = car_get_door_fl();
	uint8_t fr_door = car_get_door_fr();
	uint8_t rl_door = car_get_door_rl();
	uint8_t rr_door = car_get_door_rr();
	uint8_t tailgate = car_get_tailgate();
	uint8_t bonnet = car_get_bonnet();
	uint8_t park_break = car_get_park_break();
	uint8_t low_washer = car_get_low_washer();
	uint8_t ds_belt = car_get_ds_belt();

	uint8_t state = 0;

	if ((conf_get_car() == e_car_skoda_fabia) || (conf_get_car() == e_car_q3_2015)) {

		if (ds_belt)
			state |= 0x80;
		if (low_washer)
			state |= 0x40;
		if (park_break)
			state |= 0x20;
	}
	else if (bonnet)
		state |= 0x20;

	if (tailgate)
		state |= 0x10;
	if (rr_door)
		state |= 0x08;
	if (rl_door)
		state |= 0x04;
	if (fr_door)
		state |= 0x02;
	if (fl_door)
		state |= 0x01;

	uint8_t buf[] = { 0x01, state };

	snd_canbox_msg(0x41, buf, sizeof(buf));
}

void canbox_raise_vw_vehicle_info(void)
{
	uint16_t taho = car_get_taho();
	uint8_t t1 = (taho >> 8) & 0xff;
	uint8_t t2 = taho & 0xff;
	uint16_t speed = car_get_speed() * 100;
	uint8_t t3 = (speed >> 8) & 0xff;
	uint8_t t4 = speed & 0xff;
	uint16_t voltage = car_get_voltage() * 100;
	uint8_t t5 = (voltage >> 8) & 0xff;
	uint8_t t6 = voltage & 0xff;
	uint16_t temp = car_get_temp() * 10;
	uint8_t t7 = (temp >> 8) & 0xff;
	uint8_t t8 = temp & 0xff;
	uint32_t odo = car_get_odometer();
	uint8_t t9 = (odo >> 16) & 0xff;
	uint8_t t10 = (odo >> 8) & 0xff;
	uint8_t t11 = odo & 0xff;
	uint8_t t12 = car_get_low_fuel_level();

	uint8_t buf[13] = { 0x02, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12 };

	snd_canbox_msg(0x41, buf, sizeof(buf));

	uint8_t state = 0;
	static uint8_t low_state = 0;

	uint8_t low_voltage = car_get_low_voltage();
	uint8_t low_fuel = car_get_low_fuel_level();

	if (low_fuel)
		state |= 0x80;
	if (low_voltage)
		state |= 0x40;

	uint8_t buf_low[] = { 0x03, state };

	if (state != low_state) {

		low_state = state;
		snd_canbox_msg(0x41, buf_low, sizeof(buf_low));
	}
}

void canbox_raise_vw_ac_process(void)
{
	uint8_t buf[5] = { 0x00, 0x00, 0x00, 0x00, 0x00 };

	uint8_t ac = car_get_air_ac();
	uint8_t ac_max = car_get_air_ac_max();
	uint8_t recycling = car_get_air_recycling();
	uint8_t recycling_max = car_get_air_recycling_max();
	uint8_t recycling_min = car_get_air_recycling_min();
	uint8_t dual = car_get_air_dual();
	uint8_t rear = car_get_air_rear();
	uint8_t rear_lock = car_get_air_rear_lock();
	uint8_t aqs = car_get_air_aqs();
	uint8_t wind = car_get_air_wind();
	uint8_t middle = car_get_air_middle();
	uint8_t floor = car_get_air_floor();
	uint8_t powerfull = car_get_air_powerfull();
	uint8_t fanspeed = car_get_air_fanspeed();
	uint8_t l_temp = car_get_air_l_temp();
	uint8_t r_temp = car_get_air_r_temp();
	uint8_t l_seat = car_get_air_l_seat();
	uint8_t r_seat = car_get_air_r_seat();

	if (powerfull)
		buf[0] |= 0x80;
	if (ac)
		buf[0] |= 0x40;
	if (recycling)
		buf[0] |= 0x20;
	if (recycling_max)
		buf[0] |= 0x10;
	if (recycling_min)
		buf[0] |= 0x08;
	if (dual)
		buf[0] |= 0x04;
	if (ac_max)
		buf[0] |= 0x02;
	if (rear)
		buf[0] |= 0x01;

	if (wind)
		buf[1] |= 0x80;
	if (middle)
		buf[1] |= 0x40;
	if (floor)
		buf[1] |= 0x20;

	uint8_t speed = scale(fanspeed, 0x00, 0x0F, 0x00, 0x07);
	buf[1] |= speed & 0x07;

	if ((l_temp % 10) == 0x05)
		buf[2] |= 0x01;
	buf[2] |= ((int)l_temp) << 1;

	if ((r_temp % 10) == 0x05)
		buf[3] |= 0x01;
	buf[3] |= ((int)r_temp) << 1;

	if (aqs)
		buf[4] |= 0x80;
	if (rear_lock)
		buf[4] |= 0x08;
	if (ac_max)
		buf[4] |= 0x04;

	if (l_seat)
		buf[4] |= (l_seat << 4) & 0x30;
	if (r_seat)
		buf[4] |= (r_seat) & 0x03;

	snd_canbox_msg(0x21, buf, sizeof(buf));
}

void canbox_raise_vw_mqb_door_process(void)
{
	uint8_t fl_door = car_get_door_fl();
	uint8_t fr_door = car_get_door_fr();
	uint8_t rl_door = car_get_door_rl();
	uint8_t rr_door = car_get_door_rr();
	uint8_t tailgate = car_get_tailgate();
	uint8_t bonnet = car_get_bonnet();

	uint8_t state = 0;

	if (bonnet)
		state |= 0x4;
	if (tailgate)
		state |= 0x8;
	if (rl_door)
		state |= 0x10;
	if (rr_door)
		state |= 0x20;
	if (fl_door)
		state |= 0x40;
	if (fr_door)
		state |= 0x80;

	snd_canbox_msg(0x24, &state, 1);
}

/*
 * +---------+
 * | HIWORLD |
 * +---------+
 */
void canbox_hiworld_vw_mqb_wheel_process(void)
{
	int16_t wmin = -540;
	int16_t wmax = 540;

	int8_t wheel = 0;
	if (!car_get_wheel(&wheel))
		return;

	struct radar_t radar;
	car_get_radar(&radar);

	uint8_t _park_is_on = (e_radar_on == radar.state) ? 1 : 0;
	static uint8_t park_is_on = 0;

	if (park_is_on != _park_is_on || park_is_on) {

		park_is_on = _park_is_on;

		int16_t sangle = scale(wheel, -100, 100, wmin, wmax);
		uint8_t wbuf[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
		wbuf[0] = park_is_on ? 0x20 : 0x00;
		wbuf[4] = park_is_on ? 0x03 : 0x00;
		wbuf[6] = sangle >> 8;
		wbuf[7] = sangle;
		snd_canbox_hiworld_msg(0x11, wbuf, sizeof(wbuf));
	}
}

void canbox_hiworld_vw_mqb_radar_process(void)
{
	uint8_t pmax = (e_selector_r == car_get_selector()) ? 165 : 250;
	uint8_t pstart = (e_selector_r == car_get_selector()) ? 1 : 5;

	struct radar_t radar;
	car_get_radar(&radar);
	if (radar.state == e_radar_undef)
		return;

	uint8_t park_is_on = (e_radar_on == radar.state) ? 1 : 0;

	if (park_is_on) {

		uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		data[0] = pmax + pstart - scale(radar.rl, 0, 99, 0, pmax);
		data[1] = pmax + pstart - scale(radar.rlm, 0, 99, 0, pmax);
		data[2] = pmax + pstart - scale(radar.rrm, 0, 99, 0, pmax);
		data[3] = pmax + pstart - scale(radar.rr, 0, 99, 0, pmax);

		data[4] = pmax + pstart - scale(radar.fr, 0, 99, 0, pmax);
		data[5] = pmax + pstart - scale(radar.frm, 0, 99, 0, pmax);
		data[6] = pmax + pstart - scale(radar.flm, 0, 99, 0, pmax);
		data[7] = pmax + pstart - scale(radar.fl, 0, 99, 0, pmax);

		snd_canbox_hiworld_msg(0x41, data, sizeof(data));
	}
}

void canbox_hiworld_vw_mqb_door_process(void)
{
	uint8_t fl_door = car_get_door_fl();
	uint8_t fr_door = car_get_door_fr();
	uint8_t rl_door = car_get_door_rl();
	uint8_t rr_door = car_get_door_rr();
	uint8_t tailgate = car_get_tailgate();
	uint8_t bonnet = car_get_bonnet();

	uint8_t state = 0;

	if (bonnet)
		state |= 0x4;
	if (tailgate)
		state |= 0x8;
	if (rr_door)
		state |= 0x10;
	if (rl_door)
		state |= 0x20;
	if (fr_door)
		state |= 0x40;
	if (fl_door)
		state |= 0x80;

	uint8_t data[] = { 0x00, 0x00, state, 0x00, 0x00, 0x00, 0x00 };

	snd_canbox_hiworld_msg(0x12, data, sizeof(data));
}

/*
 * +------+
 * | Keys |
 * +------+
*/
void canbox_inc_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x01, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_dec_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x02, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_prev(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		uint8_t buf[] = { 0x03, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
	else if (e_cb_raise_vw_mqb == conf_get_canbox()) {

		uint8_t buf[] = { 0x04, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_next(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		uint8_t buf[] = { 0x04, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
	else if( e_cb_raise_vw_mqb == conf_get_canbox()) {

		uint8_t buf[] = { 0x03, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_mode(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x0a, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_cont(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x09, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_mici(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x0c, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

enum rx_state
{
	RX_WAIT_START,
	RX_LEN,
	RX_CMD,
	RX_DATA,
	RX_CRC
};

#define RX_BUFFER_SIZE 32
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static uint8_t rx_idx = 0;
static uint8_t rx_state = RX_WAIT_START;
/*header type size data chksum*/
static void canbox_raise_cmd_process(uint8_t ch)
{
	switch (rx_state) {

		case RX_WAIT_START:

			if (ch != 0x2e)
				break;

			rx_idx = 0;
			rx_buffer[rx_idx++] = ch;
			rx_state = RX_CMD;
			break;

		case RX_CMD:

			rx_buffer[rx_idx++] = ch;
			rx_state = RX_LEN;
			break;

		case RX_LEN:

			rx_buffer[rx_idx++] = ch;
			rx_state = ch ? RX_DATA : RX_CRC;
			break;

		case RX_DATA:

			rx_buffer[rx_idx++] = ch;
			{
				uint8_t len = rx_buffer[2];
				rx_state = ((rx_idx - 2) > len) ? RX_CRC : RX_DATA;
			}
			break;

		case RX_CRC:

			rx_buffer[rx_idx++] = ch;
			rx_state = RX_WAIT_START;

			{
				char buf[64];
				uint8_t cmd = rx_buffer[1];
				snprintf(buf, sizeof(buf), "\r\nnew cmd %" PRIx8 "\r\n", cmd);
				hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
			}

			break;
	}

	if (rx_idx > RX_BUFFER_SIZE)
		rx_state = RX_WAIT_START;
}

void canbox_cmd_process(uint8_t ch)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_pq == conf_get_canbox()))
		canbox_raise_cmd_process(ch);
}

void canbox_process(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x26, -540, 540);
		canbox_raise_vw_door_process();
		canbox_raise_vw_ac_process();
		canbox_raise_vw_vehicle_info();
	}
	else if (e_cb_raise_vw_mqb == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x29, -19980, 19980);
		canbox_raise_vw_mqb_door_process();
	}
	else if (e_cb_od_bmw_nbt_evo == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x29, -5400, 5400);
		canbox_raise_vw_mqb_door_process();
	}
	else if (e_cb_hiworld_vw_mqb == conf_get_canbox()) {

		canbox_hiworld_vw_mqb_wheel_process();
		canbox_hiworld_vw_mqb_door_process();
	}
}

void canbox_park_process(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		uint8_t fmax[4] = { 10, 10, 10, 10 };
		uint8_t rmax[4] = { 10, 10, 10, 10 };
		canbox_raise_vw_radar_process(fmax, rmax);
	}
	else if (e_cb_raise_vw_mqb == conf_get_canbox()) {

		uint8_t fmax[4] = { 60, 120, 120, 60 };
		uint8_t rmax[4] = { 60, 165, 165, 60 };
		canbox_raise_vw_radar_process(fmax, rmax);
	}
	else if (e_cb_od_bmw_nbt_evo == conf_get_canbox()) {

		uint8_t fmax[4] = { 10, 10, 10, 10 };
		uint8_t rmax[4] = { 10, 10, 10, 10 };
		canbox_raise_vw_radar_process(fmax, rmax);
	}
	else if (e_cb_hiworld_vw_mqb == conf_get_canbox()) {

		canbox_hiworld_vw_mqb_radar_process();
	}
}

