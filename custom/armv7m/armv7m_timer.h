/*
 * armv7m_timer.h
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#ifndef ARMV7M_ARMV7M_TIMER_H_
#define ARMV7M_ARMV7M_TIMER_H_

#include "stdint.h"
#include "stdbool.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _armv7m_timer_t armv7m_timer_t;

typedef void (*armv7m_timer_callback_t)(armv7m_timer_t *timer);

struct _armv7m_timer_t {
	armv7m_timer_t *next;
	armv7m_timer_t *previous;
	volatile armv7m_timer_callback_t callback;
	uint32_t remaining;
};

#define ARMV7M_TIMER_INIT(_callback,_timeout) { NULL, NULL, (_callback), (_timeout) }

extern void armv7m_timer_create(armv7m_timer_t *timer,
		armv7m_timer_callback_t callback);

extern bool armv7m_timer_start(armv7m_timer_t *timer, uint32_t timeout);

extern bool armv7m_timer_stop(armv7m_timer_t *timer);

extern void armv7m_timer_initialize(void);

#ifdef __cplusplus
}
#endif

#endif /* ARMV7M_ARMV7M_TIMER_H_ */
