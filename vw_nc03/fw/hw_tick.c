#include "NUC131.h"

#include "hw.h"
#include "hw_tick.h"
#include "clock.h"

void hw_systick_setup(void)
{
	/* (ahb_frequency / 8) counts per second */
	uint32_t counter = PLL_CLOCK / 4 / TICK_HZ;
	CLK_EnableSysTick(CLK_CLKSEL0_STCLK_S_HIRC_DIV2, counter);
}

void hw_systick_disable(void)
{
	CLK_DisableSysTick();
}

void SysTick_Handler(void)
{
	hw_systick_callback();
}

