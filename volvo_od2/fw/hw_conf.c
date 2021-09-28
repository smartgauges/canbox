#include <libopencm3/stm32/flash.h>
#include <libopencm3/cm3/cortex.h>

#include <inttypes.h>

#include "conf.h"
#include "hw_conf.h"

//#define debug 1
#ifdef debug
#include <string.h>
#include <stdio.h>
#include "hw_usart.h"
void mdelay(uint32_t msec);
#endif

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
	cm_disable_interrupts();
	flash_unlock();
}

void hw_conf_lock(void)
{
	flash_lock();
	cm_enable_interrupts();
}

void hw_conf_erase(void)
{
	hw_conf_unlock();

	flash_erase_page(CONF_ADDR);

	hw_conf_lock();
}

void hw_conf_setup(void)
{
}

uint32_t hw_conf_read_word(uint32_t address)
{
	uint32_t r = *(uint32_t *)(address);

#ifdef debug
	char buf[100];
	snprintf(buf, sizeof(buf), "read 0x%x:0x%x\r\n", (unsigned int)address, (unsigned int)r);
	hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	mdelay(100);
#endif

	return r;
}

void hw_conf_write_word(uint32_t address, uint32_t val)
{
	flash_program_word(address, val);
}

