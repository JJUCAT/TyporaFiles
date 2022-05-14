#ifndef __STATION_H
#define __STATION_H

#include <stdint.h>
#include "Hw_SysInitialize.h"



typedef struct 
{
  uint8_t start_state;
  uint8_t move_state;
}Station_t;

extern Station_t g_station;
void Station_Init(void);
void Station_SetStartState(uint8_t state);
uint8_t Station_GetStartState(void);
void Station_SetMoveState(uint8_t state);
uint8_t Station_GetMoveState(void);












#endif









