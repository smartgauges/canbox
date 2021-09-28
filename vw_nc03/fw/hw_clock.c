#include "NUC131.h"

#include "clock.h"
#include "hw_clock.h"

void SYS_Init(void)
{
	/* Enable Internal RC 22.1184MHz clock */
	CLK_EnableXtalRC(CLK_PWRCON_OSC22M_EN_Msk);

	/* Waiting for Internal RC clock ready */
	CLK_WaitClockReady(CLK_CLKSTATUS_OSC22M_STB_Msk);

	/* Switch HCLK clock source to Internal RC and HCLK source divide 1 */
	CLK_SetHCLK(CLK_CLKSEL0_HCLK_S_HIRC, CLK_CLKDIV_HCLK(1));

	/* Set core clock as PLL_CLOCK from PLL */
	CLK_SetCoreClock(PLL_CLOCK);
}

void hw_clock_setup()
{
	SYS_UnlockReg();

	SYS_Init();

	SYS_LockReg();
}

