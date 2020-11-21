/*
 * armv7m_svcall.h
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#ifndef ARMV7M_ARMV7M_SVCALL_H_
#define ARMV7M_ARMV7M_SVCALL_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "stdint.h"

static inline uint32_t armv7m_svcall_0(uint32_t routine)
{
    register uint32_t _a0 __asm__("r0");
    register uint32_t _routine __asm__("r12") = routine;

    __asm__ volatile ("svc 0"
		      : "=r" (_a0)
		      : "r" (_routine)
		      : "memory");

    return _a0;
}

static inline uint32_t armv7m_svcall_1(uint32_t routine, uint32_t a0)
{
    register uint32_t _a0 __asm__("r0") = a0;
    register uint32_t _routine __asm__("r12") = routine;

    __asm__ volatile ("svc 0"
		      : "+r" (_a0)
		      : "r" (_routine)
		      : "memory");

    return _a0;
}

static inline uint32_t armv7m_svcall_2(uint32_t routine, uint32_t a0, uint32_t a1)
{
    register uint32_t _a0 __asm__("r0") = a0;
    register uint32_t _a1 __asm__("r1") = a1;
    register uint32_t _routine __asm__("r12") = routine;

    __asm__ volatile ("svc 0"
		      : "+r" (_a0)
		      : "r" (_routine), "r" (_a1)
		      : "r2", "r3", "memory");

    return _a0;
}

static inline uint32_t armv7m_svcall_3(uint32_t routine, uint32_t a0, uint32_t a1, uint32_t a2)
{
    register uint32_t _a0 __asm__("r0") = a0;
    register uint32_t _a1 __asm__("r1") = a1;
    register uint32_t _a2 __asm__("r2") = a2;
    register uint32_t _routine __asm__("r12") = routine;

    __asm__ volatile ("svc 0"
		      : "+r" (_a0)
		      : "r" (_routine), "r" (_a1), "r" (_a2)
		      : "r3", "memory");

    return _a0;
}

static inline uint32_t armv7m_svcall_4(uint32_t routine, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)
{
    register uint32_t _a0 __asm__("r0") = a0;
    register uint32_t _a1 __asm__("r1") = a1;
    register uint32_t _a2 __asm__("r2") = a2;
    register uint32_t _a3 __asm__("r3") = a3;
    register uint32_t _routine __asm__("r12") = routine;

    __asm__ volatile ("svc 0"
		      : "+r" (_a0)
		      : "r" (_routine), "r" (_a1), "r" (_a2), "r" (_a3)
		      : "memory");

    return _a0;
}

extern void armv7m_svcall_initialize(void);

extern void SVC_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_SVCALL_H_ */
