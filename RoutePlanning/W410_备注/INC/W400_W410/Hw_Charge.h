#ifndef __HW_CHARGE_H
#define __HW_CHARGE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"



uint16_t Hw_Charge_GetAdc(void);
void Hw_Charge_GpioConfig(uint8_t state);
void Hw_Charge_TimerConfig(uint8_t state);
void Hw_Charge_PowerConfig(uint8_t state);
volatile uint32_t *Hw_Charge_Pwm(void);












#endif
































