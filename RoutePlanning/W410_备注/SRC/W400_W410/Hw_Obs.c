#include "Include.h"


void Hw_Obs_SetDriverState(uint8_t state)
{
    if(state)
    {
        WALL_OBS_CTRL_ON;
    }
    else
    {
        WALL_OBS_CTRL_OFF;
    }  
}























