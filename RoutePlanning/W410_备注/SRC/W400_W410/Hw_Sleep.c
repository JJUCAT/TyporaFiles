#include "Include.h"

void Hw_Sleep_SystickConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            STK_CSR &=~ 0X07;
            STK_RVR &=~ 84000;
            STK_CVR = 0;  
        break;
        case HW_SLEEP_CONFIG_EXIT:
            STK_CSR = 0X07;
            STK_RVR = 84000;
            STK_CVR = 0;                                            
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
        break;
        default:break;
    }
}

void Hw_Sleep_PowerConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            CTRL_SW_5V_3V3_OFF;
            BATTERY_CTRL_OFF;
            //CTRL_WIFI_ON;
            if(g_robot.type == ROBOT_W400)
            {
                CTRL_OBS_ON;	
            }
            else
            {
                CTRL_OBS_OFF;          
            }                                        
        break;
        case HW_SLEEP_CONFIG_EXIT:
            BATTERY_CTRL_ON; 
            #ifdef DEBUG_ENABLE
            CTRL_OBS_ON;
            #endif                                        
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
            CTRL_SW_5V_3V3_ON;
            BATTERY_CTRL_ON;
            if(g_robot.type == ROBOT_W400)
            {
                CTRL_OBS_ON;	
            }
            else
            {
                CTRL_OBS_OFF;
            }
        break;
        default:break;
    }    
}

void Hw_Sleep_ExtiIConfiguration(uint8_t state,uint8_t charge_flag)
{	
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            EXTI->IMR = EXTI_IMR_MR1|EXTI_IMR_MR12|EXTI_IMR_MR10|EXTI_IMR_MR17;//charge v + button + wifi + rtc
            if(g_robot.type == ROBOT_W410)	
            {
                EXTI->IMR |= EXTI_IMR_MR4|EXTI_IMR_MR0;//left rcon + right rcon
            }					
            else
            {
                EXTI->IMR |= EXTI_IMR_MR3;//obs rx
            }

            EXTI->EMR = EXTI_EMR_MR1|EXTI_EMR_MR12|EXTI_EMR_MR10|EXTI_EMR_MR17;
            if(g_robot.type == ROBOT_W410)	
            {
                EXTI->EMR |= EXTI_EMR_MR4|EXTI_EMR_MR0;
            }					
            else
            {
                EXTI->EMR |= EXTI_EMR_MR3;
            }

            EXTI->FTSR = EXTI_FTSR_TR1|EXTI_FTSR_TR12|EXTI_FTSR_TR10|EXTI_FTSR_TR17;

            if(g_robot.type == ROBOT_W410)	
            {
                EXTI->FTSR |= EXTI_FTSR_TR4|EXTI_FTSR_TR0;
            }					
            else
            {
                EXTI->FTSR |= EXTI_FTSR_TR3;
            }

            EXTI->RTSR = EXTI_FTSR_TR1|EXTI_RTSR_TR12|EXTI_RTSR_TR10|EXTI_RTSR_TR17;

            if(g_robot.type == ROBOT_W410)	
            {
                EXTI->FTSR |= EXTI_RTSR_TR4|EXTI_RTSR_TR0;
            }					
            else
            {
                EXTI->FTSR |= EXTI_RTSR_TR3;
            }
            
            SYSCFG->EXTICR[0] = 0;
            SYSCFG->EXTICR[0] = SYSCFG_EXTICR1_EXTI1_PB;//charge v

            if(g_robot.type == ROBOT_W410)
            {
                SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI0_PE;//right rcon       
            }
            else
            {
                SYSCFG->EXTICR[0] |= SYSCFG_EXTICR1_EXTI3_PA;//obs rx
            }
                
            SYSCFG->EXTICR[1] = 0;
            if(g_robot.type == ROBOT_W410)
            {
                SYSCFG->EXTICR[1] |= SYSCFG_EXTICR2_EXTI4_PB;//left rcon
            }

            SYSCFG->EXTICR[2] = 0;
            SYSCFG->EXTICR[2] = SYSCFG_EXTICR3_EXTI10_PD;//wifi wake up


            SYSCFG->EXTICR[3] = 0;
            SYSCFG->EXTICR[3] = SYSCFG_EXTICR4_EXTI12_PC;//button                                      
        break;
        case HW_SLEEP_CONFIG_EXIT:
            EXTI_Configuration();                
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
            EXTI_Configuration();  
        break;
        default:break;
    }       		
}

