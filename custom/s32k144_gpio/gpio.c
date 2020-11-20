/*
 * gpio.c
 *
 *  Created on: Nov 4, 2020
 *      Author: zoltan
 */

#include "../s32k144_gpio/gpio.h"

#include "S32K144.h"

void gpio_init(gpio_config_t *pcfg) {
    gpio_pin_t pin;
    gpio_pin_direction_t dir;
    gpio_pin_mode_t mode;
    for (uint32_t i = 0; i < pcfg->pin_count; i++)
    {
        pin = pcfg->pconfig[i].pin;
        dir = pcfg->pconfig[i].dir;
        mode = pcfg->pconfig[i].mode;
        gpio_set_pin_direction(pin, dir);
        gpio_set_pin_mode(pin, mode);
    }
}

void gpio_set_pin_direction(gpio_pin_t pin, gpio_pin_direction_t dir) {
    GPIO_Type *gpio;
    uint32_t group, index, pddr;
    group = (pin & GPIO_PIN_GROUP_MASK ) >> GPIO_PIN_GROUP_SHIFT;
    index = (pin & GPIO_PIN_INDEX_MASK ) >> GPIO_PIN_INDEX_SHIFT;
    gpio = (GPIO_Type*) (PTA_BASE + ((PTB_BASE - PTA_BASE) * group));
    if (dir == GPIO_DIRECTION_HIGH_Z)
    {
        uint32_t pidr = gpio->PIDR;
        pidr |= (1 << index);
        dir = GPIO_DIRECTION_INPUT;
    }
    pddr = gpio->PDDR;
    pddr &= ~(1 << index);
    pddr |= (dir << index);
    gpio->PDDR = pddr;
}

void gpio_set_pin_mode(gpio_pin_t pin, gpio_pin_mode_t mode) {
    PORT_Type *port;
    uint32_t group, index, pcr;
    group = (pin & GPIO_PIN_GROUP_MASK ) >> GPIO_PIN_GROUP_SHIFT;
    index = (pin & GPIO_PIN_INDEX_MASK ) >> GPIO_PIN_INDEX_SHIFT;
    port = (PORT_Type*) (PORTA_BASE + ((PORTB_BASE - PORTA_BASE) * group));
    pcr = port->PCR[index];
    pcr &= ~(PORT_PCR_MUX_MASK | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK
            | PORT_PCR_DSE_MASK);
    pcr |= PORT_PCR_MUX((mode & GPIO_MUX_MASK) >> GPIO_MUX_SHIFT);
    pcr |= PORT_PCR_PE((mode & GPIO_PULLEN_MASK) >> GPIO_PULLEN_SHIFT);
    pcr |= PORT_PCR_PS((mode & GPIO_PULL_MASK) >> GPIO_PULL_SHIFT);
    pcr |= PORT_PCR_DSE((mode & GPIO_DRIVE_MASK) >> GPIO_DRIVE_SHIFT);
    port->PCR[index] = pcr;
}

gpio_pin_level_t gpio_read_pin(gpio_pin_t pin) {
    GPIO_Type *gpio;
    uint32_t group, index;
    group = (pin & GPIO_PIN_GROUP_MASK ) >> GPIO_PIN_GROUP_SHIFT;
    index = (pin & GPIO_PIN_INDEX_MASK ) >> GPIO_PIN_INDEX_SHIFT;
    gpio = (GPIO_Type*) (PTA_BASE + ((PTB_BASE - PTA_BASE) * group));
    return (gpio_pin_level_t) ((gpio->PDIR >> index) & 1);
}

void gpio_write_pin(gpio_pin_t pin, gpio_pin_level_t level) {
    GPIO_Type *gpio;
    uint32_t group, index;
    group = (pin & GPIO_PIN_GROUP_MASK ) >> GPIO_PIN_GROUP_SHIFT;
    index = (pin & GPIO_PIN_INDEX_MASK ) >> GPIO_PIN_INDEX_SHIFT;
    gpio = (GPIO_Type*) (PTA_BASE + ((PTB_BASE - PTA_BASE) * group));
    if (level > GPIO_LEVEL_LOW)
    {
        gpio->PSOR = (1 << index);
    } else
    {
        gpio->PCOR = (1 << index);
    }
}
