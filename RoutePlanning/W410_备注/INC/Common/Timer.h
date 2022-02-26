#ifndef __TIMER_H
#define __TIMER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define TIME_1MS  10
#define TIME_25MS 250
#define TIME_40MS 400
#define TIME_50MS 500
#define TIME_100MS 1000
#define TIME_150MS 1500
#define TIME_200MS 2000
#define TIME_300MS 3000
#define TIME_500MS 5000
#define	TIME_800MS 8000
#define TIME_1S    10000
#define TIME_2S		(TIME_1S * 2)	
#define TIME_3S		(TIME_1S * 3)	
#define TIME_4S		(TIME_1S * 4)
#define TIME_5S		(TIME_1S * 5)
#define TIME_6S		(TIME_1S * 6)
#define TIME_7S		(TIME_1S * 7)
#define	TIME_10S	(TIME_1S * 10)	
#define	TIME_20S	(TIME_1S * 20)
#define	TIME_40S	(TIME_1S * 40)
#define	TIME_50S	(TIME_1S * 50)
#define TIME_1MIN 	(TIME_1S * 60 * 1)
#define TIME_2MIN 	(TIME_1S * 60 * 2)
#define TIME_3MIN 	(TIME_1S * 60 * 3)
#define TIME_6MIN 	(TIME_1S * 60 * 6)
#define TIME_10MIN 	(TIME_1S * 60 * 10)

extern volatile uint32_t g_timer_1ms;

uint32_t Timer_GetTime(void);
void Timer_ResetTime(void);
void Timer_IncreaseCounter(void);
uint32_t Timer_Abs(uint32_t a,uint32_t b);
uint32_t Timer_Get_DiffNow(uint32_t start);
int32_t Timer_IsTimeReach(uint32_t *pre_time,uint32_t width);

#endif














