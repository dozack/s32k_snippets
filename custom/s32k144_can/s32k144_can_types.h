/*
 * s32k144_can_types.h
 *
 *  Created on: Nov 9, 2020
 *      Author: zoltan
 */

#ifndef S32K144_CAN_TYPES_H_
#define S32K144_CAN_TYPES_H_

#include "stdint.h"
#include "stdbool.h"
#include "stddef.h"

enum {
    CAN_INSTANCE_CAN0,
    CAN_INSTANCE_CAN1,
    CAN_INSTANCE_CAN2,
    CAN_INSTANCE_NUM
};

typedef enum {
    CAN_STATE_NONE, /* !< Handle dereferenced */
    CAN_STATE_INIT, /* !< Handle created - allows peripheral usage */
    CAN_STATE_FREEZE, /* !< Configuring peripheral which is locked */
    CAN_STATE_READY, /* !< Ready for rx, tx operations */
    CAN_STATE_TRANSMIT /* !< Message transmission ongoing */
} s32k144_can_state_t;

typedef enum {
    CAN_CLK_OSC,
    CAN_CLK_PERIPH
} s32k144_can_clk_source_t;

typedef enum {
    CAN_MODE_NORMAL,
    CAN_MODE_LISTEN_ONLY,
    CAN_MODE_LOOPBACK,
    CAN_MODE_FREEZE,
    CAN_MODE_DISABLE
} s32k144_can_mode_t;

typedef struct {
    uint32_t prop_seg;
    uint32_t phase_seg1;
    uint32_t phase_seg2;
    uint32_t pre_divider;
    uint32_t rjump_width;
} s32k144_can_timing_t;

typedef enum {
    CAN_ID_STD,
    CAN_ID_EXT
} s32k144_can_id_type_t;

typedef struct {
    s32k144_can_id_type_t id_type;
    uint32_t dlc;
} s32k144_can_mb_code_t;

typedef void (*s32k144_can_callback_t)(void *, uint32_t);

typedef struct {
    void * base;
    uint32_t instance;
    s32k144_can_state_t state;
    s32k144_can_clk_source_t clock;
    s32k144_can_mode_t mode;
    s32k144_can_timing_t timing;
    s32k144_can_callback_t callback_ev;
    void * context_ev;
    s32k144_can_callback_t callback_err;
    void * context_err;
} s32k144_can_t;

#define CAN_EVENT_RX_COMPLETE               (can_event_t)(0x0000001)
#define CAN_EVENT_TX_COMPLETE               (can_event_t)(0x0000002)
#define CAN_EVENT_WAKEUP_TIMEOUT            (can_event_t)(0x0000004)
#define CAN_EVENT_WAKEUP_MATCH              (can_event_t)(0x0000008)
#define CAN_EVENT_WAKEUP_SELF               (can_event_t)(0x0000010)
#define CAN_EVENT_ERROR                     (can_event_t)(0x0000020)

#endif /* S32K144_CAN_TYPES_H_ */
