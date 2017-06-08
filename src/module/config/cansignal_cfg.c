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
 * @file    cansignal_cfg.c
 * @author  foxBMS Team
 * @date    16.09.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  CANS
 *
 * @brief   Configuration of the messages and signal settings for the CAN driver
 *
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
#include "cansignal_cfg.h"

#include "database.h"
#include "syscontrol.h"
#include "mcu.h"
#include "sox.h"

/*================== Function Prototypes ==================================*/

static uint32_t cans_setmux(uint32_t, void *);
static uint32_t cans_setvolt(uint32_t, void *);
static uint32_t cans_settempering(uint32_t, void *);
static uint32_t cans_setcanerr(uint32_t, void *);
static uint32_t cans_setsocsoh(uint32_t, void *);
static uint32_t cans_setsof(uint32_t, void *);
static uint32_t cans_setcurr(uint32_t, void *);
static uint32_t cans_setminmaxvolt(uint32_t, void *);
static uint32_t cans_setstaterequest(uint32_t, void *);
static uint32_t cans_setdebug(uint32_t, void *);

/*================== Macros and Definitions ===============================*/

const uint32_t cans_signalToMuxMapping[] = {

 };
static uint8_t cans_muxVal[sizeof(cans_signalToMuxMapping)/sizeof(cans_signalToMuxMapping[0])];

static DATA_BLOCK_CURRENT_s cans_current_tab;

/*================== Constant and Variable Definitions ====================*/

const CANS_signal_s cans_CAN0_signals_tx[] = {
};


const CANS_signal_s cans_CAN1_signals_tx[] = {
};


const CANS_signal_s cans_CAN0_signals_rx[] = {
        { {CAN0_MSG_BMS10},   0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_BMS10},   8,  8,    8, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setstaterequest,  NULL_PTR },
        { {CAN0_MSG_ISENS0},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS0},  8,  8,  255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS0}, 16, 32, 0xFFFFFFFF, -3000, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN0_MSG_ISENS1},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS1},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS1}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN0_MSG_ISENS2},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS2},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS2}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN0_MSG_ISENS3},  0,  8,    32, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS3},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN0_MSG_ISENS3}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,     FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setcurr,       NULL_PTR },
        { {CAN0_MSG_DEBUG},   0, 64,  100, 0, 0.001,    0,     FALSE,  FALSE,  FALSE,  {CAN0_SIGNAL_NONE}, &cans_setdebug,      NULL_PTR }
};

const CANS_signal_s cans_CAN1_signals_rx[] = {
        { {CAN1_MSG_BMS10},   0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_BMS10},   8,  8,    8, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setstaterequest,  NULL_PTR },
        { {CAN1_MSG_ISENS0},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS0},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS0}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN1_MSG_ISENS1},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS1},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS1}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN1_MSG_ISENS2},  0,  8,    32, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS2},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS2}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setcurr,          NULL_PTR },
        { {CAN1_MSG_ISENS3},  0,  8,    32, 0, 0.001,    0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS3},  8,  8,    255, 0, 1,        0,  FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, NULL_PTR,               NULL_PTR },
        { {CAN1_MSG_ISENS3}, 16, 32,  0xFFFFFFFF, 0, 0.001,    0,     FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setcurr,       NULL_PTR },
        { {CAN1_MSG_DEBUG},   0, 64,  100, 0, 0.001,    0,     FALSE,  FALSE,  FALSE,  {CAN1_SIGNAL_NONE}, &cans_setdebug,      NULL_PTR }
};


const uint16_t cans_CAN0_signals_tx_length = sizeof(cans_CAN0_signals_tx)/sizeof(cans_CAN0_signals_tx[0]);
const uint16_t cans_CAN1_signals_tx_length = sizeof(cans_CAN1_signals_tx)/sizeof(cans_CAN1_signals_tx[0]);

