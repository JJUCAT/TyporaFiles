#include "Include.h"

void Hw_Speaker_SetDataIoState(uint8_t state)
{
    if(state)
    {
        SPK_IO_HIGH;
    }
    else
    {
        SPK_IO_LOW;
    } 
}

uint8_t Hw_Speaker_GetBusyIoState(void)
{
    uint8_t reval = 0;
    if(SPK_BUSY_IN)
    {
        reval = 1;
    }
    return  reval;
}























