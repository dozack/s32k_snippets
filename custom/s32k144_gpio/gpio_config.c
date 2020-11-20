/*
 * gpio_config.c
 *
 *  Created on: Nov 4, 2020
 *      Author: zoltan
 */

#include "../s32k144_gpio/gpio_config.h"

gpio_pin_config_t board_pins[2] = {
        {
                .pin = (GPIO_PIN_GROUP_D | GPIO_PIN_0 ),
                .dir = GPIO_DIRECTION_OUTPUT,
                .mode = (GPIO_MUX_GPIO | GPIO_PULLEN_ON | GPIO_PULL_DOWN
                        | GPIO_DRIVE_HIGH ) },
        {
                .pin = (GPIO_PIN_GROUP_C | GPIO_PIN_12 ),
                .dir = GPIO_DIRECTION_INPUT,
                .mode = (GPIO_MUX_GPIO | GPIO_PULLEN_ON | GPIO_PULL_UP ) } };

#if 0
gpio_pin_config_t jawa_ecu_pins[17] =
{
    {
        // DI0
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_0 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI1
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_1 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // AI0
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_2 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_ANALOG )},
    {
        // AI1
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_3 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_ANALOG )},
    {
        // JTAG_TMS
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_4 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_AF7 )},
    {
        // DI2
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_5 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // ABS4
        // TODO
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_6 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // ABS3
        // TODO
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_7 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI3
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_8 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI4
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_9 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // JTAG_TDO
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_10 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_AF7 )},
    {
        // DI5
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_11 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI6
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_12 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI7
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_13 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI8
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_15 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI9
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_16 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},
    {
        // DI10
        .pin = (GPIO_PIN_GROUP_A | GPIO_PIN_17 ),
        .dir = GPIO_DIRECTION_INPUT,
        .mode = (GPIO_MUX_GPIO )},};
#endif

gpio_config_t g_gpio_init_cfg = {
        .pin_count = (sizeof(board_pins) / sizeof(board_pins[0])),
        .pconfig = &board_pins[0] };
