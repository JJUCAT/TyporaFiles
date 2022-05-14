#ifndef __HW_SLEEP_H
#define __HW_SLEEP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum 
{
    HW_SLEEP_CONFIG_INIT = 0,
    HW_SLEEP_CONFIG_EXIT,
    HW_SLEEP_CONFIG_WAKE_UP,
}HwSleepConfigState_t;

void Hw_Sleep_SystickConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_PowerConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_ExtiIConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_NvicConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_AdcConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_ExtiPendingConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_GpioConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_RccConfiguration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_Time_Configuration(uint8_t state,uint8_t charge_flag);
void Hw_Sleep_Enter(void);
#endif
































































































































