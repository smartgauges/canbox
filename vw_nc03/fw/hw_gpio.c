#include "NUC131.h"

#include "hw.h"
#include "hw_gpio.h"

static struct gpio_t rear = GPIO_INIT(A, 13);
static struct gpio_t park = GPIO_INIT(A, 12);
static struct gpio_t acc = GPIO_INIT(A, 8);
static struct gpio_t ill = GPIO_INIT(A, 9);

void hw_gpio_set(struct gpio_t * gpio)
{
	gpio->port->DOUT |= gpio->pin;
}

void hw_gpio_clr(struct gpio_t * gpio)
{
	gpio->port->DOUT &= ~gpio->pin;
}

void hw_gpio_set_float(const struct gpio_t * gpio)
{
	GPIO_SetMode(gpio->port, gpio->pin, GPIO_PMD_QUASI);
}

void hw_gpio_setup(void)
{
	GPIO_SetMode(rear.port, rear.pin, GPIO_PMD_OUTPUT);

	GPIO_SetMode(park.port, park.pin, GPIO_PMD_OUTPUT);

	GPIO_SetMode(acc.port, acc.pin, GPIO_PMD_OUTPUT);

	GPIO_SetMode(ill.port, ill.pin, GPIO_PMD_OUTPUT);
}

void hw_gpio_disable(void)
{
	hw_gpio_clr(&rear);
	hw_gpio_clr(&park);
	hw_gpio_clr(&acc);
	hw_gpio_clr(&ill);
}

#define gpio_on(pin) void hw_gpio_##pin##_on(void) { hw_gpio_set(&pin); }
#define gpio_off(pin) void hw_gpio_##pin##_off(void) { hw_gpio_clr(&pin); }
gpio_on(rear);
gpio_off(rear);
gpio_on(park);
gpio_off(park);
gpio_on(acc);
gpio_off(acc);
gpio_on(ill);
gpio_off(ill);

