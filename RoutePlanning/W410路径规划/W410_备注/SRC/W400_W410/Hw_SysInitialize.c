 #include "Include.h"

void SystemInitialize(void)
{
	RCC_Configuration();
	GPIO_Configuration();
	Systick_Configuration();
	Hw_Adc_Configuration();
	Timer1_Configuration();
	Timer2_Configuration();
	Timer4_Configuration();
	Timer5_Configuration();
	Timer9_Configuration();
	Timer10_Configuration();
	Timer11_Configuration();
	USART1_Configuration();
	USART2_Configuration();
	USART6_Configuration();
	EXTI_Configuration();
	
	NVIC_Configuration(ENABLE);

}

void RCC_Configuration(void)
{
	uint32_t TO = 0;

	RCC->CR |= RCC_CR_HSEON;
	while(!(RCC->CR & RCC_CR_HSERDY))
	{
		TO++;
		if(TO > 0xf4240)break;
	}
	TO=0;
	RCC->CR |= RCC_CR_PLLON;				
	while(! (RCC->CR&RCC_CR_PLLRDY))
	{
		TO++;
		if(TO > 0xf4240)break;
	}

	RCC->CFGR |= RCC_CFGR_SW_1;
	//84M
	RCC->AHB1ENR |= ( RCC_AHB1ENR_GPIOAEN  |
	               	  RCC_AHB1ENR_GPIOBEN  |
	               	  RCC_AHB1ENR_GPIOCEN  |
	               	  RCC_AHB1ENR_GPIODEN  |
	               	  RCC_AHB1ENR_GPIOEEN  |
	               	  RCC_AHB1ENR_DMA1EN   |
	               	  RCC_AHB1ENR_DMA2EN   );
	//42M
	RCC->APB1ENR |= ( RCC_APB1ENR_TIM2EN   |
	               	  RCC_APB1ENR_TIM3EN   |
	                  RCC_APB1ENR_TIM4EN   |
					  RCC_APB1ENR_TIM5EN   |
	                  RCC_APB1ENR_USART2EN );
	//84M
	RCC->APB2ENR |= ( RCC_APB2ENR_SYSCFGEN |
	               	  RCC_APB2ENR_USART1EN |
	               	  RCC_APB2ENR_USART6EN |
	               	  RCC_APB2ENR_TIM1EN   |
	               	  RCC_APB2ENR_TIM9EN   |
	               	  RCC_APB2ENR_TIM10EN  |
	               	  RCC_APB2ENR_TIM11EN  |
	               	  RCC_APB2ENR_ADC1EN   );
}

void Systick_Configuration(void)
{
	 STK_CSR = 0X07;
	 STK_RVR = 84000;
	 STK_CVR = 0;	
}

