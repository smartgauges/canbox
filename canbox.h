#ifndef CANBOX_H
#define CANBOX_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"
{
#endif

void canbox_process(void);

void canbox_inc_volume(uint8_t val);
void canbox_dec_volume(uint8_t val);
void canbox_prev(void);
void canbox_next(void);

void canbox_find_cmd(uint8_t ch);

#ifdef __cplusplus
}
#endif

#endif

