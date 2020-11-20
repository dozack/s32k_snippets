/*
 * gpio_config.h
 *
 *  Created on: Nov 4, 2020
 *      Author: zoltan
 */

#ifndef S32K144_GPIO_GPIO_CONFIG_H_
#define S32K144_GPIO_GPIO_CONFIG_H_

#include "../s32k144_gpio/gpio_types.h"

typedef struct {
    gpio_pin_t pin;
    gpio_pin_mode_t mode;
    gpio_pin_direction_t dir;
} gpio_pin_config_t;

typedef struct {
    uint32_t pin_count;
    gpio_pin_config_t *pconfig;
} gpio_config_t;

extern gpio_config_t g_gpio_init_cfg;

#endif /* S32K144_GPIO_GPIO_CONFIG_H_ */
