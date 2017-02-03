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
 * @file    uart.c
 * @author  foxBMS Team
 * @date    23.09.2015
 * @ingroup DRIVERS
 * @prefix  UART
 *
 * @brief Driver for the UART.
 *
 * This uart module handles sending/receiving requests using UART/RS232.
 * In its current implementation it's interrupt driven, may change in the future
 * in order to support DMA.
 *
 */

/*================== Includes =============================================*/
#include "uart.h"
#include "com.h"
#include "cmsis_os.h"

/*================== Macros and Definitions ===============================*/

#define MSG_NOT_BUSY    0
#define MSG_BUSY        1

/*================== Constant and Variable Definitions ====================*/

volatile unsigned char Msg0SendBusy=0;

uint8_t rxbuf[RXBUF_LENGTH];
uint8_t txbuf[RXBUF_LENGTH];

uint8_t *wrpoi_rxbuf=&rxbuf[0];
uint8_t *rdpoi_rxbuf=&rxbuf[0];

uint8_t *wrpoi_txbuf=&txbuf[0];
uint8_t *rdpoi_txbuf=&txbuf[0];

UART_HandleTypeDef huart3;
UART_HandleTypeDef huart2;

/*================== Function Prototypes ==================================*/

void UART_Init(void);

void UART_IntRx(void);
void UART_IntTx(void);
void UART_IntRxComp(void);
uint8_t *UART_txbuf_copy (uint8_t *ptrb, uint8_t *ptra);

/*================== Function Implementations =============================*/

/**
 * @brief UART_Init provides initialization of the UART module.
 *
 * It needs to be called during startup, send/receive will only work
 * after initialization is completed.
 * This function configures parameters of the UART interface and
 * enables the needed interrupts.
 *
 * Available baudrate is dependent on Sysclock.
 *
 * @return (type: void)
 */

void UART_Init(/*UART_HandleTypeDef *uart_cfg*/ void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 38400;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    HAL_UART_Init(&huart3);

    //Enable the UART Parity Error Interrupt
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_PE);

    //Enable the UART Error Interrupt: (Frame error, noise error, overrun error)
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_ERR);

    //Enable the UART Data Register not empty Interrupt
    __HAL_UART_ENABLE_IT(&huart3, UART_IT_RXNE);

/*  for (int i = 0; i < uart_cfg_length; i++) {
        UART_HandleTypeDef UART_InitStructure;
        UART_InitStructure.Instance = uart_cfg[i].Instance;
        UART_InitStructure.Init.BaudRate = uart_cfg[i].Init.BaudRate;
        UART_InitStructure.Init.WordLength = uart_cfg[i].Init.WordLength;
        UART_InitStructure.Init.StopBits = uart_cfg[i].Init.StopBits;
        UART_InitStructure.Init.Parity = uart_cfg[i].Init.Parity;
        UART_InitStructure.Init.Mode = uart_cfg[i].Init.Mode;
        UART_InitStructure.Init.HwFlowCtl = uart_cfg[i].Init.HwFlowCtl;
        UART_InitStructure.Init.OverSampling = uart_cfg[i].Init.OverSampling;
        HAL_UART_Init(&UART_InitStructure);

        HAL_UART_Init(&UART_InitStructure);

        //Enable the UART Parity Error Interrupt
        __HAL_UART_ENABLE_IT(&UART_InitStructure, UART_IT_PE);
        // Enable the UART Error Interrupt: (Frame error, noise error, overrun error)
        __HAL_UART_ENABLE_IT(&UART_InitStructure, UART_IT_ERR);
        // Enable the UART Data Register not empty Interrupt
        __HAL_UART_ENABLE_IT(&UART_InitStructure, UART_IT_RXNE);
    }*/
}

#if 0
*            @arg UART_IT_CTS: CTS change interrupt (not available for UART4 and UART5)
*            @arg UART_IT_LBD: LIN Break detection interrupt
*            @arg UART_IT_TXE: Transmit Data Register empty interrupt
*            @arg UART_IT_TC:  Transmission complete interrupt
*            @arg UART_IT_RXNE: Receive Data register not empty interrupt
*            @arg UART_IT_IDLE: Idle line detection interrupt
*            @arg USART_IT_ERR: Error interrupt
#endif

/**
 * @brief HAL_UART_CustomIRQHandler is responsible for handling all requests.
 *
 * This custom interrupt handler does all the handling for
 * sending/receiving data, reset and error handling.
 * In case of send/receive a subfunction is called which handles further processing.
 *
 * @return (type: void)
 */
