#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "hw.h"
#include "hw_tick.h"
#include "hw_usart.h"
#include "hw_can.h"
#include "car.h"
#include "canbox.h"
#include "conf.h"

static uint32_t rear_off_delay = 0;
static uint32_t rear_on_delay = 0;
static uint32_t rear_on_timeout = 200;
static void rear_delay_process(uint8_t ticks)
{
	uint8_t rear_state = (e_selector_r == car_get_selector()) ? 1 : 0;

	if (rear_state) {

		rear_on_delay += ticks;
		rear_off_delay = 0;
	}
	else {

		if (rear_on_delay && (rear_on_delay < 2 * rear_on_timeout))
			rear_off_delay = conf_get_rear_delay();

		rear_off_delay += ticks;
		rear_on_delay = 0;
	}
}

uint8_t get_rear_delay_state(void)
{
	uint8_t ign = car_get_ign();
	if (!ign)
		return 0;

	if ((rear_on_delay > rear_on_timeout) || (rear_off_delay < conf_get_rear_delay()))
		return 1;
	else
		return 0;
}

struct key_cb_t key_cb =
{
	.mode = canbox_mode,
	.inc_volume = canbox_inc_volume,
	.dec_volume = canbox_dec_volume,
	.prev = canbox_prev,
	.next = canbox_next,
	.cont = canbox_cont,
	.navi = canbox_mode,
	.mici = canbox_mici,
};

uint8_t debug_on = 0;
uint32_t debug_on_cnt = 0;
uint8_t msg_idx = 0;

uint8_t sniffer_on = 0;

static void clr_screen(void)
{
	const char clr[] = "\033[2J";
	hw_usart_write(hw_usart_get(), (uint8_t *)clr, sizeof(clr));

	const char home[] = "\033[H";
	hw_usart_write(hw_usart_get(), (uint8_t *)home, sizeof(home));
}

static void clr_rscreen(void)
{
	const char clr[] = "\033[1J";
	hw_usart_write(hw_usart_get(), (uint8_t *)clr, sizeof(clr));

	const char home[] = "\033[H";
	hw_usart_write(hw_usart_get(), (uint8_t *)home, sizeof(home));
}

static void hide_cursor(void)
{
	const char hide[] = "\033[?25l";
	hw_usart_write(hw_usart_get(), (uint8_t *)hide, sizeof(hide));
}

