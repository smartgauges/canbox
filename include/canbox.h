#ifndef CANBOX_H
#define CANBOX_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

void canbox_process(void);

uint8_t get_rear_delay_state(void); // Add prototype for get_rear_delay_state

// Modified canbox_park_process signature to accept fmax and rmax
void canbox_park_process(uint8_t fmax[4], uint8_t rmax[4]);

void canbox_inc_volume(uint8_t val);
void canbox_dec_volume(uint8_t val);
void canbox_prev(void);
void canbox_next(void);
void canbox_mode(void);
void canbox_cont(void);
void canbox_mici(void);

void canbox_raisepq_cmd_process(uint8_t ch);

void canbox_cmd_process(uint8_t ch);


#ifdef __cplusplus
}
#endif

#endif