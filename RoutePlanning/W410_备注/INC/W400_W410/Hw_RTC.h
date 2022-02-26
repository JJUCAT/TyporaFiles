#ifndef __HW_RTC_H
#define __HW_RTC_H

#include "Hw_SysInitialize.h"
#include <stdint.h>

void RTC_Init(void);

uint8_t RTC_Wait_Synchro(void);
uint8_t RTC_Init_Mode(void);
void RTC_Write_BKR(uint32_t BKRx,uint32_t data);
uint32_t RTC_Read_BKR(uint32_t BKRx);
uint8_t RTC_DEC2BCD(uint8_t val);
uint8_t RTC_BCD2DEC(uint8_t val);
uint8_t RTC_Set_Time(uint8_t hour,uint8_t min,uint8_t sec,uint8_t ampm);
uint8_t RTC_Set_Date(uint8_t year,uint8_t month,uint8_t date,uint8_t week);
void RTC_Get_Time(uint8_t *hour,uint8_t *min,uint8_t *sec,uint8_t *ampm);
void RTC_Get_Date(uint8_t *year,uint8_t *month,uint8_t *date,uint8_t *week);
void RTC_Set_AlarmA(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);
void RTC_Set_WakeUp(uint8_t wksel,uint8_t cnt);
uint8_t RTC_Get_Week(uint16_t year,uint8_t month,uint8_t day);

#endif

