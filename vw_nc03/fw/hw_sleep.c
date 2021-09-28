#include "NUC131.h"

#include "hw.h"

void hw_cpu_sleep(void)
{
	CLK_PowerDown();
	//CLK_Idle();
}

