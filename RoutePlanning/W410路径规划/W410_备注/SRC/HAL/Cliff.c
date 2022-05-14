#include "Include.h"

Cliff_t g_cliff;

void Cliff_Init(void)
{
	memset(&g_cliff,0,sizeof(g_cliff));
	g_cliff.on_power_ctrl = Hw_Cliff_PowerCtrl;
	g_cliff.on_set_wheel_stop = Wheel_ResetPWM;
	g_cliff.on_is_wheel_forward = Wheel_IsForward;
}

void Cliff_ForwardStopEnable(void)
{
	g_cliff.stop = ENABLE;
}

void Cliff_DetectionProcess(void) 
{
	static CliffTrig_t temp_status = CLIFF_TRIG_NO;

	if(g_cliff.adc.left < CLIFF_LIMIT)
	{
		g_cliff.l_trig_cnt++;
		if(g_cliff.l_trig_cnt > 2)
		{
			g_cliff.l_trig_cnt = 2;
			temp_status |= CLIFF_TRIG_LEFT;
		}
	}
	else
	{
		g_cliff.l_trig_cnt = 0;
		temp_status &=~ CLIFF_TRIG_LEFT;
	}

	if(g_cliff.adc.right < CLIFF_LIMIT)
	{
		g_cliff.r_trig_cnt++;
		if(g_cliff.r_trig_cnt > 2)
		{
			g_cliff.r_trig_cnt	= 2;
			temp_status |= CLIFF_TRIG_RIGHT;
		}
	}
	else
	{
		g_cliff.r_trig_cnt = 0;
		temp_status &=~ CLIFF_TRIG_RIGHT;
	}

	if(g_cliff.adc.front < CLIFF_LIMIT)
	{
		g_cliff.f_trig_cnt++;
		if(g_cliff.f_trig_cnt > 2) 
		{
			g_cliff.f_trig_cnt	=	2;
			temp_status |= CLIFF_TRIG_FRONT;
		}
	}
	else
	{
		g_cliff.f_trig_cnt = 0;
		temp_status &=~ CLIFF_TRIG_FRONT;
	}

	if(g_cliff.l_trig_cnt||g_cliff.f_trig_cnt||g_cliff.r_trig_cnt)
	{
		if(g_cliff.on_is_wheel_forward() && g_cliff.stop) 
		{
			g_cliff.on_set_wheel_stop();			
		}
	}

	g_cliff.result = temp_status;
}

uint8_t Cliff_IsTrigStop(void)
{
	if(!g_cliff.stop)return 0;
//	Usprintf("cliff left:%d  cliff right:%d   cliff front:%d    \r\n",g_cliff.adc.left,g_cliff.adc.right,g_cliff.adc.front);
	if((g_cliff.adc.left < CLIFF_LIMIT)||(g_cliff.adc.right < CLIFF_LIMIT)||(g_cliff.adc.front < CLIFF_LIMIT))
	{
		
		if(g_cliff.on_is_wheel_forward()) 
		{
			return 1;
		}
	}
	return 0;		
}

uint8_t Cliff_IsPickUp(void)
{	
	
	if(g_cliff.adc.left < CLIFF_LIMIT && g_cliff.adc.right < CLIFF_LIMIT && g_cliff.adc.front < CLIFF_LIMIT)
	{
		return 1;	
	}
	return 0;		
}

CliffTrig_t Cliff_GetDetectiontProcess_Result(void)
{
  return g_cliff.result;
}

CliffTrig_t Cliff_GetTrigStatus(void)
{
	CliffTrig_t cliff_result;
	cliff_result = Cliff_GetDetectiontProcess_Result();
	
	if(cliff_result)
	{
		osDelay(200);
		cliff_result = Cliff_GetDetectiontProcess_Result();
		if(cliff_result)
		{
			if(cliff_result&CLIFF_TRIG_LEFT)cliff_result = CLIFF_TRIG_LEFT;
			if(cliff_result&CLIFF_TRIG_FRONT)cliff_result = CLIFF_TRIG_FRONT;
			if(cliff_result&CLIFF_TRIG_RIGHT)cliff_result = CLIFF_TRIG_RIGHT;
			return 	cliff_result;	
		}
	}
	
	return CLIFF_TRIG_NO;
}

uint8_t Cliff_ContinueTrig(void)
{
	uint8_t reval = 0 ;
	if(Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_NO)
	{
		g_cliff.all_trig_cnt = 0;
	}
	if(Timer_IsTimeReach(&g_cliff.check_time,TIME_100MS))
	{
		if(Cliff_GetDetectiontProcess_Result() != CLIFF_TRIG_NO)
		{
			g_cliff.all_trig_cnt++;							
		}
		if(g_cliff.all_trig_cnt >= 1)//20
		{
			reval = 1;
		}
	}
	return reval;
}







