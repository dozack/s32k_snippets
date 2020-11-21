/*
 * armv7m_pendsv.c
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#include "armv7m_pendsv.h"
#include "armv7m_atomic.h"
#include "armv7m_nvic.h"

#include "S32K144.h"

typedef struct _armv7m_pendsv_entry_t {
	armv7m_pendsv_routine_t routine;
	void *context;
	uint32_t data;
} armv7m_pendsv_entry_t;

typedef struct _armv7m_pendsv_control_t {
	volatile armv7m_pendsv_entry_t *pendsv_read;
	volatile armv7m_pendsv_entry_t *pendsv_write;
	armv7m_pendsv_entry_t pendsv_data[ARMV7M_PENDSV_ENTRY_COUNT];
} armv7m_pendsv_control_t;

static armv7m_pendsv_control_t armv7m_pendsv_control;

volatile armv7m_pendsv_routine_t * armv7m_pendsv_enqueue(
		armv7m_pendsv_routine_t routine, void *context, uint32_t data) {
	volatile armv7m_pendsv_entry_t *pendsv_write, *pendsv_write_next;

	do {
		pendsv_write = armv7m_pendsv_control.pendsv_write;
		pendsv_write_next = pendsv_write + 1;

		if (pendsv_write_next
				== &armv7m_pendsv_control.pendsv_data[ARMV7M_PENDSV_ENTRY_COUNT]) {
			pendsv_write_next = &armv7m_pendsv_control.pendsv_data[0];
		}

		if (pendsv_write_next == armv7m_pendsv_control.pendsv_read) {
			return NULL;
		}
	} while (!armv7m_atomic_compare_exchange(
			(volatile uint32_t*) &armv7m_pendsv_control.pendsv_write,
			(uint32_t*) &pendsv_write, (uint32_t) pendsv_write_next));

	pendsv_write->routine = routine;
	pendsv_write->context = context;
	pendsv_write->data = data;

	S32_SCB->ICSR = S32_SCB_ICSR_PENDSVSET_MASK;

	return &pendsv_write->routine;
}

static __attribute__((used)) void armv7m_pendsv_dequeue(void) {
	volatile armv7m_pendsv_entry_t *pendsv_read;
	armv7m_pendsv_routine_t routine;
	void *context;
	uint32_t data;

	pendsv_read = armv7m_pendsv_control.pendsv_read;

	while (pendsv_read != armv7m_pendsv_control.pendsv_write) {

		routine = pendsv_read->routine;
		context = pendsv_read->context;
		data = pendsv_read->data;

		pendsv_read = pendsv_read + 1;

		if (pendsv_read
				== &armv7m_pendsv_control.pendsv_data[ARMV7M_PENDSV_ENTRY_COUNT]) {
			pendsv_read = &armv7m_pendsv_control.pendsv_data[0];
		}

		armv7m_pendsv_control.pendsv_read = pendsv_read;

		(*routine)(context, data);
	}
}

void armv7m_pendsv_initialize(void) {
	armv7m_pendsv_control.pendsv_read = &armv7m_pendsv_control.pendsv_data[0];
	armv7m_pendsv_control.pendsv_write = &armv7m_pendsv_control.pendsv_data[0];

	armv7m_nvic_set_priority(PendSV_IRQn, ((1 << ARMV7M_NVIC_PRIO_BITS) - 1));
}

void PendSV_Handler(void) {
	armv7m_pendsv_dequeue();
}

