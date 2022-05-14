#ifndef __HW_SYSTEM_H
#define __HW_SYSTEM_H

#include "Hw_SysInitialize.h"
#include <stdint.h>


#define I_CTRL_ON			          	(GPIOD->BSRRL|=MCU_I_CTRL)
#define I_CTRL_OFF			        	(GPIOD->BSRRH|=MCU_I_CTRL)


uint16_t Hw_System_GetAdc(void);
void Hw_System_SetPowerState(uint8_t state);











#endif































