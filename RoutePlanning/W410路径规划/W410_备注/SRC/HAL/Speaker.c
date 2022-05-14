#include "Include.h"

Speaker_t g_speaker;

void Speaker_Init(void)
{
	memset(&g_speaker,0,sizeof(g_speaker));
	g_speaker.on_set_data_io_state = Hw_Speaker_SetDataIoState;
	g_speaker.on_get_busy_io_state = Hw_Speaker_GetBusyIoState;
	g_speaker.on_set_power_state = Hw_Power_Set_3V3_5V_State;
	g_speaker.enable = ENABLE;
}

void Speaker_Initialize(void)
{
	g_speaker.on_set_data_io_state(ON);
}

void Speaker_SetPowerState(uint8_t state)
{
	if(state)
	{
		g_speaker.on_set_power_state(ON);
		g_speaker.power = TRUE;			
	}
	else
	{
		g_speaker.on_set_power_state(OFF);
		g_speaker.power = FALSE;			
	}
}

uint8_t Speaker_GetPowerState(void)
{
	return g_speaker.power; 
}

uint32_t Speaker_GetPowerTime(void)
{
	return g_speaker.power_time;
}

void Speaker_RefreshPowerTime(void)
{
	g_speaker.power_time = Timer_GetTime();
}

void Speaker_Voice(uint8_t output_val)
{
#ifndef	 SPEAKER_ENABLE
	return;
#endif
	uint8_t is_old_speaker = 0;

	if(!g_speaker.power)
	{
		g_speaker.on_set_power_state(ON);
		g_speaker.power = TRUE;	
		osDelay(50);
	}

	g_speaker.power_time = Timer_GetTime();

	if(output_val != SPK_FIND_ROBOT)
	{
		if(!g_speaker.enable)return;
	}	
	Usprintf("output value = %d \n",output_val);	
	if(g_robot.type == ROBOT_W400)
	{
		//is_old_speaker = TRUE;
	}
#ifdef	SPEAKER_OLD	
	is_old_speaker = TRUE;
#endif
	if(is_old_speaker)
	{
		switch(output_val)	
		{
			case SPK_DY:output_val = SPK_OLD_DY;break;	
			case SPK_DON:output_val = SPK_OLD_DON;break;			
			case SPK_AUTO_CLEAN_START:output_val = SPK_OLD_CLEAN_START;break;
			case SPK_AREA_CLEAN_START:output_val = SPK_OLD_CLEAN_START;break;
			case SPK_CLEAN_FINISH:output_val = SPK_OLD_CLEAN_FINISH;break;	
			case SPK_LOW_POWER:output_val = SPK_OLD_LOW_POWER;break;		
			case SPK_CHARGING_START:output_val = SPK_OLD_CHARGING_START;break;			
			case SPK_NO_WATER_TANK:output_val = SPK_OLD_NO_WATER_TANK;break;			
			case SPK_CHECK_CLEAN_TANK:output_val = SPK_OLD_CHECK_CLEAN_TANK;break;		
			case SPK_CHECK_DIRTY_TANK:output_val = SPK_OLD_CHECK_DIRTY_TANK;break;		
			case SPK_CHECK_LEFT_WHEEL:output_val = SPK_OLD_CHECK_LEFT_WHEEL;break;		
			case SPK_CHECK_RIGHT_WHEEL:output_val = SPK_OLD_CHECK_RIGHT_WHEEL;break;			
			case SPK_CHECK_BRUSH:output_val = SPK_OLD_CHECK_BRUSH;break;	
			case SPK_CHECK_BUMPER:output_val = SPK_OLD_CHECK_BUMPER;break;			
			case SPK_CHECK_CLIFF:output_val = SPK_OLD_CHECK_CLIFF;break;			
			case SPK_CHECK_MOBILITY:output_val = SPK_OLD_CHECK_MOBILITY;break;			
			case SPK_PICK_UP:output_val = SPK_OLD_PICK_UP;break;			
			case SPK_STUCK:output_val = SPK_OLD_STUCK_E;break;				
			case SPK_WIFI_CONNECTING:output_val = SPK_OLD_WIFI_CONNECTING;break;				
			case SPK_WIFI_CONNECTED:output_val = SPK_OLD_WIFI_CONNECTED;break;		
			case SPK_VOICE_DISABLE:output_val = SPK_OLD_DY;break;			
			case SPK_VIOCE_ENABLE:output_val = SPK_OLD_DY;break;			
			case SPK_MEDIUM_WATERFLOW:output_val = SPK_OLD_DY;break;		
			case SPK_SMALL_WATERFLOW:output_val = SPK_OLD_DY;break;	
			case SPK_MAX_WATERFLOW:output_val = SPK_OLD_DY;break;			
			case SPK_REGULAR_CLEANING:output_val = SPK_OLD_DY;break;		
			case SPK_DEEP_CLEANING:output_val = SPK_OLD_DY;break;	
			case SPK_ONE_TIME_CLEANING:output_val = SPK_OLD_DY;break;			
			case SPK_TWO_TIME_CLEANING:output_val = SPK_OLD_DY;break;		
			case SPK_FIND_ROBOT:output_val = SPK_OLD_DY;break;		
			case SPK_PRESS_BOTTON:output_val = SPK_OLD_SELECT_MODE;break;						
			case SPK_SPOT_MODE:output_val = SPK_OLD_CLEAN_START;break;		
			case SPK_START_GO_HOME:output_val = SPK_OLD_DY;break;				
			case SPK_ENGLISH:output_val = SPK_OLD_DY;break;								
			default:break;
		}
	}


	g_speaker.v_on = 1;	
	g_speaker.sound = output_val;
	g_speaker.sound_temp = output_val;
	g_speaker.cycle_cnt = 0;
	g_speaker.idx = 0;      
	g_speaker.out_cnt = 1;
	g_speaker.check_flag = 1;
	g_speaker.on_set_data_io_state(OFF);	
}	

