#ifndef __SYSTEM_H
#define __SYSTEM_H

#include <stdint.h>
#include "Hw_SysInitialize.h"
typedef void(*System_SetPowerState)(uint8_t state);
typedef uint16_t (*System_GetAdc)(void);
typedef struct 
{
  int16_t current;
  int32_t baseline;
  System_SetPowerState on_set_power_state;
  System_GetAdc on_get_adc;
}System_t;

extern System_t g_system;

void System_Init(void);
int32_t System_GetBaselineAdc(void);
void System_SetCurrent(int16_t current);
int16_t System_GetCurrent(void);
void System_CurrentDetectionCycle(float adc2current);
void System_StoreBaselineAdc(void);
void System_UpdateBaselineAdc(void);
void System_SetBaseLineAdc(int32_t baseline);



#endif


