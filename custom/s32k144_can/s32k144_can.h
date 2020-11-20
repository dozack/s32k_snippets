/*
 * s32k144_can.h
 *
 *  Created on: Nov 9, 2020
 *      Author: zoltan
 */

#ifndef S32K144_CAN_H_
#define S32K144_CAN_H_

#include "s32k144_can_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

void s32k144_can_create(s32k144_can_t *can, uint32_t instance, s32k144_can_cfg_t *config);

void s32k144_can_destroy(s32k144_can_t *can);

void s32k144_can_enable(s32k144_can_t *can);

void s32k144_can_disable(s32k144_can_t *can);

void s32k144_can_freeze(s32k144_can_t *can);

void s32k144_can_unfreeze(s32k144_can_t *can);

void s32k144_can_set_timing(s32k144_can_t *can, s32k144_can_timing_t timing);

s32k144_can_timing_t s32k144_can_get_timing(s32k144_can_t *can);

void s32k144_can_set_mode(s32k144_can_t *can, s32k144_can_mode_t mode);

s32k144_can_mode_t s32k144_can_get_mode(s32k144_can_t *can);

void s32k144_can_notify_on_event(s32k144_can_t *can, s32k144_can_callback_t cb, void *context);

void s32k144_can_notify_on_error(s32k144_can_t *can, s32k144_can_callback_t cb, void *context);

#if 0
void can_set_clock_source();

void can_set_mailbox_count();

void can_set_message_buffer();

void can_get_message_buffer();

#endif

#ifdef __cplusplus
}
#endif

#endif /* S32K144_CAN_H_ */
