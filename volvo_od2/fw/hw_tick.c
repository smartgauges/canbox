#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/cm3/systick.h>

#include "hw_tick.h"

void hw_systick_disable(void)
{
	systick_interrupt_disable();
	systick_counter_disable();
}

void hw_systick_setup(void)
{
	/* 72MHz / 8 => 9000000 counts per second */
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
	/* clear counter so it starts right away */
	STK_CVR = 0;

	systick_set_reload(9000000 / TICK_HZ);

	systick_interrupt_enable();

	/* Start counting. */
	systick_counter_enable();

}

void sys_tick_handler(void)
{
	hw_systick_callback();
}

