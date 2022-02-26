/**
  ******************************************************************************
  * @file    stm32f10x_exti.h
  * @author  MCD Application Team
  * @version V3.5.0
  * @date    11-March-2011
  * @brief   This file contains all the functions prototypes for the EXTI firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USART_H
#define __USART_H

#include "stm32f4xx.h"

#define USART_APB1_BAUDRATE 		(uint32_t)0x0000016C //115200
#define USART_APB2_BAUDRATE			(uint32_t)0x000002D9 //115200


extern uint8_t data_received[60];



void USART1_Configuration(void);
void USART2_Configuration(void);
void USART6_Configuration(void);

void USART1_Transmit_Byte(char Data);
void USART2_Transmit_Byte(char Data);
void USART6_Transmit_Byte(char Data);

void USART1_DMA_String(uint16_t Length,char *data);
void USART2_DMA_String(uint16_t Length,char *data);
void USART6_DMA_String(uint16_t Length,char *data);

void USART1_Transmit_String(uint16_t Length,char *data);
void USART2_Transmit_String(uint16_t length,char *data);
void USART6_Transmit_String(uint16_t length,char *data);

void ups(char * data);
void upn(int32_t data);
void ups3(char * data);

#endif /* __USART_H */


