#include "Include.h"

uint8_t Hw_Rcon_GetFlState(void)
{
    uint8_t reval = 0;
    if(RCON_FL_IN)
    {
        reval = 1;
    }
    return reval;  
}

uint8_t Hw_Rcon_GetFrState(void)
{
    uint8_t reval = 0;
    if(RCON_FR_IN)
    {
        reval = 1;
    }
    return reval;  
}










































