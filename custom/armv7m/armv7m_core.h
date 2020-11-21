/*
 * Copyright (c) 2016 Thomas Roell.  All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal with the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 *  1. Redistributions of source code must retain the above copyright notice,
 *     this list of conditions and the following disclaimers.
 *  2. Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimers in the
 *     documentation and/or other materials provided with the distribution.
 *  3. Neither the name of Thomas Roell, nor the names of its contributors
 *     may be used to endorse or promote products derived from this Software
 *     without specific prior written permission.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * WITH THE SOFTWARE.
 */

#ifndef ARMV7M_ARMV7M_CORE_H_
#define ARMV7M_ARMV7M_CORE_H_

#include "stdint.h"

#ifdef __cplusplus
extern "C" {
#endif

static inline void armv7m_core_yield(void) {
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
