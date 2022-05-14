#include "Include.h"

uint16_t Hw_Vacuum_GetAdc(void)
{
    return g_adc_value.vac;
}

volatile uint32_t *Hw_Vacuum_Pwm(void)
{
    return &TIM10->CCR1;
}

void Hw_Vacuum_SetPowerState(uint8_t state)
{
    if(state)
    {
        BLDC_ON;
    }
    else
    {
        BLDC_OFF;
    }  
}


















