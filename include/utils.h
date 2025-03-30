#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

float scale(float value, float in_min, float in_max, float out_min, float out_max);
uint8_t canbox_checksum(uint8_t * buf, uint8_t len);
void snd_canbox_msg(uint8_t type, uint8_t * msg, uint8_t size);
uint8_t canbox_hiworld_checksum(uint8_t * buf, uint8_t len);
void snd_canbox_hiworld_msg(uint8_t type, uint8_t * msg, uint8_t size);


#ifdef __cplusplus
}
#endif

#endif // UTILS_H