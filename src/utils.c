#include "utils.h"
#include "hw_usart.h" // Include hw_usart.h as snd_canbox_msg uses hw_usart_write
#include <string.h>  // Include string.h for memcpy

float scale(float value, float in_min, float in_max, float out_min, float out_max)
{
    return (((value - in_min) * (out_max - out_min)) / (in_max - in_min)) + out_min;
}

uint8_t canbox_checksum(uint8_t * buf, uint8_t len)
{
    uint8_t sum = 0;
    for (uint8_t i = 0; i < len; i++)
        sum += buf[i];

    sum = sum ^ 0xff;
    return sum;
}

uint8_t canbox_hiworld_checksum(uint8_t * buf, uint8_t len)
{
    uint8_t sum = 0;
    for (uint8_t i = 0; i < len; i++)
        sum += buf[i];

    sum = sum - 1;
    return sum;
}

void snd_canbox_hiworld_msg(uint8_t type, uint8_t * msg, uint8_t size)
{
    uint8_t buf[5 + size]; // Corrected buffer size calculation
    buf[0] = 0x5a;
    buf[1] = 0xa5;
    buf[2] = size;
    buf[3] = type;
    memcpy(buf + 4, msg, size);
    buf[4 + size] = canbox_hiworld_checksum(buf + 2, size + 2);
    hw_usart_write(hw_usart_get(), buf, sizeof(buf));
}

void snd_canbox_msg(uint8_t type, uint8_t * msg, uint8_t size)
{
    uint8_t buf[4 + size]; // Corrected buffer size calculation
    buf[0] = 0x2e;
    buf[1] = type;
    buf[2] = size;
    memcpy(buf + 3, msg, size);
    buf[3 + size] = canbox_checksum(buf + 1, size + 2);
    hw_usart_write(hw_usart_get(), buf, sizeof(buf));
}
