#ifndef PROTOCOL_INTERFACE_H
#define PROTOCOL_INTERFACE_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct protocol_ops_t {
    void (*radar_process)(uint8_t fmax[4], uint8_t rmax[4]);
    void (*wheel_process)(uint8_t type, int16_t min, int16_t max);
    void (*door_process)(void);
    void (*vehicle_info_process)(void);
    void (*ac_process)(void);
    void (*inc_volume)(uint8_t val);
    void (*dec_volume)(uint8_t val);
    void (*prev)(void);
    void (*next)(void);
    void (*mode)(void);
    void (*cont)(void);
    void (*mici)(void);
    void (*cmd_process)(uint8_t ch);
    void (*park_process)(uint8_t fmax[4], uint8_t rmax[4]); // Corrected signature
} protocol_ops_t;

extern const protocol_ops_t raise_vw_pq_protocol_ops;
extern const protocol_ops_t raise_vw_mqb_protocol_ops;
extern const protocol_ops_t od_bmw_nbt_evo_protocol_ops;
extern const protocol_ops_t hiworld_vw_mqb_protocol_ops;

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_INTERFACE_H