void GPIO_Configuration(void)
{
	/*----------------------------------- GPIOA -----------------------------------------*/
	GPIOA->MODER  = ( GPIO_MODER_MODER0    |     //WHILE_L_I
	                  GPIO_MODER_MODER1_0  |     //CLEAN DRV
	                  GPIO_MODER_MODER2_1  | 	 //OBS USART TX
	                  GPIO_MODER_MODER3_1  |     //OBS USART RX
	                  GPIO_MODER_MODER4    |     //CLEAN SENSOR
	                  GPIO_MODER_MODER5    |     //LEFT BUMPER
	                  GPIO_MODER_MODER6    |     //RIGHT BUMPER
	                  GPIO_MODER_MODER7    |     //DIRTY SENSOR
					  GPIO_MODER_MODER8_0  |     //VACUUM PWM
	                  GPIO_MODER_MODER9_1  |     //USART1_TX
	                  GPIO_MODER_MODER10_1 |     //USART1_RX
	                  GPIO_MODER_MODER13_1 |	 //SW DIO
					  GPIO_MODER_MODER14_1 );	 //SWCLK

	 GPIOA->AFR[0] |= ((AF7<<AFR_GPIO_2)|(AF7<<AFR_GPIO_3));  
	 GPIOA->AFR[1] |= ((AF7<<AFR_GPIO_9)|(AF7<<AFR_GPIO_10)); 
	/*----------------------------------- GPIOB -----------------------------------------*/	
	 GPIOB->MODER   = (GPIO_MODER_MODER0    |     //WHILE_R_I
		               GPIO_MODER_MODER1  	|     //CHARGER_V_DETECT
	                   GPIO_MODER_MODER2_0  |     //WHILE_LW_SLEEP
					   GPIO_MODER_MODER6_0  |	  //CLIFF CTRL
	                   GPIO_MODER_MODER7_0  |     //OBS WALL CTRL
                       GPIO_MODER_MODER8_1  |     //BLDC PWM
	                   GPIO_MODER_MODER9_0  |     //CTRL_BAT_VOLTAGE
	                   GPIO_MODER_MODER10_0 |     //DIRTY_DRV
	                   GPIO_MODER_MODER15_1 );    //RW PWM
	 
	 GPIOB->AFR[1] |=((AF3<<AFR_GPIO_8)|(AF1<<AFR_GPIO_15));
	/*----------------------------------- GPIOC -----------------------------------------*/	
	 GPIOC->MODER  =  ( GPIO_MODER_MODER0	|	 //BATTERY VOLTAGE
		 				GPIO_MODER_MODER1	|	 //VACUUM CURRENT
                       GPIO_MODER_MODER2    |    //SYSTEM CURRENT
                       GPIO_MODER_MODER3    |    //MAINBRUSH CURRENT
                       GPIO_MODER_MODER4    |    //OBS WALL AD
                       GPIO_MODER_MODER6_1  |    //WIFI TX
                       GPIO_MODER_MODER7_1  |    //WIFI RX
                       //GPIO_MODER_MODER8_1  |    //CHARGER PWM
                       GPIO_MODER_MODER9_0  	 //WIFI CTRL 3V3
					   );    

	 GPIOC->AFR[0] |= (((uint32_t)(AF8)<<AFR_GPIO_6)|((uint32_t)(AF8)<<AFR_GPIO_7)); 
	 GPIOC->AFR[1] |= (AF2<<AFR_GPIO_8);  
	/*----------------------------------- GPIOD -----------------------------------------*/
	 GPIOD->MODER   = (
		 			    GPIO_MODER_MODER0_0  |	 //4051_A2
						GPIO_MODER_MODER1_0  |	 //4051_A1
						GPIO_MODER_MODER2_0  |	 //4051_A0
                        GPIO_MODER_MODER6_0  |   //CURRENT CTRL									 
	                    GPIO_MODER_MODER8_0  |   //RW DIR
						GPIO_MODER_MODER10_0 |   //WIFI WAKE UP
						GPIO_MODER_MODER12_0 |   //RW SLEEP
						GPIO_MODER_MODER13_0 |	 //VCC OFF
	                    GPIO_MODER_MODER15_0 );	 //AUDIO DATA
	/*----------------------------------- GPIOE -----------------------------------------*/	
	GPIOE->MODER = (
	                GPIO_MODER_MODER2_0  |	   //SW 3V3 5V
	                GPIO_MODER_MODER5_1  |     //PUMP PWM
					GPIO_MODER_MODER6_1  |     //BRUSH PWM
	                GPIO_MODER_MODER8_0  |     //KEY DIO
	                GPIO_MODER_MODER9_0  |     //KEY CLK
	                GPIO_MODER_MODER10_0 |     //KEY STB
	                GPIO_MODER_MODER11_0 |     //OBS CTRL 5V
					GPIO_MODER_MODER12_0 |     //LW DIR
	                GPIO_MODER_MODER14_1 );    //LW PWM 
					
	 GPIOE->OTYPER |= GPIO_OTYPER_OT_5|GPIO_OTYPER_OT_6;

	 GPIOE->AFR[0] |= ((AF3<<AFR_GPIO_5)|(AF3<<AFR_GPIO_6));
	 GPIOE->AFR[1] |= ((AF1<<AFR_GPIO_14));   
}

void EXTI_Configuration(void)
{ 
	SYSCFG->EXTICR[0] =	(SYSCFG_EXTICR1_EXTI0_PE|       //FR_RCON
						SYSCFG_EXTICR1_EXTI3_PE);	    //PUMPER	

	SYSCFG->EXTICR[1] = (SYSCFG_EXTICR2_EXTI4_PB|   	//FL_RCON
	                    SYSCFG_EXTICR2_EXTI7_PD);   	//MOBILITY 
	                  
	SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI11_PA; 		//BLDC OUT
										
	SYSCFG->EXTICR[3] = (SYSCFG_EXTICR4_EXTI13_PB|   	//R WHEEL SPEED
	                    SYSCFG_EXTICR4_EXTI15_PE);  	//L WHEEL SPEED

	EXTI->IMR =  (
					EXTI_IMR_MR0 |  			
					EXTI_IMR_MR3 |  			
					EXTI_IMR_MR4 | 				
					EXTI_IMR_MR7|  			
					EXTI_IMR_MR11| 				  			
					EXTI_IMR_MR13|  		
					EXTI_IMR_MR15|  		
					EXTI_IMR_MR17
				); 			

	EXTI->FTSR = (
					EXTI_FTSR_TR0|  			
					EXTI_FTSR_TR3|  			
					EXTI_FTSR_TR4|  			
					EXTI_FTSR_TR7|				
					EXTI_FTSR_TR11| 			
					EXTI_FTSR_TR13| 			
					EXTI_FTSR_TR15| 			
					EXTI_FTSR_TR17
				);			


	EXTI->RTSR = (	
					EXTI_RTSR_TR0|   			
					EXTI_FTSR_TR3|	 			
					EXTI_FTSR_TR4|	 			
					EXTI_RTSR_TR7|  			
					EXTI_RTSR_TR11|  			
					EXTI_RTSR_TR13|  			
					EXTI_RTSR_TR15|  			
					EXTI_RTSR_TR17
				);
}

