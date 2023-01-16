#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/can.h>
#include <libopencm3/stm32/f1/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/exti.h>

#include "hw_gpio.h"
#include "hw_can.h"

#define MSGS_SIZE 80
typedef struct can_t
{
	uint32_t rcc;
	uint32_t baddr;
	uint8_t fid;

	uint32_t irq;

	struct gpio_t tx;
	struct gpio_t rx;
	struct gpio_t s;

	uint32_t nums;
	msg_can_t msgs[MSGS_SIZE];
	uint8_t msgs_size;
} can_t;

static struct can_t can1 =
{
	.rcc = RCC_CAN,
	.baddr = CAN1,
	.irq = NVIC_USB_LP_CAN_RX0_IRQ,
	.fid = 0,
	.tx = GPIO_INIT(A, 12),
	.rx = GPIO_INIT(A, 11),
	.s = GPIO_INIT(B, 6),

	.nums = 0,
	.msgs = { },
	.msgs_size = 0,
};

struct can_t * hw_can_get_mscan(void)
{
	return &can1;
}

uint8_t hw_can_set_speed(struct can_t * can, e_speed_t speed)
{
	(void)can;
	(void)speed;

	return 0;
}

enum e_can_types
{
	e_can_simple = 0x0,
	e_can_statistic = 0x1,
	e_can_odd = 0x2,
	e_can_ext = 0x40,
	e_can_rtr = 0x80,
};

uint8_t hw_can_setup(struct can_t * can, e_speed_t speed)
{
	(void)speed;

	//fl2 2007
	msg_can_t msg_fl2_7e = { .id = 0x7e, .num = 1, .type = 0, .len = 8, .data = { 0x0, 0x0, 0x98, 0x07, 0xd0, 0x0, 0x80, 0x0 } };

	can->msgs[can->msgs_size] = msg_fl2_7e;
	can->msgs[can->msgs_size].num = 1;
	can->msgs_size++;

	//fl2 2007
	msg_can_t msg_fl2_6c = { .id = 0x6c, .num = 1, .type = 0, .len = 8, .data = { 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1 } };
	can->msgs[can->msgs_size] = msg_fl2_6c;
	can->msgs[can->msgs_size].num = 1;
	can->msgs_size++;

	//fl2 2007
	msg_can_t msg_fl2_188 = { .id = 0x188, .num = 1, .type = 0, .len = 8, .data = { 0x70, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x1f } };
	can->msgs[can->msgs_size] = msg_fl2_188;
	can->msgs[can->msgs_size].num = 1;
	can->msgs_size++;

	return 0;
}

void hw_can_disable(struct can_t * can)
{
	(void)can;
}

uint8_t hw_can_get_msg_nums(can_t * can)
{
	return can->msgs_size;
}

uint32_t hw_can_get_pack_nums(struct can_t * can)
{
	if (can->msgs_size)
		can->nums++;
	
	return can->nums;
}

uint8_t hw_can_get_msg(struct can_t * can, struct msg_can_t * msg, uint8_t idx)
{
	if (idx >= can->msgs_size)
		return 0;

	can->msgs[idx].num += 100;

	*msg = can->msgs[idx];

	return 1;
}

uint32_t can_isr_cnt = 0;

void hw_can_snd_msg(struct can_t * can, struct msg_can_t * msg)
{
	(void)can;
	(void)msg;
}

void hw_can_clr(struct can_t * can)
{
	(void)can;

	can->nums = 0;
	can->msgs_size = 0;
}

void hw_can_sleep(struct can_t * can)
{
	(void)can;
}

