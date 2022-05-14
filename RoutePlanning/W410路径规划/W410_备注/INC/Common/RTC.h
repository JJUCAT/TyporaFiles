#ifndef __RTC_H
#define __RTC_H
#include <stdint.h>
#include "Hw_SysInitialize.h"

#define RTC_WAKE_ON  1
#define RTC_WAKE_OFF 0

#define RTC_ALarm_ON  1
#define RTC_ALarm_OFF 0

void RTC_System_Init(void);
void SET_RTC_Wake_UP_Time(uint8_t wksel,uint8_t cnt);
void SET_RTC_ALarm_Time(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec);

uint8_t IS_RTC_Wake_UP(void);
void Set_RTC_Wake_FLAG(void);
void Rest_RTC_Wake_FLAG(void);

uint8_t IS_RTC_ALarm(void);
void Set_RTC_ALarm_FLAG(void);
void Rest_RTC_ALarm_FLAG(void);

#endif