void Speaker_InterruptHandler(void)
{
	if(g_speaker.out_cnt)
	{
		if(g_speaker.idx == 0)
		{
			if(g_speaker.cycle_cnt > 49)
			{
				g_speaker.cycle_cnt = 0;
				g_speaker.idx = 1;
				g_speaker.on_set_data_io_state(ON);
			}
		}
		else
		{
			if(g_speaker.sound & 0x01)
			{
				if(g_speaker.cycle_cnt == 12)
				{
					g_speaker.on_set_data_io_state(OFF);
				}
			}
			else
			{
				if(g_speaker.cycle_cnt == 4)
				{
					g_speaker.on_set_data_io_state(OFF);
				}
			}

			if(g_speaker.cycle_cnt > 15)
			{
				g_speaker.cycle_cnt = 0;
				if(g_speaker.idx > 7)
				{
					g_speaker.out_cnt = 0;
					g_speaker.idx = 0;
					g_speaker.finish = 1;
				}
				g_speaker.sound >>= 1;
				g_speaker.on_set_data_io_state(ON);
				g_speaker.idx++;
			}
		}
		g_speaker.cycle_cnt++;
		return;
	}
	
	if(g_speaker.finish && g_speaker.check_flag)
	{
		if(!g_speaker.on_get_busy_io_state())
		{
		 	g_speaker.finish = 0;  
			g_speaker.check_cnt = 0;
			g_speaker.check_flag = 0;
		}
		else
		{
     		 g_speaker.check_cnt++;
			if(g_speaker.check_cnt > 199)
			{
				Usprintf("spk error!\n");
				g_speaker.check_cnt = 0;
				g_speaker.finish = 0;			
				Speaker_Voice(g_speaker.sound_temp);
				g_speaker.check_flag = 0;
			}
		}	
	}	
}
															
uint8_t Speaker_IsChecking(void)
{
	return g_speaker.check_flag;
}















