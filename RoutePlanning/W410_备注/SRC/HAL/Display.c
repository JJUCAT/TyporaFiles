#include "Include.h"

Display_t g_display;
								 
void Display_Init(void)
{
	memset(&g_display,0,sizeof(g_display));
	Hw_Display_Init();
	g_display.on_set_brightness = Hw_Display_SetBrightness; 
	g_display.on_clean_led_off = Hw_Display_CleanLedOff;
	g_display.on_clean_led_red = Hw_Display_CleanLedRed;
	g_display.on_clean_led_green = Hw_Display_CleanLedGreen;
	g_display.on_clean_led_orange = Hw_Display_CleanLedOrange;
	g_display.on_area_led_off = Hw_Display_AreaLedOff;
	g_display.on_area_led_green = Hw_Display_AreaLedGreen;
	g_display.on_wifi_led_off = Hw_Display_WifiLedOff;
	g_display.on_wifi_led_green = Hw_Display_WifiLedGreen;
	g_display.on_battery_led_off = Hw_Display_BatteryLedOff;
	g_display.on_battery_led_red = Hw_Display_BatteryLedRed;
	g_display.on_battery_led_green = Hw_Display_BatteryLedGreen;
	g_display.on_battery_led_orange = Hw_Display_BatteryLedOrange;	
	g_display.on_waterflow_led_off = Hw_Display_WaterFlowLedOff;
	g_display.on_waterflow_led_red = Hw_Display_WaterFlowLedRed;
	g_display.on_waterflow_led_green = Hw_Display_WaterFlowLedGreen;
	g_display.on_waterflow_led_orange = Hw_Display_WaterFlowLedOrange;
	g_display.on_wifi_led_off = Hw_Display_WifiLedOff;
	g_display.on_wifi_led_green = Hw_Display_WifiLedGreen;	
	g_display.on_warnkey_led_off = Hw_Display_WarnKeyStateLedOff;
	g_display.on_warnkey_led_red = Hw_Display_WarnKeyStateLedRed;	
	g_display.on_modekey_led_off = Hw_Display_ModeKeyStateLedOff;
	g_display.on_modekey_led_green = Hw_Display_ModeKeyStateLedGreen;	
	g_display.on_waterwarn_led_off = Hw_Display_WaterWarnStateLedOff;
	g_display.on_waterwarn_led_red = Hw_Display_WaterWarnStateLedRed;
	g_display.on_waterwarn_led_green = Hw_Display_WaterWarnStateLedGreen;		
	g_display.on_cleancnt_led_off = Hw_Display_CleanCntStateLedOff;
	g_display.on_cleancnt_led_green = Hw_Display_CleanCntStateLedGreen;	
	g_display.on_modenav_led_off = Hw_Display_ModeNavStateLedOff;
	g_display.on_modenav_led_green = Hw_Display_ModeNavStateLedGreen;	
	g_display.on_modearea_led_off = Hw_Display_ModeAreaStateLedOff;
	g_display.on_modearea_led_green = Hw_Display_ModeAreaStateLedGreen;	
	g_display.on_modespot_led_off = Hw_Display_ModeSpotStateLedOff;
	g_display.on_modespot_led_green = Hw_Display_ModeSpotStateLedGreen;	
	g_display.on_modewall_led_off = Hw_Display_ModeWallStateLedOff;
	g_display.on_modewall_led_green = Hw_Display_ModeWallStateLedGreen;	
	g_display.on_brightness_high = Hw_Display_BrightnessHigh;

	g_display.state = ENABLE;
}

void Display_All_Off(void)
{
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetAreaState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetWaterFlowState(LED_STATE_OFF);
	Display_SetWarnKeyState(LED_STATE_OFF);
	Display_SetWaterWarnState(LED_STATE_OFF);
	Display_SetCleanCntState(LED_STATE_OFF);
	Display_SetModeNavState(LED_STATE_OFF);
	Display_SetModeAreaState(LED_STATE_OFF);
	Display_SetModeSpotState(LED_STATE_OFF);
	Display_SetModeWallState(LED_STATE_OFF);
	Display_SetModeKeyState(LED_STATE_OFF);	
}

void Display_WifiBlinkToStatic(void)
{
	if(g_display.wifi_led_mode == LED_MODE_BLINK)
	{
		Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);	
	}
}

