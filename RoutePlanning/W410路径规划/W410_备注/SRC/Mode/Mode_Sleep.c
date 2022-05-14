#include "Include.h"

void Mode_Sleep(void)
{
	uint16_t rcon_cnt = 0;
	uint32_t temp_status = 0;
	Mode_t temp_mode = MODE_NONE;
    Usprintf("------- Mode Sleep Start---------\n");
    Robot_MotorsSetState(DISABLE);
    Robot_SensorsSetState(DISABLE);	
	if(g_robot.type == ROBOT_W400)
	{
		//ObsMsg_SetMode(OBSMSG_SLEEP);
		ObsMsg_SetMode(OBSMSG_DISABLE);
	}	
	Display_All_Off();
	Button_WaitCleanUp();
	Wifi_ResetConnected();	
	Wifi_Stop();
	osDelay(100);
	
	Wifi_ResetWorking();
	Sleep_ResetInterrupt();
	Rcon_ResetStatus();
	g_sleep.state = ENABLE;
	g_sleep.is_charging = Charge_IsAdapterInput();
	
    while(Mode_GetMode() == MODE_SLEEP)
    {
		#ifdef TUYA
		if(Button_GetCleanStatus())
		{
			if(Charge_IsAdapterInput())
			{
				Usprintf("button wake up charge\n");
				temp_mode = MODE_CHARGE;	
			}
			else
			{
				if(Button_GetCleanStatus())
				{
					Usprintf("button wake up userinterface\n");
					temp_mode = MODE_USERINTERFACE;		
				}											
			}
		}
		if(g_sleep.is_charging != Charge_IsAdapterInput())
		{
			if(Charge_IsAdapterInput())
			{
				temp_mode = MODE_CHARGE;
				Usprintf("charge wake up charge\n");		
			}
			else
			{
				temp_mode = MODE_USERINTERFACE;	
				Usprintf("charge wake up userinterface\n");	
			} 			
		}	
		temp_status = Rcon_GetStatus();
		if(temp_status)
		{
			if((temp_status&RCON_FL_HOME_L)||(temp_status&RCON_FR_HOME_R))
			{
				rcon_cnt++;
				if(rcon_cnt >= (20*5))
				{
					Usprintf("rcon charger wake up userinterface\n");
					temp_mode = MODE_USERINTERFACE;
				}
			}
			Rcon_ResetStatus();
		}
		//remote
        if(Remote_IsRemoteEvent(REMOTE_SIGNAL_AREA)||Remote_IsRemoteEvent(REMOTE_SIGNAL_SPOT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_WALL)||Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
        {
            Usprintf("remote Useless\n");
        }
        if(Remote_IsRemoteEvent(REMOTE_SIGNAL_BACK)||Remote_IsRemoteEvent(REMOTE_SIGNAL_FORWARD)||Remote_IsRemoteEvent(REMOTE_SIGNAL_LEFT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_RIGHT))
        {
            Usprintf("remote Useless\n");
        }		
		
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV))
		{	
			if(Charge_IsAdapterInput())
			{
				temp_mode = MODE_CHARGE;
				Usprintf("remote nav wake up charge\n");			
			}		
			else
			{
				temp_mode = MODE_USERINTERFACE;
				Usprintf("remote nav wake up userinterface\n");				
			}						
		}        
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_NAV))
		{	
			if(Charge_IsAdapterInput())
			{
				if(Station_GetMoveState())
				{
					Station_SetStartState(TRUE);
					temp_mode = MODE_NAVIGATION;
					Usprintf("remote wifi nav wake up navigation\n");
				}
				else
				{
					temp_mode = MODE_CHARGE;
					Usprintf("remote wifi nav wake up charge\n");				
				}	
			}		
			else
			{
				Speaker_Voice(SPK_AUTO_CLEAN_START);
				temp_mode = MODE_NAVIGATION;
				Usprintf("remote wifi nav wake up userinterface\n");					
			}						
		}				

		if(temp_mode != MODE_NONE)
		{						
			Mode_SetMode(temp_mode);
            if(temp_mode == MODE_CHARGE)
            {
                Charge_SetFromSleep(1);
            }
			break;				
		}				
		#endif
		#ifdef ALICLOUD

        Sleep_InitConfiguration();

        Sleep_EnterStopMode();

        Sleep_ExitConfiguration();

        Usprintf("Sleep Wake Up\n");

        delay(500);

		

		//obs 
		if(Sleep_IsInterrupt(SLEEP_INIERRUPT_OBS))
		{
			Sleep_ResetTheInterrupt(SLEEP_INIERRUPT_OBS);
			Usprintf("obs wake up\n");
		}

		//wifi
		Usprintf(" wifi power state : %d \r\n",g_wifi.power_state);
		if(Sleep_IsInterrupt(SLEEP_INIERRUPT_WIFI))
		{
			Usprintf("wifi wake up \n");
			Sleep_ResetTheInterrupt(SLEEP_INIERRUPT_WIFI);
			if(g_wifi.power_state)
			{
				if(Charge_IsAdapterInput())
				{
					temp_mode = MODE_CHARGE;
					Usprintf("wifi wake up charge\n");			
				}		
				else
				{
					temp_mode = MODE_USERINTERFACE;
					Usprintf("wifi wake up userinterface\n");				
				}
			}
		}
		//charge
		if(Sleep_IsInterrupt(SLEEP_INIERRUPT_CHARGE))
		{
			Usprintf("charge wake up \n");
			Sleep_ResetTheInterrupt(SLEEP_INIERRUPT_CHARGE);
			if(g_sleep.is_charging != Charge_IsAdapterInput())
			{
				if(Charge_IsAdapterInput())
				{
					temp_mode = MODE_CHARGE;
					Usprintf("charge wake up charge\n");		
				}
				else
				{
					temp_mode = MODE_USERINTERFACE;	
					Usprintf("charge wake up userinterface\n");	
				} 
			}           
        }        

		//button
		if(Sleep_IsInterrupt(SLEEP_INIERRUPT_BUTTON))
		{
			Usprintf("button wake up \n");
			Sleep_ResetTheInterrupt(SLEEP_INIERRUPT_BUTTON);
			if(Button_GetCleanStatus())
			{
				if(Charge_IsAdapterInput())
				{
					Usprintf("button wake up charge\n");
					temp_mode = MODE_CHARGE;	
				}
				else
				{
					if(Button_GetCleanStatus())
					{
						Usprintf("button wake up userinterface\n");
						Speaker_Voice(SPK_DY);	
						temp_mode = MODE_USERINTERFACE;		
					}											
				}
			}		
		}
		if(Sleep_IsInterrupt(SLEEP_INIERRUPT_TIME))
		{
			Sleep_ResetTheInterrupt(SLEEP_INIERRUPT_TIME);
			Usprintf("\r\n TIM4 wake up Battery GetAdc : %d \r\n",Hw_Battery_GetAdc());
			if((Battery_GetReChargeVoltage()< BATTERY_ReCharge_VOLTAGE) && Charge_IsAdapterInput())
			{
				Usprintf("\r\n TIM4 wake up charge \r\n");
				temp_mode = MODE_CHARGE;
				Charge_SetFromSleep(1);
			}
		}
		//rcon 
		temp_status = Rcon_GetStatus();
		if(temp_status)
		{
			if((temp_status&RCON_FL_HOME_L)||(temp_status&RCON_FR_HOME_R))
			{
				rcon_cnt++;
				if(rcon_cnt >= (20*5))
				{
					Usprintf("rcon charger wake up userinterface\n");
					temp_mode = MODE_USERINTERFACE;
				}
			}
			Rcon_ResetStatus();
		}

		//remote
        if(Remote_IsRemoteEvent(REMOTE_SIGNAL_AREA)||Remote_IsRemoteEvent(REMOTE_SIGNAL_SPOT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_WALL)||Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
        {
            Usprintf("remote Useless\n");
        }
        if(Remote_IsRemoteEvent(REMOTE_SIGNAL_BACK)||Remote_IsRemoteEvent(REMOTE_SIGNAL_FORWARD)||Remote_IsRemoteEvent(REMOTE_SIGNAL_LEFT)||Remote_IsRemoteEvent(REMOTE_SIGNAL_RIGHT))
        {
            Usprintf("remote Useless\n");
        }		
		
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV))
		{	
			if(Charge_IsAdapterInput())
			{
				temp_mode = MODE_CHARGE;
				Usprintf("remote nav wake up charge\n");			
			}		
			else
			{
				Speaker_Voice(SPK_DY);
				temp_mode = MODE_USERINTERFACE;
				Usprintf("remote nav wake up userinterface\n");				
			}						
		}        
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_NAV))
		{	
			if(Charge_IsAdapterInput())
			{
				if(Station_GetMoveState())
				{
					Station_SetStartState(TRUE);
					temp_mode = MODE_NAVIGATION;
					Usprintf("remote wifi nav wake up navigation\n");
				}
				else
				{
					temp_mode = MODE_CHARGE;
					Usprintf("remote wifi nav wake up charge\n");				
				}	
			}		
			else
			{
				Speaker_Voice(SPK_DY);
				temp_mode = MODE_NAVIGATION;
				Usprintf("remote wifi nav wake up userinterface\n");					
			}						
		}				
		
		Usprintf("\r\n charge voltage: %d \r\n",g_charge.voltage);
		if(temp_mode != MODE_NONE)
		{
			
			Sleep_WakeUpConfiguration();							
			Mode_SetMode(temp_mode);
            if(temp_mode == MODE_CHARGE && g_sleep.is_charging == 1)
            {
               Charge_SetFromSleep(1);
            }
			break;				
		}
		#endif
    }
	g_sleep.state = DISABLE;
    Wifi_WakeUp();
	Wifi_GetState();
    Usprintf("------- Mode Sleep End---------\n");
}