void Hw_Sleep_GpioConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            if(g_robot.type == ROBOT_W400)
            {
                GPIOA->MODER = GPIO_MODER_MODER13|GPIO_MODER_MODER14;//sw
                GPIOA->PUPDR = GPIO_PUPDR_PUPDR3_0;
            }
            else
            {
                GPIOA->MODER = GPIO_MODER_MODER13|GPIO_MODER_MODER14;
            }     		
            GPIOB->MODER = GPIO_MODER_MODER9_0;//battery ctrl
            GPIOC->MODER = GPIO_MODER_MODER0|GPIO_MODER_MODER9_0;//battery voltage + wifi   
            GPIOD->MODER = GPIO_MODER_MODER13_0;//Vcc off 					 
            GPIOE->MODER = GPIO_MODER_MODER2_0|GPIO_MODER_MODER11_0;//sw 3v3 5v + obs ctrl                                  
        break;
        case HW_SLEEP_CONFIG_EXIT:
            if(g_robot.type == ROBOT_W400)
            {
                GPIOA->MODER = GPIO_MODER_MODER2_1|GPIO_MODER_MODER3_1;//obs tx rx
            }
            #ifdef DEBUG_ENABLE
            GPIOA->MODER = GPIO_MODER_MODER2_1|GPIO_MODER_MODER3_1;
            #endif
            GPIOB->MODER = GPIO_MODER_MODER1|GPIO_MODER_MODER9_0;//charger detect and battery ctrl 
            GPIOC->MODER = GPIO_MODER_MODER0|GPIO_MODER_MODER9_0|GPIO_MODER_MODER6_1|GPIO_MODER_MODER7_1;//battery voltage + wifi ctrl + wifi tx rx
            GPIOD->MODER = GPIO_MODER_MODER13_0;//Vcc off 	
            GPIOE->MODER = GPIO_MODER_MODER2_0|GPIO_MODER_MODER11_0;//sw 3v3 5v + obs ctrl              
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
            GPIO_Configuration();
        break;
        default:break;
    }   
}

void Hw_Sleep_NvicConfiguration(uint8_t state,uint8_t charge_flag)
{
	NVIC_InitTypeDef NVIC_InitStructure;
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); 		
            NVIC_InitStructure.NVIC_IRQChannel = TIM1_BRK_TIM9_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
						
						if(!charge_flag)
						{
            NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
						}

            NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn; 
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_TIM10_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = TIM1_TRG_COM_TIM11_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 7;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
            
            NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = USART6_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
            //0 1 3 4 10 12 17
            NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn ;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);

            NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;
            NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
            NVIC_Init(&NVIC_InitStructure);
            
            NVIC_InitStructure.NVIC_IRQChannel = DMA1_Stream6_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);
            
            NVIC_InitStructure.NVIC_IRQChannel = DMA2_Stream7_IRQn;
            NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 14;
            NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
            NVIC_Init(&NVIC_InitStructure);                               
        break;
        case HW_SLEEP_CONFIG_EXIT:
             NVIC_Configuration(ENABLE);
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:

        break;
        default:break;
    } 
}

void Hw_Sleep_AdcConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            ADC1->CR2 &= ~(ADC_CR2_ADON|ADC_CR2_SWSTART);

            DMA2_Stream0->CR &= ~DMA_SxCR_EN;                      
        break;
        case HW_SLEEP_CONFIG_EXIT:
            ADC1->CR2 |= (ADC_CR2_ADON|ADC_CR2_SWSTART);

            DMA2_Stream0->CR |= DMA_SxCR_EN;
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:

        break;
        default:break;
    } 
}


void Hw_Sleep_RccConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
            RCC->AHB1ENR &= ~( RCC_AHB1ENR_GPIOAEN  |
                            RCC_AHB1ENR_GPIOBEN  |
                            RCC_AHB1ENR_GPIOCEN  |
                            RCC_AHB1ENR_GPIODEN  |
                            RCC_AHB1ENR_GPIOEEN  |
                            RCC_AHB1ENR_DMA1EN   |
                            RCC_AHB1ENR_DMA2EN   );

            RCC->APB1ENR &= ~( RCC_APB1ENR_TIM2EN   |
                            RCC_APB1ENR_TIM3EN   |                            
                            RCC_APB1ENR_TIM5EN   |
                            RCC_APB1ENR_USART2EN );
				
						if(!charge_flag)RCC->APB1ENR &= ~(RCC_APB1ENR_TIM4EN);   
						

            RCC->APB2ENR &= ~( RCC_APB2ENR_SYSCFGEN |
                            RCC_APB2ENR_USART1EN |
                            RCC_APB2ENR_USART6EN |
                            RCC_APB2ENR_TIM1EN   |
                            RCC_APB2ENR_TIM9EN   |
                            RCC_APB2ENR_TIM10EN  |
                            RCC_APB2ENR_TIM11EN  |
                            RCC_APB2ENR_ADC1EN   );

            RCC->CFGR &=~ RCC_CFGR_SW_1;
            
            RCC->CR &=~ RCC_CR_HSEON;                    
        break;
        case HW_SLEEP_CONFIG_EXIT:
            RCC_Configuration();
				
        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
            RCC_Configuration();
						
        break;
        default:break;
    }
}

void Hw_Sleep_ExtiPendingConfiguration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
             EXTI->PR |=0X007BFFFF;     
        break;
        case HW_SLEEP_CONFIG_EXIT:

        break;
        case HW_SLEEP_CONFIG_WAKE_UP:

        break;
        default:break;
    }     
}

void Hw_Sleep_Time_Configuration(uint8_t state,uint8_t charge_flag)
{
    switch(state)
    {
        case HW_SLEEP_CONFIG_INIT:
					
            if(charge_flag)
							{
								Timer4_Wakeup_Configuration();
							}
        break;
        case HW_SLEEP_CONFIG_EXIT:
					
					if(charge_flag)
							{
								//Timer4_WAKEUP_Configuration();
							}

        break;
        case HW_SLEEP_CONFIG_WAKE_UP:
							Timer4_Configuration();
        break;
        default:break;
    }     
}



void Hw_Sleep_Enter(void)
{
    __WFI();
}





























