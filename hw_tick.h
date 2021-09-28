#ifndef HW_TICK_H
#define HW_TICK_H

#include <inttypes.h>

#define TICK_HZ 1000
#define SEC_TO_TICK(SEC) (TICK_HZ * SEC)
#define MSEC_TO_TICK(MSEC) ((MSEC * TICK_HZ)/1000)

typedef struct
{
	volatile uint16_t flag_tick;
	volatile uint16_t flag_5ms;
	volatile uint16_t flag_100ms;
	volatile uint16_t flag_250ms;
	volatile uint16_t flag_1000ms;
	volatile uint32_t msec;
	volatile uint32_t sec;
} tick_t;
extern volatile tick_t timer;

void hw_systick_setup(void);
void hw_systick_disable(void);
void hw_systick_callback(void);

#endif

