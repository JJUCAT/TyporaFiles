/**
  ******************************************************************************
  * @file    AI Cleaning Robot
  * @author  ILife Team XING
  * @version V1.0
  * @date    2017-7-4
  * @brief   System Initialize
  * @define a lot of IO function for a easier look
  ******************************************************************************
  * Initialize the System Clock.ADC converter.EXTI.Timer and USART3
  * <h2><center>&copy; COPYRIGHT 2011 ILife CO.LTD</center></h2>
  ******************************************************************************
  */  

#ifndef __SysInitialize_H
#define __SysInitialize_H

#include "stm32f4xx.h"
#include "config.h"

/* PA function define -------------------------------------------------------*/
#define MCU_WHL_L_I							    ((uint16_t)0X0001)//0
#define MCU_CLN_DRV			  					((uint16_t)0X0002)//1
#define MCU_USART2_TX							  ((uint16_t)0X0004)//2
#define MCU_USART2_RX							  ((uint16_t)0X0008)//3
#define MCU_CLN_SENSOR		          ((uint16_t)0X0010)//4
#define MCU_LEFT_BUMPER_SW		      ((uint16_t)0X0020)//5
#define MCU_RIGHT_BUMPER_SW				 	((uint16_t)0X0040)//6
#define MCU_DIRTY_SENSOR						((uint16_t)0X0080)//7
#define MCU_VACUUM_PWM						  ((uint16_t)0X0100)//8
#define MCU_GYRO_TX								  ((uint16_t)0X0200)//9
#define MCU_GYRO_RX							  	((uint16_t)0X0400)//10
#define MCU_BLDC_OUT                ((uint16_t)0X0800)//11
#define SWDIO									      ((uint16_t)0x2000)//13
#define SWCLK								      	((uint16_t)0x4000)//14
/* PB function define -------------------------------------------------------*/
#define MCU_WHL_R_I                	((uint16_t)0X0001)//0
#define MCU_CHARGER_V_DETECT        ((uint16_t)0X0002)//1
#define MCU_L_WHEEL_SLP            	((uint16_t)0X0004)//2
#define MCU_LEFT_B_IRM		         	((uint16_t)0X0010)//4
#define MCU_CLIFF_CTRL				 	    ((uint16_t)0X0040)//6
#define MCU_WALL_OBS_CTRL          	((uint16_t)0X0080)//7
#define MCU_BLDC_PWM              	((uint16_t)0X0100)//8
#define MCU_CTRL_BAT_VOLT						((uint16_t)0X0200)//9
#define MCU_DIRTY_DRV							  ((uint16_t)0X0400)//10
#define MCU_R_WHEEL_SPEED						((uint16_t)0X2000)//13
#define MCU_RW_PWM								  ((uint16_t)0X8000)//15
//* PC function define -------------------------------------------------------*/
#define MCU_BAT_VOLT               	((uint16_t)0X0001)//0
#define MCU_FAN_M_I			          	((uint16_t)0X0002)//1
#define MCU_CURRENT_DETECT					((uint16_t)0X0004)//2
#define MCU_BRUSH_M_I              	((uint16_t)0X0008)//3
#define MCU_OBS_AD        					((uint16_t)0X0010)//4
#define MCU_WIFI_TX						     	((uint16_t)0X0040)//6
#define MCU_WIFI_RX								  ((uint16_t)0X0080)//7
#define MCU_CHARGER_PWM							((uint16_t)0X0100)//8
#define MCU_CTRL_WIFI_3V3						((uint16_t)0X0200)//9
#define MCU_WIFI_TX_091             ((uint16_t)0X0400)//10
#define MCU_WIFI_RX_091             ((uint16_t)0X0800)//11
#define MCU_O_CLEAN							    ((uint16_t)0X1000)//12
//* PD function define -------------------------------------------------------*/
#define MCU_4051_A2								  ((uint16_t)0X0001)//0
#define MCU_4051_A1							    ((uint16_t)0X0002)//1
#define MCU_4051_A0								  ((uint16_t)0X0004)//2
#define MCU_I_CTRL						      ((uint16_t)0X0040)//6
#define MCU_MOBILITY_DEC						((uint16_t)0X0080)//7
#define MCU_RW_DIR						      ((uint16_t)0X0100)//8
#define MCU_R_FAULT						    	((uint16_t)0X0200)//9
#define MCU_WIFI_WAKE_UP			  		((uint16_t)0X0400)//10
#define MCU_R_WHEEL_SLP							((uint16_t)0X1000)//12
#define MCU_VCC_OFF			  					((uint16_t)0X2000)//13
#define MCU_AUDIO_BUSY							((uint16_t)0X4000)//14
#define MCU_AUDIO_DATA							((uint16_t)0X8000)//15
//* PE function define -------------------------------------------------------*/
#define MCU_RIGHT_B_IRM							((uint16_t)0X0001)//0
#define MCU_CTRL_SW_3V3_5V					((uint16_t)0X0004)//2
#define MCU_PUMPER_FAULT						((uint16_t)0X0008)//3
#define MCU_L_FAULT							    ((uint16_t)0X0010)//4
#define MCU_PUMPER_DIR						  ((uint16_t)0X0020)//5
#define MCU_BRUSH_DIR						    ((uint16_t)0X0040)//6
#define MCU_WATER_TANK							((uint16_t)0X0080)//7
#define MCU_LEDKEY_DIO             	((uint16_t)0X0100)//8
#define MCU_LEDKEY_CLK  						((uint16_t)0X0200)//9
#define MCU_LEDKEY_STB						  ((uint16_t)0X0400)//10
#define MCU_CTRL_OBS_5V						 	((uint16_t)0X0800)//11
#define MCU_LW_DIR						      ((uint16_t)0X1000)//12
#define MCU_LW_PWM							    ((uint16_t)0X4000)//14
#define MCU_L_WHEEL_SPEED_SENSOR		((uint16_t)0X8000)//15

