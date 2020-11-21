/*
 * armv7m_systick.h
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#ifndef ARMV7M_ARMV7M_SYSTICK_H_
#define ARMV7M_ARMV7M_SYSTICK_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*armv7m_systick_callback_t)(void *context, uint32_t heartbeat);

extern uint64_t armv7m_systick_millis(void);

extern uint64_t armv7m_systick_micros(void);

extern void armv7m_systick_delay(uint32_t delay);

extern void armv7m_systick_notify(armv7m_systick_callback_t callback,
		void *context);

extern void armv7m_systick_initialize(unsigned int priority);

extern void armv7m_systick_enable(void);

extern void armv7m_systick_disable(void);

extern void S32_SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_SYSTICK_H_ */
