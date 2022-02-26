#ifndef __HW_VACUUM_H
#define __HW_VACUUM_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define BLDC_ON			(GPIOA->BSRRL|=MCU_VACUUM_PWM)
#define BLDC_OFF		(GPIOA->BSRRH|=MCU_VACUUM_PWM)

uint16_t Hw_Vacuum_GetAdc(void);
volatile uint32_t *Hw_Vacuum_Pwm(void);
void Hw_Vacuum_SetPowerState(uint8_t state);







#endif

















































































