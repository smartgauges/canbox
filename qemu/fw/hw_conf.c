#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/cortex.h>

#include <inttypes.h>

#include "conf.h"
#include "hw_conf.h"

/* sector 63 1KB */
#define CONF_ADDR     0x0800fc00
#define CONF_SIZE     1024
uint32_t hw_conf_get_addr(void)
{
	return CONF_ADDR;
}

uint32_t hw_conf_get_sz(void)
{
	return CONF_SIZE;
}

void hw_conf_unlock(void)
{
}

void hw_conf_lock(void)
{
}

void hw_conf_erase(void)
{
}

void hw_conf_setup(void)
{
}

uint32_t hw_conf_read_word(uint32_t address)
{
	(void)address;

	return 0xffffffff;
}

void hw_conf_write_word(uint32_t address, uint32_t val)
{
	(void)address;
	(void)val;
}

