/*
 * s32k144_can_config.h
 *
 *  Created on: Nov 9, 2020
 *      Author: zoltan
 */

#ifndef S32K144_CAN_CONFIG_H_
#define S32K144_CAN_CONFIG_H_

#include "s32k144_can_types.h"

typedef struct {
    s32k144_can_clk_source_t clock_source;
    s32k144_can_mode_t mode;
    s32k144_can_timing_t timing;
} s32k144_can_cfg_t;

#endif /* S32K144_CAN_CONFIG_H_ */
