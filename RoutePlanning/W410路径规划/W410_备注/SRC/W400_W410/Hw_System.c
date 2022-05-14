#include "Include.h"

uint16_t Hw_System_GetAdc(void)
{
    return g_adc_value.system;
}

void Hw_System_SetPowerState(uint8_t state)
{
    if(state)
    {
        I_CTRL_ON;
    }
    else
    {
        I_CTRL_OFF;
    }  
}



























