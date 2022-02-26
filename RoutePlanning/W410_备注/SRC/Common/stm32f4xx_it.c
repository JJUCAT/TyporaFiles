/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c 
  * @author  MCD Application Team
  * @version V1.6.0
  * @date    04-September-2015
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and 
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *

  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"
#include "stm32f4xx.h"
#include "Include.h"

void NMI_Handler(void)
{
	
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}


/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

void DMA1_Stream6_IRQHandler(void)
{
#ifdef DEBUG_ENABLE	
	static portBASE_TYPE xHigherPriorityTaskWoken;
#endif	
	if(DMA1->HISR & DMA_HISR_TCIF6)
	{		
		DMA1->HIFCR |= (DMA_HIFCR_CTCIF6);	
		#ifdef DEBUG_ENABLE	
		xSemaphoreGiveFromISR(g_binary_semaphore_UsartTXF,&xHigherPriorityTaskWoken);	
		#endif		
	}
	if(DMA1->HISR & (DMA_HISR_HTIF6 | DMA_HISR_FEIF6|DMA_HISR_TEIF6))
	{
		DMA1->HIFCR |=(DMA_HIFCR_CHTIF6 | DMA_HIFCR_CFEIF6|DMA_HIFCR_CTEIF6 );		
	}
}

void DMA2_Stream7_IRQHandler(void)
{
	if(DMA2->HISR & DMA_HISR_TCIF7)
	{
		DMA2->HIFCR |= DMA_HIFCR_CTCIF7 ;		
		
	}
	if(DMA2->HISR & (DMA_HISR_HTIF7 | DMA_HISR_FEIF7|DMA_HISR_TEIF7))
	{
		DMA2->HIFCR |=(DMA_HIFCR_CHTIF7 | DMA_HIFCR_CFEIF7|DMA_HIFCR_CTEIF7 );		
	}
}

void RTC_Alarm_IRQHandler(void)
{
	if(RTC->ISR & (RTC_ISR_ALRAF))
	{
		RTC->ISR &= ~RTC_ISR_ALRAF;
		EXTI->PR |= EXTI_PR_PR17;
	
	}
}
//RTC WAKE UP中断服务函数
void RTC_WKUP_IRQHandler(void)
{    
	if(RTC->ISR&(1<<10))//WK_UP中断?
	{ 
		RTC->ISR&=~(1<<10);	//清除中断标志
		EXTI->PR|=1<<22;	//清除中断线22的中断标志 		
		
	}   
						
}

void USART1_IRQHandler (void) 
{
	uint8_t length = 0;
	int8_t i = 0;

	if(USART1->SR & USART_SR_IDLE) 
	{
		length = USART1->SR;
		length = USART1->DR;     
		length = GYRO_BUFFER_SIZE - DMA2_Stream2->NDTR;		
		
		if(length == GYRO_MSG_LENGTH)
		{
			if((g_gyro.rxbuffer[0] == 0xaa) && (g_gyro.rxbuffer[1] == 0x00))
			{
				DMA2_Stream2->CR &= ~ DMA_SxCR_EN ;
				Gyro_ParseMsg(&g_gyro.rxbuffer[2]);
				DMA2_Stream2->NDTR = GYRO_BUFFER_SIZE;
				DMA2->LIFCR |= (DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2|DMA_LIFCR_CFEIF2);
				DMA2_Stream2->CR |= DMA_SxCR_EN ;
			}
		}
		else if(length > GYRO_MSG_LENGTH)
		{		
			DMA2_Stream2->CR &= ~ DMA_SxCR_EN ; 

			for(i = (length - GYRO_MSG_LENGTH); i >= 0; i--)
			{
				if((g_gyro.rxbuffer[i] == 0xaa) && (g_gyro.rxbuffer[i+1] == 0x00))
				{
					Gyro_ParseMsg(&g_gyro.rxbuffer[2]);
				}
			}

			DMA2_Stream2->NDTR = GYRO_BUFFER_SIZE;
			DMA2->LIFCR |= (DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2|DMA_LIFCR_CFEIF2);
			DMA2_Stream2->CR |= DMA_SxCR_EN ;
		}
	}
	if(USART1->SR & USART_SR_ORE) 
	{
		length = USART1->SR;
		length = USART1->DR;
		DMA2_Stream2->CR &= ~ DMA_SxCR_EN ;
		Gyro_ParseMsg(&g_gyro.rxbuffer[2]);
		DMA2_Stream2->NDTR = GYRO_BUFFER_SIZE;
		DMA2->LIFCR |= (DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2|DMA_LIFCR_CFEIF2);
		DMA2_Stream2->CR |= DMA_SxCR_EN ;
	}
}

