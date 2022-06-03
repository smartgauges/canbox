#ifndef HW_CAN_H
#define HW_CAN_H

#include <inttypes.h>

typedef enum e_speed_t
{
	e_speed_100 = 0,
	e_speed_125,
	e_speed_250,
	e_speed_500,
	e_speed_1000,
	e_speed_nums
} e_speed_t;

typedef struct msg_can_t
{
	uint32_t id;
	uint32_t num;
	uint8_t type;
	uint8_t len;
	uint8_t data[8];
} __attribute__ ((__packed__)) msg_can_t;

struct can_t;
struct can_t * hw_can_get_mscan(void);

uint8_t hw_can_setup(struct can_t * can, e_speed_t speed);
void hw_can_disable(struct can_t * can);
void hw_can_sleep(struct can_t * can);
uint8_t hw_can_set_speed(struct can_t * can, e_speed_t speed);

void hw_can_silent(void);
void hw_can_active(void);

uint32_t hw_can_get_pack_nums(struct can_t * can);
uint8_t hw_can_get_msg_nums(struct can_t * can);
uint8_t hw_can_get_msg(struct can_t * can, struct msg_can_t * msg, uint8_t idx);
uint8_t hw_can_get_sts(struct can_t * can);
void hw_can_clr(struct can_t * can);

void hw_can_rcv_msg(struct can_t * can, msg_can_t * msg);
void hw_can_snd_msg(struct can_t * can, struct msg_can_t * msg);

#endif

