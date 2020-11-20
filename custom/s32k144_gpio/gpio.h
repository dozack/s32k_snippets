/*
 * gpio.h
 *
 *  Created on: Nov 4, 2020
 *      Author: zoltan
 */

#ifndef S32K144_GPIO_GPIO_H_
#define S32K144_GPIO_GPIO_H_

#include "../s32k144_gpio/gpio_config.h"

#ifdef __cplusplus
extern "C"
{
#endif

void gpio_init(gpio_config_t *pcfg);

void gpio_set_pin_direction(gpio_pin_t pin, gpio_pin_direction_t dir);

void gpio_set_pin_mode(gpio_pin_t pin, gpio_pin_mode_t mode);

gpio_pin_level_t gpio_read_pin(gpio_pin_t pin);

void gpio_write_pin(gpio_pin_t pin, gpio_pin_level_t level);

#ifdef __cplusplus
}
#endif

#endif /* S32K144_GPIO_GPIO_H_ */
