/*
 * armv7m_atomic.h
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#ifndef ARMV7M_ARMV7M_ATOMIC_H_
#define ARMV7M_ARMV7M_ATOMIC_H_

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

extern uint32_t armv7m_atomic_load(volatile uint32_t *p_data);

extern void armv7m_atomic_store(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_exchange(volatile uint32_t *p_data, uint32_t data);

extern bool armv7m_atomic_compare_exchange(volatile uint32_t *p_data,
		uint32_t *p_data_expected, uint32_t data);

extern uint32_t armv7m_atomic_add(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_sub(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_and(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_or(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_xor(volatile uint32_t *p_data, uint32_t data);

extern uint32_t armv7m_atomic_modify(volatile uint32_t *p_data, uint32_t mask,
		uint32_t data);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_ATOMIC_H_ */
