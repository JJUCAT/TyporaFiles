




#ifndef __TUYA_MATH_H
#define __TUYA_MATH_H

#include <stdint.h>
#include "Hw_SysInitialize.h"



unsigned char TuyaMath_GetCheckSum(unsigned char *pack, unsigned short pack_len);
void *TuyaMath_Memcpy(void *dest, const void *src, unsigned short count);
unsigned long TuyaMath_Strlen(unsigned char *str);
unsigned char TuyaMath_GetDownloadCmdTotal(void);
unsigned char TuyaMath_GetDowmloadDpidIndex(unsigned char dpid);
unsigned char TuyaMath_GetDpType(unsigned char index);
unsigned char TuyaMath_GetDpDownloadEnum(const unsigned char value[],unsigned short len);
unsigned char TuyaMath_GetDpDownloadBool(const unsigned char value[],unsigned short len);












#endif

















