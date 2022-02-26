#include "Include.h"

void Hw_Hc4051_SetChannelA0State(uint8_t state)
{
    if(state)
    {
        MCU_4051_A0_ON;
    }
    else
    {
        MCU_4051_A0_OFF;
    }
}

void Hw_Hc4051_SetChannelA1State(uint8_t state)
{
    if(state)
    {
        MCU_4051_A1_ON;
    }
    else
    {
        MCU_4051_A1_OFF;
    }
}


void Hw_Hc4051_SetChannelA2State(uint8_t state)
{
    if(state)
    {
        MCU_4051_A2_ON;
    }
    else
    {
        MCU_4051_A2_OFF;
    }
}





















