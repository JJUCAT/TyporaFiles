 #include "Include.h"

void Event_AdjustProcess(void)
{
	if(Mode_GetMode() == MODE_SLEEP || Mode_GetMode() == MODE_TEST || Mode_GetMode() == MODE_NONE)return;

	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WATER))
	{
		if(g_pump.cycle_cnt > PUMP_WATER_FLOW_LEVEL_NORMAL)
		{
			Speaker_Voice(SPK_MEDIUM_WATERFLOW);
			g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;			
		}
		else if(g_pump.cycle_cnt > PUMP_WATER_FLOW_LEVEL_HIGH)
		{			
			g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_HIGH;
			Speaker_Voice(SPK_MAX_WATERFLOW);
		}
		else
		{
			Speaker_Voice(SPK_SMALL_WATERFLOW);
			g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_lOW;				
		}
	}

	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_MAINBRUSH))
	{
		if(g_mainbrush.adjust_voltage < MAINBRUSH_VOLTAGE)
		{
			g_mainbrush.voltage = MAINBRUSH_VOLTAGE;
			g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;
			Speaker_Voice(SPK_REGULAR_CLEANING);
		}
		else if(g_mainbrush.adjust_voltage < MAINBRUSH_SPOT_VOLTAGE)
		{
			g_mainbrush.voltage = MAINBRUSH_SPOT_VOLTAGE;
			g_mainbrush.adjust_voltage = MAINBRUSH_SPOT_VOLTAGE;
			Speaker_Voice(SPK_DEEP_CLEANING);		
		}
		else
		{
			g_mainbrush.voltage = MAINBRUSH_VOLTAGE;
			g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;
			Speaker_Voice(SPK_REGULAR_CLEANING);			
		}
	}
	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_CLEAN_CNT))
	{
		if(Mode_GetMode() == MODE_USERINTERFACE || Mode_GetMode() == MODE_CHARGE )
		{
			if(g_cleancycle_count.set_cnt == 1)
			{
				g_cleancycle_count.set_cnt = 2;
				Speaker_Voice(SPK_TWO_TIME_CLEANING);	
			}
			else
			{
				g_cleancycle_count.set_cnt = 1;
				Speaker_Voice(SPK_ONE_TIME_CLEANING);
			}
		}		
	}
	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_SPEAKER))
	{
		if(g_speaker.enable)
		{
			Speaker_Voice(SPK_VOICE_DISABLE);
			g_speaker.enable = 0;		
		}
		else
		{
			g_speaker.enable = 1;
			Speaker_Voice(SPK_VIOCE_ENABLE);			
		}	
	} 
}

void Event_DisplayProcess(void)
{
	if(Mode_GetMode() == MODE_SLEEP || Mode_GetMode() == MODE_TEST || Mode_GetMode() == MODE_NONE)return;

	uint8_t bat_state_change = 0;
	//water flow
	if((g_pump.cycle_cnt != g_pump.pre_cycle_cnt))
	{
		g_pump.pre_cycle_cnt = g_pump.cycle_cnt; 
		if(g_pump.cycle_cnt == PUMP_WATER_FLOW_LEVEL_HIGH)	
		{			
			Display_SetWaterFlowState(LED_STATE_ORANGE);
		}
		else if(g_pump.cycle_cnt == PUMP_WATER_FLOW_LEVEL_lOW)
		{
			Display_SetWaterFlowState(LED_STATE_GREEN);
		}
		else
		{
			Display_SetWaterFlowState(LED_STATE_OFF);
		}		
	}
	//clean cnt
	if((g_cleancycle_count.set_cnt != g_cleancycle_count.pre_set_cnt))
	{
		g_cleancycle_count.pre_set_cnt = g_cleancycle_count.set_cnt; 
		if(g_cleancycle_count.set_cnt == 1)	
		{
			Display_SetCleanCntState(LED_STATE_OFF);
		}
		else
		{
			Display_SetCleanCntState(LED_STATE_GREEN);
		}		
	}
	//battery
	if(Mode_GetMode() == MODE_CHARGE)
	{
		if(g_robot.type == ROBOT_W410)
		{
			Display_SetWaterFlowState(LED_STATE_OFF);
			Display_SetCleanCntState(LED_STATE_OFF);
			return;
		}
	}
	
	g_battery.level = g_battery.capacity.level;

	if(Math_Diff_int(g_battery.level,g_battery.pre_level) > BATTERY_LEVEL_DIF) 
	{
		if(g_battery.pre_level != g_battery.level)
		{
			g_battery.pre_level = g_battery.level; 
			bat_state_change = 1;
		}	
	}
	
	if(bat_state_change||(Error_GetCode() == ERROR_LOW_BATTERY))
	{
		if(g_battery.capacity.level > BATTERY_LEVEL_GREEN)
		{
			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_GREEN);	
		}
		else if(g_battery.capacity.level > BATTERY_LEVEL_ORANGE)
		{
			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_ORANGE);	
		}
		else if(g_battery.capacity.level < BATTERY_LEVEL_RED)	
		{
			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
		}	
		else
		{
			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_ORANGE);
		}
		if(Error_GetCode() == ERROR_LOW_BATTERY)
		{
			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
		}	
	}
}

