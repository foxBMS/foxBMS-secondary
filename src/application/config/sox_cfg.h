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
 * @file    sox_cfg.h
 * @author  foxBMS Team
 * @date    18.08.2015 (date of creation)
 * @ingroup APPLICATION_CONF
 * @prefix  SOX
 *
 * @brief   Configuration header for SOX
 *
 */

#ifndef SOX_CFG_H_
#define SOX_CFG_H_

/*================== Includes =============================================*/
#include "general.h"
#include "database.h"

/*================== Macros and Definitions ===============================*/

#define SOX_SOC_INIT_CURRENT_LIMIT      100


#define SOX_DELTA_MIN_LIMIT             10


#define SOX_DELTA_MAX_LIMIT             10


#define SOX_CELL_CAPACITY               20000.0


#define SOX_CURRENT_MAX_CONTINUOUS_CHARGE 120.00


#define SOX_CURRENT_MAX_CONTINUOUS_DISCHARGE 120.00


#define SOX_CURRENT_LIMP_HOME 20.00


#define SOX_TEMP_LOW_CUTOFF_DISCHARGE 0.0


#define SOX_TEMP_LOW_LIMIT_DISCHARGE -10.0


#define SOX_TEMP_LOW_CUTOFF_CHARGE 0.0


#define SOX_TEMP_LOW_LIMIT_CHARGE -10.0


#define SOX_TEMP_HIGH_CUTOFF_DISCHARGE 45.0


#define SOX_TEMP_HIGH_LIMIT_DISCHARGE 55.0


#define SOX_TEMP_HIGH_CUTOFF_CHARGE 45.0


#define SOX_TEMP_HIGH_LIMIT_CHARGE 55.0


#define SOX_SOC_CUTOFF_CHARGE 8500


#define SOX_SOC_LIMIT_CHARGE 9500


#define SOX_SOC_CUTOFF_DISCHARGE 1500


#define SOX_SOC_LIMIT_DISCHARGE 500


#define SOX_VOLT_CUTOFF_CHARGE 2400


#define SOX_VOLT_LIMIT_CHARGE 2550


#define SOX_VOLT_CUTOFF_DISCHARGE 2000


#define SOX_VOLT_LIMIT_DISCHARGE 1750

/*================== Constant and Variable Definitions ====================*/

/**
 * structure for configuration of SoF Calculation
 */
typedef struct {
    float I_DischaMax_Cont;
    float I_ChargeMax_Cont;
    float I_Limphome;

    float Cutoff_TLow_Discha;
    float Limit_TLow_Discha;

    float Cutoff_TLow_Charge;
    float Limit_TLow_Charge;

    float Cutoff_THigh_Discha;
    float Limit_THigh_Discha;

    float Cutoff_THigh_Charge;
    float Limit_THigh_Charge;

    float Cutoff_Soc_Charge ;
    float Limit_Soc_Charge;

    float Cutoff_Soc_Discha ;
    float Limit_Soc_Discha;

    float Cutoff_Voltage_Charge;
    float Limit_Voltage_Charge;

    float Cutoff_Voltage_Discha;
    float Limit_Voltage_Discha;
} SOX_SOF_CONFIG_s;

extern SOX_SOF_CONFIG_s sox_sof_config;

/*================== Function Prototypes ==================================*/


/*================== Function Implementations =============================*/

#endif   // SOX_CFG_H_
