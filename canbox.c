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

void canbox_raise_vw_radar_process(uint8_t fmax[4], uint8_t rmax[4])
{
	uint8_t buf[8] = { 0x70, 0xf3, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 };
	if (!car_get_radar(buf))
		return;

	uint8_t _park_is_on = ((0x70 == (buf[0] & 0xf0)) || (0x60 == (buf[0] & 0xf0))) ? 1 : 0;
	static uint8_t park_is_on = 0;

	if (park_is_on != _park_is_on || park_is_on) {

		park_is_on = _park_is_on;

		if (e_cb_raise_vw_pq == conf_get_canbox()) {

			uint8_t b = park_is_on ? 0x02 : 0x00;
			snd_canbox_msg(0x25, &b, sizeof(b));
		}

		if (e_cb_od_bmw_nbt_evo == conf_get_canbox()) {

			uint8_t b[] = { 0x0, park_is_on ? 0x08 : 0x00 };
			snd_canbox_msg(0x24, b, sizeof(b));
		}
	}

	if (!park_is_on)
		return;

	uint32_t f = ((uint32_t)buf[5] << 16) | ((uint32_t)buf[6] << 8) | buf[7];
	uint8_t f0 = (f >> 15) & 0x1f;
	uint8_t f1 = (f >> 10) & 0x1f;
	uint8_t f2 = (f >> 5) & 0x1f;
	uint8_t f3 = f & 0x1f;

	uint8_t fbuf[] = { 0x00, 0x00, 0x00, 0x00 };
	fbuf[0] = fmax[0] + 1 - scale(f3, 0, 31, 0, fmax[0]);
	fbuf[1] = fmax[1] + 1 - scale(f2, 0, 31, 0, fmax[1]);
	fbuf[2] = fmax[2] + 1 - scale(f1, 0, 31, 0, fmax[2]);
	fbuf[3] = fmax[3] + 1 - scale(f0, 0, 31, 0, fmax[3]);
	snd_canbox_msg(0x23, fbuf, sizeof(fbuf));

	uint32_t r = ((uint32_t)buf[2] << 16) | ((uint32_t)buf[3] << 8) | buf[4];
	uint8_t r0 = (r >> 15) & 0x1f;
	uint8_t r1 = (r >> 10) & 0x1f;
	uint8_t r2 = (r >> 5) & 0x1f;
	uint8_t r3 = r & 0x1f;

	uint8_t rbuf[] = { 0x00, 0x00, 0x00, 0x00 };
	rbuf[0] = rmax[0] + 1 - scale(r0, 0, 31, 0, rmax[0]);
	rbuf[1] = rmax[1] + 1 - scale(r1, 0, 31, 0, rmax[1]);
	rbuf[2] = rmax[2] + 1 - scale(r2, 0, 31, 0, rmax[2]);
	rbuf[3] = rmax[3] + 1 - scale(r3, 0, 31, 0, rmax[3]);
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
	uint8_t lwasher = car_get_washer();
	uint8_t dsbelt = car_get_dsbelt();

	uint8_t state = 0;

	if (conf_get_car() == e_car_skoda_fabia) {

		if (dsbelt)
			state |= 0x80;
		if (lwasher)
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

void canbox_raise_vw_aircon_process(void)
{
	uint8_t ac = car_get_ac();
	uint8_t recirc = car_get_ac_recirc();
	uint8_t auto_1 = car_get_ac_auto();
	uint8_t dual = car_get_ac_dual();
	uint8_t fdef = car_get_ac_fdef();
	uint8_t rdef = car_get_ac_rdef();

	uint8_t p0 = 0;
	uint8_t p1 = 0;
	uint8_t p2 = 0;
	uint8_t p3 = 0;
	uint8_t p4 = 0;

	if (ac)
		p0 |= 0x40;
	if (recirc)
		p0 |= 0x20;
	if (auto_1)
		p0 |= 0x08;
	if (dual)
		p0 |= 0x04;
	if (fdef)
		p0 |= 0x02;
	if (rdef)
		p0 |= 0x01;

	uint8_t buf[] = { p0, p1, p2, p3, p4, };

	snd_canbox_msg(0x21, buf, sizeof(buf));
}

void canbox_raise_vw_carinfo_process(void)
{
	uint8_t mbuf[3] = { 0x1, 0x1, 0x1 };
	//	if (!car_get_mileage(buf))
	//		return;

	uint16_t t_1 = car_get_taho();
	uint16_t t1 = t_1 >> 8 & 0xff;
	uint16_t t2 = t_1 - (t1*100);
	uint16_t t_3 = car_get_speed();
	uint16_t t3 = t_3 >> 8 & 0xff;
	uint16_t t4 = t_3 - (t3*100);
	uint8_t t5 = 0;
	uint8_t t6 = 0;
	uint8_t t7 = 0;
	uint8_t t8 = 0;
	uint8_t t9 = mbuf[0];
	uint8_t t10 = mbuf[1];
	uint8_t t11 = mbuf[2];
	uint8_t t12 = 15;
	uint8_t buf[] = { 0x02, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, };

	snd_canbox_msg(0x41, buf, sizeof(buf));
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

void canbox_hiworld_vw_mqb_radar_wheel_process(void)
{
	uint8_t pmax = (e_selector_r == car_get_selector()) ? 165 : 250;
	uint8_t pstart = (e_selector_r == car_get_selector()) ? 1 : 5;

	int16_t wmin = -540;
	int16_t wmax = 540;

	int8_t wheel = 0;
	if (!car_get_wheel(&wheel))
		return;

	uint8_t buf[8] = { 0x70, 0xf3, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1 };
	if (!car_get_radar(buf))
		return;

	uint8_t _park_is_on = ((0x70 == (buf[0] & 0xf0)) || (0x60 == (buf[0] & 0xf0))) ? 1 : 0;
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

	if (park_is_on) {

		uint32_t f = (buf[5] << 16) | (buf[6] << 8) | buf[7];
		uint8_t f0 = (f >> 15) & 0x1f;
		uint8_t f1 = (f >> 10) & 0x1f;
		uint8_t f2 = (f >> 5) & 0x1f;
		uint8_t f3 = f & 0x1f;

		uint32_t r = (buf[2] << 16) | (buf[3] << 8) | buf[4];
		uint8_t r0 = (r >> 15) & 0x1f;
		uint8_t r1 = (r >> 10) & 0x1f;
		uint8_t r2 = (r >> 5) & 0x1f;
		uint8_t r3 = r & 0x1f;

		uint8_t data[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

		data[0] = pmax + pstart - scale(r0, 0, 31, 0, pmax);
		data[1] = pmax + pstart - scale(r1, 0, 31, 0, pmax);
		data[2] = pmax + pstart - scale(r2, 0, 31, 0, pmax);
		data[3] = pmax + pstart - scale(r3, 0, 31, 0, pmax);

		data[4] = pmax + pstart - scale(f3, 0, 31, 0, pmax);
		data[5] = pmax + pstart - scale(f2, 0, 31, 0, pmax);
		data[6] = pmax + pstart - scale(f1, 0, 31, 0, pmax);
		data[7] = pmax + pstart - scale(f0, 0, 31, 0, pmax);

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

void canbox_process(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x26, -540, 540);
		uint8_t fmax[4] = { 10, 10, 10, 10 };
		uint8_t rmax[4] = { 10, 10, 10, 10 };
		canbox_raise_vw_radar_process(fmax, rmax);
		canbox_raise_vw_door_process();
		canbox_raise_vw_aircon_process();
		canbox_raise_vw_carinfo_process();
	}
	else if (e_cb_raise_vw_mqb == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x29, -19980, 19980);
		uint8_t fmax[4] = { 60, 120, 120, 60 };
		uint8_t rmax[4] = { 60, 165, 165, 60 };
		canbox_raise_vw_radar_process(fmax, rmax);
		canbox_raise_vw_mqb_door_process();
	}
	else if (e_cb_od_bmw_nbt_evo == conf_get_canbox()) {

		canbox_raise_vw_wheel_process(0x29, -5400, 5400);
		uint8_t fmax[4] = { 10, 10, 10, 10 };
		uint8_t rmax[4] = { 10, 10, 10, 10 };
		canbox_raise_vw_radar_process(fmax, rmax);
		canbox_raise_vw_mqb_door_process();
	}
	else if (e_cb_hiworld_vw_mqb == conf_get_canbox()) {

		canbox_hiworld_vw_mqb_radar_wheel_process();
		canbox_hiworld_vw_mqb_door_process();
	}
}

void canbox_inc_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_pq == conf_get_canbox())) {

		uint8_t buf[] = { 0x01, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_dec_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_pq == conf_get_canbox())) {

		uint8_t buf[] = { 0x02, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_prev(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_pq == conf_get_canbox())) {

		uint8_t buf[] = { 0x04, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_next(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_pq == conf_get_canbox())) {

		uint8_t buf[] = { 0x03, 0x01 };
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
void canbox_find_cmd(uint8_t ch)
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

