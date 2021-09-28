#ifndef HW_GPIO_H
#define HW_GPIO_H

#include <inttypes.h>

struct GPIO_T;
struct gpio_t
{
	GPIO_T * port;
	uint32_t pin;
};
#define GPIO_INIT(PORT,PIN) { P##PORT, BIT##PIN }

void hw_gpio_set(struct gpio_t *);
void hw_gpio_clr(struct gpio_t *);
void hw_gpio_set_float(const struct gpio_t * gpio);

#endif