void USART2_IRQHandler (void)
{
	uint8_t length = 0;
	int8_t i = 0;

	if(USART2->SR & USART_SR_IDLE)
	{
		length = USART2->SR;
		length = USART2->DR;                     
		length = OBS_BUFFER_SIZE - DMA1_Stream5->NDTR;
		g_obsmsg.length = length;
		g_obsmsg.update = 1;
		if((length == OBS_USART_MAX) || (length == OBS_REMOTE_USART_MAX)||(length == OBS_CMD_USART_MAX))
		{
			DMA1_Stream5->CR &= ~ DMA_SxCR_EN ;
			ObsMsg_Parse(&g_obsmsg.rxbuffer[2]);
			DMA1_Stream5->NDTR = OBS_BUFFER_SIZE;
			DMA1->HIFCR |= (DMA_HIFCR_CTCIF5|DMA_HIFCR_CHTIF5|DMA_HIFCR_CFEIF5);
			DMA1_Stream5->CR |= DMA_SxCR_EN ;
		}
		else if((length > OBS_CMD_USART_MAX) && (length < OBS_REMOTE_USART_MAX))
		{
			for(i = (length - OBS_CMD_USART_MAX) ;i >= 0;i--)
			{
				if((g_obsmsg.rxbuffer[i] == 0xAA) && (g_obsmsg.rxbuffer[i+1] == 0X55))
				{
					if(ObsMsg_Parse(&g_obsmsg.rxbuffer[i+2]))              
					{
						DMA1_Stream5->CR &= ~ DMA_SxCR_EN ;
						Waiting_Disable_DMA(DMA1_Stream5);
						DMA1_Stream5->NDTR = OBS_BUFFER_SIZE;
						DMA1->HIFCR |= (DMA_HIFCR_CTCIF5|DMA_HIFCR_CHTIF5|DMA_HIFCR_CFEIF5);
						DMA1_Stream5->CR |= DMA_SxCR_EN ;
					}
				}
			}
		}
		else if(length > OBS_REMOTE_USART_MAX)
		{
			DMA1_Stream5->CR &= ~ DMA_SxCR_EN ;
			
			for(i = (length - OBS_CMD_USART_MAX); i >= 0; i--)
			{
				if((g_obsmsg.rxbuffer[i] == 0xAA) && (g_obsmsg.rxbuffer[i+1] == 0X55))
				{
					ObsMsg_Parse(&g_obsmsg.rxbuffer[i+2]);
				}
			}
			
			DMA1_Stream5->NDTR = OBS_BUFFER_SIZE;
			DMA1->HIFCR |= (DMA_HIFCR_CTCIF5|DMA_HIFCR_CHTIF5|DMA_HIFCR_CFEIF5);
			DMA1_Stream5->CR |= DMA_SxCR_EN ;
		}
	}
	if(USART2->SR & USART_SR_ORE) 
	{
		length = USART2->SR;
		length = USART2->DR;
		DMA1_Stream5->CR &= ~ DMA_SxCR_EN ;
		DMA1_Stream5->NDTR = OBS_BUFFER_SIZE;
		DMA1->HIFCR |= (DMA_HIFCR_CTCIF5|DMA_HIFCR_CHTIF5|DMA_HIFCR_CFEIF5);
		DMA1_Stream5->CR |= DMA_SxCR_EN ;
	}
}

void USART6_IRQHandler (void) 
{
	Wifi_RecieveData();
}

void  EXTI0_IRQHandler(void)//FR RCON
{
	if(EXTI->PR & EXTI_PR_PR0)
	{
		EXTI->PR |= EXTI_PR_PR0;
		Rcon_Decoder(&g_rcon.fr,g_rcon.on_get_fr_state(),RCONSIDE_FR);
	}
}

void  EXTI1_IRQHandler(void)//charge v
{
	if(EXTI->PR & EXTI_PR_PR1)
	{
		EXTI->PR |= EXTI_PR_PR1;	
		if(g_sleep.state == ENABLE)
		{
			Sleep_EditInterrupt(SLEEP_INIERRUPT_CHARGE);
		}		
	}
}

void  EXTI2_IRQHandler(void)//NONE
{
	if(EXTI->PR & EXTI_PR_PR2)
	{
		EXTI->PR |= EXTI_PR_PR2;
	}
}

