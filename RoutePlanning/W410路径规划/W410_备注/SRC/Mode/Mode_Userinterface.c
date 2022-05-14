#include "Include.h"

void Mode_UserInterface(void)
{
	uint8_t error_led_set = 1;
	uint8_t is_mode_exit = 0;
	Mode_t exit_mode = MODE_USERINTERFACE; 
	uint8_t button_value = 0;   
	uint8_t detect_charge_cnt = 0;
	uint8_t error_report_cnt = 0;
	uint32_t send_ir_time = Timer_GetTime();
	uint32_t start_time = Timer_GetTime();
	uint32_t error_start_time = Timer_GetTime();
	uint32_t check_charge_time = Timer_GetTime();

  uint32_t clean_button_time = 0;
	uint32_t wifi_config_start_time = 0;    
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	Usprintf("------- Mode Userinterface ---------\n");

	Robot_MotorsSetState(DISABLE);

	Robot_SensorsSetState(DISABLE);

	Cliff_ForwardStopEnable();

	Station_SetStartState(FALSE);

	Event_BatteryProcess();

	Display_SetCleanState(LED_MODE_BREATH,LED_STATE_GREEN);

	Button_WaitCleanUp();

	if(g_robot.type == ROBOT_W410)
	{
		Display_SetAreaState(LED_MODE_STATIC,LED_STATE_OFF);
		g_obs.state = DISABLE;
        g_cliff.state = DISABLE;	
        g_watertank.state = DISABLE;
	}
	else
	{
		Display_SetModeLed(g_clean_mode.selection);
		Display_SetModeKeyState(LED_STATE_GREEN);
		ObsMsg_SetMode(OBSMSG_DISABLE);			
	}
	
	g_robot.update_position = FALSE;
	
	while(Mode_GetMode() == MODE_USERINTERFACE)
	{
	
		if(!WaterTank_IsInstall())
		{
            if(Error_GetCode() == ERROR_CLEAN_TANK)
            {
                Error_ResetCode();
				Usprintf("reset the error clean water\n");
            }
		}
		button_value = Button_KeysEvent();	
		switch(button_value)
		{
			case BUTTON_VALUE_ONE:
							if(g_robot.type == ROBOT_W410)
							{
                                exit_mode = MODE_AREA;
                                is_mode_exit = 1;
								Usprintf("MODE_AREA\n");
							}
							else
							{
								Speaker_Voice(SPK_DY);
								Mode_SelectionCycle();
								Display_SetModeLed(g_clean_mode.selection);	
								Usprintf("g_clean_mode.selection:%d\n",g_clean_mode.selection);							
							}								
			break;	
			case BUTTON_VALUE_TWO:
							Robot_ReportError(Error_GetCode());
							error_start_time = start_time = Timer_GetTime();	
							Usprintf("BUTTON_VALUE_TWO:%d\n",Error_GetCode());								
			break;
			case BUTTON_VALUE_WIFI:
							Display_SetWifiState(LED_MODE_BLINK,LED_STATE_GREEN);
							Speaker_Voice(SPK_WIFI_CONNECTING);
							start_time = Timer_GetTime();
							if(Wifi_Config_Start())
							{
								wifi_config_start_time = Timer_GetTime();
							}	
							Flash_WriteOneByte(FLASH_START_ADDRESS,0Xff);
							Usprintf("BUTTON_VALUE_WIFI\n");																
			break;
			default:break;
		}

		if(wifi_config_start_time)	
		{
			if(Timer_Get_DiffNow(wifi_config_start_time) > TIME_3S)	
			{
				wifi_config_start_time = 0;
				Usprintf("Wifi_Config\n");
				Wifi_Config();				
			}
		}

		clean_button_time = Button_CleanEventTime();

		if(clean_button_time >= TIME_3S)
		{
			Usprintf("shut down\n");
			Speaker_Voice(SPK_DON);	
			Error_SetCode(ERROR_NONE);
			Wifi_SaveState();
			vTaskDelay(500/portTICK_RATE_MS);
			Power_KillAllVcc();
		}

		if(clean_button_time >= TIME_100MS)	
		{
			if(g_robot.type == ROBOT_W410)
			{
				if(exit_mode != MODE_NAVIGATION)
				{
					exit_mode = MODE_NAVIGATION;
					Usprintf("button MODE_NAVIGATION\n");
				}
			}
			else
			{
				exit_mode = (Mode_t)g_clean_mode.selection;
				Usprintf("button g_clean_mode.selection:%d\n",g_clean_mode.selection);
			}		
		}	

		if(clean_button_time == 0)
		{
			if(exit_mode != MODE_USERINTERFACE)	
			{
				is_mode_exit = 1;	
				Usprintf("button exit\n");
			}
		}
		
		if(Error_GetCode())
		{
			CodeShow_SetData(0,Error_GetCode(),0);
			CodeShow_SetState(ENABLE);
			if(g_robot.type == ROBOT_W400)
			{
				if(g_obsmsg.cur_mode != OBSMSG_ENABLE)
				{
					ObsMsg_SetMode(OBSMSG_ENABLE);
				}
			}			
		}
		else
		{
			CodeShow_SetData(0,0,0);
			CodeShow_SetState(DISABLE);
			if(g_robot.type == ROBOT_W400)
			{
				if(g_obsmsg.cur_mode != OBSMSG_DISABLE)
				{
					ObsMsg_SetMode(OBSMSG_DISABLE);
				}
			}			
		}
		
		if(Timer_IsTimeReach(&check_charge_time, TIME_800MS))
		{
			if((Rcon_GetStatus() & RCON_FL_HOME_L )||(Rcon_GetStatus() &RCON_FR_HOME_R))
			{
				if(!Charge_IsMovStation())
				{
					Charge_SetMovStation(TRUE);
					Usprintf("detect charge signal,is mov station\n");
				}			
			}
			if(Rcon_GetStatus() == (RCON_FL_HOME_L|RCON_FR_HOME_R))
			{
				detect_charge_cnt++;
				Usprintf("detect_charge_cnt :%d\n",detect_charge_cnt);
				if(Timer_Get_DiffNow(start_time) >= TIME_1MIN)
				{
					if(detect_charge_cnt >= 10)
					{
						if(WaterTank_IsInstall())
						{
							Station_SetStartState(TRUE);
							exit_mode = MODE_GO_HOME;	
							is_mode_exit = 1;	
							Usprintf("MODE_GO_HOME\n");				
						}
						else
						{
							detect_charge_cnt = 0;							
						}
					}	
				}				
			}
			else
			{
				detect_charge_cnt = 0;
			}
			Rcon_ResetStatus();
		}

		if(Timer_IsTimeReach(&error_start_time, TIME_10S)||(error_report_cnt == 0))
		{
			if(Error_GetCode())
			{
				error_report_cnt++;
				if(error_report_cnt < 4)
				{
					Robot_ReportError(Error_GetCode());	
					if(Error_GetCode() == ERROR_PICK_UP || Error_GetCode() == ERROR_LOW_BATTERY)
					{
						error_report_cnt = 4;
					}
					Usprintf("Robot_ReportError:%d\n",Error_GetCode());
				}				
			}			
		}

		if(Timer_Get_DiffNow(start_time) >= TIME_3MIN)
		{
			if(!Wifi_IsSendingHistory())
			{
				Mode_SetMode(MODE_SLEEP);
				Usprintf("MODE_SLEEP\n");	
			}						
		}

		if(Wifi_IsParameterUpdate())
		{
			start_time = Timer_GetTime();
			Usprintf("refrash time\n");
		}
		if(error_led_set)
		{
			error_led_set = 0;	
			exit_mode = MODE_USERINTERFACE; 
			if(Error_GetCode() != ERROR_NONE)
			{
				Usprintf(" set error led\n");
				Display_SetWarnKeyState(LED_STATE_RED);	
				if(g_robot.type == ROBOT_W400)
				{
					Display_SetLedBrightness(LED_BRIGHTNESS_HIGH_VALUE);
					Display_SetCleanState(LED_MODE_STATIC,LED_STATE_RED);
				}
				if(Error_GetCode() == ERROR_TANK || Error_GetCode() == ERROR_DIRT_TANK || Error_GetCode() == ERROR_CLEAN_TANK)
				{
					if(g_robot.type == ROBOT_W400)
					{
						Display_SetWaterWarnState(LED_STATE_RED);	
					}
					else
					{
						Display_SetWaterWarnState(LED_STATE_GREEN);
					}						
				}
			}
		}
		
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV)||Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_NAV))
		{
			exit_mode = MODE_NAVIGATION;
			is_mode_exit = 1;	
			Usprintf("remote MODE_NAVIGATION\n");
		}
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_AREA))
		{
			exit_mode = MODE_AREA;	
			is_mode_exit = 1;
			Usprintf("remote MODE_AREA\n");
		}
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_SPOT))
		{
			exit_mode = MODE_SPOT;
			is_mode_exit = 1;	
			Usprintf("remote MODE_SPOT\n");
		}
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
		{
			//exit_mode = MODE_GO_HOME;
			//is_mode_exit = 1;
			Usprintf("remote MODE_GO_HOME\n");
		}
		
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WALL))
		{
			exit_mode = MODE_WALL;
			is_mode_exit = 1;
			Usprintf("remote MODE_WALL\n");
		}		
		if(Remote_IsRemoteExist(REMOTE_SIGNAL_FORWARD)||Remote_IsRemoteExist(REMOTE_SIGNAL_LEFT)||Remote_IsRemoteExist(REMOTE_SIGNAL_RIGHT)||Remote_IsRemoteExist(REMOTE_SIGNAL_BACK))
		{
			exit_mode = MODE_REMOTE;	
			is_mode_exit = 1;
			Usprintf("remote MODE_REMOTE\n");
		}
		if(Remote_IsRemoteExist(REMOTE_SIGNAL_WIFI_FORWARD)||Remote_IsRemoteExist(REMOTE_SIGNAL_WIFI_BACK)||Remote_IsRemoteExist(REMOTE_SIGNAL_WIFI_LEFT)||Remote_IsRemoteExist(REMOTE_SIGNAL_WIFI_RIGHT))
		{
			exit_mode = MODE_REMOTE;	
			is_mode_exit = 1;	
			Usprintf("remote MODE_REMOTE\n");
		}

        if(Charge_IsAdapterInput())
        {
			Mode_SetMode(MODE_CHARGE);
			Usprintf("MODE_CHARGE\n");
        }

        if(is_mode_exit)
        {
            is_mode_exit = 0;
			CodeShow_SetState(DISABLE);
			if(g_robot.type == ROBOT_W400)
			{
				ObsMsg_SetMode(OBSMSG_ENABLE);
			}
			else
			{
				g_cliff.state = ENABLE;
				osDelay(50);
			}
			
			if(ObsMsg_IsIdle()&&(g_robot.type == ROBOT_W400))
			{
				Usprintf("Obs enable error\n");
				Error_SetCode(ERROR_OBS);
				error_led_set = 1;
			}
			else if(Cliff_IsPickUp()) 
			{
				Usprintf("Cliff All Trig\n");
				Error_SetCode(ERROR_CLIFF);
				error_led_set = 1;				
			}
			else if(!WaterTank_IsInstall())
			{
				Usprintf("No Water Tank\n");
				Error_SetCode(ERROR_TANK);
				error_led_set = 1;
			}
			else if(Battery_IsWeak()&& exit_mode != MODE_REMOTE && exit_mode != MODE_GO_HOME)
			{
				Usprintf("Low Battery! %d\n",Battery_GetVoltage());
				Error_SetCode(ERROR_LOW_BATTERY);
				error_led_set = 1;
			}	
			else
			{	
				if(exit_mode==MODE_AREA || exit_mode==MODE_NAVIGATION || exit_mode==MODE_SPOT || exit_mode==MODE_WALL)
				{
					if(Error_GetCode() == ERROR_CLEAN_TANK)
					{
						Speaker_Voice(SPK_CHECK_CLEAN_TANK);
						error_led_set = 1;
					}	
					if(Error_GetCode() == ERROR_LOW_BATTERY)
					{
						Speaker_Voice(SPK_LOW_POWER);
						error_led_set = 1;
					}																																												
				}	
				if(error_led_set)
				{
					if(exit_mode == MODE_REMOTE)
					{
						Remote_Reset();
					}
					exit_mode = MODE_USERINTERFACE;	
				}
							
				switch (exit_mode)
				{
					case 	MODE_AREA:
										Speaker_Voice(SPK_AREA_CLEAN_START);											
						break;
					case 	MODE_NAVIGATION:
										Speaker_Voice(SPK_AUTO_CLEAN_START);											
						break;
					case   	MODE_GO_HOME:
										Speaker_Voice(SPK_START_GO_HOME);
						break;	
					case 	MODE_SPOT:
										Speaker_Voice(SPK_SPOT_MODE);											
						break;
					case 	MODE_REMOTE:
										
						break;
					case 	MODE_WALL:
										Speaker_Voice(SPK_AUTO_CLEAN_START);
						break;

					default:
						break;
				}													
				Display_SetWarnKeyState(LED_STATE_OFF);	
				Display_SetWaterWarnState(LED_STATE_OFF);	
				if(g_robot.type == ROBOT_W400)Display_SetModeKeyState(LED_STATE_OFF);
				Button_WaitCleanUp();
				if(exit_mode != MODE_USERINTERFACE)
				{					
					Robot_ModeDisplay();
					Display_WifiBlinkToStatic();
					Mode_SetMode(exit_mode);
					Usprintf("------- Mode Userinterface Out---------\n");
				}		
				if(error_led_set)
				{				
					error_report_cnt = 0;
					error_start_time = start_time = Timer_GetTime();
					exit_mode = MODE_USERINTERFACE; 
					if(g_robot.type == ROBOT_W400)
					{
						Usprintf("set the error led,mode selection:%d\n",g_clean_mode.selection);
						Display_SetModeKeyState(LED_STATE_GREEN);
						Robot_ModeDisplay();
						ObsMsg_SetMode(OBSMSG_ENABLE);
					}				
				}																												
			}								        
        }

        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }

    Button_WaitCleanUp();
}

















