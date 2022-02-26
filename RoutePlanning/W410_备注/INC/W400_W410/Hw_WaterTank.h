#ifndef __HW_WATERTANK_H
#define __HW_WATERTANK_H

#include "Hw_SysInitialize.h"
#include <stdint.h>

#define MCU_CLN_DRV_OFF					(GPIOA->BSRRL|=MCU_CLN_DRV)
#define MCU_CLN_DRV_ON					(GPIOA->BSRRH|=MCU_CLN_DRV)
#define MCU_DIRTY_DRV_OFF				(GPIOB->BSRRL|=MCU_DIRTY_DRV)
#define MCU_DIRTY_DRV_ON				(GPIOB->BSRRH|=MCU_DIRTY_DRV)


uint8_t Hw_WaterTank_GetTankState(void);
void Hw_WaterTank_SetCleanDriverState(uint8_t state);
void Hw_WaterTank_SetDirtyDriverState(uint8_t state);










#endif






























