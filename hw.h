#ifndef HW_H
#define HW_H

#include <inttypes.h>

void hw_setup(void);
void hw_disable(void);
void hw_sleep(void);
void hw_cpu_sleep(void);

void hw_gpio_setup(void);
void hw_gpio_disable(void);

void hw_gpio_rear_on(void);
void hw_gpio_rear_off(void);
void hw_gpio_park_on(void);
void hw_gpio_park_off(void);
void hw_gpio_acc_on(void);
void hw_gpio_acc_off(void);
void hw_gpio_ill_on(void);
void hw_gpio_ill_off(void);

#endif

