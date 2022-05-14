#ifndef __TUYA_TX_H
#define __TUYA_TX_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void (*TuyaTx_UsartSend)(uint16_t length,char *data);
extern TuyaTx_UsartSend on_tuya_tx_usart_send;

void TuyaTx_Init(void);
unsigned short TuyaTx_SetUsartByte(unsigned short dest, unsigned char byte);
void TuyaTx_WriteFrame(unsigned char fr_type, unsigned char fr_ver, unsigned short len);
static void TuyaTx_WriteData(unsigned char *in, unsigned short len);
unsigned short TuyaTx_SetUsartBuffer(unsigned short dest, const unsigned char *src, unsigned short len);
unsigned char TuyaTx_DpEnumUpdate(unsigned char dpid,unsigned char value);








#endif





















