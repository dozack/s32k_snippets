/*
 * armv7m_nvic.h
 *
 *  Created on: Nov 16, 2020
 *      Author: zoltan
 */

#ifndef ARMV7M_NVIC_H_
#define ARMV7M_NVIC_H_

#include "stdint.h"

#define ARMV7M_NVIC_PRIO_BITS 4

#ifdef __cplusplus
extern "C"
{
#endif

void armv7m_nvic_enable_irq(int32_t irq);

void armv7m_nvic_disable_irq(int32_t irq);

void armv7m_nvic_set_priority(int32_t irq, uint8_t priority);

uint8_t armv7m_nvic_get_priority(int32_t irq);

void armv7m_nvic_set_pending(int32_t irq);

uint8_t armv7m_nvic_get_pending(int32_t irq);

uint8_t armv7m_nvic_get_active(int32_t irq);

void armv7m_nvic_clear_pending(int32_t irq);

#ifdef __cplusplus
}
#endif


#endif /* ARMV7M_NVIC_H_ */
