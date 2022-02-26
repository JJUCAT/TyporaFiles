#include "Include.h"


void Hw_Cliff_PowerCtrl(uint8_t state)
{
    if(state)
    {
        GPIOB->BSRRH|=MCU_CLIFF_CTRL;
    }
    else
    {
        GPIOB->BSRRL|=MCU_CLIFF_CTRL;
    }
}













