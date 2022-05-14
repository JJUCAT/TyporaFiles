#ifndef __REALTIMEMONITOR_H
#define __REALTIMEMONITOR_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void (*RealTimeMonitor_UsartSend)(char *p_data,uint8_t length);
extern RealTimeMonitor_UsartSend on_realtimemonitor_usart_send;

void RealTimeMonitor_Init(void);
void RealTimeMonitor_LoadData(int32_t x, int32_t y, int32_t angle, uint8_t type,char *data);
void RealTimeMonitor_SendData(int32_t x, int32_t y, int32_t angle, uint8_t type);










#endif










