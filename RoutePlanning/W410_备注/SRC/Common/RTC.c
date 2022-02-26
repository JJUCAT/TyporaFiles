#include "Include.h"

uint8_t RTC_WAKE_FLAG;
uint8_t RTC_ALarm_FLAG;


void RTC_System_Init(void)
{
	RTC_Init();
	
}


void SET_RTC_Wake_UP_Time(uint8_t wksel,uint8_t cnt)
{
	RTC_Set_WakeUp(wksel,cnt);
	Rest_RTC_Wake_FLAG();
}

void SET_RTC_ALarm_Time(uint8_t week,uint8_t hour,uint8_t min,uint8_t sec)
{
	RTC_Set_AlarmA( week, hour, min, sec);
	Rest_RTC_ALarm_FLAG();
}


uint8_t IS_RTC_Wake_UP(void)
{
	return RTC_WAKE_FLAG;
}

void Set_RTC_Wake_FLAG(void)
{
	RTC_WAKE_FLAG = RTC_WAKE_ON;
}

void Rest_RTC_Wake_FLAG(void)
{
	RTC_WAKE_FLAG = RTC_WAKE_OFF;
}

uint8_t IS_RTC_ALarm(void)
{
	return RTC_ALarm_FLAG;
}

void Set_RTC_ALarm_FLAG(void)
{
	RTC_ALarm_FLAG = RTC_ALarm_ON;
}

void Rest_RTC_ALarm_FLAG(void)
{
	RTC_ALarm_FLAG = RTC_ALarm_OFF;
}



