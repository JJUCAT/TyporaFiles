#include "Include.h"


uint16_t Hw_Charge_GetAdc(void)
{
    return g_adc_value.charger;
}

void Hw_Charge_GpioConfig(uint8_t state)
{
    if(state)
    {
        GPIOC->MODER |= GPIO_MODER_MODER8_1;
    }
    else
    {
        GPIOC->MODER &= ~GPIO_MODER_MODER8;
    }
}

void Hw_Charge_TimerConfig(uint8_t state)
{
    if(state)
    {
        Timer3_Configuration();
    }
    else
    {
        TIM3->CR1&=~TIM_CR1_CEN;  
    }
}

void Hw_Charge_PowerConfig(uint8_t state)
{
    if(state)
    {
        CTRL_SW_5V_3V3_ON;
    }
    else
    {
        CTRL_SW_5V_3V3_OFF;
    }
}

volatile uint32_t *Hw_Charge_Pwm(void)
{
    return &TIM3->CCR3;
}


















