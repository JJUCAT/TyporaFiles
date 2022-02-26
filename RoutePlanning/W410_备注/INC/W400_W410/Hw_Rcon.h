#ifndef __HW_RCON_H
#define __HW_RCON_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define RCON_FL_IN (GPIOB->IDR&MCU_LEFT_B_IRM)
#define RCON_FR_IN (GPIOE->IDR&MCU_RIGHT_B_IRM)

uint8_t Hw_Rcon_GetFlState(void);
uint8_t Hw_Rcon_GetFrState(void);















#endif



