void Display_SetCleanState(DisplayMode_t mode,LedState_t state)
{
	g_display.clean_led_mode = mode;
	switch(state)
	{
		case LED_STATE_OFF:
							g_display.on_clean_led_off();
		break;
		case LED_STATE_RED:
							g_display.on_clean_led_red();
		break;
		case LED_STATE_GREEN:
							g_display.on_clean_led_green();	
		break;
		case LED_STATE_ORANGE:
							g_display.on_clean_led_orange();
		break;
		default:
				Usprintf("Error clean led state\n");
		break;						
	}
}

void Display_SetAreaState(DisplayMode_t mode,LedState_t state)
{
	g_display.area_led_mode = mode;
	switch(state)
	{
		case LED_STATE_OFF:
							g_display.on_area_led_off();
		break;
		case LED_STATE_RED:
							Usprintf("Error area led state --- red\n");
		break;
		case LED_STATE_GREEN:
							g_display.on_area_led_green();
		break;
		case LED_STATE_ORANGE:
							Usprintf("Error area led state ---- orange\n");
		break;
		default:
				Usprintf("Error area led state\n");
		break;						
	}
}

void Display_SetWifiState(DisplayMode_t mode,LedState_t state)
{
	g_display.wifi_led_mode = mode;
	g_display.wifi_led_state = state;
	switch(state)
	{
		case LED_STATE_OFF:
							g_display.on_wifi_led_off();
		break;
		case LED_STATE_RED:
							Usprintf("Error wifi led state --- red\n");
		break;
		case LED_STATE_GREEN:
							g_display.on_wifi_led_green();							
		break;
		case LED_STATE_ORANGE:
							Usprintf("Error wifi led state ---- orange\n");
		break;
		default:
				Usprintf("Error wifi led state\n");
		break;						
	}
}

void Display_SetBatteryState(DisplayMode_t mode,LedState_t state)
{
	g_display.battery_led_mode = mode;
	Usprintf("battery display state:%d\n",state);
	switch(state)
	{
		case LED_STATE_OFF:
							g_display.on_battery_led_off();
		break;
		case LED_STATE_RED:
							g_display.on_battery_led_red();							
		break;
		case LED_STATE_GREEN:
							g_display.on_battery_led_green();							
		break;
		case LED_STATE_ORANGE:
							g_display.on_battery_led_orange();							
		break;
		default:
				Usprintf("Error battery led state\n");
		break;						
	}	
}

void Display_SetWaterFlowState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_waterflow_led_off();						
		break;
		case LED_STATE_RED:
							g_display.on_waterflow_led_red();
		break;
		case LED_STATE_GREEN:
							g_display.on_waterflow_led_green();
		break;
		case LED_STATE_ORANGE:
							g_display.on_waterflow_led_orange();
		break;
		default:
		Usprintf("Error water led state\n");
		break;						
	}
}

void Display_SetWarnKeyState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_warnkey_led_off();							
		break;
		case LED_STATE_RED:
							g_display.on_warnkey_led_red();
		break;
		case LED_STATE_GREEN:
							Usprintf("Error warn key led state ---- green\n");
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error warn key led state ---- orange\n");
		break;
		default:
				Usprintf("Error warn key led state\n");
		break;						
	}
}

void Display_SetWaterWarnState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_waterwarn_led_off();						
		break;
		case LED_STATE_RED:
							g_display.on_waterwarn_led_red();												
		break;
		case LED_STATE_GREEN:
							g_display.on_waterwarn_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error water warn led state ---- orange\n");
		break;
		default:
				Usprintf("Error water warn led state\n");
		break;						
	}
}

void Display_SetCleanCntState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_cleancnt_led_off();						
		break;
		case LED_STATE_RED:
							Usprintf("Error clean cnt led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_cleancnt_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error clean cnt led state ---- orange\n");
		break;
		default:
				Usprintf("Error clean cnt led state\n");
		break;						
	}
}

void Display_SetModeNavState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_modenav_led_off();							
		break;
		case LED_STATE_RED:
							Usprintf("Error area led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_modenav_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error area led state ---- orange\n");
		break;
		default:
				Usprintf("Error area led state\n");
		break;						
	}
}

void Display_SetModeAreaState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_modearea_led_off();							
		break;
		case LED_STATE_RED:
							Usprintf("Error area led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_modearea_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error area led state ---- orange\n");
		break;
		default:
				Usprintf("Error area led state\n");
		break;						
	}
}

