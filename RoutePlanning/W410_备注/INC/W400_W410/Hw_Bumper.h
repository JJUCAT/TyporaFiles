#ifndef __HW_BUMPER_H
#define __HW_BUMPER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define BUMPER_TRIG_V_MIN             2500
#define BUMPER_TRIG_V_MAX             2850
#define BUMPER_TRIG_DIF				  250


uint8_t Hw_Bumper_LeftTrig(void);
uint8_t Hw_Bumper_RightTrig(void);










#endif