void  EXTI3_IRQHandler(void)//PUMPER + obs rx
{
	if(EXTI->PR & EXTI_PR_PR3)
	{
		EXTI->PR |= EXTI_PR_PR3;
		if(g_sleep.state == ENABLE)
		{
			Sleep_EditInterrupt(SLEEP_INIERRUPT_OBS);
		}
		else
		{
			g_pump.cnt++;
		}		
	}
}

void  EXTI4_IRQHandler(void)//FL RCON
{
	if(EXTI->PR & EXTI_PR_PR4)
	{
		EXTI->PR |= EXTI_PR_PR4;
		Rcon_Decoder(&g_rcon.fl,g_rcon.on_get_fl_state(),RCONSIDE_FL);
	}
}

void EXTI9_5_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR6)//NONE	
	{
		EXTI->PR |= EXTI_PR_PR6;

	}
	if(EXTI->PR & EXTI_PR_PR7)//MOBILITY	
	{
		EXTI->PR |= EXTI_PR_PR7;
		g_mobility.step++;
	}

	if(EXTI->PR&EXTI_PR_PR8)//NONE	
	{
		EXTI->PR |= EXTI_PR_PR8;
	}

	if(EXTI->PR & EXTI_PR_PR9)//NONE	
	{
		EXTI->PR |= EXTI_PR_PR9;
	}
}

void EXTI15_10_IRQHandler(void)
{
	if(EXTI->PR & EXTI_PR_PR10)//wifi wake up	
	{
		EXTI->PR |= EXTI_PR_PR10;	
		if(g_sleep.state == ENABLE)
		{
			Sleep_EditInterrupt(SLEEP_INIERRUPT_WIFI);
		}	
	}

	if(EXTI->PR & EXTI_PR_PR11)//BLDC
	{
		EXTI->PR |= EXTI_PR_PR11;
		g_vacuum.pulse_cnt++;
	}

	if(EXTI->PR & EXTI_PR_PR12)//button	
	{
		EXTI->PR |= EXTI_PR_PR12;
		if(g_sleep.state == ENABLE)
		{
			Sleep_EditInterrupt(SLEEP_INIERRUPT_BUTTON);
		}		
	}

	if(EXTI->PR & EXTI_PR_PR13)//R WHEEL	
	{
		EXTI->PR |= EXTI_PR_PR13;
		Wheel_RightInterrupt();
	}

	if(EXTI->PR & EXTI_PR_PR14)//NONE	
	{
		EXTI->PR |= EXTI_PR_PR14;
	}

	if(EXTI->PR & EXTI_PR_PR15)//L WHEEL	
	{
		EXTI->PR |= EXTI_PR_PR15;
		Wheel_LeftInterrupt();		
	}	
}

void SysTick_Handler(void) //1ms
{
	osSystickHandler();
}

void TIM2_IRQHandler(void) //0.1ms
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~ TIM_SR_UIF;		
		Timer_IncreaseCounter();
		Rcon_IncreaseCounter();
		Speaker_InterruptHandler();
		delay_increase_counter();
		CodeShow_Cycle();
		#ifdef	MOTOR_ENABLE
		Pump_Process();	
		#endif		
	}
}

void TIM3_IRQHandler(void) 
{
	if(TIM3->SR & TIM_SR_UIF)
	{
		TIM3->SR &= ~ TIM_SR_UIF;	
	}
}

void TIM4_IRQHandler(void) 
{
	if(TIM4->SR & TIM_SR_UIF)
	{
		TIM4->SR &= ~ TIM_SR_UIF;
		Sleep_EditInterrupt(SLEEP_INIERRUPT_TIME);
		
	}
}

void TIM5_IRQHandler(void) //20K
{
	if(TIM5->SR & TIM_SR_UIF)
	{
		TIM5->SR &= ~ TIM_SR_UIF;
		Battery_AddCapacityCurrentCycle(g_adc_value.system);
		SensorDriver_Process(g_cliff.state,g_obs.state,g_watertank.state,g_code_show.is_working);	
	}
}

void TIM1_BRK_TIM9_IRQHandler(void) 
{
	if(TIM9->SR & TIM_SR_UIF)
	{
		TIM9->SR &=~ TIM_SR_UIF;
	}
}

void TIM1_UP_TIM10_IRQHandler(void) 
{
	if(TIM10->SR & TIM_SR_UIF)
	{		
		TIM10->SR &=~ TIM_SR_UIF; 
		
	}
}

void TIM1_TRG_COM_TIM11_IRQHandler(void) //10k display
{
	if(TIM11->SR & TIM_SR_UIF)
	{
		TIM11->SR &=~ TIM_SR_UIF;	
		Hw_Display_IntHandler();	
	}
}

void DMA1_Stream7_IRQHandler(void)
{

}



/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
