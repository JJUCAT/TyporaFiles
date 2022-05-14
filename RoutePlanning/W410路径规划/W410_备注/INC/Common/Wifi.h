#ifndef __WIFI_H
#define __WIFI_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define WIFI_FLASH_VALUE 3

typedef struct 
{
  uint8_t power_state;

}Wifi_t;

extern Wifi_t g_wifi; 

void Wifi_Cycle(void);
void Wifi_Init(void);
void Wifi_Reset(void);
void Wifi_SaveState(void);
void Wifi_PowerConfig(void);
void Wifi_ResetWorkMode(void);
void Wifi_ResetWorking(void);
void Wifi_ResetParameters(void);
void Wifi_RecieveData(void);
void Wifi_NavStart(void);
void Wifi_NavEnd(void);
void Wifi_ResetStartNav(void);
void Wifi_AddRealMapToSend(Point16_t point);
void Wifi_ParameterInit(void);
uint8_t Wifi_Config_Start(void);
void Wifi_Config(void);
uint8_t Wifi_IsParameterUpdate(void);
void Wifi_GetState(void);
uint8_t Wifi_IsSendingHistory(void);
void Wifi_ResetConnected(void);
uint8_t Wifi_IsConnected(void);
void Wifi_SetFactoryTestMode(uint8_t state);
void Wifi_SendFactoryCmd(void);
void Wifi_TestStart(void);
void Wifi_TestCycle(void);
void Wifi_TestInit(void);
void Wifi_Stop(void);
void Wifi_WakeUp(void);
void Wifi_UpdateRobot(Point16_t cell,int16_t angle);
void Wifi_UpdateRealMapEdg(int16_t x_min,int16_t x_max,int16_t y_min,int16_t y_max);
#endif




