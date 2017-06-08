/**
 *
 * @copyright &copy; 2010 - 2016, Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V. All rights reserved.
 *
 * BSD 3-Clause License
 * Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 * 1.  Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * We kindly request you to use one or more of the following phrases to refer to foxBMS in your hardware, software, documentation or advertising materials:
 *
 * &Prime;This product uses parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product includes parts of foxBMS&reg;&Prime;
 *
 * &Prime;This product is derived from foxBMS&reg;&Prime;
 *
 */

/**
 * @file    timer.c
 * @author  foxBMS Team
 * @date    30.09.2015 (date of creation)
 * @ingroup DRIVERS
 * @prefix  TIM
 *
 * @brief   Driver to enable/disable/change the peripheral timers
 *
 * Source file to enable/disable/change peripheral timers.
 */


/*================== Includes =============================================*/
/* recommended include order of header files:
 * 
 * 1.    include general.h
 * 2.    include module's own header
 * 3...  other headers
 *
 */
#include "general.h"
#include "timer.h"

#include "io.h"
#include "intermcu.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/
void TIM_Init(void) {

#if BUILD_MODULE_ENABLE_SAFETY_FEATURES
    TIM_SW_PWM_OUT_Init();
#endif

}


void TIM_SW_PWM_OUT_Init(void) {

    __TIM3_CLK_ENABLE();

    HAL_TIM_Base_Init(&htim3);

    HAL_TIM_Base_Start_IT(&htim3);
}


void TIM_IRQHandler(void) {

    static uint8_t toggle = 0;

    if(toggle % 2 == 0) {
        // Time until next interrupt: 100us

        htim3.Instance->ARR = 840 - 1;
        IO_TogglePin(IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_0);
        IO_TogglePin(IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_NSS);
    } else {
        // Time until next interrupt : 900us

        htim3.Instance->ARR = 7560 - 1;
        IO_TogglePin(IO_PIN_MCU_1_BMS_INTERFACE_0_GPIO_0);
        IO_TogglePin(IO_PIN_MCU_1_TO_MCU_0_INTERFACE_SPI_NSS);
    }

    // Clear pending interrupt flag
    __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);

    toggle++;

}
