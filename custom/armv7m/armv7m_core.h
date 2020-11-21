/*
 * armv7m_core.h
 *
 *  Created on: Nov 16, 2020
 *      Author: zoltan
 */

#ifndef ARMV7M_ARMV7M_CORE_H_
#define ARMV7M_ARMV7M_CORE_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void armv7m_core_yield(void) {
	/* This odd aequence seems to be required for at least STM32L4. Traces on the logic analyzer
	 * showed that after blocking on wfe, then the subsequent wfe would not block. The only WAR
	 * is to explicitly clear the EVENT flag via the SEV; WFE sequence.
	 */
	__asm__ volatile ("wfe; sev; wfe");
}

__attribute__((always_inline)) static inline void armv7m_core_enable_irq(void) {
	__asm volatile ("cpsie i" : : : "memory");
}

__attribute__((always_inline)) static inline void armv7m_core_disable_irq(void) {
	__asm volatile ("cpsid i" : : : "memory");
}

__attribute__( ( always_inline ))static inline uint32_t armv7m_core_get_primask(void) {
	uint32_t result;
	__asm volatile ("MRS %0, primask" : "=r" (result) );
	return (result);
}

__attribute__( ( always_inline ) ) static inline void armv7m_core_set_primask(
		uint32_t priMask) {
	__asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

__attribute__( ( always_inline ))static inline uint32_t armv7m_core_get_ipsr(void) {
	uint32_t result;
	__asm volatile ("MRS %0, ipsr" : "=r" (result) );
	return (result);
}

__attribute__( ( always_inline ))static inline uint32_t armv7m_core_get_basepri(void) {
	uint32_t result;

	__asm volatile ("MRS %0, basepri" : "=r" (result) );
	return (result);
}

__attribute__( ( always_inline ) ) static inline void armv7m_core_set_basepri(
		uint32_t value) {
	__asm volatile ("MSR basepri, %0" : : "r" (value) : "memory");
}

__attribute__( ( always_inline ) ) static inline void armv7m_core_set_basepri_max(
		uint32_t value) {
	__asm volatile ("MSR basepri_max, %0" : : "r" (value) : "memory");
}

__attribute__( ( always_inline )) static inline uint32_t armv7m_core_get_faultmask(void) {
	uint32_t result;

	__asm volatile ("MRS %0, faultmask" : "=r" (result) );
	return (result);
}

__attribute__( ( always_inline ) ) static inline void armv7m_core_set_faultmask(
		uint32_t faultMask) {
	__asm volatile ("MSR faultmask, %0" : : "r" (faultMask) : "memory");
}

extern int armv7m_core_priority(void);

extern void armv7m_core_udelay(uint32_t udelay);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_CORE_H_ */
