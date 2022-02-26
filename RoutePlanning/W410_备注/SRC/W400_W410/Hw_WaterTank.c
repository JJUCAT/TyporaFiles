 #include "Include.h"

uint8_t Hw_WaterTank_GetTankState(void)
{
    if(GPIOE->IDR & MCU_WATER_TANK)
    {
        return 1;
    }
    return 0;
}

void Hw_WaterTank_SetCleanDriverState(uint8_t state)
{
    if(state)
    {
        MCU_CLN_DRV_ON;
    }
    else
    {
        MCU_CLN_DRV_OFF;
    }
    
}

void Hw_WaterTank_SetDirtyDriverState(uint8_t state)
{
    if(state)
    {
        MCU_DIRTY_DRV_ON;
    }
    else
    {
        MCU_DIRTY_DRV_OFF;
    }   
}

















