#ifndef __HW_BUTTON_H
#define __HW_BUTTON_H

#include "Hw_SysInitialize.h"
#include <stdint.h>

#define BUTTON_LEFT		    (uint8_t)0x02
#define BUTTON_RIGHT	    (uint8_t)0x10


uint8_t Hw_Button_CleanTrig(void);
uint8_t Hw_Button_LeftKeyTrig(void);
uint8_t Hw_Button_RightKeyTrig(void);

















#endif



