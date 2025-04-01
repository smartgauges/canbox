#ifndef PROTOCOL_RAISERZC_H
#define PROTOCOL_RAISERZC_H

#include "protocol/interface.h" // Include the common interface
#include <stdint.h>             // Include for uint8_t etc.

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Instance of the protocol operations structure for the RZC PSA protocol.
 *        This structure provides the function pointers for canbox.c to call
 *        for handling various vehicle events and sending data to the head unit
 *        using the RZC 0xFD-based serial protocol.
 */
extern const protocol_ops_t raise_rcz_protocol_ops;

// No other public functions needed for typical usage via protocol_ops_t

#ifdef __cplusplus
}
#endif

#endif // PROTOCOL_RAISERZC_H