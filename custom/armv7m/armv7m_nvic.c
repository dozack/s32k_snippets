/*
 * armv7m_nvic.c
 *
 *  Created on: Nov 16, 2020
 *      Author: zoltan
 */

#include "armv7m_nvic.h"

#include "S32K144.h"
#include "S32K144_features.h"

void armv7m_nvic_enable_irq(int32_t irq) {
    /* Enable interrupt */
    S32_NVIC->ISER[(uint32_t) (irq) >> 5U] = (uint32_t) (1UL << ((uint32_t) (irq) & (uint32_t) 0x1FU));
}

void armv7m_nvic_disable_irq(int32_t irq) {
    /* Disable interrupt */
    S32_NVIC->ICER[((uint32_t) (irq) >> 5U)] = (uint32_t) (1UL << ((uint32_t) (irq) & (uint32_t) 0x1FU));
}

void armv7m_nvic_set_priority(int32_t irq, uint8_t priority) {
    uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);
    if ((int32_t) irq < 0)
    {
        uint32_t intVectorId = ((uint32_t) (irq) & 0xFU);
        uint32_t regId = intVectorId / 4U;
        /* Compute pointer to SHPR register - avoid MISRA violation. */
        volatile uint8_t * shpr_reg_ptr = (
                (regId == 1U) ?
                        (volatile uint8_t *) &S32_SCB->SHPR1 :
                        ((regId == 2U) ? (volatile uint8_t *) &S32_SCB->SHPR2 : (volatile uint8_t *) &S32_SCB->SHPR3));
        /* Set Priority for Cortex-M4 System Interrupts */
        shpr_reg_ptr[intVectorId % 4U] = (uint8_t) (((((uint32_t) priority) << shift)) & 0xffUL);
    } else
    {
        /* Set Priority for device specific Interrupts */
        S32_NVIC->IP[(uint32_t) (irq)] = (uint8_t) (((((uint32_t) priority) << shift)) & 0xFFUL);
    }
}

uint8_t armv7m_nvic_get_priority(int32_t irq) {
    uint8_t priority = 0U;
    uint8_t shift = (uint8_t) (8U - FEATURE_NVIC_PRIO_BITS);
    if ((int32_t) irq < 0)
    {
        uint32_t intVectorId = ((uint32_t) (irq) & 0xFU);
        uint32_t regId = intVectorId / 4U;
        /* Compute pointer to SHPR register - avoid MISRA violation. */
        volatile const uint8_t * shpr_reg_ptr = (
                (regId == 1U) ?
                        (volatile uint8_t *) &S32_SCB->SHPR1 :
                        ((regId == 2U) ? (volatile uint8_t *) &S32_SCB->SHPR2 : (volatile uint8_t *) &S32_SCB->SHPR3));
        /* Get Priority from Cortex-M  System Interrupts */
        priority = (uint8_t) (shpr_reg_ptr[intVectorId % 4U] >> (shift));
    } else
    {
        /* Get Priority for device specific Interrupts  */
        priority = (uint8_t) (S32_NVIC->IP[(uint32_t) (irq)] >> shift);
    }
    return priority;
}

void armv7m_nvic_set_pending(int32_t irq) {
    /* Set Pending Interrupt */
    S32_NVIC->ISPR[(uint32_t) (irq) >> 5U] = (uint32_t) (1UL << ((uint32_t) (irq) & (uint32_t) 0x1FU));
}

uint8_t armv7m_nvic_get_pending(int32_t irq) {
    /* Get Pending Interrupt */
    return ((uint32_t) (
            ((S32_NVIC->ISPR[(((uint32_t) irq) >> 5UL)] & (1UL << (((uint32_t) irq) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

uint8_t armv7m_nvic_get_active(int32_t irq) {
    /* Get Active Interrupt */
    return ((uint32_t) (
            ((S32_NVIC->IABR[(((uint32_t) irq) >> 5UL)] & (1UL << (((uint32_t) irq) & 0x1FUL))) != 0UL) ? 1UL : 0UL));
}

void armv7m_nvic_clear_pending(int32_t irq) {
    /* Clear Pending Interrupt */
    S32_NVIC->ICPR[(uint32_t) (irq) >> 5U] = (uint32_t) (1UL << ((uint32_t) (irq) & (uint32_t) 0x1FU));
}
