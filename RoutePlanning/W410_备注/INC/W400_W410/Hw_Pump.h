#ifndef __HW_PUMP_H
#define __HW_PUMP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"




void Hw_Pump_SetState(uint8_t state);
volatile uint32_t *Hw_Pump_Pwm(void);










#endif







