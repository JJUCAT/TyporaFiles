#include "Include.h"


void Hw_Power_Set_3V3_5V_State(uint8_t state)
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


void Hw_Power_Set_Obs_State(uint8_t state)
{
    if(state)
    {
        CTRL_OBS_ON;
    }
    else
    {
        CTRL_OBS_OFF; 
    }   
}


void Hw_Power_Set_Battery_State(uint8_t state)
{
    if(state)
    {
        BATTERY_CTRL_ON;
    }
    else
    {
        BATTERY_CTRL_OFF; 
    }   
}

void Hw_Power_Set_Wifi_State(uint8_t state)
{
    if(state)
    {
        CTRL_WIFI_ON;
    }
    else
    {
        CTRL_WIFI_OFF; 
    }   
}

void Hw_Power_Set_Kill_Vcc_State(uint8_t state)
{
    if(state)
    {
        VCC_ALL_OFF_ON;
    }
    else
    {
        VCC_ALL_OFF_OFF; 
    }   
}










