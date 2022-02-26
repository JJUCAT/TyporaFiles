#include "Include.h"


void Mode_Charge(void)
{
    uint32_t debug_time = Timer_GetTime();
    uint32_t charge_time = Timer_GetTime();
    uint32_t charge_current_increase_time = Timer_GetTime();
    uint32_t charge_cycle_time = Timer_GetTime(); 
    uint32_t charge_finish_time = Timer_GetTime();   
		uint32_t sent_mode_time = Timer_GetTime();   
		uint8_t is_sent_flag = 0;
    uint16_t minute_cnt = 0;
    uint8_t is_mode_exit = 0;
    Mode_t exit_mode = MODE_NONE;
    int16_t charging_current = 0;
    uint16_t temp_pwm = CHARGE_PWM_START;
    int16_t target_current = CHARGE_TARGET_CURRENT_LOWEST;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

    Usprintf("------- Mode Charge ---------\n");

    Robot_MotorsSetState(DISABLE);

    Robot_SensorsSetState(DISABLE);

    Charge_ConfigurationIn();

    Display_All_Off();

    Display_SetBatteryState(LED_MODE_BREATH,LED_STATE_ORANGE);
 
    if(!Charge_IsFromSleep())Speaker_Voice(SPK_CHARGING_START);

    Button_WaitCleanUp();

    Charge_SetFromSleep(0);

    Charge_SetCharging(1);

    Charge_SetMode(CHARGE_MODE_CC);

    if(Error_GetCode() != ERROR_NONE)
    {
        if(Error_GetCode() == ERROR_CLEAN_TANK || Error_GetCode() == ERROR_DIRT_TANK)
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
        else
        {
            Error_ResetCode();
        }       
    }

    while(Mode_GetMode() == MODE_CHARGE)
    {
			if(Speaker_GetPowerState())
			{
					if(Timer_Get_DiffNow(Speaker_GetPowerTime()) > TIME_5S)
					{           
							Speaker_SetPowerState(OFF);    
							Usprintf("Speaker power off\n");  
					}
			}

			if((Rcon_GetStatus()&RCON_FR_HOME_L)||(Rcon_GetStatus()&RCON_FR_HOME_R)||(Rcon_GetStatus()&RCON_FL_HOME_L)||(Rcon_GetStatus()&RCON_FL_HOME_R)||Charge_IsMovStation())
			{
				if(!Station_GetMoveState())
				{
					Usprintf("move station\n");
					Station_SetMoveState(1);
					Charge_SetMovStation(FALSE);
					sent_mode_time = Timer_GetTime();
					is_sent_flag = TRUE;
				} 	
			}
		
			if(is_sent_flag)
			{
				if(Timer_Get_DiffNow(sent_mode_time) > TIME_1S)
				{
					is_sent_flag= FALSE;
					Wifi_ResetWorkMode();
				}
			}
			
			
			if(!WaterTank_IsInstall())
			{				
				if(Error_GetCode() == ERROR_CLEAN_TANK || Error_GetCode() == ERROR_DIRT_TANK)
				{
									Usprintf("reset error code\n");
					Error_ResetCode();
									Display_SetWaterWarnState(LED_STATE_OFF);	  
				}
			}	

			if(Remote_IsRemoteEvent(REMOTE_SIGNAL_AREA)||Remote_IsRemoteEvent(REMOTE_SIGNAL_SPOT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_WALL)||Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
			{
					Usprintf("REMOTE Useless\n");
			}
			if(Remote_IsRemoteEvent(REMOTE_SIGNAL_BACK)||Remote_IsRemoteEvent(REMOTE_SIGNAL_FORWARD)||Remote_IsRemoteEvent(REMOTE_SIGNAL_LEFT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_RIGHT))
			{
					Usprintf("REMOTE Useless\n");
			}

			if(Button_CleanKeyEventTime()||Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV)||Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_NAV))
			{	
				if(Battery_IsChargeLow())
				{
					if(Station_GetMoveState())
					{                    
						Speaker_Voice(SPK_LOW_POWER);
						Usprintf("low power\n");
					}
					else
					{
						Usprintf("on normal station\n");
					}				
				}
				else
				{
					if(Station_GetMoveState())
					{
						if(Error_GetCode()!=ERROR_CLEAN_TANK)
						{
							if(WaterTank_IsInstall())
							{
								exit_mode = MODE_NAVIGATION;
								is_mode_exit = 1;                        
								Station_SetStartState(TRUE);
								//Station_SetMoveState(0);						
								Usprintf("station to nav\n");
							}
						}				
						else
						{           
							Robot_ReportError(Error_GetCode());	
						}															
					}
					else
					{
						Usprintf("on normal station\n");
					}				
				}				
			}

			if(!Charge_IsAdapterInput())
			{
					Usprintf("charge adapter out\n");
					//Station_SetMoveState(0);
					exit_mode = MODE_USERINTERFACE;
					is_mode_exit = 1;
			}

			charging_current = g_battery.capacity.current;
			if(Timer_IsTimeReach(&debug_time,TIME_1S))
			{
					Usprintf("charging i:%d v:%d pwm:%d \r\n",charging_current,Battery_GetVoltage(),temp_pwm);
			}
		
			if(Timer_IsTimeReach(&charge_current_increase_time,TIME_150MS))
			{			
				if(Battery_GetVoltage() > CHARGE_BATTERY_NORMAL_VOLTAGE)
				{
					target_current++;
					if(target_current > CHARGE_TARGET_CURRENT)target_current = CHARGE_TARGET_CURRENT;
				}
				else
				{
					target_current = CHARGE_TARGET_CURRENT_LOWEST;
				}
			}

			if(Timer_IsTimeReach(&charge_time,TIME_1MIN))
			{
					minute_cnt++;  
					if(minute_cnt >= 360)
					{   
							minute_cnt = 0;
							Charge_SetMode(CHARGE_MODE_FINISH);
					}
			}
			if(Charge_GetMode() == CHARGE_MODE_CC)
			{
					if(charging_current > target_current)
					{
							if(temp_pwm < 2)temp_pwm = 2;
							temp_pwm -= 2;
					}
					if(charging_current < target_current)
					{
							if(Battery_GetVoltage() <= CHARGE_BATTERY_FULL_VOLTAGE)
							{
									temp_pwm++;
							}
							if(temp_pwm > CHARGE_PWM_RANGE)temp_pwm = CHARGE_PWM_RANGE;
					} 
			}
			if(Timer_IsTimeReach(&charge_cycle_time,TIME_1S))
			{
					switch(Charge_GetMode())
					{
							case CHARGE_MODE_CC:
																	if(Battery_GetVoltage() >= CHARGE_BATTERY_FULL_VOLTAGE)
																	{                                        
																			Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_GREEN);
																			Display_SetCleanState(LED_MODE_STATIC,LED_STATE_OFF);
																			Charge_SetMode(CHARGE_MODE_CV);	
																			Battery_ResetCapacity();																													 	
																	}                                               
							break;
							case CHARGE_MODE_CV:
																	if(Battery_GetVoltage() > CHARGE_BATTERY_FULL_VOLTAGE)
																	{
																			if(temp_pwm > 0)temp_pwm--;
																	}
																	else
																	{
																			if(charging_current < CHARGE_TARGET_CURRENT)
																			{
																					if(temp_pwm < CHARGE_PWM_RANGE)temp_pwm++;
																			}
																	}
																	if(charging_current < CHARGE_RESET_CAPACITY_CURRENT)
																	{
																			Battery_ResetCapacity();
																	}

																	if(charging_current < CHARGE_FINISH_CURRENT)
																	{                                       
																			Charge_SetMode(CHARGE_MODE_FINISH);
																			charge_finish_time = Timer_GetTime();
																	}                
							break;
							case CHARGE_MODE_FINISH:
																	temp_pwm = 0;
																	if(Timer_Get_DiffNow(charge_finish_time) >= (TIME_50S * 2))
																	{
																		 exit_mode = MODE_SLEEP;
																		 is_mode_exit =1;
																	}                                   
							break;

							default:break;
					}             
			}  
			*g_charge.p_pwm = temp_pwm;

			if(is_mode_exit)
			{
					is_mode_exit = 0;
					Mode_SetMode(exit_mode);
					Charge_SetCharging(0);
					Charge_ConfigurationOut();
					*g_charge.p_pwm = 0;
					Usprintf("------- Mode Charge Out---------\n");
			}

			vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
    Button_WaitCleanUp();
    if(exit_mode == MODE_NAVIGATION)
    {
        osDelay(20);
        Speaker_Voice(SPK_AUTO_CLEAN_START);
    }    
}







