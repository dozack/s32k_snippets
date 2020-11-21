/*
 * armv7m_pendsv.h
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#ifndef ARMV7M_ARMV7M_PENDSV_H_
#define ARMV7M_ARMV7M_PENDSV_H_

#include "stddef.h"
#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

#define ARMV7M_PENDSV_ENTRY_COUNT 32

typedef void (*armv7m_pendsv_routine_t)(void *context, uint32_t data);

extern volatile armv7m_pendsv_routine_t * armv7m_pendsv_enqueue(
		armv7m_pendsv_routine_t routine, void *context, uint32_t data);

extern void armv7m_pendsv_initialize(void);

extern void PendSV_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_PENDSV_H_ */
