 #include "Include.h"

volatile uint32_t g_timer_1ms = 0;	

uint32_t Timer_GetTime(void)
{
	return g_timer_1ms;
}

void Timer_ResetTime(void)
{
	g_timer_1ms = 0;
}

void Timer_IncreaseCounter(void)
{
	g_timer_1ms++;
}

uint32_t Timer_Abs(uint32_t a,uint32_t b)
{
	uint32_t reval = 0;

	if(a > b)
	{
		reval = a - b;	
	}
	else
	{
		reval = b - a;	
	}	
	return reval;
}

uint32_t Timer_Get_DiffNow(uint32_t start)
{
	return Timer_Abs(Timer_GetTime(),start);
}
 
int32_t Timer_IsTimeReach(uint32_t *pre_time,uint32_t width)
{
	uint32_t delta_time,cur_time = Timer_GetTime();

	if ((cur_time > 0xFFFF0000) && (*pre_time > (*pre_time + width)))
	{
		Usprintf("time overflow:%d,%d\n",cur_time,*pre_time);
		return 0;
	}
	
	if (cur_time >= (*pre_time + width))
	{
		delta_time = cur_time - *pre_time;
		*pre_time = cur_time;
		return delta_time;
	}
	else if(cur_time < *pre_time)
	{
		*pre_time = cur_time;
	}
	
	return 0;
}
