void Display_SetModeSpotState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_modespot_led_off();							
		break;
		case LED_STATE_RED:
							Usprintf("Error spot led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_modespot_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error spot led state ---- orange\n");
		break;
		default:
				Usprintf("Error spot led state\n");
		break;						
	}
}

void Display_SetModeWallState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_modewall_led_off();						
		break;
		case LED_STATE_RED:
							Usprintf("Error wall led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_modewall_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error wall led state ---- orange\n");
		break;
		default:
				Usprintf("Error wall led state\n");
		break;						
	}
}

void Display_SetModeKeyState(LedState_t state)
{
	switch(state)
	{
		case LED_STATE_OFF:	
							g_display.on_modekey_led_off();							
		break;
		case LED_STATE_RED:
							Usprintf("Error mode led state ---- green\n");							
		break;
		case LED_STATE_GREEN:
							g_display.on_modekey_led_green();
		break;				
		case LED_STATE_ORANGE:
							Usprintf("Error mode led state ---- orange\n");
		break;
		default:
				Usprintf("Error mode led state\n");
		break;						
	}	
}

void Display_SetModeLed(uint16_t selection)
{
	Display_SetModeNavState(LED_STATE_OFF);
	Display_SetModeAreaState(LED_STATE_OFF);
	Display_SetModeSpotState(LED_STATE_OFF);
	Display_SetModeWallState(LED_STATE_OFF);		
	switch(selection)
	{
		case MODE_NAVIGATION:
								Display_SetModeNavState(LED_STATE_GREEN);
		break;
		case MODE_SPOT:
								Display_SetModeSpotState(LED_STATE_GREEN);
		break;
		case MODE_WALL:
								Display_SetModeWallState(LED_STATE_GREEN);			
		break;
		case MODE_AREA:
								Display_SetModeAreaState(LED_STATE_GREEN);		
		break;
		default:break;				
	}
	g_display.on_brightness_high();	
}

void Display_Process(void)
{
	if(g_display.state == DISABLE)return;

	Display_Blink();

	Display_Breath();	

	Display_Static();	
}

void Display_Static(void)
{
	/*if(g_display.clean_led_mode == LED_MODE_STATIC)
	{
		g_display.on_brightness_high();
	}*/
}

void Display_Blink(void)
{
	static uint32_t blink_time;
	static uint8_t on = 0;
	if(Timer_IsTimeReach(&blink_time,TIME_500MS))
	{
		on = 1 - on;
		if(on)
		{
			if(g_display.clean_led_mode == LED_MODE_BLINK)
			{
				g_display.on_brightness_high();
				g_display.on_clean_led_green();
			}						
			if(g_display.wifi_led_mode == LED_MODE_BLINK)
			{
				g_display.on_brightness_high();
				g_display.on_wifi_led_green();
			}
			if(g_display.area_led_mode == LED_MODE_BLINK)
			{
				g_display.on_brightness_high();
				g_display.on_area_led_green();
			}							
		}
		else
		{
			if(g_robot.type == ROBOT_W410)
			{
				if(g_display.wifi_led_mode == LED_MODE_BLINK)g_display.on_wifi_led_off();
				if(g_display.clean_led_mode == LED_MODE_BLINK)g_display.on_clean_led_off();
				if(g_display.area_led_mode == LED_MODE_BLINK)g_display.on_area_led_off();
			}
			else
			{
				if(g_display.clean_led_mode == LED_MODE_BLINK)g_display.on_clean_led_off();	
				if(g_display.wifi_led_mode == LED_MODE_BLINK)g_display.on_wifi_led_off();
			}	
		}		
	}
}

void Display_Breath(void)
{
	static uint32_t breath_time;
	static uint8_t up_down = 1;
	static uint8_t temp_brighthness = 0;
	if(g_display.clean_led_mode == LED_MODE_BREATH || g_display.battery_led_mode == LED_MODE_BREATH)
	{
		if(Timer_IsTimeReach(&breath_time,TIME_500MS))
		{
			if(up_down)
			{
				temp_brighthness++;
				if(temp_brighthness > 7)
				{
					up_down = 0;
				}
			}
			else
			{
				temp_brighthness--;			
				if(temp_brighthness <= 0)
				{
					up_down = 1;
				}				
			}	
			g_display.on_set_brightness(temp_brighthness);	
		}
	}	
}

void Display_SetLedBrightness(uint8_t value)
{
	g_display.on_set_brightness(value);		
}




























