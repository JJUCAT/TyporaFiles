#include "Include.h"

uint16_t Hw_MainBrush_GetAdc(void)
{
    return g_adc_value.mainbrush;
}

volatile uint32_t *Hw_MainBrush_Pwm(void)
{
    return &TIM9->CCR2;
}














