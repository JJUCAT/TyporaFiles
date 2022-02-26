#ifndef __HW_OBS_H
#define __HW_OBS_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define WALL_OBS_CTRL_OFF				(GPIOB->BSRRL|=MCU_WALL_OBS_CTRL)
#define WALL_OBS_CTRL_ON				(GPIOB->BSRRH|=MCU_WALL_OBS_CTRL)


void Hw_Obs_SetDriverState(uint8_t state);













#endif





