#define Timer1_Count_Range          ((uint16_t)100)
#define Timer1_Prescaler_10K        ((uint16_t)83)
#define Timer2_Count_Range          ((uint16_t)100)
#define Timer2_Prescaler_10k        ((uint16_t)83)
#define Timer3_Count_Range          ((uint16_t)1000)
#define Timer3_Prescaler_42k        ((uint16_t)1)
#define Timer4_Count_Range          ((uint16_t)105)
#define Timer4_Prescaler_20k        ((uint16_t)20)
#define Timer5_Count_Range          ((uint16_t)50)
#define Timer5_Prescaler_20k        ((uint16_t)83)
#define Timer9_Count_Range          ((uint16_t)100)
#define Timer9_Prescaler_10K        ((uint16_t)83)
#define Timer10_Count_Range         ((uint16_t)100)
#define Timer10_Prescaler_20K       ((uint16_t)41)
#define Timer11_Count_Range          ((uint16_t)100)
#define Timer11_Prescaler_10k        ((uint16_t)83)


#define BLDC_PWM					TIM10->CCR1
#define LEFT_WHEEL_PWM    TIM1->CCR4   	
#define RIGHT_WHEEL_PWM   TIM1->CCR3   	
#define PUMP_PWM					TIM9->CCR1
#define MAINBRUSH_PWM			TIM9->CCR2
#define CHARGE_PWM        TIM3->CCR3

void SystemInitialize(void);
void RCC_Configuration(void);
void GPIO_Configuration(void);
void EXTI_Configuration(void);
void NVIC_Configuration(FunctionalState temp);
void Systick_Configuration(void);
void Timer1_Configuration(void);
void Timer2_Configuration(void);
void Timer3_Configuration(void);
void Timer4_Configuration(void);
void Timer4_Wakeup_Configuration(void);
void Timer5_Configuration(void);
void Timer9_Configuration(void);
void Timer10_Configuration(void);
void Timer11_Configuration(void);
void Waiting_Disable_DMA(DMA_Stream_TypeDef *Stream);
void Waiting_Sending_DMA(DMA_Stream_TypeDef *Stream);

#endif
