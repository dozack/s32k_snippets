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

#include "stdio.h"

#include "armv7m_systick.h"
#include "armv7m_nvic.h"
#include "armv7m_core.h"
#include "armv7m_pendsv.h"

#include "S32K144.h"
#include "system_S32K144.h"

typedef struct _armv7m_systick_control_t {
	volatile uint64_t micros;
	volatile uint64_t millis;
	uint32_t clock;
	uint32_t cycle;
	uint32_t frac;
	uint32_t accum;
	uint32_t scale;
	armv7m_systick_callback_t callback;
	void *context;
} armv7m_systick_control_t;

static armv7m_systick_control_t armv7m_systick_control;

uint64_t armv7m_systick_millis(void) {
	return armv7m_systick_control.millis;
}

uint64_t armv7m_systick_micros(void) {
	uint64_t micros;
	uint32_t count;

	do {
		micros = armv7m_systick_control.micros;
		count = S32_SysTick->CVR;
	} while (micros != armv7m_systick_control.micros);

	micros += ((((armv7m_systick_control.cycle - 1) - count)
			* armv7m_systick_control.scale) >> 22);

	return micros;
}

void armv7m_systick_delay(uint32_t delay) {
	uint64_t millis;

	millis = armv7m_systick_control.millis;

	do {
		armv7m_core_yield();
	} while ((armv7m_systick_control.millis - millis) < delay);
}

void armv7m_systick_notify(armv7m_systick_callback_t callback, void *context) {
	armv7m_systick_control.callback = NULL;
	armv7m_systick_control.context = context;
	armv7m_systick_control.callback = callback;
}

void armv7m_systick_initialize(unsigned int priority) {
	armv7m_nvic_set_priority(SysTick_IRQn, priority);

	S32_SysTick->CSR |= S32_SysTick_CSR_TICKINT_MASK;
}

void armv7m_systick_enable(void) {
	uint32_t count, cycle;

	if (armv7m_systick_control.clock != SystemCoreClock) {
		cycle = armv7m_systick_control.cycle;
		count = (armv7m_systick_control.cycle - 1) - S32_SysTick->CVR;

		armv7m_systick_control.clock = SystemCoreClock;
		armv7m_systick_control.cycle = armv7m_systick_control.clock / 1000;
		armv7m_systick_control.frac = armv7m_systick_control.clock
				- (armv7m_systick_control.cycle * 1000);
		armv7m_systick_control.accum = 0;

		S32_SysTick->CVR = (armv7m_systick_control.cycle - 1)
				- ((count * armv7m_systick_control.cycle) / cycle);
		S32_SysTick->RVR = (armv7m_systick_control.cycle - 1);
		S32_SysTick->CSR |= (S32_SysTick_CSR_CLKSOURCE_MASK
				| S32_SysTick_CSR_ENABLE_MASK);

		/* To get from the current counter to the microsecond offset,
		 * the ((cycle - 1) - Systick->CVR) value is scaled so that the resulting
		 * microseconds fit into the upper 10 bits of a 32bit value. Then
		 * this is post diveded by 2^22. That ensures proper scaling.
		 */
		armv7m_systick_control.scale = (uint64_t) 4194304000000ull
				/ (uint64_t) armv7m_systick_control.clock;
	} else {
		S32_SysTick->CSR |= (S32_SysTick_CSR_CLKSOURCE_MASK
				| S32_SysTick_CSR_ENABLE_MASK);
	}
}

void armv7m_systick_disable(void) {
	S32_SysTick->CSR &= ~S32_SysTick_CSR_ENABLE_MASK;
}

void S32_SysTick_Handler(void) {
	armv7m_systick_control.micros += 1000;
	armv7m_systick_control.millis += 1;

	/* If SYSCLK is driven throu MSI with LSE PLL then the frequency
	 * is not a multiple of 1000. Hence use a fractional scheme to
	 * distribute the fractional part.
	 */
	if (armv7m_systick_control.frac) {
		armv7m_systick_control.accum += armv7m_systick_control.frac;

		if (armv7m_systick_control.accum >= 1000) {
			armv7m_systick_control.accum -= 1000;

			S32_SysTick->RVR = (armv7m_systick_control.cycle - 1) + 1;
		} else {
			S32_SysTick->RVR = (armv7m_systick_control.cycle - 1);
		}
	}

	if (armv7m_systick_control.callback) {
		armv7m_pendsv_enqueue(
				(armv7m_pendsv_routine_t) armv7m_systick_control.callback,
				armv7m_systick_control.context,
				(uint32_t) armv7m_systick_control.millis);
	}
}

