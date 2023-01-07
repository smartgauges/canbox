#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/pwr.h>
#include <libopencm3/cm3/scb.h>

#include "hw.h"

void hw_cpu_sleep(void)
{
	rcc_periph_clock_enable(RCC_PWR);

	SCB_SCR &= ~(SCB_SCR_SLEEPDEEP | SCB_SCR_SLEEPONEXIT);
	SCB_SCR &= ~SCB_SCR_SLEEPONEXIT;

	pwr_clear_wakeup_flag();

#if 1
	pwr_set_stop_mode();
	pwr_voltage_regulator_low_power_in_stop();
	//pwr_voltage_regulator_on_in_stop();

	SCB_SCR |= SCB_SCR_SLEEPDEEP;
#endif
	__asm__("wfi");

	SCB_SCR &= ~SCB_SCR_SLEEPDEEP;
}

