#include "Include.h"

void Hw_CodeShow_GpioConfig(uint8_t state)
{
    if(state)
    {
        GPIOB->MODER &=	~GPIO_MODER_MODER7_0;
        GPIOB->MODER |=	GPIO_MODER_MODER7_1;
        GPIOB->AFR[0] |=(AF2<<AFR_GPIO_7);        
    }
    else
    {
        GPIOB->MODER &=	~GPIO_MODER_MODER7_1;
        GPIOB->MODER |=	GPIO_MODER_MODER7_0;
        GPIOB->AFR[0] &=~(AF2<<AFR_GPIO_7);        
    }
}

void Hw_CodeShow_TimerConfig(uint8_t state)
{
    if(state)
    {
        TIM4->CR1  |= TIM_CR1_CEN;
    }
    else
    {
        TIM4->CR1 &= ~TIM_CR1_CEN;
    }
}
















