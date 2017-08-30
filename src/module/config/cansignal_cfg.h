/**
 *
 * @copyright &copy; 2010 - 2017, Fraunhofer-Gesellschaft zur Foerderung der angewandten Forschung e.V. All rights reserved.
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
 * @file    cansignal_cfg.h
 * @author  foxBMS Team
 * @date    16.09.2015 (date of creation)
 * @ingroup DRIVERS_CONF
 * @prefix  CANS
 *
 * @brief   Headers for the configuration of the messages and signal settings for the CAN driver
 *
 */

#ifndef CANIF_CFG_H_
#define CANIF_CFG_H_

/*================== Includes =============================================*/
#include "can_cfg.h"
/*================== Macros and Definitions ===============================*/

/*fox
 * task timeslot where the CANS main function is called. Repetition time of periodic CAN messages must be multiple of this
 * @var CANSIGNAL Tick Time
 * @level read-only
 * @group CAN
 * @type select(3)
 * @default 1
 */
//#define CANS_TICK_MS 1
#define CANS_TICK_MS 10
//#define CANS_TICK_MS 100

/**
 * symbolic names for TX CAN messages. Every used TX message needs to get an individual message name.
 */
typedef enum {

    /* Insert here symbolic names for CAN0 messages */
    CAN0_MSG_BMS1, //!< BMS state
    CAN0_MSG_BMS2, //!< Cell Voltages of Cells 0 1 2
    CAN0_MSG_BMS3, //!< Cell Voltages of Cells 3 4 5
    CAN0_MSG_BMS4, //!< Cell Voltages of Cells 6 7 8
    CAN0_MSG_BMS5, //!< Cell Voltages of Cells 9 10 11
    CAN0_MSG_BMS6, //!< temperture/tempering
    CAN0_MSG_BMS7, //!< SOC/SOH
    CAN0_MSG_BMS8, //!< SOF
    CAN0_MSG_BMS9, //!< Isolation/Balancing
    CAN0_MSG_BMS11, //!< Temperatures of modules
    CAN0_MSG_BMS12, //!< Cell Voltages of Cells 12 13 14
    CAN0_MSG_BMS13, //!< Temperatures extended of modules
    CAN0_MSG_BMS14, //!< Cell Voltages Max Min Average
#ifdef CAN_ISABELLENHUETTE_TRIGGERED
    CAN0_MSG_BMS_CurrentTrigger,    //!< Cell Voltages Max Min Average
#endif

    /* Insert here symbolic names for CAN1 messages */



} CANS_messagesTx_e;

/**
 * symbolic names for RX CAN messages
 */
typedef enum {

    /* Insert here symbolic names for CAN0 messages */
    CAN0_MSG_BMS10,                           //!< state request
    CAN0_MSG_SW_RESET,                       //!< can message for SW reset
    CAN0_MSG_DOWNLOAD_APPLICATION,           //!< download new application code via CAN bootloader
    CAN0_MSG_ISENS0,                         //!< current sensing
    CAN0_MSG_ISENS1,                         //!< current sensor voltage 1
    CAN0_MSG_ISENS2,                         //!< current sensor voltage 2
    CAN0_MSG_ISENS3,                         //!< current sensor voltage 3
    CAN0_MSG_DEBUG,                           //!< debug messages

    /* Insert here symbolic names for CAN1 messages */
    CAN1_MSG_BMS10,                           //!< state request
    CAN1_MSG_SW_RESET,                       //!< can message for SW reset
    CAN1_MSG_DOWNLOAD_APPLICATION,           //!< download new application code via CAN bootloader
    CAN1_MSG_ISENS0,                         //!< current sensing
    CAN1_MSG_ISENS1,                         //!< current sensor voltage 1
    CAN1_MSG_ISENS2,                         //!< current sensor voltage 2
    CAN1_MSG_ISENS3,                         //!< current sensor voltage 3
    CAN1_MSG_DEBUG,                           //!< debug messages

} CANS_messagesRx_e;

/**
 * symbolic names for CAN0 transmission signals
 */
typedef enum {

    CAN0_SIGNAL_NONE = 0xFFFF
} CANS_CAN0_signalsTx_e;

/**
 * symbolic names for CAN1 transmission signals
 */
typedef enum {
    CAN1_SIGNAL_NONE = 0xFFFF,
} CANS_CAN1_signalsTx_e;


/**
 * symbolic names for CAN 0 receive signals
 */