void NVIC_Configuration(FunctionalState temp)
{
	NVIC_InitTypeDef NVIC_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 		
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn ;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);

	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
	NVIC_InitStructure.NVIC_IRQChannelCmd = temp;
	NVIC_Init(&NVIC_InitStructure);
}

void Timer1_Configuration(void)//rw lw
{
	TIM1->ARR = Timer1_Count_Range;
	TIM1->PSC = Timer1_Prescaler_10K;
	TIM1->EGR = TIM_EGR_UG;
	TIM1->BDTR = TIM_BDTR_MOE;
	TIM1->CCER = TIM_CCER_CC3NE|TIM_CCER_CC4E;
	TIM1->CCMR2 = TIM_CCMR2_OC3PE|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2
				 |TIM_CCMR2_OC4PE|TIM_CCMR2_OC4M_1|TIM_CCMR2_OC4M_2;	
	TIM1->CR1 = TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer2_Configuration(void) //base timer
{
	TIM2->ARR = Timer2_Count_Range;
	TIM2->PSC = Timer2_Prescaler_10k;
	TIM2->EGR = TIM_EGR_UG;
	TIM2->DIER = TIM_DIER_UIE;
	TIM2->CR1  |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer3_Configuration(void)//charge
{
	TIM3->ARR = Timer3_Count_Range;
	TIM3->PSC = Timer3_Prescaler_42k;
	TIM3->EGR = TIM_EGR_UG;
	TIM3->CCER = TIM_CCER_CC3E;
	TIM3->CCMR2 = TIM_CCMR2_OC3PE|TIM_CCMR2_OC3M_1|TIM_CCMR2_OC3M_2;
	TIM3->CR1  |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer4_Configuration(void)//codeshow
{
	TIM4->ARR = Timer4_Count_Range;
	TIM4->PSC = Timer4_Prescaler_20k;
	TIM4->CCER = TIM_CCER_CC2E;
	TIM4->DIER &= ~ TIM_DIER_UIE;
	TIM4->CCMR1 = TIM_CCMR1_OC2PE|TIM_CCMR1_OC2M;
	TIM4->CCR2 = 80;
	TIM4->EGR = TIM_EGR_UG;
	TIM4->CR1 |= TIM_CR1_ARPE;
	TIM4->CR1 &=~TIM_CR1_CEN;
}

void Timer4_Wakeup_Configuration(void) //20S
{
	RCC->APB1LPENR |= RCC_APB1LPENR_TIM4LPEN;
	TIM4->ARR = 19999;
	TIM4->PSC = 41999;
	TIM4->EGR = TIM_EGR_UG;
	TIM4->DIER = TIM_DIER_UIE;
	TIM4->CR1  |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer5_Configuration(void) //obs driver and adc start
{
	TIM5->ARR = Timer5_Count_Range;
	TIM5->PSC = Timer5_Prescaler_20k;
	TIM5->EGR = TIM_EGR_UG;
	TIM5->DIER = TIM_DIER_UIE;
	TIM5->CR1  |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer9_Configuration(void) //pump mainbrush
{
	TIM9->ARR  = Timer9_Count_Range;
	TIM9->PSC  = Timer9_Prescaler_10K;
	TIM9->EGR  = TIM_EGR_UG;

	TIM9->CCER = TIM_CCER_CC1E;
	TIM9->CCER |= TIM_CCER_CC2E;

	TIM9->CCMR1 = (TIM_CCMR1_OC1PE|TIM_CCMR1_OC1M_0|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2); 
	TIM9->CCMR1 |= (TIM_CCMR1_OC2PE|TIM_CCMR1_OC2M_0|TIM_CCMR1_OC2M_1|TIM_CCMR1_OC2M_2); 

	TIM9->CR1 |=TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer10_Configuration(void) //bldc
{
	TIM10->ARR  = Timer10_Count_Range;
	TIM10->PSC  = Timer10_Prescaler_20K;
	TIM10->EGR  = TIM_EGR_UG;
	TIM10->CCER = TIM_CCER_CC1E;
	TIM10->CCMR1 = (TIM_CCMR1_OC1PE|TIM_CCMR1_OC1M_1|TIM_CCMR1_OC1M_2);
	TIM10->CR1 |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Timer11_Configuration(void)//display
{
	TIM11->ARR = Timer11_Count_Range;
	TIM11->PSC = Timer11_Prescaler_10k;
	TIM11->EGR = TIM_EGR_UG;
	TIM11->CR1  |= TIM_CR1_CEN|TIM_CR1_ARPE;
}

void Waiting_Disable_DMA(DMA_Stream_TypeDef *Stream)
{
	uint32_t TimeOut = 0;

	while(Stream->CR & DMA_SxCR_EN) 
	{
		TimeOut++;
		if(TimeOut>0xffff)break;
	}

}

void Waiting_Sending_DMA(DMA_Stream_TypeDef *Stream)
{
	uint32_t TimeOut = 0;

	while(Stream->NDTR)  
	{
		TimeOut++;
		if(TimeOut>0xfffff)break;
	}
}



