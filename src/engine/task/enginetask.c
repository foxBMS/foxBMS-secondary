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
 * @file    enginetask.c
 * @author  foxBMS Team
 * @date    27.08.2015 (date of creation)
 * @ingroup ENGINE
 * @prefix  ENG
 *
 * @brief   Calls of functions within engine task
 */

/*================== Includes =============================================*/
#include "enginetask.h"
#include "ltc.h"
#include "syscontrol.h"
#include "bmsctrl.h"
#include "can.h"
#include "cansignal.h"
#include "led.h"
#include "sox.h"
#include "wdg.h"
#include "intermcu.h"

/*================== Macros and Definitions ===============================*/


/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

void ENG_Init(void) {
    //ISO_Init();
    //SOF_Init();

}

void ENG_TSK_Cyclic_1ms(void) {
    BMSCTRL_Trigger();
    LTC_Ctrl(LTC_HAS_TO_MEASURE);
    LTC_Trigger();
}

void ENG_TSK_Cyclic_10ms(void) {
    static uint8_t BMS_init = 0;

    //SYSCTRL_Trigger(SYS_MODE_CYCLIC_EVENT);
    BMSCTRL_Ctrl();
    //CANS_MainFunction();
#if CAN_USE_CAN_NODE0
    //CAN_TxMsgBuffer(CAN_NODE0);
#endif
#if CAN_USE_CAN_NODE1
    //CAN_TxMsgBuffer(CAN_NODE1);
#endif

#if BUILD_MODULE_ENABLE_SAFETY_FEATURES == 0
    LED_Ctrl();
#endif

    //SOC_Ctrl();
    //SOF_Ctrl();

    if (BMS_init == 0) {
        BMSCTRL_SetStateRequest(BMSCTRL_STATE_IDLE_REQUEST);
        BMS_init = 1;
    }


#if BUILD_MODULE_ENABLE_WATCHDOG
   WDG_IWDG_Refresh();
#endif
}

void ENG_TSK_Cyclic_100ms(void) {
    static uint8_t counter = 0;
    uint8_t imc_receive = 0;

    //ADC_Ctrl();
    //BAL_Ctrl();

    // Read every 200ms because of possible jitter and lowest Bender frequency 10Hz -> 100ms
    //if(counter % 2 == 0) {
        //ISO_MeasureInsulation();
    //}
    // FIXME evaluate if better to put this in ENG_TSK_INIT?
    //if (BMS_init == 0) {
    //    BMSCTRL_SetStateRequest(BMSCTRL_STATE_IDLE_REQUEST);
    //    BMS_init = 1;
    //}
    //counter++;

#if BUILD_MODULE_ENABLE_SAFETY_FEATURES == 1 && MCU_SLAVE == 1

    if(counter % 5 == 0) {
        IO_TogglePin(PIN_MCU_1_DEBUG_LED_0);
        if(IO_ReadPin(PIN_MCU_1_DEBUG_LED_0)){
            IMC_sendByte(0x99);
        } else {
            IMC_sendByte(0x66);
        }

        imc_receive = spi_devices[2].Instance->DR;

        if(imc_receive == 0x33) {
            IO_WritePin(PIN_MCU_1_DEBUG_LED_1, RESET);
        }

        if(imc_receive == 0x55) {
            IO_WritePin(PIN_MCU_1_DEBUG_LED_1, SET);
        }

    }
#endif

    counter++;
}
	
void ENG_TSK_EventHandler(void) {
    ;
}

void ENG_TSK_Diagnosis(void) {
    ;
}
