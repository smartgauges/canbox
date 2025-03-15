#include <libopencm3/stm32/rcc.h>

#include "hw_clock.h"

void hw_clock_setup(void)
{
	rcc_clock_setup_pll(&rcc_hse_configs[RCC_CLOCK_HSE8_72MHZ]);

	/* Enable AFIO clock. */
	rcc_periph_clock_enable(RCC_AFIO);
}