typedef enum {
    CAN0_SIG_BMS10_Overflow,         //!< current sensor measurement type
    CAN0_SIG_BMS10_Request,         //!< current sensor counter
    CAN0_SIG_ISENS0_I_Byte0,         //!< current sensor measurement type
    CAN0_SIG_ISENS0_I_Byte1,         //!< current sensor counter
    CAN0_SIG_ISENS0_I_Measurement,   //!< current sensor measurement I
    CAN0_SIG_ISENS1_U1_Byte0,        //!< current sensor measurement type
    CAN0_SIG_ISENS1_U1_Byte1,        //!< current sensor counter
    CAN0_SIG_ISENS1_U1_Measurement,  //!< current sensor measurement U1
    CAN0_SIG_ISENS2_U2_Byte0,        //!< current sensor measurement type
    CAN0_SIG_ISENS2_U2_Byte1,        //!< current sensor counter
    CAN0_SIG_ISENS2_U2_Measurement,  //!< current sensor measurement U2
    CAN0_SIG_ISENS3_U3_Byte0,        //!< current sensor measurement type
    CAN0_SIG_ISENS3_U3_Byte1,        //!< current sensor counter
    CAN0_SIG_ISENS3_U3_Measurement,  //!< current sensor measurement U3
    CAN0_SIG_DEBUG_Data              //!< Data of debug message
} CANS_CAN0_signalsRx_e;


/**
 * symbolic names for CAN 1 receive signals
 */
typedef enum {
    CAN1_SIG_BMS10_Overflow,         //!< current sensor measurement type
    CAN1_SIG_BMS10_Request,         //!< current sensor counter
    CAN1_SIG_ISENS0_I_Byte0,         //!< current sensor measurement type
    CAN1_SIG_ISENS0_I_Byte1,         //!< current sensor counter
    CAN1_SIG_ISENS0_I_Measurement,   //!< current sensor measurement I
    CAN1_SIG_ISENS1_U1_Byte0,        //!< current sensor measurement type
    CAN1_SIG_ISENS1_U1_Byte1,        //!< current sensor counter
    CAN1_SIG_ISENS1_U1_Measurement,  //!< current sensor measurement U1
    CAN1_SIG_ISENS2_U2_Byte0,        //!< current sensor measurement type
    CAN1_SIG_ISENS2_U2_Byte1,        //!< current sensor counter
    CAN1_SIG_ISENS2_U2_Measurement,  //!< current sensor measurement U2
    CAN1_SIG_ISENS3_U3_Byte0,        //!< current sensor measurement type
    CAN1_SIG_ISENS3_U3_Byte1,        //!< current sensor counter
    CAN1_SIG_ISENS3_U3_Measurement,  //!< current sensor measurement U3
    CAN1_SIG_DEBUG_Data              //!< Data of debug message
} CANS_CAN1_signalsRx_e;

typedef enum {
    CAN_RX_DIRECTION = 0,
    CAN_TX_DIRECTION = 1
} CANS_messageDirection_t;

typedef union {
    CANS_messagesTx_e Tx;
    CANS_messagesRx_e Rx;
} CANS_messages_t;

typedef union {
    CANS_CAN0_signalsTx_e Tx;
    CANS_CAN0_signalsRx_e Rx;
} CANS_signals_t;

/**
 * type definition for structure of a CAN signal
 *
 * until now, multiplexed signal handling is hard coded
 * in the corresponding getters/setters. For use of multiplexed
 * signals refer to description in documentation.
 *
 * support for automatic scaling is planned, but not implemented yet,
 * so min, max, factor and offset are not relevant.
 */
typedef struct  {
    CANS_messages_t msgIdx;
    uint8_t bit_position;
    uint8_t bit_length;
    float max;
    float min;
    float factor;
    float offset;
    boolean isMuxed;
    boolean isMuxor;
    uint8_t muxValue;
    CANS_signals_t muxor;
    can_callback_funcPtr setter;
    can_callback_funcPtr getter;
} CANS_signal_s;

/*================== Constant and Variable Definitions ====================*/

/**
 * array for transmission CAN0 signals definition
 */
extern const CANS_signal_s cans_CAN0_signals_tx[];

/**
 * array for transmission CAN1 signals definition
 */
extern const CANS_signal_s cans_CAN1_signals_tx[];

/**
 * array for received CAN0 signals definition
 */
extern const CANS_signal_s cans_CAN0_signals_rx[];

/**
 * array for received CAN1 signals definition
 */
extern const CANS_signal_s cans_CAN1_signals_rx[];

/**
 * length of the array for the CAN0 tx signals
 */
extern const uint16_t cans_CAN0_signals_tx_length;

/**
 * length of the array for the CAN1 tx signals
 */
extern const uint16_t cans_CAN1_signals_tx_length;

/**
 * length of the array for the CAN0 rx signals
 */
extern const uint16_t cans_CAN0_signals_rx_length;

/**
 * length of the array for the CAN1 rx signals
 */
extern const uint16_t cans_CAN1_signals_rx_length;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

#endif /* CANIF_CFG_H_ */
