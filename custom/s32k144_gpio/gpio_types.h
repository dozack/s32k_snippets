/*
 * gpio_types.h
 *
 *  Created on: Nov 4, 2020
 *      Author: zoltan
 */

#ifndef S32K144_GPIO_GPIO_TYPES_H_
#define S32K144_GPIO_GPIO_TYPES_H_

#include "stdint.h"
#include "stdbool.h"

typedef uint32_t gpio_pin_t;
typedef uint32_t gpio_pin_mode_t;
typedef uint32_t gpio_pin_level_t;
typedef uint32_t gpio_pin_direction_t;

/* gpio_pin_t defines */
#define GPIO_PIN_GROUP_MASK             (uint32_t)(0x000f)
#define GPIO_PIN_GROUP_SHIFT            (uint32_t)(0x0000)
#define GPIO_PIN_GROUP_A                (gpio_pin_t)(0x0000)
#define GPIO_PIN_GROUP_B                (gpio_pin_t)(0x0001)
#define GPIO_PIN_GROUP_C                (gpio_pin_t)(0x0002)
#define GPIO_PIN_GROUP_D                (gpio_pin_t)(0x0003)
#define GPIO_PIN_GROUP_E                (gpio_pin_t)(0x0004)

#define GPIO_PIN_INDEX_MASK             (uint32_t)(0x0ff0)
#define GPIO_PIN_INDEX_SHIFT            (uint32_t)(0x0004)
#define GPIO_PIN_0                      (gpio_pin_t)(0x0000)
#define GPIO_PIN_1                      (gpio_pin_t)(0x0010)
#define GPIO_PIN_2                      (gpio_pin_t)(0x0020)
#define GPIO_PIN_3                      (gpio_pin_t)(0x0030)
#define GPIO_PIN_4                      (gpio_pin_t)(0x0040)
#define GPIO_PIN_5                      (gpio_pin_t)(0x0050)
#define GPIO_PIN_6                      (gpio_pin_t)(0x0060)
#define GPIO_PIN_7                      (gpio_pin_t)(0x0070)
#define GPIO_PIN_8                      (gpio_pin_t)(0x0080)
#define GPIO_PIN_9                      (gpio_pin_t)(0x0090)
#define GPIO_PIN_10                     (gpio_pin_t)(0x00a0)
#define GPIO_PIN_11                     (gpio_pin_t)(0x00b0)
#define GPIO_PIN_12                     (gpio_pin_t)(0x00c0)
#define GPIO_PIN_13                     (gpio_pin_t)(0x00d0)
#define GPIO_PIN_14                     (gpio_pin_t)(0x00e0)
#define GPIO_PIN_15                     (gpio_pin_t)(0x00f0)
#define GPIO_PIN_16                     (gpio_pin_t)(0x0100)
#define GPIO_PIN_17                     (gpio_pin_t)(0x0110)

#define GPIO_PIN(port, pin)             (gpio_pin_t)((port << GPIO_PIN_GROUP_SHIFT) | (pin << GPIO_PIN_INDEX_SHIFT))

/* gpio_mode_t defines */
#define GPIO_MUX_MASK                   (uint32_t)(0x000f)
#define GPIO_MUX_SHIFT                  (uint32_t)(0x0000)
#define GPIO_MUX_ANALOG                 (uint32_t)(0x00 << GPIO_MUX_SHIFT)
#define GPIO_MUX_GPIO                   (uint32_t)(0x01 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF2                    (uint32_t)(0x02 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF3                    (uint32_t)(0x03 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF4                    (uint32_t)(0x04 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF5                    (uint32_t)(0x05 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF6                    (uint32_t)(0x06 << GPIO_MUX_SHIFT)
#define GPIO_MUX_AF7                    (uint32_t)(0x07 << GPIO_MUX_SHIFT)

#define GPIO_PULLEN_MASK                (uint32_t)(0x00f0)
#define GPIO_PULLEN_SHIFT               (uint32_t)(0x0004)
#define GPIO_PULLEN_OFF                 (uint32_t)(0x00 << GPIO_PULLEN_SHIFT)
#define GPIO_PULLEN_ON                  (uint32_t)(0x01 << GPIO_PULLEN_SHIFT)

#define GPIO_PULL_MASK                  (uint32_t)(0x0f00)
#define GPIO_PULL_SHIFT                 (uint32_t)(0x0008)
#define GPIO_PULL_DOWN                  (uint32_t)(0x00 << GPIO_PULL_SHIFT)
#define GPIO_PULL_UP                    (uint32_t)(0x01 << GPIO_PULL_SHIFT)

#define GPIO_DRIVE_MASK                 (uint32_t)(0xf000)
#define GPIO_DRIVE_SHIFT                (uint32_t)(0x000c)
#define GPIO_DRIVE_LOW                  (uint32_t)(0x00 << GPIO_DRIVE_SHIFT)
#define GPIO_DRIVE_HIGH                 (uint32_t)(0x01 << GPIO_DRIVE_SHIFT)

#define GPIO_MODE(mx, pen, pse, dse)    (gpio_pin_mode_t)((mx << GPIO_MUX_SHIFT) | (pen << GPIO_PULLEN_SHIFT) | (pse << GPIO_PULL_SHIFT) | (dse << GPIO_DRIVE_SHIFT))

/* gpio_pin_level_t defines */
#define GPIO_LEVEL_LOW                  (gpio_pin_level_t)(0x00)
#define GPIO_LEVEL_HIGH                 (gpio_pin_level_t)(0x01)

/* gpio_direction_t defines */
#define GPIO_DIRECTION_INPUT            (gpio_pin_direction_t)(0x000)
#define GPIO_DIRECTION_OUTPUT           (gpio_pin_direction_t)(0x001)
#define GPIO_DIRECTION_HIGH_Z           (gpio_pin_direction_t)(0x002)

#endif /* S32K144_GPIO_GPIO_TYPES_H_ */
