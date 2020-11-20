/*
 * armv7m_core.h
 *
 *  Created on: Nov 16, 2020
 *      Author: zoltan
 */

#ifndef ARMV7M_ARMV7M_CORE_H_
#define ARMV7M_ARMV7M_CORE_H_

__attribute__((always_inline)) static inline void __enable_irq(void) {
    __asm volatile ("cpsie i" : : : "memory");
}

__attribute__((always_inline)) static inline void __disable_irq(void) {
    __asm volatile ("cpsid i" : : : "memory");
}

__attribute__( ( always_inline )) static inline uint32_t __get_PRIMASK(void) {
    uint32_t result;
    __asm volatile ("MRS %0, primask" : "=r" (result) );
    return (result);
}

__attribute__( ( always_inline ) ) static inline void __set_PRIMASK(
        uint32_t priMask) {
    __asm volatile ("MSR primask, %0" : : "r" (priMask) : "memory");
}

#endif /* ARMV7M_ARMV7M_CORE_H_ */
