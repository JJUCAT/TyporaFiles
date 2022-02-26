#include "Include.h"


void Hw_Pump_SetState(uint8_t state)
{
    if(state)
    {
        TIM9->CCMR1 |= (uint16_t)(TIM_CCMR1_OC1M_1);
    }
    else
    {
        TIM9->CCMR1 &= ~(uint16_t)(TIM_CCMR1_OC1M_1);	
    }
}


volatile uint32_t *Hw_Pump_Pwm(void)
{
    return &TIM9->CCR1;
}

























