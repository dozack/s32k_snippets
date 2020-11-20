/* ###################################################################
 **     Filename    : main.c
 **     Processor   : S32K1xx
 **     Abstract    :
 **         Main module.
 **         This module contains user's application code.
 **     Settings    :
 **     Contents    :
 **         No public methods
 **
 ** ###################################################################*/
/*!
 ** @file main.c
 ** @version 01.00
 ** @brief
 **         Main module.
 **         This module contains user's application code.
 */
/*!
 **  @addtogroup main_module main module documentation
 **  @{
 */
/* MODULE main */

/* Including necessary module. Cpu.h contains other modules needed for compiling.*/
#include "Cpu.h"

volatile int exit_code = 0;

/* User includes (#include below this line is not maintained by Processor Expert) */
#include "gpio.h"
#include "armv7m.h"
#include "s32k144_can.h"
/*! 
 \brief The main function for the project.
 \details The startup initialization sequence is the following:
 * - startup asm routine
 * - main()
 */
int main(void) {
    /* Write your local variable definition here */

    /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
#ifdef PEX_RTOS_INIT
    PEX_RTOS_INIT(); /* Initialization of the selected RTOS. Macro is defined by the RTOS component. */
#endif
    /*** End of Processor Expert internal initialization.                    ***/
    /* Write your code here */
    s32k144_can_t test_can;
    s32k144_can_cfg_t test_can_config = {
            .clock_source = CAN_CLK_PERIPH,
            .mode = CAN_MODE_NORMAL,
            .timing = {
                    .prop_seg = 7,
                    .phase_seg1 = 4,
                    .phase_seg2 = 1,
                    .pre_divider = 5,
                    .rjump_width = 1 } };

    s32k144_can_create(&test_can, CAN_INSTANCE_CAN0, &test_can_config);
    s32k144_can_enable(&test_can);
    s32k144_can_disable(&test_can);
    s32k144_can_destroy(&test_can);

    module_clk_config_t led_config = {
            .source = CLK_SRC_SOSC,
            .gating = true,
            .div = DIVIDE_BY_ONE,
            .mul = MULTIPLY_BY_ONE };
    CLOCK_DRV_SetModuleClock(PORTC_CLK, &led_config);
    module_clk_config_t sw2_config = {
            .source = CLK_SRC_SOSC,
            .gating = true,
            .div = DIVIDE_BY_ONE,
            .mul = MULTIPLY_BY_ONE };

    CLOCK_DRV_SetModuleClock(PORTD_CLK, &sw2_config);

    gpio_init(&g_gpio_init_cfg);
    gpio_pin_t led = g_gpio_init_cfg.pconfig[0].pin;
    gpio_pin_t button = g_gpio_init_cfg.pconfig[1].pin;
    /* For example: for(;;) { } */
    while (1)
    {
        gpio_pin_level_t button_level = gpio_read_pin(button);
        if (GPIO_LEVEL_HIGH == button_level)
        {
            gpio_write_pin(led, GPIO_LEVEL_LOW);
        } else
        {
            gpio_write_pin(led, GPIO_LEVEL_HIGH);
        }
    }
    /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** RTOS startup code. Macro PEX_RTOS_START is defined by the RTOS component. DON'T MODIFY THIS CODE!!! ***/
  #ifdef PEX_RTOS_START
    PEX_RTOS_START();                  /* Startup of the selected RTOS. Macro is defined by the RTOS component. */
  #endif
  /*** End of RTOS startup code.  ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;) {
    if(exit_code != 0) {
      break;
    }
  }
  return exit_code;
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END main */
/*!
 ** @}
 */
/*
 ** ###################################################################
 **
 **     This file was created by Processor Expert 10.1 [05.21]
 **     for the NXP S32K series of microcontrollers.
 **
 ** ###################################################################
 */
