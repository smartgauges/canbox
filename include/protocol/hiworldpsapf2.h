#ifndef PROTOCOL_HIWORLDPSA_H
#define PROTOCOL_HIWORLDPSA_H

#include "protocol/interface.h" // Include the common interface

#ifdef __cplusplus
extern "C" {
#endif

// Declare the protocol operations structure for external use
extern const protocol_ops_t hiworld_psa_protocol_ops;

// Optional: Declare individual functions if they need to be called directly from elsewhere
// void canbox_hiworld_psa_radar_process(uint8_t fmax[4], uint8_t rmax[4]);
// void canbox_hiworld_psa_basic_info_process(void);
// void canbox_hiworld_psa_detailed_info_process(void);
// void canbox_hiworld_psa_ac_process(void);
// void canbox_hiworld_psa_inc_volume(uint8_t val);
// void canbox_hiworld_psa_dec_volume(uint8_t val);
// void canbox_hiworld_psa_prev(void);
// void canbox_hiworld_psa_next(void);
// void canbox_hiworld_psa_mode(void);
// void canbox_hiworld_psa_cont(void);
// void canbox_hiworld_psa_mici(void);
// void canbox_hiworld_psa_cmd_process(uint8_t ch);
// void canbox_hiworld_psa_park_process(uint8_t fmax[4], uint8_t rmax[4]);


#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_HIWORLDPSA_H