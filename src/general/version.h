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
 * @file    version.h
 * @author  foxBMS Team
 * @date    21.09.2015 (date of creation)
   @ingroup GENERAL
   @prefix  VER
 *
 * @brief   Functions for setting a version number, headers
 *
 */

#ifndef VERSION_H_
#define VERSION_H_

/*================== Includes =============================================*/
#include  "general.h"


/*================== Macros and Definitions ===============================*/

/**
 * @brief   Software Version Number
 *
 * Use Case:
 * uint8_t  Version[8] example:     "V 1.00 "   ( 7 char + '\0' )
 */
#define VER_SW_VERSION                    "V0.10  "


/**
 * @brief   Software Project Name
 * Use Case:
 * uint8_t  Version[8] example:     "FSEM123"   ( 7 char + '\0' )
 */
#define VER_PROJECT                    "foxBMS "



typedef enum {
  VER_BUILDVARIANT_UNKNOWN      = 0,
  VER_BUILDVARIANT_LABOR        = 1,
  VER_BUILDVARIANT_TESTBENCH    = 2,
} VER_BUILDVARIANT_e;


/**
 * Adress-Range of Checksum-Test (SW-Startup) will depend on following data which
 * is place at the end of the flash memory (see SECTION FLASH_HEADER)
 * */
typedef struct
{
/*1*/   uint64_t Valid_u64;              /* not used (Application-Validation marker) */
/*2*/   uint64_t Invalid_u64;         /* not used (Application-Invalidation marker)*/
/*3*/   uint32_t Checksum_u32;         /* 16 bit checksum for validating the Application SW */
        uint32_t dummy_3;
/*4*/   uint8_t  Version[8];            /*Version: " V 1.00"*/
/*5*/   uint8_t  Project[8];            /*Project: " AudiTT"*/
/*6*/    VER_BUILDVARIANT_e  BuildVariant; /*BuildVariant (Labor,Strasse,etc)*/
        uint8_t  dummy_u8_1;
        uint8_t  dummy_u8_2;
        uint8_t  dummy_u8_3;
        uint32_t dummy_1;
/*7*/     uint32_t dummy_2;
        uint32_t dummy_4;     /* number of memory ranges for checksum calculation, addresses in the following */
/*8*/   uint32_t dummy_a;     /* the address at which Checksum-Test starts (Start-Address of Application SW)  */
        uint32_t dummy_b;      /* end address of first memory range */
/*9*/   uint32_t dummy_00a;
        uint32_t dummy_00b;
/*10*/  uint32_t dummy_01a;
        uint32_t dummy_01b;
/*11*/  uint32_t dummy_02a;
        uint32_t dummy_02b;
/*12*/    uint32_t dummy_03a;
        uint32_t dummy_03b;
/*13*/    uint32_t dummy_04a;
        uint32_t dummy_04b;
/*14*/    uint32_t dummy_05a;
        uint32_t dummy_05b;
/*15*/    uint32_t dummy_06a;
        uint32_t dummy_06b;
/*16*/    uint32_t dummy_07a;
        uint32_t dummy_07b;
/*17*/    uint32_t dummy_08a;
        uint32_t dummy_08b;
/*18*/    uint32_t dummy_09a;
        uint32_t dummy_09b;
/*19*/    uint32_t dummy_10a;
        uint32_t dummy_10b;
/*20*/
        uint32_t dummy_11a;
        uint32_t dummy_11b;
/*21,5*/
        uint8_t date[12];
 /*23*/
        uint8_t time[12];
/*32*/ uint64_t dummy_u64[0x100/8 - 23];
} VER_ValidStruct_s;



/*================== Constant and Variable Definitions ====================*/
extern const VER_ValidStruct_s ver_sw_validation;


/*================== Function Prototypes ==================================*/

/*================== Function Implementations =============================*/

#endif /* VERSION_H_ */
