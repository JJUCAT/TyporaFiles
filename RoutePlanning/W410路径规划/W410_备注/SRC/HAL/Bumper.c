 #include "Include.h"
Bumper_t g_bumper;

void Bumper_Init(void)
{
	memset(&g_bumper,0,sizeof(g_bumper));	
	g_bumper.on_left_trig = Hw_Bumper_LeftTrig;
	g_bumper.on_right_trig = Hw_Bumper_RightTrig;
}

BumperTrig_t Bumper_GetTrigStatus(void)
{
	BumperTrig_t status = BUMPER_TRIG_NO;
	if(g_bumper.on_left_trig())
	{
		status |= BUMPER_TRIG_LEFT;	
	}
	if(g_bumper.on_right_trig())
	{
		status |= BUMPER_TRIG_RIGHT;	
	}
	return status;
}

uint8_t Bumper_IsStuck(void)
{
	static uint32_t time_cycle_bumper = 0;
	if(Timer_IsTimeReach(&time_cycle_bumper,TIME_200MS))
	{
		if(Bumper_GetTrigStatus())
		{
			g_bumper.trig_cnt++;
		}
		else
		{
			g_bumper.trig_cnt = 0;
		}
		if(g_bumper.trig_cnt > 30)
		{
			g_bumper.trig_cnt = 0;
			return 1;
		}	
		else 
		{		
			return 0;
		}
	}	
	return 0;		
}
















