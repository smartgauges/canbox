#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/f1/nvic.h>

#include "ring.h"
#include "hw_gpio.h"

struct usart_t
{
	uint32_t baddr;
	uint32_t rcc;
	uint32_t irq;
	struct gpio_t tx;
	struct gpio_t rx;
	struct ring_t tx_ring;
	struct ring_t rx_ring;
	uint16_t baudrate;
	uint32_t rx_cnt;
	uint32_t tx_cnt;
};

static struct usart_t usart1 = 
{
	.baddr = USART1,
	.rcc = RCC_USART1,
	.tx = GPIO_INIT(A, 9),
	.rx = GPIO_INIT(A, 10),
	.irq = NVIC_USART1_IRQ,
	.baudrate = 0,
	.rx_cnt = 0,
	.tx_cnt = 0,
};

struct usart_t * hw_usart_get(void)
{
	return &usart1;
}

void hw_usart_setup(struct usart_t * usart, uint32_t speed, uint8_t * txbuf, uint32_t txbuflen, uint8_t * rxbuf, uint32_t rxbuflen)
{
	ring_init(&usart->tx_ring, txbuf, txbuflen);
	ring_init(&usart->rx_ring, rxbuf, rxbuflen);

	usart_disable(usart->baddr);

	/* Enable clocks for USART1. */
	rcc_periph_clock_enable(usart->rcc);

	rcc_periph_clock_enable(RCC_GPIOA);

	/* Enable the USART1 interrupt. */
	nvic_enable_irq(usart->irq);

	gpio_set_mode(usart->tx.port, GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, usart->tx.pin);
	gpio_set_mode(usart->rx.port, GPIO_MODE_INPUT, GPIO_CNF_INPUT_FLOAT, usart->rx.pin);

	/* Setup UART parameters. */
	usart_set_baudrate(usart->baddr, speed);
	usart_set_databits(usart->baddr, 8);
	usart_set_stopbits(usart->baddr, USART_STOPBITS_1);
	usart_set_parity(usart->baddr, USART_PARITY_NONE);
	usart_set_flow_control(usart->baddr, USART_FLOWCONTROL_NONE);
	usart_set_mode(usart->baddr, USART_MODE_TX_RX);

	/* Enable USART1 Receive interrupt. */
	USART_CR1(usart->baddr) |= USART_CR1_RXNEIE;

	/* Finally enable the USART. */
	usart_enable(usart->baddr);
}

uint32_t usart_isr_cnt = 0;
void usart_isr(struct usart_t * usart)
{
	usart_isr_cnt++;

	/* Check if we were called because of RXNE. */
	if (((USART_CR1(usart->baddr) & USART_CR1_RXNEIE) != 0) &&
	    ((USART_SR(usart->baddr) & USART_SR_RXNE) != 0)) {

		usart->rx_cnt++;

		/* Retrieve the data from the peripheral. */
		ring_write_ch(&usart->rx_ring, usart_recv(USART1));
	}

	/* Check if we were called because of TXE. */
	if (((USART_CR1(usart->baddr) & USART_CR1_TXEIE) != 0) &&
	    ((USART_SR(usart->baddr) & USART_SR_TXE) != 0)) {

		uint8_t ch;
		if (!ring_read_ch(&usart->tx_ring, &ch)) {

			/* Disable the TXE interrupt, it's no longer needed. */
			USART_CR1(usart->baddr) &= ~USART_CR1_TXEIE;
		} else {

			usart->tx_cnt++;

			/* Put data into the transmit register. */
			usart_send(usart->baddr, ch);
		}
	}
}

void usart1_isr(void)
{
	usart_isr(&usart1);
}

int hw_usart_write(struct usart_t * usart, const uint8_t * ptr, int len)
{
	int ret = ring_write(&usart->tx_ring, (uint8_t *)ptr, len);

	USART_CR1(USART1) |= USART_CR1_TXEIE;

	return ret;
}

void hw_usart_disable(struct usart_t * usart)
{
	usart_disable(usart->baddr);

	hw_gpio_set_float(&usart->rx);
	hw_gpio_set_float(&usart->tx);
}

uint8_t hw_usart_read_ch(struct usart_t * usart, uint8_t *ch)
{
	return ring_read_ch(&usart->rx_ring, ch);
}

uint32_t hw_usart_get_rx_overflow(struct usart_t * usart)
{
	return ring_get_overflow(&usart->rx_ring);
}

uint32_t hw_usart_get_tx_overflow(struct usart_t * usart)
{
	return ring_get_overflow(&usart->tx_ring);
}

uint32_t hw_usart_get_tx(struct usart_t * usart)
{
	return usart->tx_cnt;
}

uint32_t hw_usart_get_rx(struct usart_t * usart)
{
	return usart->rx_cnt;
}

