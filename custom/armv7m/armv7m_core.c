/*
 * armv7m_core.c
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#include "armv7m_core.h"
#include "armv7m_nvic.h"

#include "system_S32K144.h"

typedef struct _armv7m_core_control_t {
	uint32_t clock;
	uint32_t scale;
} armv7m_core_control_t;

static armv7m_core_control_t armv7m_core_control;

int armv7m_core_priority(void) {
	uint32_t ipsr, faultmask, primask;
	int priority, basepri;
	ipsr = armv7m_core_get_ipsr();
	if (ipsr == 0) {
		priority = 256 >> (8 - ARMV7M_NVIC_PRIO_BITS);
	} else {
		if (ipsr >= 4) {
			priority = armv7m_nvic_get_priority((int) ipsr - 16);
		} else {
			priority = (int) ipsr - 4; /* -1 for HardFault, and -2 for NMI */
		}
	}
	faultmask = armv7m_core_get_faultmask();
	if (faultmask) {
		if (priority > -1) {
			priority = -1;
		}
	} else {
		primask = armv7m_core_get_primask();

		if (primask) {
			if (priority > 0) {
				priority = 0;
			}
		} else {
			basepri = armv7m_core_get_basepri() >> (8 - ARMV7M_NVIC_PRIO_BITS);

			if (basepri) {
				if (priority > basepri) {
					priority = basepri;
				}
			}
		}
	}
	return priority;
}

void armv7m_core_udelay(uint32_t delay) {
	uint32_t n;
	if (armv7m_core_control.clock != SystemCoreClock) {
		armv7m_core_control.clock = SystemCoreClock;
		armv7m_core_control.scale = SystemCoreClock / 1000000;
	}
	n = (delay * armv7m_core_control.scale + 2) / 3;
	__asm__ __volatile__(
			"1: subs %0, #1 \n"
			"   bne  1b     \n"
			: "+r" (n));
}

