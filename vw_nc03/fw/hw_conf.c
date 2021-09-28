#include <inttypes.h>
#include <string.h>
#include <stdio.h>

#include "NUC131.h"

#include "conf.h"
#include "hw_conf.h"
//#include "hw_usart.h"

#define CONF_ADDR 0x1F000
#define CONF_SIZE 4096

#define CONFIG0_DFEN     0x01
#define CONFIG0_LOCK     0x02
#define CONFIG0_DFVSEN   0x04
#define CONFIG0_CBS_MASK 0xc0
#define CONFIG0_CBS_APP  0x80

uint32_t hw_conf_get_addr(void)
{
	return CONF_ADDR;
}

uint32_t hw_conf_get_sz(void)
{
	return CONF_SIZE;
}

uint32_t hw_conf_read_word(uint32_t address)
{
	return FMC_Read(address);
}

void hw_conf_unlock(void)
{
	SYS_UnlockReg();
	FMC_Open();
}

void hw_conf_lock(void)
{
	FMC_Close();
	SYS_LockReg();
}

void hw_conf_erase(void)
{
	//cm_disable_interrupts();
	hw_conf_unlock();

	for (uint8_t i = 0; i < CONF_SIZE/512; i++)
		FMC_Erase(CONF_ADDR + i*512);

	hw_conf_lock();
	//cm_enable_interrupts();
}

void hw_conf_write_word(uint32_t address, uint32_t val)
{
	FMC_Write(address, val);
}

void hw_conf_setup(void)
{
	uint32_t au32Config[2];

	SYS_UnlockReg();

	FMC_Open();

	/* Read current User Configuration */
	FMC_ReadConfig(au32Config, 2);

	//char buf[100];
	//snprintf(buf, sizeof(buf), "conf0:0x%x conf1:0x%x src:%d\r\n", (unsigned int)au32Config[0], (unsigned int)au32Config[1], FMC_GetBootSource());
	//hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
	/* Just return when Data Flash has been enabled and CBS is APPROM */
	if (!(au32Config[0] & CONFIG0_DFEN) && (au32Config[0] & CONFIG0_DFVSEN) && !FMC_GetBootSource()) {

		FMC_EnableAPUpdate();
		return;
	}

	//hw_usart_write(hw_usart_get(), (uint8_t *)"change conf\r\n", 13);

	/* Enable User Configuration Update */
	FMC_EnableConfigUpdate();

	/* Erase User Configuration */
	FMC_Erase(FMC_CONFIG_BASE);

	/* Write User Configuration to Enable Data Flash
	 * LOCK = 1, Flash unlocked
	 * DFVSEN = 1, DATA Flash Size is 4K bytes
	 */
	au32Config[0] &= ~CONFIG0_DFEN;
	au32Config[0] |= CONFIG0_LOCK;
	au32Config[0] |= CONFIG0_DFVSEN;
	au32Config[0] &= ~CONFIG0_CBS_MASK;
	au32Config[0] |= CONFIG0_CBS_APP;
	//au32Config[1] = CONF_ADDR;

	if (FMC_WriteConfig(au32Config, 2))
		return;

	FMC_Close();

	/* Perform chip reset to make new User Config take effect */
	SYS_ResetChip();

	SYS_LockReg();
}