void Event_PumperProcess(void)
{
	if(!g_pump.start_control)return;
	if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
	{
		if(Wheel_GetLeftSpeed()&&Wheel_GetRightSpeed())
		{
			if((Map_IsUncleanCellsOnFront()&&(g_gyro.is_ready))||Mode_GetMode() == MODE_REMOTE)
			{				
				Pump_SetState(ENABLE);
			}
			else
			{
				Pump_SetState(DISABLE);	
			}							
		}
		else
		{
			Pump_SetState(DISABLE);		
		}		
	}
	else
	{
		Pump_SetState(DISABLE);		
	}
	
	if((Math_Diff_int(Wheel_GetLeftSpeed(),Wheel_GetRightSpeed()) > 10))
	{
		Pump_SetState(DISABLE);	
	}	
}

void Event_BatteryProcess(void)
{
	if(g_battery.capacity.level > BATTERY_LEVEL_GREEN)
	{
		Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_GREEN);	
	}
	else if(g_battery.capacity.level > BATTERY_LEVEL_ORANGE)
	{
		Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_ORANGE);	
	}
	else if(g_battery.capacity.level < BATTERY_LEVEL_RED)	
	{
		Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
	}	
	else
	{
		Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_ORANGE);
	}
	if(Error_GetCode() == ERROR_LOW_BATTERY)
	{
		Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
	}	
}

void Event_Remote(void)
{
    if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV))
    {
        Mode_SetMode(MODE_USERINTERFACE);  
    }

    if(Remote_IsRemoteEvent(REMOTE_SIGNAL_SPOT))
    {
        Mode_SetMode(MODE_USERINTERFACE);  
    }

    if(Remote_IsRemoteEvent(REMOTE_SIGNAL_AREA))
    {
        Mode_SetMode(MODE_USERINTERFACE);  
    }

    if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WALL))
    {
        Mode_SetMode(MODE_USERINTERFACE);  
    }

    if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_STOP))
    {
        Mode_SetMode(MODE_USERINTERFACE);  
    }
}

void Event_Button(void)
{
    if(Button_CleanKeyEventTime())
    {
        Mode_SetMode(MODE_USERINTERFACE);
    } 
    if(Button_KeysEvent() == BUTTON_VALUE_ONE)
    {
		if(g_robot.type == ROBOT_W410)
		{
			Mode_SetMode(MODE_USERINTERFACE);
		}      
    }     
}

void Event_PickUp(void)
{
	if(Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)
	{
		Mode_SetMode(MODE_USERINTERFACE);	
		Error_SetCode(ERROR_PICK_UP);
		Usprintf("event pick up\n");
	}  
}

void Event_ChargeAdapterInput(void)
{
	if(Gyro_IsReady())
	{
		if(Charge_IsAdapterInput())
		{
			Mode_SetMode(MODE_CHARGE);	
			Usprintf("event charge adapter\n");
		}
	}
}

void Event_Handler(void)
{
    Event_Remote();
    Event_Button();
	Event_PickUp();
	Event_ChargeAdapterInput();
}



















