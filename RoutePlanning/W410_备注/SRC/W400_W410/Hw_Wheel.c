#include "Include.h"


void Hw_Wheel_SetLeftEnable(uint8_t state)
{
    if(state)
    {
        LW_SLEEP_OFF; 
    }
    else
    {
        LW_SLEEP_ON;
    }    
}

void Hw_Wheel_SetRightEnable(uint8_t state)
{
    if(state)
    {
        RW_SLEEP_OFF;
    }
    else
    {
        RW_SLEEP_ON;
    }    
}

void Hw_Wheel_SetLeftDir(uint8_t state)
{
    if(state)
    {
        LW_DIR_FORWARD;
    }
    else
    {
        LW_DIR_BACKWARD;
    }    
}

void Hw_Wheel_SetRightDir(uint8_t state)
{
    if(state)
    {
        RW_DIR_FORWARD;
    }
    else
    {
        RW_DIR_BACKWARD;
    }    
}


uint16_t Hw_Wheel_GetLeftAdc(void)
{
    return g_adc_value.left_wheel;
}

uint16_t Hw_Wheel_GetRightAdc(void)
{
    return g_adc_value.right_wheel;
}


volatile uint32_t *Hw_Wheel_LeftPwm(void)
{
    return &TIM1->CCR4;
}

volatile uint32_t *Hw_Wheel_RightPwm(void)
{
    return &TIM1->CCR3;
}









