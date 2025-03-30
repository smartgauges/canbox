#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "hw_usart.h"
#include "conf.h"
#include "utils.h"
#include "car.h"
#include "protocol/interface.h"
#include "protocol/raisepq.h"
#include "protocol/raisemqb.h"
#include "protocol/bmwnbtevo.h"
#include "protocol/hiworldmqb.h"
#include "protocol/hiworldpsapf2.h"


#ifdef E_CANBOX_RAISE_VW_PQ
#include "protocol/raisepq.c"
#endif

#ifdef E_CANBOX_RAISE_VW_MQB
#include "protocol/raisemqb.c"
#endif

#ifdef E_CANBOX_OD_BMW_NBT_EVO
#include "protocol/bmwnbtevo.c"
#endif

#ifdef E_CANBOX_HIWORLD_VW_MQB
#include "protocol/hiworldmqb.c"
#endif

#ifdef E_CANBOX_HIWORLD_PSA_PF2
#include "protocol/hiworldpsapf2.c" 
#endif

const protocol_ops_t *protocol_ops[] = {
    #ifdef E_CANBOX_RAISE_VW_PQ
        &raise_vw_pq_protocol_ops,
    #endif
    #ifdef E_CANBOX_RAISE_VW_MQB
        &raise_vw_mqb_protocol_ops,
    #endif
    #ifdef E_CANBOX_OD_BMW_NBT_EVO
        &od_bmw_nbt_evo_protocol_ops,
    #endif
    #ifdef E_CANBOX_HIWORLD_VW_MQB
        &hiworld_vw_mqb_protocol_ops,
    #endif
    #ifdef E_CANBOX_HIWORLD_PSA_PF2 
        &hiworld_psa_protocol_ops
    #endif
};


void canbox_process(void) {
    enum e_canbox_t canbox_type = conf_get_canbox();
    const protocol_ops_t *ops = protocol_ops[canbox_type];

    if (ops) {
        if (ops->wheel_process) {
            if (canbox_type == e_cb_raise_vw_pq)
                ops->wheel_process(0x26, -540, 540);
            else if (canbox_type == e_cb_raise_vw_mqb)
                ops->wheel_process(0x29, -19980, 19980);
            else if (canbox_type == e_cb_od_bmw_nbt_evo)
                ops->wheel_process(0x29, -5400, 5400);
            else if (e_cb_hiworld_vw_mqb == canbox_type)
                ops->wheel_process(0x11, 0, 0);
        }
        if (ops->door_process) ops->door_process();
        if (ops->ac_process) ops->ac_process();
        if (ops->vehicle_info_process) ops->vehicle_info_process();
    }
}

void canbox_park_process(uint8_t fmax[4], uint8_t rmax[4]) {
    enum e_canbox_t canbox_type = conf_get_canbox();
    const protocol_ops_t *ops = protocol_ops[canbox_type];

    if (ops && ops->park_process) {
        ops->park_process(fmax, rmax);
    }
}


// --- cmd_process (Now calling protocol-specific cmd_process) ---
void canbox_cmd_process(uint8_t ch) {
    enum e_canbox_t canbox_type = conf_get_canbox();
    const protocol_ops_t *ops = protocol_ops[canbox_type];
    if (ops && ops->cmd_process) ops->cmd_process(ch);
    // No fallback needed anymore as RaisePQ command processing is now in protocol_raisepq.c
}

// --- RX State machine for Raise VW PQ protocol (Moved from protocol_raisepq.c to canbox.c as RX handling logic remains here) ---
enum rx_state
{
    RX_WAIT_START,
    RX_LEN,
    RX_CMD,
    RX_DATA,
    RX_CRC
};

#define RX_BUFFER_SIZE 32


void canbox_inc_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x01, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_dec_volume(uint8_t val)
{
	(void)val;

	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x02, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_prev(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		uint8_t buf[] = { 0x03, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
	else if (e_cb_raise_vw_mqb == conf_get_canbox()) {

		uint8_t buf[] = { 0x04, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_next(void)
{
	if (e_cb_raise_vw_pq == conf_get_canbox()) {

		uint8_t buf[] = { 0x04, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
	else if( e_cb_raise_vw_mqb == conf_get_canbox()) {

		uint8_t buf[] = { 0x03, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_mode(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x0a, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_cont(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x09, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}

void canbox_mici(void)
{
	if ((e_cb_raise_vw_pq == conf_get_canbox()) || (e_cb_raise_vw_mqb == conf_get_canbox())) {

		uint8_t buf[] = { 0x0c, 0x01 };
		snd_canbox_msg(0x20, buf, sizeof(buf));

		buf[1] = 0x00;
		snd_canbox_msg(0x20, buf, sizeof(buf));
	}
}


static void canbox_raise_cmd_process_internal(uint8_t ch, enum rx_state *rx_state, uint8_t *rx_buffer, uint8_t *rx_idx) // Made internal, taking state machine vars as arguments
{
    switch (*rx_state) {

        case RX_WAIT_START:
            if (ch != 0x2e) break;
            *rx_idx = 0;
            rx_buffer[(*rx_idx)++] = ch;
            *rx_state = RX_CMD;
            break;

        case RX_CMD:
            rx_buffer[(*rx_idx)++] = ch;
            *rx_state = RX_LEN;
            break;

        case RX_LEN:
            rx_buffer[(*rx_idx)++] = ch;
            *rx_state = ch ? RX_DATA : RX_CRC;
            break;

        case RX_DATA:
            rx_buffer[(*rx_idx)++] = ch;
            {
                uint8_t len = rx_buffer[2];
                *rx_state = ((*rx_idx) - 2 > len) ? RX_CRC : RX_DATA;
            }
            break;

        case RX_CRC:
            rx_buffer[(*rx_idx)++] = ch;
            *rx_state = RX_WAIT_START;
            {
                uint8_t ack = 0xff;
                hw_usart_write(hw_usart_get(), &ack, 1);

                char buf[64];
                uint8_t cmd = rx_buffer[1];
                snprintf(buf, sizeof(buf), "\r\nnew cmd %" PRIx8 "\r\n", cmd);
                hw_usart_write(hw_usart_get(), (uint8_t *)buf, strlen(buf));
            }
            break;
    }
    if (*rx_idx > RX_BUFFER_SIZE)
        *rx_state = RX_WAIT_START;
}

void canbox_raisepq_cmd_process(uint8_t ch) { // Command process for Raise VW PQ
    static enum rx_state rx_state = RX_WAIT_START; // Static state for command processing
    static uint8_t rx_buffer[RX_BUFFER_SIZE];
    static uint8_t rx_idx = 0;
    canbox_raise_cmd_process_internal(ch, &rx_state, rx_buffer, &rx_idx); // Call internal state machine function
}