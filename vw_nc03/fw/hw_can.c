#include "NUC131.h"

#include "hw.h"
#include "hw_can.h"
#include "hw_gpio.h"
#include "hw_usart.h"

#define MSGS_SIZE 80
typedef struct can_t
{
	CAN_T * baddr;
	uint32_t clk;
	uint32_t rst;
	uint32_t irq;
	struct gpio_t rx;
	struct gpio_t tx;
	struct gpio_t stby;
	uint32_t nums;
	msg_can_t msgs[MSGS_SIZE];
	uint8_t msgs_size;
} can_t;

static struct can_t can0 =
{
	.baddr = CAN0,
	.clk = CAN0_MODULE,
	.rst = CAN0_RST,
	.irq = CAN0_IRQn,
	.rx = GPIO_INIT(D, 6),
	.tx = GPIO_INIT(D, 7),
	.stby = GPIO_INIT(C, 3),
	.nums = 0,
	.msgs = { },
	.msgs_size = 0,
};

struct can_t * hw_can_get_mscan(void)
{
	return &can0;
}

void hw_can_rcv_msg(struct can_t * can, msg_can_t * msg)
{
	uint8_t found = 0;
	for (uint8_t i = 0; i < can->msgs_size; i++) {

		if (can->msgs[i].id == msg->id) {

			can->msgs[i].len = msg->len;
			for (uint8_t j = 0; j < 8; j++)
				can->msgs[i].data[j] = msg->data[j];
			can->msgs[i].num++;
			found = 1;
			break;
		}
	}

	if (!found && can->msgs_size < MSGS_SIZE) {

		can->msgs[can->msgs_size] = *msg;
		can->msgs[can->msgs_size].num = 1;
		can->msgs_size++;
	}
}

uint32_t can_isr_cnt = 0;
static void can_isr(struct can_t * can)
{
	uint32_t u8IIDRstatus;

	can_isr_cnt++;

	u8IIDRstatus = can->baddr->IIDR;

	/* Check Status Interrupt Flag (Error status Int and Status change Int) */
	if (u8IIDRstatus == 0x00008000)
	{
		uint32_t sts = can->baddr->STATUS;

		if (sts & CAN_STATUS_RXOK_Msk)
		{
			/* Clear RxOK status*/
			can->baddr->STATUS &= ~CAN_STATUS_RXOK_Msk;
		}

		if(sts & CAN_STATUS_TXOK_Msk)
		{
			/* Clear TxOK status*/
			can->baddr->STATUS &= ~CAN_STATUS_TXOK_Msk;
		}
	}
	else if ((u8IIDRstatus >= 0x1) || (u8IIDRstatus <= 0x20))
	{
		STR_CANMSG_T rrMsg;
		CAN_Receive(can->baddr, u8IIDRstatus - 1, &rrMsg);

		can->nums++;
		msg_can_t msg;
		msg.id = rrMsg.Id;
		msg.len = rrMsg.DLC;
		for (uint8_t i = 0; i < 8; i++)
			msg.data[i] = rrMsg.Data[i];

		//msg.type = e_can_statistic;
		//if (rrMsg == CAN_EXT_ID)
		//	msg.type |= e_can_ext;

		hw_can_rcv_msg(can, &msg);

		/* Clear Interrupt Pending */
		CAN_CLR_INT_PENDING_BIT(can->baddr, (u8IIDRstatus - 1));
	}
	else if (can->baddr->WU_STATUS == 1)
	{
		/* Write '0' to clear */
		can->baddr->WU_STATUS = 0;
	}
}

void CAN0_IRQHandler(void)
{
	can_isr(&can0);
}

uint8_t hw_can_get_msg_nums(can_t * can)
{
	return can->msgs_size;
}

uint32_t hw_can_get_pack_nums(struct can_t * can)
{
	return can->nums;
}

uint8_t hw_can_get_msg(can_t * can, struct msg_can_t * msg, uint8_t idx)
{
	if (idx >= can->msgs_size)
		return 0;

	*msg = can->msgs[idx];

	return 1;
}

uint32_t speeds[e_speed_nums] = { 100000, 125000, 250000, 500000, 1000000 };

int32_t CAN_SetRxMsgObjAndMsk(CAN_T *tCAN, uint8_t u8MsgObj, uint8_t u8idType, uint32_t u32id, uint32_t u32idmask, uint8_t u8singleOrFifoLast);

uint8_t hw_can_setup(struct can_t * can, e_speed_t speed)
{
	CLK_EnableModuleClock(can->clk);

	GPIO_SetMode(can->stby.port, can->stby.pin, GPIO_PMD_OPEN_DRAIN);
	hw_gpio_clr(&can->stby);

	SYS_ResetModule(can->rst);

	/* Set PD multi-function pins for CANTX0, CANRX0 */
	SYS->GPD_MFP &= ~(SYS_GPD_MFP_PD6_Msk | SYS_GPD_MFP_PD7_Msk);
	SYS->GPD_MFP |= SYS_GPD_MFP_PD6_CAN0_RXD | SYS_GPD_MFP_PD7_CAN0_TXD;

	CAN_Open(can->baddr,  speeds[speed], CAN_NORMAL_MODE);

	/* Enable CAN interrupt and corresponding NVIC of CAN */
	CAN_EnableInt(can->baddr, CAN_CON_IE_Msk | CAN_CON_SIE_Msk);
	/* Install CAN call back functions */
	NVIC_SetPriority(can->irq, (1 << __NVIC_PRIO_BITS) - 2);
	NVIC_EnableIRQ(can->irq);

	if (CAN_SetRxMsgObjAndMsk(can->baddr, MSG(0), CAN_STD_ID, 0x0, 0x0, FALSE) == FALSE)
	//if (CAN_SetRxMsg(can->baddr, MSG(0), CAN_STD_ID, 0x7ff) == FALSE)
	{
		return 1;
	}

	if (CAN_SetRxMsgObjAndMsk(can->baddr, MSG(5), CAN_EXT_ID, 0x0, 0x0, FALSE) == FALSE)
	{
		return 1;
	}

	return 0;
}

uint8_t hw_can_set_speed(struct can_t * can, e_speed_t speed)
{
	CAN_SetBaudRate(can->baddr, speeds[speed]);
	return 0;
}

void hw_can_disable(struct can_t * can)
{
	/* Disable CAN */
	CAN_Close(can->baddr);

	/* Disable CAN Clock and Reset it */
	SYS_ResetModule(can->rst);
	CLK_DisableModuleClock(can->clk);
}

void hw_can_clr(struct can_t * can)
{
	can->nums = 0;
	can->msgs_size = 0;
}

void hw_can_sleep(struct can_t * can)
{
	hw_can_disable(can);

	hw_gpio_set(&can->stby);

	/* Configure PD.6 as Input mode and enable interrupt by falling edge trigger */
	PD->PMD = (PD->PMD & (~GPIO_PMD_PMD6_Msk)) | (GPIO_PMD_QUASI << GPIO_PMD_PMD6_Pos);
	PD->IMD |= (GPIO_IMD_EDGE << 6);
	PD->IEN |= (BIT6 << GPIO_IEN_IF_EN_Pos);
	NVIC_EnableIRQ(GPCDEF_IRQn);
}

void GPCDEF_IRQHandler(void)
{
	PC->ISRC = PC->ISRC;
	PD->ISRC = PD->ISRC;
	PE->ISRC = PE->ISRC;
	PF->ISRC = PF->ISRC;
}

void hw_can_snd_msg(struct can_t * can, struct msg_can_t * msg)
{
	(void)can;
	(void)msg;
}

