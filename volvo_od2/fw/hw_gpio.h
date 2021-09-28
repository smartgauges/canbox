#ifndef HW_GPIO_H
#define HW_GPIO_H

#include <inttypes.h>

struct gpio_t
{
	uint32_t rcc;
	uint32_t port;
	uint32_t pin;
};
#define GPIO_INIT(PORT,PIN) { RCC_GPIO##PORT, GPIO##PORT, GPIO##PIN }

void hw_gpio_set(struct gpio_t *);
void hw_gpio_clr(struct gpio_t *);
void hw_gpio_set_float(const struct gpio_t * gpio);

#endif

