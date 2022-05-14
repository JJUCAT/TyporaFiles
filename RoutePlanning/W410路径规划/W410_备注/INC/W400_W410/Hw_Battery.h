#ifndef __HW_BATTERY_H
#define __HW_BATTERY_H

#include <stdint.h>
#include "Hw_SysInitialize.h"



uint16_t Hw_Battery_GetAdc(void);
uint8_t Hw_Battery_Voltage2Level(uint16_t voltage);
















#endif