static void print_line(void)
{
	char buf[32];

	snprintf(buf, sizeof(buf), "--------------------\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
}

static void debug_ch_process(uint8_t ch)
{
	if (ch == 'o') {

		debug_on = 0;
		sniffer_on = 0;

		clr_screen();
	}
	else if (ch == 'c') {

		int car = car_get_car();
		car++;
		if (car >= e_car_nums)
			car = 0;

		conf_set_car(car);
		car_init(conf_get_car(), &key_cb);
		hw_can_clr(hw_can_get_mscan());
	}
	else if (ch == 'b') {

		int cb = conf_get_canbox();
		cb++;
		if (cb >= e_cb_nums)
			cb = 0;

		conf_set_canbox(cb);
	}
	else if (ch == 'm') {

		uint8_t msgs_num = hw_can_get_msg_nums(hw_can_get_mscan());

		if (++msg_idx >= msgs_num)
			msg_idx = 0;
	}
	else if (ch == 's') {

		if (sniffer_on)
			sniffer_on = 0;
		else
			conf_write();
	}
	else if (ch == 'I') {

		uint8_t illum = conf_get_illum();
		illum++;
		if (illum >= 100)
			illum = 100;

		conf_set_illum(illum);
	}
	else if (ch == 'i') {

		uint8_t illum = conf_get_illum();
		illum--;
		if (illum <= 0 || illum >= 100)
			illum = 0;

		conf_set_illum(illum);
	}
	else if (ch == 'D') {

		uint16_t delay = conf_get_rear_delay();
		delay += 100;
		if (delay >= MAX_REAR_DELAY)
			delay = MAX_REAR_DELAY;

		conf_set_rear_delay(delay);
	}
	else if (ch == 'd') {

		uint16_t delay = conf_get_rear_delay();
		delay -= 100;
		if (delay <= 0 || delay >= MAX_REAR_DELAY)
			delay = 0;

		conf_set_rear_delay(delay);
	}
	else if (ch == 'S') {

		sniffer_on = 1;

		clr_rscreen();
	}
}

static void usart_process(void)
{
	uint8_t ch = 0;
	if (!hw_usart_read_ch(hw_usart_get(), &ch))
		return;

	if (!debug_on) {

		canbox_cmd_process(ch);

		if (ch == 'O') {

			if (debug_on_cnt++ > 10) {

				debug_on = 1;
				sniffer_on = 0;
				msg_idx = 0;

				clr_screen();
				hide_cursor();
			}
		}
		else
			debug_on_cnt = 0;
	}
	else
		debug_ch_process(ch);
}

extern uint32_t can_isr_cnt;

int max_show_sniff_num = 10;

void print_sniffer(void)
{
	char buf[200];

	clr_rscreen();

	snprintf(buf, sizeof(buf), "Sniffer\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	print_line();

	uint8_t ids = hw_can_get_msg_nums(hw_can_get_mscan());
	uint32_t msgs = hw_can_get_pack_nums(hw_can_get_mscan());
	snprintf(buf, sizeof(buf), "Can: IDs:%" PRIu8 " Msgs:%" PRIu32 " Irqs:%" PRIu32 " \r\n", ids, msgs, can_isr_cnt);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	print_line();

	uint8_t nums = ids - msg_idx;
	if (nums > max_show_sniff_num)
		nums = max_show_sniff_num;

	for (int i = msg_idx; i < (msg_idx + nums); i++) {

		struct msg_can_t msg;
		if (!hw_can_get_msg(hw_can_get_mscan(), &msg, i))
			break;

		snprintf(buf, sizeof(buf), "0x%X : %02X %02X %02X %02X %02X %02X %02X %02X \r\n",
			(int)msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
		hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	}

	print_line();

	snprintf(buf, sizeof(buf), "Ctrl keys: s - to main window; m - shift msgs\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
}

uint32_t wakeups = 0;
//extern uint32_t usart_isr_cnt;

void print_debug(void)
{
	char buf[100];

	uint8_t acc = car_get_acc();
	uint8_t ign = car_get_ign();
	uint8_t sel = car_get_selector();
	struct radar_t radar;
	car_get_radar(&radar);
	int8_t angle = 0;
	car_get_wheel(&angle);
	uint8_t park_lights = car_get_park_lights();
	uint8_t near_lights = car_get_near_lights();
	uint8_t ill = car_get_illum();

	clr_rscreen();

	char svin[18];
	car_get_vin((uint8_t *)svin);

	uint32_t odo = car_get_odometer();

	const char * scar = "";
	enum e_car_t car = car_get_car();
	switch (car) {
		case e_car_lr2_2007my:
			scar = "LR2 2007MY";
			break;
		case e_car_lr2_2013my:
			scar = "LR2 2013MY";
			break;
		case e_car_xc90_2007my:
			scar = "XC90 2007MY";
			break;
		case e_car_skoda_fabia:
			scar = "SKODA FABIA 2006MY";
			break;
		case e_car_q3_2015:
			scar = "Audi Q3 2015";
			break;
		case e_car_toyota_premio_26x:
			scar = "TOYOTA PREMIO 26x";
			break;
		default:
			break;
	}

	const char * scb = "";
	enum e_canbox_t cb = conf_get_canbox();
	switch (cb) {

		case e_cb_raise_vw_pq:
			scb = "Raise VW(PQ)";
			break;
		case e_cb_raise_vw_mqb:
			scb = "Raise VW(MQB)";
			break;
		case e_cb_od_bmw_nbt_evo:
			scb = "Oudi BMW(NBT)";
			break;
		case e_cb_hiworld_vw_mqb:
			scb = "HiWorld VW(MQB)";
			break;

		default:
			break;
	}

	snprintf(buf, sizeof(buf), "Main\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	print_line();

	snprintf(buf, sizeof(buf), "Configuration(%d)\r\n", conf_get_idx());
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	print_line();

	snprintf(buf, sizeof(buf), "Car: %s Vin: %s  %lu km        \r\n", scar, svin, odo);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	snprintf(buf, sizeof(buf), "CanBox: %s          \r\n", scb);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	snprintf(buf, sizeof(buf), "Conf: Illum:%d Rear Delay:%d          \r\n", conf_get_illum(), conf_get_rear_delay());
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	snprintf(buf, sizeof(buf), "State\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	print_line();

	snprintf(buf, sizeof(buf), "Uptime: %.5" PRIu32 ".%.3" PRIu32 " Wakeups: %" PRIu32 "     \r\n", timer.sec, timer.msec, wakeups);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	const char * ssel = "x";
	switch (sel) {
		case e_selector_p:
			ssel = "P";
			break;
		case e_selector_r:
			ssel = "R";
			break;
		case e_selector_n:
			ssel = "N";
			break;
		case e_selector_d:
			ssel = "D";
			break;
		case e_selector_m:
			ssel = "M";
			break;
		case e_selector_m_p:
			ssel = "M+";
			break;
		case e_selector_m_m:
			ssel = "M-";
			break;
		case e_selector_s:
			ssel = "S";
			break;
		default:
			break;
	}

	snprintf(buf, sizeof(buf), "Acc:%d Ign:%d Selector:%s R:%d Wheel:%d  \r\n", acc, ign, ssel, get_rear_delay_state(), angle);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	snprintf(buf, sizeof(buf), "Illum:%d ParkLights:%d NearLights:%d     \r\n", ill, park_lights, near_lights);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	snprintf(buf, sizeof(buf), "Park:%02x Front:%02x %02x %02x %02x Rear:%02x %02x %02x %02x\r\n",
		radar.state,
		radar.fl, radar.flm, radar.frm, radar.fr,
		radar.rl, radar.rlm, radar.rrm, radar.rr);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	//uint32_t rx_ovr = hw_usart_get_rx_overflow(hw_usart_get());
	//uint32_t tx_ovr = hw_usart_get_tx_overflow(hw_usart_get());
	//uint32_t rx_cnt = hw_usart_get_rx(hw_usart_get());
	//uint32_t tx_cnt = hw_usart_get_tx(hw_usart_get());
	//snprintf(buf, sizeof(buf), "Usart: RX:%" PRIu32 " TX:%" PRIu32 " RXOver:%" PRIu32 " TXOver:%" PRIu32 " On:%" PRIu32 " Isr:%" PRIu32 "\r\n", rx_cnt, tx_cnt, rx_ovr, tx_ovr, debug_on_cnt, usart_isr_cnt);
	//hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	uint8_t n1 = hw_can_get_msg_nums(hw_can_get_mscan());
	uint32_t n2 = hw_can_get_pack_nums(hw_can_get_mscan());
	snprintf(buf, sizeof(buf), "Can: IDs:%" PRIu8 " Msgs:%" PRIu32 " Irqs:%" PRIu32 "     \r\n", n1, n2, can_isr_cnt);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));

	uint8_t msgs_num = hw_can_get_msg_nums(hw_can_get_mscan());
	if (msgs_num && msg_idx < msgs_num) {

		struct msg_can_t msg;
		if (hw_can_get_msg(hw_can_get_mscan(), &msg, msg_idx)) {

			snprintf(buf, sizeof(buf), "Can%d/%d:  %08x:%02x.%02x.%02x.%02x.%02x.%02x.%02x.%02x     \r\n",
					msg_idx + 1, msgs_num, (int)msg.id, msg.data[0], msg.data[1], msg.data[2], msg.data[3], msg.data[4], msg.data[5], msg.data[6], msg.data[7]);
			hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
		}
	}

	print_line();

	snprintf(buf, sizeof(buf), "Ctrl keys: o - exit; m - shift msgs\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	snprintf(buf, sizeof(buf), "i/I - set Illum; d/D - set rear Delay\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	snprintf(buf, sizeof(buf), "c - set Car; b - set Canbox\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	snprintf(buf, sizeof(buf), "s - save conf; S - to Sniffer window\r\n");
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
}

static void gpio_process(void)
{
	uint8_t acc = car_get_acc();
//	uint8_t ign = car_get_ign();
//	uint8_t park_lights = car_get_park_lights();
	uint8_t ill = car_get_illum();

	if (acc)
		hw_gpio_acc_on();
	else
		hw_gpio_acc_off();

	if (ill > conf_get_illum())
		hw_gpio_ill_on();
	else
		hw_gpio_ill_off();

	if (get_rear_delay_state())
		hw_gpio_rear_on();
	else
		hw_gpio_rear_off();
}

int main(void)
{
	hw_setup();

	conf_read();

	car_init(conf_get_car(), &key_cb);

	uint8_t acc = car_get_acc();
	uint32_t ms_can_nums = 0;
	uint32_t ms_can_stop_counter = 0;

	while(1) {

		gpio_process();
		usart_process();

		if (timer.flag_tick) {

			timer.flag_tick = 0;

			rear_delay_process(1);
		}

		if (timer.flag_5ms) {

			timer.flag_5ms = 0;

			car_process(5);
		}

		if (timer.flag_100ms) {

			timer.flag_100ms = 0;

			if (!debug_on)
				canbox_park_process();
		}

		if (timer.flag_250ms) {

			timer.flag_250ms = 0;

			if (debug_on) {

				if (sniffer_on)
					print_sniffer();
				else
					print_debug();
			}
			else {

				canbox_process();
			}
		}

		if (timer.flag_1000ms) {

			timer.flag_1000ms = 0;

			debug_on_cnt = 0;

			uint32_t nums = hw_can_get_pack_nums(hw_can_get_mscan());
			if (nums == ms_can_nums)
				ms_can_stop_counter++;
			else
				ms_can_stop_counter = 0;

			if (acc && (conf_get_car() == e_car_skoda_fabia))
				ms_can_stop_counter = 1;

			ms_can_nums = nums;

			if (ms_can_stop_counter > 10) {

				conf_write();

				hw_sleep();

				ms_can_stop_counter = 0;

				hw_setup();

				conf_read();

				debug_on = 0;

				wakeups++;
			}
		}
	}
}

