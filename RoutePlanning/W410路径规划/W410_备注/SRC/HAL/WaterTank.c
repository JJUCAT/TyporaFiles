
 #include "Include.h"

WaterTank_t g_watertank;

void WaterTank_Init(void)
{
	memset(&g_watertank,0,sizeof(g_watertank));
	g_watertank.on_get_tank_state = Hw_WaterTank_GetTankState;
	g_watertank.on_set_clean_driver_state = Hw_WaterTank_SetCleanDriverState;
	g_watertank.on_set_dirty_driver_state = Hw_WaterTank_SetDirtyDriverState;
}

void WaterTank_SetCheckState(FunctionalState state)
{
  g_watertank.check_state = state;
}

FunctionalState WaterTank_GetCheckState(void)
{
  return  g_watertank.check_state; 
}

uint8_t WaterTank_IsInstall(void)
{
	if(!(g_watertank.on_get_tank_state()))return 1;
	return 0;
}

void WaterTank_ResetErrorCnt(void)
{
	g_watertank.dirt_error_cnt = 0;
	g_watertank.clean_error_cnt = 0;
}

void WaterTank_CheckProcess(FunctionalState state)
{
	#ifndef   WATER_ENABLE
	return;
	#endif	
	if(g_watertank.check_state == DISABLE)return;

	if(g_watertank.adc.dirt < WATERTANK_DIRTY_L_LIMIT)
	{			
		g_watertank.dirt_error_cnt++;
		if(g_watertank.dirt_error_cnt > WATERTANK_ERROR_TIME_LIMIT)
		{
			g_watertank.dirt_error_cnt = WATERTANK_ERROR_TIME_LIMIT;
		}
	}
	else
	{
		if(g_watertank.adc.dirt > WATERTANK_DIRTY_H_LIMIT)
		{
			g_watertank.dirt_error_cnt = 0;
		}
	}
	if(state != ENABLE)return;		
	if(g_watertank.adc.clean > WATERTANK_CLEAN_H_LIMIT)
	{	
		g_watertank.clean_error_cnt++;
		if(g_watertank.clean_error_cnt >= WATERTANK_ERROR_TIME_LIMIT)
		{
			g_watertank.clean_error_cnt = WATERTANK_ERROR_TIME_LIMIT;										
		}
	}
	else
	{					
		if(g_watertank.adc.clean < WATERTANK_CLEAN_L_LIMIT)
		{
			g_watertank.clean_error_cnt = 0;
		}
	}		
}