const uint16_t cans_CAN0_signals_rx_length = sizeof(cans_CAN0_signals_rx)/sizeof(cans_CAN0_signals_rx[0]);
const uint16_t cans_CAN1_signals_rx_length = sizeof(cans_CAN1_signals_rx)/sizeof(cans_CAN1_signals_rx[0]);

/*================== Function Implementations =============================*/

uint32_t cans_setmux(uint32_t sigIdx, void *value) {
    uint32_t i, locMuxIdx =0xFFFFFFFF;
    for (i = 0; i< sizeof(cans_signalToMuxMapping)/sizeof(cans_signalToMuxMapping[0]); i++){
        if (sigIdx == cans_signalToMuxMapping[i]) locMuxIdx=i;
    }
    if(value != NULL_PTR) {
        cans_muxVal[locMuxIdx] = *(uint8_t*) value;
    }
    return 0;
}


uint32_t cans_setvolt(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_settempering(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_setcanerr(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_setsocsoh(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_setsof(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_setminmaxvolt(uint32_t sigIdx, void *value) {
    static uint32_t access_dummy;
    if(value != NULL_PTR) {
        *(uint32_t *)value = access_dummy;
    }
    access_dummy++;
    return 0;
}


uint32_t cans_setcurr(uint32_t sigIdx, void *value) {
    int32_t currentValue;
    int32_t voltageValue[3];
    uint32_t idx =0;
    uint8_t dummy[4] = {0,0,0,0};
    dummy[0] = *(uint32_t *)value & 0x000000FF;
    dummy[1] = (*(uint32_t *)value & 0x0000FF00) >> 8;
    dummy[2] = (*(uint32_t *)value & 0x00FF0000) >> 16;
    dummy[3] = (*(uint32_t *)value & 0xFF000000) >> 24;

    if(value != NULL_PTR) {
        switch(sigIdx){
            case CAN0_SIG_ISENS0_I_Measurement:
            // case CAN1_SIG_ISENS0_I_Measurement:  uncommented because identical position in CAN0 and CAN1 rx signal struct
                currentValue = (int32_t)(dummy[3] | dummy[2] << 8
                        | dummy[1] << 16 | dummy[0] << 24);
                cans_current_tab.previous_timestamp = cans_current_tab.timestamp;
                cans_current_tab.timestamp = MCU_GetTimeStamp();
                cans_current_tab.current=(float)(currentValue);
                cans_current_tab.state_current++;
                DATA_StoreDataBlock(&cans_current_tab,DATA_BLOCK_ID_CURRENT);
                break;
            case CAN0_SIG_ISENS1_U1_Measurement:
            // case CAN1_SIG_ISENS1_U1_Measurement:  uncommented because identical position in CAN0 and CAN1 rx signal struct
                idx =0;
                voltageValue[idx] = (int32_t)(dummy[3] | dummy[2] << 8
                        | dummy[1] << 16 | dummy[0] << 24);
                cans_current_tab.voltage[idx]=(float)(voltageValue[idx])*cans_CAN0_signals_rx[sigIdx].factor;
                cans_current_tab.state_voltage++;
                DATA_StoreDataBlock(&cans_current_tab,DATA_BLOCK_ID_CURRENT);
                break;
            case CAN0_SIG_ISENS2_U2_Measurement:
            // case CAN1_SIG_ISENS2_U2_Measurement:  uncommented because identical position in CAN0 and CAN1 rx signal struct
                idx = 1;
                voltageValue[idx] = (int32_t)(dummy[3] | dummy[2] << 8
                        | dummy[1] << 16 | dummy[0] << 24);
                cans_current_tab.voltage[idx]=(float)(voltageValue[idx])*cans_CAN0_signals_rx[sigIdx].factor;
                cans_current_tab.state_voltage++;
                DATA_StoreDataBlock(&cans_current_tab,DATA_BLOCK_ID_CURRENT);
                break;
            case CAN0_SIG_ISENS3_U3_Measurement:
            // case CAN1_SIG_ISENS3_U3_Measurement:  uncommented because identical position in CAN0 and CAN1 rx signal struct
                idx = 2;
                voltageValue[idx] = (int32_t)(dummy[3] | dummy[2] << 8
                        | dummy[1] << 16 | dummy[0] << 24);
                cans_current_tab.voltage[idx]=(float)(voltageValue[idx])*cans_CAN0_signals_rx[sigIdx].factor;
                cans_current_tab.state_voltage++;
                DATA_StoreDataBlock(&cans_current_tab,DATA_BLOCK_ID_CURRENT);
                break;
        }
    }
    return 0;
}


uint32_t cans_setstaterequest(uint32_t sigIdx, void *value) {
    DATA_BLOCK_STATEREQUEST_s staterequest_tab;
    uint8_t staterequest;

    DATA_GetTable(&staterequest_tab,DATA_BLOCK_ID_STATEREQUEST);

    if(value != NULL_PTR) {
        if (sigIdx == CAN0_SIG_BMS10_Request || sigIdx == CAN1_SIG_BMS10_Request ) {
            staterequest = *(uint8_t *)value;
            staterequest_tab.previous_state_request = staterequest_tab.state_request;
            staterequest_tab.state_request = staterequest;
            if ((staterequest_tab.state_request != staterequest_tab.previous_state_request)|| \
                    (MCU_GetTimeStamp()- staterequest_tab.timestamp) > 3000){
                staterequest_tab.state_request_pending = staterequest;
            }
            staterequest_tab.previous_timestamp = staterequest_tab.timestamp;
            staterequest_tab.timestamp = MCU_GetTimeStamp();
            staterequest_tab.state++;
            DATA_StoreDataBlock(&staterequest_tab,DATA_BLOCK_ID_STATEREQUEST);
            SYSCRTL_VCUPresent(SYSCTRL_VCUPRESENCE_OK);
        }
    }
    return 0;
}


uint32_t cans_setdebug(uint32_t sigIdx, void *value) {

    uint8_t data[8] = {0,0,0,0,0,0,0,0};
    static DATA_BLOCK_BALANCING_CONTROL_s balancing_tab;


    data[0] = *(uint64_t *)value & 0x00000000000000FF;
    data[1] = (*(uint64_t *)value & 0x000000000000FF00) >> 8;
    data[2] = (*(uint64_t *)value & 0x0000000000FF0000) >> 16;
    data[3] = (*(uint64_t *)value & 0x00000000FF000000) >> 24;
    data[4] = (*(uint64_t *)value & 0x000000FF00000000) >> 32;
    data[5] = (*(uint64_t *)value & 0x0000FF0000000000) >> 40;
    data[6] = (*(uint64_t *)value & 0x00FF000000000000) >> 48;
    data[7] = (*(uint64_t *)value & 0xFF00000000000000) >> 56;


    if(value != NULL_PTR) {
        switch(data[0]){
            case 11: // Set Soc directly with value. Unit in CAN message: 0.01 percent --> range 0...10000 means 0%Soc...100%Soc
                SOC_SetValue( ((float)((data[1])<<8 | (data[2])))/100.0 ); //divide by 100 to get SOC between 0 and 100
                break;
            case 14: // debug Message for Balancing on pack level
                // dataptr[1];          balancing request
                // dataptr[2]/1000.0;   balancing threshold, mV -> V
                //LTC_BalancingEnablePack(dataptr[1],(((float)(dataptr[2]))/1000.0));
                if (data[1] == 1) {
                    DATA_GetTable(&balancing_tab, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
                    balancing_tab.enable_balancing=1;
                    balancing_tab.threshold = data[2]; //get threshold in mV
                    DATA_StoreDataBlock(&balancing_tab, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
                } else {
                    DATA_GetTable(&balancing_tab, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
                    balancing_tab.enable_balancing=0;
                    balancing_tab.threshold = data[2]; //get threshold in mV
                    DATA_StoreDataBlock(&balancing_tab, DATA_BLOCK_ID_BALANCING_CONTROL_VALUES);
                }
                break;
            default:
                break;
        }

    }
    return 0;
}

