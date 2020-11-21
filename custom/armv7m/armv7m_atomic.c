/*
 * armv7m_atomic.c
 *
 *  Created on: Nov 21, 2020
 *      Author: zdole
 */

#include "armv7m_atomic.h"

uint32_t armv7m_atomic_load(volatile uint32_t *p_data) {
	return __atomic_load_n(p_data, __ATOMIC_RELAXED);
}

void armv7m_atomic_store(volatile uint32_t *p_data, uint32_t data) {
	__atomic_store_n(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_exchange(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_exchange_n(p_data, data, __ATOMIC_RELAXED);
}

bool armv7m_atomic_compare_exchange(volatile uint32_t *p_data,
		uint32_t *p_data_expected, uint32_t data) {
	return __atomic_compare_exchange_n(p_data, p_data_expected, data, false,
	__ATOMIC_RELAXED, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_add(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_fetch_add(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_sub(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_fetch_sub(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_and(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_fetch_and(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_or(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_fetch_or(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_xor(volatile uint32_t *p_data, uint32_t data) {
	return __atomic_fetch_xor(p_data, data, __ATOMIC_RELAXED);
}

uint32_t armv7m_atomic_modify(volatile uint32_t *p_data, uint32_t mask,
		uint32_t data) {
	uint32_t o_data, n_data;

	o_data = *p_data;

	do {
		n_data = (o_data & ~mask) | (data & mask);
	} while (!__atomic_compare_exchange_n(p_data, &o_data, n_data, false,
	__ATOMIC_RELAXED, __ATOMIC_RELAXED));

	return o_data;
}

