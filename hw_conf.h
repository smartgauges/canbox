#ifndef HW_CONF_H
#define HW_CONF_H

#include <inttypes.h>

void hw_conf_setup(void);
uint32_t hw_conf_get_addr(void);
uint32_t hw_conf_get_sz(void);
void hw_conf_lock(void);
void hw_conf_unlock(void);
void hw_conf_erase(void);
void hw_conf_write_word(uint32_t address, uint32_t v);
uint32_t hw_conf_read_word(uint32_t address);

#endif

