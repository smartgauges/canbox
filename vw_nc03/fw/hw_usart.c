#include "NUC131.h"

#include "hw.h"
#include "hw_usart.h"
#include "hw_gpio.h"
#include "ring.h"

struct usart_t
{
	UART_T * baddr;
	uint32_t clk;
	uint32_t rst;
	uint32_t irq;
	struct gpio_t tx;
	struct gpio_t rx;
	struct ring_t tx_ring;
	struct ring_t rx_ring;
	uint16_t baudrate;
	uint32_t rx_cnt;
	uint32_t tx_cnt;
};

static struct usart_t usart0 =
{
	.baddr = UART0,
	.clk = UART0_MODULE,
	.rst = UART0_RST,
	.tx = GPIO_INIT(B, 1),
	.rx = GPIO_INIT(B, 0),
	.baudrate = 0,
	.rx_cnt = 0,
	.tx_cnt = 0,
};

int hw_usart_write(struct usart_t * usart, const uint8_t * ptr, int len)
{
	int ret = ring_write(&usart->tx_ring, (uint8_t *)ptr, len);
	UART_EnableInt(usart->baddr, UART_IER_THRE_IEN_Msk | UART_IER_RDA_IEN_Msk);

	return ret;
}

uint8_t hw_usart_read_ch(struct usart_t * usart, uint8_t *ch)
{
	return ring_read_ch(&usart->rx_ring, ch);
}

struct usart_t * hw_usart_get(void)
{
	return &usart0;
}

uint32_t usart_isr_cnt = 0;
void usart_isr(struct usart_t * usart)
{
	uint32_t isr = usart->baddr->ISR;

	if (isr & UART_ISR_RDA_INT_Msk)
	{
		/* Get all the input characters */
//		while (UART_IS_RX_READY(usart->baddr))
//		{
			usart->rx_cnt++;

			/* Get the character from UART Buffer */
			uint8_t ch = UART_READ(usart->baddr);
			ring_write_ch(&usart->rx_ring, ch);
//		}

		if (!(isr & UART_ISR_THRE_INT_Msk))
			usart_isr_cnt++;
	}

	if (isr & UART_ISR_THRE_INT_Msk)
	{
		uint8_t ch;
		if (!ring_read_ch(&usart->tx_ring, &ch)) {
			
			/* Disable the TXE interrupt, it's no longer needed. */
			UART_DISABLE_INT(usart->baddr, UART_IER_THRE_IEN_Msk);
		} else {

			usart->tx_cnt++;

			/* Put data into the transmit register. */
			UART_WRITE(usart->baddr, ch);
		}
	}
}

void UART02_IRQHandler(void)
{
	usart_isr(&usart0);
}

void hw_usart_disable(struct usart_t * usart)
{
	UART_DisableInt(usart->baddr, UART_IER_THRE_IEN_Msk | UART_IER_RDA_IEN_Msk);

	UART_Close(usart->baddr);

	hw_gpio_set_float(&usart->rx);
	hw_gpio_set_float(&usart->tx);
}

void hw_usart_setup(struct usart_t * usart, uint32_t speed, uint8_t * txbuf, uint32_t txbuflen, uint8_t * rxbuf, uint32_t rxbuflen)
{
	ring_init(&usart->tx_ring, txbuf, txbuflen);
	ring_init(&usart->rx_ring, rxbuf, rxbuflen);

	/* Enable UART module clock */
	CLK_EnableModuleClock(usart->clk);

	/* Select UART module clock source */
	CLK_SetModuleClock(usart->clk, CLK_CLKSEL1_UART_S_HIRC, CLK_CLKDIV_UART(1));

	/* Set GPB multi-function pins for UART0 RXD(PB.0) and TXD(PB.1) */
	SYS->GPB_MFP &= ~(SYS_GPB_MFP_PB0_Msk | SYS_GPB_MFP_PB1_Msk);
	SYS->GPB_MFP |= (SYS_GPB_MFP_PB0_UART0_RXD | SYS_GPB_MFP_PB1_UART0_TXD);

	SYS_ResetModule(usart->rst);

	UART_Open(usart->baddr, speed);

	UART_EnableInt(usart->baddr, UART_IER_THRE_IEN_Msk | UART_IER_RDA_IEN_Msk);
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

