#include <inttypes.h>

#include <string.h>
#include "conf.h"
#include "hw_conf.h"

#define HEADER_SIGN 0xdeadbeef
#define TAIL_SIGN 0xdeadbabe

//#define debug 1

#ifdef debug
#include <stdio.h>
#include "hw_usart.h"
void mdelay(uint32_t msec);
#endif

#pragma pack(1)
typedef struct conf_t
{
	uint32_t header;

	uint16_t idx;
	uint8_t car;
	uint8_t illum;
	uint16_t rear_delay;
	uint8_t canbox;

	uint8_t align;

	uint32_t tail;
} __attribute__ ((__packed__, aligned(4))) conf_t;
#pragma pack()

struct conf_t conf =
{
	.header = HEADER_SIGN,

	.idx = 0,
	.car = e_car_peugeot_407,
	.illum = 50,
	.rear_delay = 1500,
	.canbox = e_cb_raise_vw_pq,

	.align = 0,

	.tail = TAIL_SIGN,
};

void conf_write(void)
{
	uint32_t conf_sz = sizeof(struct conf_t);
	uint32_t conf_flash_addr = hw_conf_get_addr();
	uint32_t conf_flash_sz = hw_conf_get_sz();
	uint32_t conf_nums = conf_flash_sz / conf_sz;

	if (conf.idx >= conf_nums)
		conf.idx = 0;

	//read from flash
	struct conf_t c;
	uint32_t * pc = (uint32_t *)&c;
	for (uint32_t i = 0; i < conf_sz/4; i++)
		pc[i] = hw_conf_read_word(conf_flash_addr + conf.idx * conf_sz + i*4);

	//compare
	uint8_t flag_diff = 0;
	uint32_t * pconf = (uint32_t *)&conf;
	for (uint32_t i = 0; i < conf_sz/4; i++)
		if (pc[i] != pconf[i])
			flag_diff = 1;

	if (flag_diff) {

		if (++conf.idx >= conf_nums)
			conf.idx = 0;

		if (conf.idx == 0) {
#ifdef debug
			char buf[100];
			snprintf(buf, sizeof(buf), "conf erase\r\n");
			hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
			mdelay(100);
#endif
			hw_conf_erase();
		}

		hw_conf_unlock();

		for (uint32_t i = 0; i < conf_sz/4; i++)
			hw_conf_write_word(conf_flash_addr + conf.idx * conf_sz + i*4, pconf[i]);

		hw_conf_lock();

#ifdef debug
		char buf[100];
		snprintf(buf, sizeof(buf), "conf write idx:0x%x\r\n", (unsigned int)conf.idx);
		hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
		mdelay(100);
#endif
	}
}

void conf_read(void)
{
	uint32_t conf_sz = sizeof(struct conf_t);
	uint32_t conf_flash_addr = hw_conf_get_addr();
	uint32_t conf_flash_sz = hw_conf_get_sz();
	uint32_t conf_nums = conf_flash_sz / conf_sz;

	//find valid record
	for (uint16_t i = 0; i < conf_nums; i++) {

		struct conf_t c;
		memset(&c, 0xf, sizeof(c));
		uint32_t * pc = (uint32_t *)&c;
		for (uint32_t j = 0; j < conf_sz/4; j++)
			pc[j] = hw_conf_read_word(conf_flash_addr + i * conf_sz + j*4);

#ifdef debug
		char buf[100];
		snprintf(buf, sizeof(buf), "conf idx:0x%x header:0x%x tail:0x%x\r\n", (unsigned int)c.idx, (unsigned int)c.header, (unsigned int)c.tail);
		hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
		mdelay(100);
#endif

		//copy valid record
		uint32_t * pconf = (uint32_t *)&conf;
		if ((c.header == HEADER_SIGN) && (c.tail == TAIL_SIGN) && (c.idx == i)) {

			for (uint32_t j = 0; j < conf_sz/4; j++)
				pconf[j] = pc[j];
		}
		//invalid first record
		else if (i == 0) {

			conf.idx = conf_nums - 1;
			break;
		}
		//valid next record
		else if ((c.header == 0xffffffff) && (c.tail == 0xffffffff) && (c.idx == 0xffff)) {

			break;
		}
		//invalid next record
		else if ((c.header != 0xffffffff) && (c.tail != 0xffffffff) && (c.idx != 0xffff)) {

			conf.idx = conf_nums - 1;
			break;
		}
	}

#ifdef debug
	char buf[100];
	snprintf(buf, sizeof(buf), "conf read idx:%d\r\n", conf.idx);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	mdelay(100);
#endif
}

enum e_car_t conf_get_car(void)
{
	return (enum e_car_t)conf.car;
}

void conf_set_car(enum e_car_t car)
{
	if (car < e_car_nums)
		conf.car = car;
}

uint8_t conf_get_illum(void)
{
	return conf.illum;
}

void conf_set_illum(uint8_t illum)
{
	if (illum <= 100)
		conf.illum = illum;
}

uint16_t conf_get_rear_delay(void)
{
	return conf.rear_delay;
}

void conf_set_rear_delay(uint16_t rear_delay)
{
	if (rear_delay <= MAX_REAR_DELAY)
		conf.rear_delay = rear_delay;
}

enum e_canbox_t conf_get_canbox(void)
{
	return (enum e_canbox_t)conf.canbox;
}

void conf_set_canbox(enum e_canbox_t cb)
{
	if (cb < e_cb_nums)
		conf.canbox = cb;
}

uint16_t conf_get_idx(void)
{
	return conf.idx;
}

