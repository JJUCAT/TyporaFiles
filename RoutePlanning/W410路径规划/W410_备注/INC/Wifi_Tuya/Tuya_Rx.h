#ifndef __TUYA_RX_H
#define __TUYA_RX_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


extern TuyaStruct_t *p_tuya_rx;

void TuyaRx_Init(void);
void TuyaRx_RecieveData(unsigned char value);
unsigned char TuyaRx_GetQueueTotalData(void);
unsigned char TuyaRx_QueueReadByte(void);

















#endif