void HAL_UART_CustomIRQHandler(UART_HandleTypeDef *huart)
{

      uint32_t tmp1 = 0;
      uint32_t tmp2 = 0;

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_PE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_PE);
      /* UART parity error interrupt occurred ------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
        __HAL_UART_CLEAR_PEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_PE;
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_FE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
      /* UART frame error interrupt occurred -------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
        __HAL_UART_CLEAR_FEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_FE;
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_NE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
      /* UART noise error interrupt occurred -------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
        __HAL_UART_CLEAR_NEFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_NE;
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_ORE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_ERR);
      /* UART Over-Run interrupt occurred ----------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
        __HAL_UART_CLEAR_OREFLAG(huart);

        huart->ErrorCode |= HAL_UART_ERROR_ORE;
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_RXNE);
      /* UART in mode Receiver ---------------------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
          UART_IntRx(); //@todo akdere: UART_Receive_IT(huart);
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TXE);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TXE);
      /* UART in mode Transmitter ------------------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
          UART_IntTx(); //@todo akdere: UART_Transmit_IT(huart);
      }

      tmp1 = __HAL_UART_GET_FLAG(huart, UART_FLAG_TC);
      tmp2 = __HAL_UART_GET_IT_SOURCE(huart, UART_IT_TC);
      /* UART in mode Transmitter end --------------------------------------------*/
      if((tmp1 != RESET) && (tmp2 != RESET))
      {
          UART_IntRxComp(); //@todo akdere: UART_EndTransmit_IT(huart);
      }

      if(huart->ErrorCode != HAL_UART_ERROR_NONE)
      {
        /* Set the UART state ready to be able to start again the process */
        huart->State = HAL_UART_STATE_READY;

        HAL_UART_ErrorCallback(huart);
      }
}

/**
 * @brief UART_IntRx is responsible for handling receive requests.
 *
 * This is invoked from with the interrupt handler and processes
 * userdata on receive event.
 * It copies data from data register into a ringbuffer,
 * provides some very basic application example.
 *
 * @return (type: void)
 */

void UART_IntRx(void)
{
//  *asc0_wrpoi_rxbuf=(ASC0_RBUF.U & 0xFF);     // read 8-Bit receive buffer register (like ASC0_usGetData(void) )
    *wrpoi_rxbuf = (uint8_t)(huart3.Instance->DR/* & huart2.Mask*/);

    /*pointer handling of ringbuffer*/
    if(++wrpoi_rxbuf >= &rxbuf[RXBUF_LENGTH])
        wrpoi_rxbuf = &rxbuf[0];
    //check overwrite of read pointer

    // todo: this is just a very simple debug/demonstration implementation for decoder
    *(com_receivedbyte + com_receive_slot) = *rdpoi_rxbuf;
    com_receive_slot++;

    /*pointer handling of ringbuffer*/
    if(++rdpoi_rxbuf >= &rxbuf[RXBUF_LENGTH])
       rdpoi_rxbuf = &rxbuf[0];
}

/**
 * @brief UART_IntTx is responsible for handling send requests.
 *
 * This is invoked from with the interrupt handler and processes
 * userdata on send event.
 * It copies data from a ringbuffer into the data register
 * as long as the read pointer doesn't match the write pointer
 * (which means there's still unsent data in the ringbuffer)
 *
 * @return (type: void)
 */
void UART_IntTx(void)
{
    if(rdpoi_txbuf >= &txbuf[RXBUF_LENGTH])
        rdpoi_txbuf = &txbuf[0];

    if(rdpoi_txbuf != wrpoi_txbuf)
    {
        Msg0SendBusy=MSG_BUSY;
//      ASC0_vSendData(*asc0_rdpoi_txbuf++); // send when pointers unequal (values in buffer)
        huart3.Instance->DR = (uint16_t) (*rdpoi_txbuf++);
    }
    else
    {
        /* Disable the UART Transmit Data Register Empty Interrupt */
        __HAL_UART_DISABLE_IT(&huart3, UART_IT_TXE);

        /* Enable the UART Transmit Complete Interrupt */
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_TC);

        Msg0SendBusy=MSG_NOT_BUSY;
    }
}

void UART_IntRxComp(void)
{
  /* Disable the UART Transmit Complete Interrupt */
  __HAL_UART_DISABLE_IT(&huart3, UART_IT_TC);

  // optional: call TX complete function
}

/**
 * @brief UART_vWrite provides an interface to send data.
 *
 * This function copies data from input buffer to transmit ringbuffer
 * and processes this data in case it's not already busy transitting data.
 * To avoid data corruption caused by simultaneous accesses some kind of
 * critical section is used.
 *
 * @return (type: uint8_t)
 */
void UART_vWrite(const uint8_t *source)
{
    taskENTER_CRITICAL();

    wrpoi_txbuf=UART_txbuf_copy (wrpoi_txbuf,(uint8_t*) source);
    if (Msg0SendBusy==MSG_NOT_BUSY)
    {
        /* Enable the UART Transmit Data Register Empty Interrupt */
        __HAL_UART_ENABLE_IT(&huart3, UART_IT_TXE);
    }

    taskEXIT_CRITICAL();

}

/**
 * @brief UART_txbuf_copy is a helper function.
 *
 * This function just copies content from buffer a to ringbuffer b.
 *
 * @return (type: uint8_t)
 */
uint8_t *UART_txbuf_copy (uint8_t *ptrb, uint8_t *ptra)
{

    while(*ptra)
    {
        *ptrb++=*ptra++;
        if(ptrb >= &txbuf[RXBUF_LENGTH])
        {
            ptrb = &txbuf[0];
        }

    }
    /*  do not send NULL-Value */

    return ptrb;
}
