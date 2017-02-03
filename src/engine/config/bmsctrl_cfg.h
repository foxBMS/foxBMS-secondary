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
 * @file    bmsctrl_cfg.h
 * @author  foxBMS Team
 * @date    04.12.2015 (date of creation)
 * @ingroup ENGINE_CONF
 * @prefix  BMSCTRL
 *
 * @brief   Bmscontrol configuration header
 *
 * Configuration declarations and defines of the BMSCTRL module
 */
#ifndef BMSCTRL_CFG_H_
#define BMSCTRL_CFG_H_


/*================== Includes =============================================*/


/*================== Macros and Definitions ===============================*/
/*fox
 * maximum temperature safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * @var maximum temperature safe operating limit
 * @level user
 * @type int
 * @validator -40<x<80
 * @unit °C
 * @default 55
 * @group BMSCTRL
 */
#define BMSCTRL_TEMPMAX 55

/*fox
 * minimum temperature safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * @var minimum temperature safe operating limit
 * @level user
 * @type int
 * @validator -40<x<80
 * @unit °C
 * @default -10
 * @group BMSCTRL
 */
#define BMSCTRL_TEMPMIN -10 //°C

/*fox
 * maximum cell voltage safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * @var maximum cell voltage safe operating limit
 * @level user
 * @type int
 * @validator 0<x<5000
 * @unit mV
 * @default 2800
 * @group BMSCTRL
 */
#define BMSCTRL_VOLTMAX 3600

/*fox
 * minimum cell voltage safe operating limit
 * When limit is violated, error state is requested and contactors will open.
 * @var minimum cell voltage safe operating limit
 * @level user
 * @type int
 * @validator 0<x<5000
 * @unit mV
 * @default 2100
 * @group BMSCTRL
 */
#define BMSCTRL_VOLTMIN 2100


#define BMSCTRL_CURRENTMAX 15000


#define BMSCTRL_CURRENTMIN -15000


#define BMSCTRL_CAN_TIMING_TEST TRUE
//#define BMSCTRL_CAN_TIMING_TEST FALSE

#define BMSCTRL_MAX_CAN_FAIL_WAIT   100

/*fox
 * Number of SOA violation before interlock opens.
 * @var number of SOA violations before interlock opens
 * @level user
 * @type int
 * @validator 0<x<20
 * @default 10
 * @group BMSCTRL
 */
#define BMSCTRL_MAX_MEAS_FAIL_WAIT  10

/*fox
 * Delay in ms before MCU1 check becomes active.
 * @var delay in ms before MCU1 check becomes active
 * @level user
 * @type int
 * @validator 0<x<=10000
 * @unit ms
 * @default 3000
 * @group BMSCTRL
 */
#define BMSCTRL_MS_BEFORE_CHECK  3000


/*fox
 * checking if safe operating area limits of cells is violated. Must only be used for debug purposes.
 * Extremely dangerous if used in system.
 * @var Cell limits test enable
 * @type select(2)
 * @default 0
 * @level debug
 * @group BMSCTRL
 */
#define BMSCTRL_TEST_CELL_LIMITS TRUE
//#define BMSCTRL_TEST_CELL_LIMITS FALSE


//#define BMSCTRL_TEST_CELL_SOF_LIMITS  TRUE
#define BMSCTRL_TEST_CELL_SOF_LIMITS FALSE


#define BMSCTRL_REQ_ID_STANDBY 8


#define BMSCTRL_REQ_ID_NORMAL 3
/*================== Constant and Variable Definitions ====================*/


/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/


#endif   // BMSCTRL_CFG_H_
