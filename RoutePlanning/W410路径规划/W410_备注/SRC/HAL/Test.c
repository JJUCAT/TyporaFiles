 #include "Include.h"

Test_t g_test;

void Test_Init(void)
{
    g_test.all_function = TEST_AF_LED;
    g_test.choice = TEST_CHOICE_ALLFUNCTION;
    g_test.mobility = TEST_MOBILITY_WIFI;
    g_test.self_check = TEST_SELFCHECK_BASE_VOL; 
}

uint8_t Test_IsEnter(void)
{
	uint8_t reval = 0;
	static uint8_t count = 0;
	osDelay(100);
	if((Button_GetCleanStatus())&&(g_hw_display.button_final_value == BUTTON_RIGHT))
	{	
		while((Button_GetCleanStatus())&&(g_hw_display.button_final_value == BUTTON_RIGHT))
		{			
			Speaker_Voice(SPK_DY);
			count++;	
			if(count >= 2)reval = 1;
			vTaskDelay(800/portTICK_RATE_MS);
		}	
	}
	return reval;	
}

void Test_SelectMode(void)
{
	uint8_t button_value;
	TestChoice_t temp_chioce = TEST_CHOICE_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

    Usprintf_Test("test select mode\n");

	Button_WaitAllKeyUp();
	Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_OFF);	
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data3 = 0;
		ObsMsg_SetMode(OBSMSG_ENABLE);
	}	
	g_code_show.enable = 1;
	CodeShow_SetData(TEST_SUB_VERSION,0,TEST_VERSION);
	while(Mode_GetMode()==MODE_TEST)
	{
		if(temp_chioce != TEST_CHOICE_NONE)
		{
			CodeShow_SetData(0,temp_chioce,0);	
		}
		if(Button_CleanKeyEvent())
		{
			if((temp_chioce != TEST_CHOICE_NONE)&&(temp_chioce != TEST_CHOICE_END))
			{
				Usprintf_Test("test select mode:%d \n",temp_chioce);
				Speaker_Voice(SPK_DY);
				g_test.choice = temp_chioce;
				break;
			}			
		}
		button_value = Button_KeysEvent();	
		if(button_value == BUTTON_VALUE_ONE)//w410
		{
			Speaker_Voice(SPK_DY);			
			if(temp_chioce >= (TEST_CHOICE_NONE + 1))temp_chioce--;
			Usprintf_Test("temp_chioce:%d \n",temp_chioce);
		}
		else if(button_value == BUTTON_VALUE_TWO)
		{
			Speaker_Voice(SPK_DY);
			if(temp_chioce <= (TEST_CHOICE_END - 1))temp_chioce++;
			Usprintf_Test("temp_chioce:%d \n",temp_chioce);
		}		

		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Button_WaitAllKeyUp();
}

void Test_SelfCheck(void)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

    Usprintf_Test("------- test self check start------\n");

	while(Mode_GetMode()==MODE_TEST)
	{
		switch(g_test.self_check)
		{
			case TEST_SELFCHECK_BASE_VOL:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_BaseVol());
			break;
			case TEST_SELFCHECK_RTC:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_Rtc());
			break;
			case TEST_SELFCHECK_BATTERY:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_Battery());
			break;
			case TEST_SELFCHECK_STATIC_CURRENT:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_BaseCurrent());			
			break;
			case TEST_SELFCHECK_GYRO:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_Gyro());
			break;
			case TEST_SELFCHECK_OBS:
											Test_SelfCheck_ResultHandler(Test_SelfCheck_Obs());
			break;													
			default:
			break;
		}
		if(g_test.self_check == TEST_SELFCHECK_END)
		{
			if(g_robot.type == ROBOT_W400)
			{
				ObsMsg_SetMode(OBSMSG_ENABLE);
			}			
			break;
		}	
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}

    Usprintf_Test("------- test self check end------\n");
}

void Test_IsExit(void)
{
	static uint32_t check_time;
	static uint16_t cnt;
	if(Mode_GetMode() == MODE_TEST)
	{
		if(Timer_IsTimeReach(&check_time,TIME_100MS))
		{			
			if(Button_GetCleanStatus())
			{
				cnt++;
			}
			else
			{
				if(cnt >= 20)
				{					
					Power_KillAllVcc();
				}				
				cnt = 0;
			}
			if(cnt == 20)Speaker_Voice(SPK_DON);
		}	
	}
}

TestResult_t Test_SelfCheck_BaseVol(void)
{
	TestResult_t result = TEST_RESULT_NONE;
	int32_t base_vol = 0;
	Usprintf_Test("test selfcheck base vol start\n");

	while(Mode_GetMode() == MODE_TEST)
	{
		base_vol = System_GetBaselineAdc();

		System_SetBaseLineAdc(base_vol);

		base_vol = base_vol*ADC_REFERENCE_VOLTAGE /4096;

		Usprintf_Test("test selfcheck base_vol:%d\n",base_vol);

		if(base_vol < TEST_BASEVOL_LIMIT_L )//82
		{
			g_test.error_state = 1;
			g_test.error_data = base_vol;
			result = TEST_RESULT_FAIL;	
		}
		else if(base_vol > TEST_BASEVOL_LIMIT_H)
		{
			g_test.error_state = 2;
			g_test.error_data = base_vol;
			result = TEST_RESULT_FAIL;	
		}
		else
		{
			result = TEST_RESULT_SUCCESSFUL;	
		}
		
		if(result != TEST_RESULT_NONE)break;
	}

    Usprintf_Test("test selfcheck base vol end\n");

	return result;
}

TestResult_t Test_SelfCheck_BaseCurrent(void)
{
	TestResult_t result = TEST_RESULT_NONE;
	int32_t i,current = 0;
	Usprintf_Test("test selfcheck base current start\n");
	while(Mode_GetMode() == MODE_TEST)
	{
		for(i = 0;i < 10;i++)
		{
			current += g_adc_value.system;	
			delay(10);
		}
		current /= 10;

		current = (current - System_GetBaselineAdc()) *ADC_REFERENCE_VOLTAGE * 40/4096;

		Usprintf_Test("test selfcheck base current:%d\n",current);

		if(current <= TEST_SYSCURRENT_LIMIT_L )//87
		{
			g_test.error_state = 1;
			g_test.error_data = current;
			result = TEST_RESULT_FAIL;	
		}
		else if ( current >= TEST_SYSCURRENT_LIMIT_H)
		{
			g_test.error_state = 2;
			g_test.error_data = current;
			result = TEST_RESULT_FAIL;
		}
		else
		{
			result = TEST_RESULT_SUCCESSFUL;	
		}
		
		if(result != TEST_RESULT_NONE)break;
	}
    Usprintf_Test("test selfcheck base current end\n");
	return result;
}

TestResult_t Test_SelfCheck_Rtc(void)
{
	TestResult_t result = TEST_RESULT_NONE;

	Usprintf_Test("test selfcheck Rtc start\n");
	while(Mode_GetMode() == MODE_TEST)
	{
		result = TEST_RESULT_SUCCESSFUL;	

		if(result != TEST_RESULT_NONE)break;
	}
    Usprintf_Test("test selfcheck Rtc end\n");
	return result;
}

TestResult_t Test_SelfCheck_Battery(void)
{
	TestResult_t result = TEST_RESULT_NONE;
	uint16_t temp_battery_vol = 0;

	Usprintf_Test("selfcheck Battery start\n");
	while(Mode_GetMode() == MODE_TEST)
	{
		temp_battery_vol = g_adc_value.battery * ADC_TO_VOL_BATTERY;

		Usprintf_Test("selfcheck battery_vol:%d\n",temp_battery_vol);	
		if(temp_battery_vol <= TEST_BATTREY_VOL_LIMIT_L)
		{
			g_test.error_state = 1;
			g_test.error_data = temp_battery_vol;
			result = TEST_RESULT_FAIL;	
		}
		else if(temp_battery_vol >= TEST_BATTREY_VOL_LIMIT_H)
		{
			g_test.error_state = 2;
			g_test.error_data = temp_battery_vol;
			result = TEST_RESULT_FAIL;	
		}
		else
		{
			result = TEST_RESULT_SUCCESSFUL;	
		}
		
		if(result != TEST_RESULT_NONE)break;
	}
    Usprintf_Test("selfcheck Battery end\n");
	return result;
}

TestResult_t Test_SelfCheck_Gyro(void)
{
	TestResult_t result = TEST_RESULT_NONE;
	uint32_t check_time = Timer_GetTime();
	uint8_t try_cnt = 0;
	Usprintf_Test("test selfcheck Gyro start\n");
	Gyro_Cmd(ENABLE);
	while(Mode_GetMode() == MODE_TEST)
	{
		if(Timer_IsTimeReach(&check_time,TIME_100MS))	
		{
			Gyro_Cmd(ENABLE);
			try_cnt++;
			if(try_cnt > 10)
			{
				g_test.error_state = 1;
				g_test.error_data = 0;				
				result = TEST_RESULT_FAIL;
			}
		}		
		if(Gryo_GetUpdateFlag())
		{
			result = TEST_RESULT_SUCCESSFUL;
		}

		if(result != TEST_RESULT_NONE)break;
	}
    Usprintf_Test("test selfcheck Gyro end\n");
	return result;
}

TestResult_t Test_SelfCheck_Obs(void)
{
	TestResult_t result = TEST_RESULT_NONE;

	Usprintf_Test("test selfcheck Obs start\n");
	while(Mode_GetMode() == MODE_TEST)
	{
		if(g_robot.type == ROBOT_W410)
		{
			result = TEST_RESULT_SUCCESSFUL;
		}
		else
		{
			if(g_obsmsg.version >= TEST_OBS_VERSION)
			{
				result = TEST_RESULT_SUCCESSFUL;
			}
			else
			{
				g_test.error_state = 1;
				g_test.error_data = g_obsmsg.version;                
				result = TEST_RESULT_FAIL;
			}			
		}
		if(result != TEST_RESULT_NONE)break;
	}
    Usprintf_Test("test selfcheck Obs end\n");
	return result;
}

void Test_SelfCheck_ResultHandler(TestResult_t result)
{
	Button_WaitAllKeyUp();
	Usprintf_Test("test selfcheck result :%d\n",result);
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);	
	switch(result)
	{
		case TEST_RESULT_FAIL:
            Usprintf_Test("TEST_RESULT_FAIL\n")
            Test_SelfCheck_ResultFail();
		break;
		case TEST_RESULT_SUCCESSFUL:
            Usprintf_Test("TEST_RESULT_SUCCESSFUL\n")																				
            g_test.self_check++;													
		break;
		case TEST_RESULT_NEXT_CHOICE:
            Usprintf_Test("TEST_RESULT_NEXT_CHOICE\n")
			if(g_test.self_check < TEST_SELFCHECK_OBS)g_test.self_check++;
		break;
		case TEST_RESULT_PREV_CHOICE:
            Usprintf_Test("TEST_RESULT_PREV_CHOICE\n")
			if(g_test.self_check > TEST_SELFCHECK_BASE_VOL)g_test.self_check--;
		break;								
		default:break;		
	}
	Button_WaitAllKeyUp();
}

void Test_SelfCheck_ResultFail(void)
{
    Speaker_Voice(SPK_DON);
    Display_SetCleanState(LED_MODE_STATIC,LED_STATE_RED);
    Display_SetWarnKeyState(LED_STATE_RED);	    
    CodeShow_SetData(g_test.self_check,g_test.error_state,g_test.error_data);
    if(g_robot.type == ROBOT_W400)
    {
        ObsMsg_SetMode(OBSMSG_ENABLE);
    }    
    while(Test_SelfCheck_SwitchChoice())	
    {
        if(Button_CleanKeyEvent())
        {
            Speaker_Voice(SPK_DY);
            break;
        }							
    }
    g_test.error_state = 0;
    g_test.error_data = 0;
    Display_SetWarnKeyState(LED_STATE_OFF);    
}

uint8_t Test_SelfCheck_SwitchChoice(void)
{
	uint8_t reval = 1;

	TestChange_t temp_change_type = Test_ChangeStep();

	if(temp_change_type == TEST_CHANGE_LAST)
	{
		if(g_test.self_check > TEST_SELFCHECK_BASE_VOL)
		{
			g_test.self_check--;
			reval = 0;
		}		
	}
	if(temp_change_type == TEST_CHANGE_NEXT)
	{
		if(g_test.self_check < TEST_SELFCHECK_OBS)
		{
			g_test.self_check++;
			reval = 0;
		}
	}
	return reval;
}

TestChange_t Test_ChangeStep(void)
{
	TestChange_t temp_change_type = TEST_CHANGE_NONE;
	uint8_t button_value = Button_KeysEvent();
	if(button_value == BUTTON_VALUE_ONE)
	{
		temp_change_type = TEST_CHANGE_LAST;
		Usprintf_Test("%s(%d):test change last\n",__FUNCTION__, __LINE__);
	}

	if(button_value == BUTTON_VALUE_TWO)
	{
		temp_change_type = TEST_CHANGE_NEXT;
		Usprintf_Test("%s(%d):test change next\n",__FUNCTION__, __LINE__);
	}	

	if(temp_change_type != TEST_CHANGE_NONE)
	{
		Speaker_Voice(SPK_DY);
	}
	return temp_change_type;
}

void Test_AllFunction(void)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test All function Start\n");
	while(Mode_GetMode()==MODE_TEST)	
	{
		switch(g_test.all_function)
		{	
			case TEST_AF_LED: 						
											Test_AllFunction_ResultHandler(Test_AllFunction_Led());					 
			break;	
			case TEST_AF_OBS:		 
											Test_AllFunction_ResultHandler(Test_AllFunction_OBS());															 
			break;	
			case TEST_AF_BUMPER:		
											Test_AllFunction_ResultHandler(Test_AllFunction_Bumper());
			break;
			case TEST_AF_CLIFF:  		
											Test_AllFunction_ResultHandler(Test_AllFunction_Cliff());
			break;
			case TEST_AF_RCON:	 
											Test_AllFunction_ResultHandler(Test_AllFunction_Rcon());									 
			break;	
     		case TEST_AF_MOBSENSOR: 	
			 								Test_AllFunction_ResultHandler(Test_AllFunction_Mobility());			                       
			break;	
			case TEST_AF_LEFTWHEEL:						
											Test_AllFunction_ResultHandler(Test_AllFunction_LeftWheel());
			break;				
			case TEST_AF_LEFTWHEEL_PLUG:
				                       		Test_AllFunction_ResultHandler(Test_AllFunction_LeftWheel()); 		
			break;			 															 																																																							 
			case TEST_AF_RIGHTWHEEL:
											Test_AllFunction_ResultHandler(Test_AllFunction_RightWheel());															 
			break;
			case TEST_AF_RIGHTWHEEL_PLUG:
											Test_AllFunction_ResultHandler(Test_AllFunction_RightWheel());															 
			break;																	 																 
			case TEST_AF_BRUSH:
											Test_AllFunction_ResultHandler(Test_AllFunction_MainBrush()) ;															 
			break;	
			case TEST_AF_BRUSH_PLUG: 
											Test_AllFunction_ResultHandler(Test_AllFunction_MainBrush()) ;															 
			break;
			case TEST_AF_VACUUM: 		
											Test_AllFunction_ResultHandler(Test_AllFunction_Vacuum()) ;															 
			break;																						 													 
			case TEST_AF_PUMPER:
											Test_AllFunction_ResultHandler(Test_AllFunction_Pumper()) ;															 
			break;																			 															 
			case TEST_AF_CHARGE:	
											Test_AllFunction_ResultHandler(Test_AllFunction_Charging()) ;															 
			break;													 															 														 
		    default: break;
		}		
	 	vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);		
	}
    Usprintf_Test("Test All function End\n");
}

uint8_t Test_AllFunction_SwitchChoice(void)
{
	uint8_t reval = 1;

	TestChange_t temp_change_type = Test_ChangeStep();

	if(temp_change_type == TEST_CHANGE_LAST)
	{
		if(g_test.all_function > TEST_AF_LED)
		{
			g_test.all_function--;
			reval = 0;
		}		
	}
	if(temp_change_type == TEST_CHANGE_NEXT)
	{
		if(g_test.all_function < TEST_AF_CHARGE)
		{
			g_test.all_function++;
			reval = 0;
		}
	}
	return reval;
}

void Test_AllFunction_ResultFail(void)
{
    Speaker_Voice(SPK_DON);
    if(g_robot.type == ROBOT_W400)
    {
        Display_SetCleanState(LED_MODE_STATIC,LED_STATE_OFF);
        Display_SetCleanState(LED_MODE_STATIC,LED_STATE_RED);
    }
    else
    {
        Display_SetWarnKeyState(LED_STATE_RED);
    }	
    CodeShow_SetData(g_test.all_function,g_test.error_state,g_test.error_data);
    while(Test_AllFunction_SwitchChoice())
    {
        if(Button_CleanKeyEvent())
        { 
            break;
        }							
    }
    g_test.error_state = 0;
    g_test.error_data = 0;
    Display_SetWarnKeyState(LED_STATE_OFF);
    Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
    Speaker_Voice(SPK_DY);
}

void Test_AllFunction_ResultHandler(uint8_t result)
{
	Button_WaitAllKeyUp();
	Usprintf("test all function result :%d\n",result);
	if(result != TEST_RESULT_FAIL)Speaker_Voice(SPK_DY);
	Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);			
	switch(result)
	{
		case TEST_RESULT_FAIL:
            Usprintf("TEST_RESULT_FAIL\n");
            Test_AllFunction_ResultFail();    
		break;
		case TEST_RESULT_SUCCESSFUL:
            Usprintf("TEST_RESULT_SUCCESSFUL\n");
            if(g_test.all_function == TEST_AF_CHARGE)	
            {
                CodeShow_SetData(0,0,8888);
                while(Mode_GetMode() == MODE_TEST);
            }	
            else
            {
                g_test.all_function++;	
            }						
		break;
		case TEST_RESULT_NEXT_CHOICE:	
            Usprintf("TEST_RESULT_NEXT_CHOICE\n");					
            g_test.all_function++;
            if(g_test.all_function > TEST_AF_CHARGE)
            {
                g_test.all_function = TEST_AF_LED;	
            }
		break;
		case TEST_RESULT_PREV_CHOICE:
            Usprintf("TEST_RESULT_PREV_CHOICE\n");
            g_test.all_function--;
            if(g_test.all_function < TEST_AF_LED)
            {
                g_test.all_function = TEST_AF_LED;	
            }						
		break;								
		default:break;
	}
	Button_WaitAllKeyUp();
}

TestResult_t Test_AllFunction_Led(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t ir_send_time = Timer_GetTime();
	uint32_t blink_time = Timer_GetTime();
	uint8_t test_step = 0;
	uint8_t blink = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test All Function Led Start\n");
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
	CodeShow_SetData(g_test.all_function,0,0);
	while(Mode_GetMode() == MODE_TEST)
	{          								
		if(Timer_IsTimeReach(&blink_time,TIME_1S))
		{
			blink = 1 - blink;
			if(blink)
			{
				Hw_Display_ALL_On();
				Usprintf_Test("display on\n");
			}
			else
			{
				Display_All_Off();
				Usprintf_Test("display off\n");
			}			
		}

		switch(test_step)
		{
			case 0:
					if((Button_KeysEvent() == BUTTON_VALUE_ONE))
					{
						Speaker_Voice(SPK_DY);
						test_step++;
						Usprintf_Test("AreaKey\n");
					}
			break;
			case 1:
					if(Button_CleanKeyEvent())
					{
						Speaker_Voice(SPK_DY);
						test_step++;
						Usprintf_Test("CleanKey\n");
					}					
			break;
			case 2:
					if((Button_KeysEvent() == BUTTON_VALUE_TWO))
					{						
						temp_result = TEST_RESULT_SUCCESSFUL;	
						Usprintf_Test("WarnKey\n");	
					}			
			break;

			default:break;
		}
		if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV))
		{
			g_test.error_state = test_step;
			temp_result = TEST_RESULT_FAIL;	
			Usprintf_Test("Remote nav\n");	
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			Display_All_Off();
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
    Usprintf_Test("Test All Function Led End\n");	
	return temp_result;
}

TestResult_t Test_AllFunction_OBS(void)
{
	uint32_t debug_time,check_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;	
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint16_t obs_l_low = 5000,obs_f_low = 5000,obs_r_low = 5000,wall_l_low = 5000,wall_r_low = 5000;
	uint16_t obs_l_high = 0,obs_f_high = 0,obs_r_high = 0,wall_l_high = 0,wall_r_high = 0;
	uint8_t l_status = 0,f_status = 0,r_status = 0,l_wall_status = 0,r_wall_status = 0;
	uint8_t showcode_enable = 0;
	CodeShow_SetData(g_test.all_function,0,0);
	if(g_robot.type == ROBOT_W400)
	{
		ObsMsg_SetMode(OBSMSG_ENABLE);
	}	
	else
	{
		g_obs.state = ENABLE;
        g_wall.state = ENABLE;
	}
	Usprintf_Test("Test All Function Obs Start\n");
	while(Test_AllFunction_SwitchChoice())
	{	
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			showcode_enable = 1 - showcode_enable;
			g_code_show.enable = showcode_enable;
			Usprintf_Test("all function obs:(%d,%d,%d)wall(%d %d)\n",g_obs.adc.left,g_obs.adc.front,g_obs.adc.right,g_wall.adc.left,g_wall.adc.right);			
		}

		if(Timer_Get_DiffNow(check_time) > TIME_100MS)
		{
			if(g_robot.type == ROBOT_W410)
			{
				if(g_obs.adc.left <= TEST_OBS_L_LIMIT)l_status |= 0x01;	
				if(g_obs.adc.left >= TEST_OBS_H_LIMIT)l_status |= 0x02;	
				if(g_obs.adc.front <= TEST_OBS_L_LIMIT)f_status |= 0x01;	
				if(g_obs.adc.front >= TEST_OBS_H_LIMIT)f_status |= 0x02;	
				if(g_obs.adc.right <= TEST_OBS_L_LIMIT)r_status |= 0x01;	
				if(g_obs.adc.right >= TEST_OBS_H_LIMIT)r_status |= 0x02;						
			}
			else
			{
				if(g_obs.adc.left <= TEST_OBS_L_LIMIT)l_status |= 0x01;	
				if(g_obs.adc.left >= TEST_OBS_H_LIMIT)l_status |= 0x02;	
				if(g_obs.adc.front <= TEST_OBS_L_LIMIT)f_status |= 0x01;	
				if(g_obs.adc.front >= TEST_OBS_H_LIMIT)f_status |= 0x02;	
				if(g_obs.adc.right <= TEST_OBS_L_LIMIT)r_status |= 0x01;	
				if(g_obs.adc.right >= TEST_OBS_H_LIMIT)r_status |= 0x02;				
			}
			if(g_obs.adc.left < obs_l_low)obs_l_low = g_obs.adc.left;	
			if(g_obs.adc.front < obs_f_low)obs_f_low = g_obs.adc.front;	
			if(g_obs.adc.right < obs_r_low)obs_r_low = g_obs.adc.right;	
			if(g_obs.adc.left > obs_l_high)obs_l_high = g_obs.adc.left;	
			if(g_obs.adc.front > obs_f_high)obs_f_high = g_obs.adc.front;	
			if(g_obs.adc.right > obs_r_high)obs_r_high = g_obs.adc.right;	

			if(Math_Diff_int(obs_l_low,obs_l_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_l_high > obs_l_low)
				{
					l_status = 0x03;
				}				
			}
			if(Math_Diff_int(obs_f_low,obs_f_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_f_high > obs_f_low)
				{
					f_status = 0x03;
				}					
			}
			
			if(Math_Diff_int(obs_r_low,obs_r_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_r_high > obs_r_low)
				{
					r_status = 0x03;
				}					
			}			
			if(g_wall.adc.left <= TEST_WALL_L_LIMIT)l_wall_status |= 0x01;	
			if(g_wall.adc.left >= TEST_WALL_H_LIMIT)l_wall_status |= 0x02;	
			if(g_wall.adc.right <= TEST_WALL_L_LIMIT)r_wall_status |= 0x01;	
			if(g_wall.adc.right >= TEST_WALL_H_LIMIT)r_wall_status |= 0x02;

			if(g_wall.adc.left < wall_l_low)wall_l_low = g_obs.adc.left;	
			if(g_wall.adc.right < wall_r_low)wall_r_low = g_obs.adc.right;	
			if(g_wall.adc.left > wall_l_high)wall_l_high = g_obs.adc.left;	
			if(g_wall.adc.right > wall_r_high)wall_r_high = g_obs.adc.right;

			if(Math_Diff_int(wall_l_low,wall_l_high) >= TEST_WALL_DIF_VALUE)
			{
				if(wall_l_high > wall_l_low)
				{
					l_wall_status = 0x03;
				}				
			}

			if(Math_Diff_int(wall_r_low,wall_r_high) >= TEST_WALL_DIF_VALUE)
			{
				if(wall_r_high > wall_r_low)
				{
					r_wall_status = 0x03;
				}					
			}
		}

		if(Button_CleanKeyEvent())
		{
			if(l_status != 0x03)
			{
				g_test.error_state = 1;
				g_test.error_data = g_obs.adc.left;		
			}
			else if(f_status != 0x03)
			{
				g_test.error_state = 2;		
				g_test.error_data = g_obs.adc.front;	
			}
			else if(r_status != 0x03)
			{
				g_test.error_state = 3;	
				g_test.error_data = g_obs.adc.right;		
			}
			else if(l_wall_status != 0x03)
			{
				g_test.error_state = 4;	
				g_test.error_data = g_wall.adc.left;		
			}
			else if(r_wall_status != 0x03)
			{
				g_test.error_state = 5;	
				g_test.error_data = g_wall.adc.right;		
			}
			temp_result = TEST_RESULT_FAIL;												
		}

		if(l_status == 0x03 && f_status == 0x03 && r_status == 0x03 && l_wall_status == 0x03 && r_wall_status == 0x03)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;
		}

		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	g_code_show.enable = 1;	
	if(g_robot.type == ROBOT_W410)
	{
		g_obs.state = DISABLE;
        g_wall.state = DISABLE;
	}	
    Usprintf_Test("Test All Function Obs End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_Bumper(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint8_t l_status = 0,r_status = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(g_test.all_function,0,0);
	Usprintf_Test("Test All Function Bumper Start\n");
	while(Test_AllFunction_SwitchChoice())
	{		
		if(Bumper_GetTrigStatus()&BUMPER_TRIG_LEFT)	l_status |= 0x01;
		else l_status |= 0x02;
		if(Bumper_GetTrigStatus()&BUMPER_TRIG_RIGHT) r_status |= 0x01;
		else r_status |= 0x02;

		if(Button_CleanKeyEvent())
		{
			if(l_status != 0x03)
			{
				g_test.error_state = 1;
				g_test.error_data = g_adc_value.l_bumper;		
			}
			else if(r_status != 0x03)
			{
				g_test.error_state = 2;	
				g_test.error_data = g_adc_value.r_bumper;	
			}
			temp_result = TEST_RESULT_FAIL;												
		}

		if(l_status == 0x03 && r_status == 0x03)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;
		}

		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}			
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
    Usprintf_Test("Test All Function Bumper End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_Cliff(void)	
{
	uint32_t debug_time,check_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint16_t   cliff_limit = 400,cliff_pick_up_limit = 40;//100-70  50-40
	uint8_t   l_cliff_status = 0,f_cliff_status = 0,r_cliff_status = 0;
	uint8_t showcode_enable = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(g_test.all_function,0,0);
	if(g_robot.type == ROBOT_W410)
	{
		g_cliff.state = ENABLE;
	}
	else
	{
		cliff_limit = 200;
	}	
	Usprintf_Test("Test All Function Cliff Start\n");
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			showcode_enable = 1 - showcode_enable;
			g_code_show.enable = showcode_enable;			
			Usprintf_Test("l cliff:%d f cliff:%d r cliff :%d\n",g_cliff.adc.left,g_cliff.adc.front,g_cliff.adc.right);
		}
		if(Timer_Get_DiffNow(check_time) > TIME_100MS)
		{
			if(g_cliff.adc.left > cliff_limit)l_cliff_status |= 0x01; 
			if(g_cliff.adc.left < cliff_pick_up_limit)l_cliff_status |= 0x02;
			if(g_cliff.adc.front > cliff_limit)f_cliff_status |= 0x01; 
			if(g_cliff.adc.front < cliff_pick_up_limit)f_cliff_status |= 0x02;
			if(g_cliff.adc.right > cliff_limit)r_cliff_status |= 0x01; 
			if(g_cliff.adc.right < cliff_pick_up_limit)r_cliff_status |= 0x02;
		}
		
		if((l_cliff_status == 0x03)&&(f_cliff_status == 0x03)&&(r_cliff_status == 0x03))
		{
			temp_result = TEST_RESULT_SUCCESSFUL;				
		}
		if(Button_CleanKeyEvent())
		{
			if(l_cliff_status != 0x03)
			{
				g_test.error_state = 1;
				g_test.error_data = g_cliff.adc.left;		
			}
			else if(f_cliff_status != 0x03)
			{
				g_test.error_state = 2;
				g_test.error_data = g_cliff.adc.front;		
			}
			else if(r_cliff_status != 0x03)
			{
				g_test.error_state = 3;	
				g_test.error_data = g_cliff.adc.right;	
			}			
			temp_result = TEST_RESULT_FAIL;												
		}		
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	if(g_robot.type == ROBOT_W410)g_cliff.state = DISABLE;	
	g_code_show.enable = 1;
    Usprintf_Test("Test All Function Cliff End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_Rcon(void)
{
	uint8_t rcon_status = 0,status = 0;
	TestResult_t temp_result = TEST_RESULT_NONE;	
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(g_test.all_function,0,0);
	Usprintf_Test("Test All Function Rcon Start\n");	
	Rcon_ResetStatus();
	while(Test_AllFunction_SwitchChoice())
	{
		rcon_status = Rcon_GetStatus();	
		//charger
		if( (rcon_status&RCON_FL_HOME_L) || (rcon_status & RCON_FL_HOME_R))
		{
			status |= 0x01;
		}
		if((rcon_status&RCON_FR_HOME_L) || (rcon_status & RCON_FR_HOME_R))
		{
			status |= 0x02;		
		}		
		//vw
		if((rcon_status&RCON_FL_VW_N) || (rcon_status&RCON_FL_VW_T))
		{
			status |= 0x01;
		}
		if((rcon_status&RCON_FR_VW_N) || (rcon_status&RCON_FR_VW_T))
		{
			status |= 0x02;		
		}
		
		if(status == 0x03)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;		
		}

		if(Button_CleanKeyEvent())
		{
			if(!(status & 0x01))
			{
				g_test.error_state = 1;		
			}
			else if(!(status & 0x02))
			{
				g_test.error_state = 2;		
			}			
			temp_result = TEST_RESULT_FAIL;												
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}					
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
    Usprintf_Test("Test All Function Rcon End\n");	
	return temp_result;
}

TestResult_t Test_AllFunction_Mobility(void)
{
	uint32_t debug_time;
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(g_test.all_function,0,0);
	Mobility_ResetStep();
	Usprintf_Test("Test All Function Mobility Start\n");	
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("mobility steps %d\n",Mobility_GetStep());
		}		
		if(Mobility_GetStep() >3 && Mobility_GetStep() < 100)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;			
		}
		if(Button_CleanKeyEvent())
		{
			g_test.error_state = 1;		
			g_test.error_data = Mobility_GetStep();			
			temp_result = TEST_RESULT_FAIL;												
		}							
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
    Usprintf_Test("Test All Function Mobility End\n");	
	return temp_result;
}

TestResult_t Test_AllFunction_LeftWheel(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t debug_time,check_time = Timer_GetTime();
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test All Function Left Wheel Start\n");	
	CodeShow_SetData(g_test.all_function,0,0);
	Action_MoveForward(FORWARD_SPEED,0,MAX_DISTANCE,MOVE_ACT_FORWARD);
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("set speed :%d  real speed :%d left wheel current:%d pwm:%d\n",Wheel_GetLeftTargetSpeed(),g_wheel.left_real_speed,Wheel_GetLeftCurrent(),*g_wheel.p_pwm_left);
		}

		if(g_test.all_function == TEST_AF_LEFTWHEEL)		
		{				
			if(Timer_Get_DiffNow(check_time) > TIME_3S)
			{
				if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
				{
					if(g_wheel.left_real_speed < 5)	
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 5;	
						g_test.error_data = g_wheel.left_real_speed;			
					}					
					Wheel_SetDir(WHEEL_DIR_BACKWARD);
					check_time = Timer_GetTime();
				}
				else
				{
					temp_result = TEST_RESULT_SUCCESSFUL;
				}				
			}			
			if(Timer_Get_DiffNow(check_time) > TIME_1S)
			{
				if(Wheel_GetLeftCurrent() < TEST_WHEEL_L_LIMIT||Wheel_GetLeftCurrent() > TEST_WHEEL_H_LIMIT)//70
				{
					if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 1;
						g_test.error_data = Wheel_GetLeftCurrent();									
					}
					else
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 3;	
						g_test.error_data = Wheel_GetLeftCurrent();					
					}				
				}					
				if(*g_wheel.p_pwm_left > TEST_WHEEL_PWM_LIMIT)//28
				{
					if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
					{
						temp_result = TEST_RESULT_FAIL;		
						g_test.error_state = 2;
						g_test.error_data = *g_wheel.p_pwm_left;																		
					}
					else
					{
						temp_result = TEST_RESULT_FAIL;		
						g_test.error_state = 4;	
						g_test.error_data = *g_wheel.p_pwm_left;																	
					}
				}				
			}								
		}
		if(g_test.all_function == TEST_AF_LEFTWHEEL_PLUG)
		{			
			if(Wheel_GetLeftCurrent() > 500 && *g_wheel.p_pwm_left > 80) 
			{
				temp_result = TEST_RESULT_SUCCESSFUL;					
			}
			if(Button_CleanKeyEvent())
			{
				g_test.error_state = 1;	
				g_test.error_data = Wheel_GetLeftCurrent();				
				temp_result = TEST_RESULT_FAIL;												
			}
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
	Wheel_SetLeftTargetSpeed(0);
	Wheel_SetRightTargetSpeed(0);
	Wheel_Configure(DISABLE);
	Wheel_SetTuneState(DISABLE);
	Wheel_ResetStep();	
    Usprintf_Test("Test All Function Left Wheel End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_RightWheel(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t debug_time,check_time = Timer_GetTime();		
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
    Usprintf_Test("Test All Function Right Wheel Start\n");	
	CodeShow_SetData(g_test.all_function,0,0);
	Action_MoveForward(0,FORWARD_SPEED,MAX_DISTANCE,MOVE_ACT_FORWARD);
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("set speed :%d  real speed :%d right wheel current:%d pwm:%d\n",Wheel_GetRightTargetSpeed(),g_wheel.right_real_speed,Wheel_GetRightCurrent(),*g_wheel.p_pwm_right);
		}
		if(g_test.all_function == TEST_AF_RIGHTWHEEL)		
		{				
			if(Timer_Get_DiffNow(check_time) > TIME_3S)
			{
				if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
				{
					if(g_wheel.right_real_speed < 5)	
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 5;	
						g_test.error_data = g_wheel.right_real_speed;				
					}					
					Wheel_SetDir(WHEEL_DIR_BACKWARD);
					check_time = Timer_GetTime();
				}
				else
				{
					temp_result = TEST_RESULT_SUCCESSFUL;
				}				
			}			
			if(Timer_Get_DiffNow(check_time) > TIME_1S)
			{
				if(Wheel_GetRightCurrent() < TEST_WHEEL_L_LIMIT||Wheel_GetRightCurrent() > TEST_WHEEL_H_LIMIT)//70
				{
					if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 1;
						g_test.error_data = Wheel_GetRightCurrent();								
					}
					else
					{
						temp_result = TEST_RESULT_FAIL;	
						g_test.error_state = 3;	
						g_test.error_data = Wheel_GetRightCurrent();						
					}				
				}					
				if(*g_wheel.p_pwm_right > TEST_WHEEL_PWM_LIMIT)//28
				{
					if(Wheel_GetDir() == WHEEL_DIR_FORWARD)
					{
						temp_result = TEST_RESULT_FAIL;		
						g_test.error_state = 2;
						g_test.error_data = *g_wheel.p_pwm_right;																		
					}
					else
					{
						temp_result = TEST_RESULT_FAIL;		
						g_test.error_state = 4;	
						g_test.error_data = *g_wheel.p_pwm_right;																	
					}
				}				
			}								
		}
		if(g_test.all_function == TEST_AF_RIGHTWHEEL_PLUG)
		{			
			if(Wheel_GetRightCurrent() > 500 && *g_wheel.p_pwm_right > 80) 
			{
				temp_result = TEST_RESULT_SUCCESSFUL;					
			}
			if(Button_CleanKeyEvent())
			{
				g_test.error_state = 1;
				g_test.error_data = Wheel_GetRightCurrent();						
				temp_result = TEST_RESULT_FAIL;												
			}
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
	Wheel_SetLeftTargetSpeed(0);
	Wheel_SetRightTargetSpeed(0);
	Wheel_Configure(DISABLE);
	Wheel_SetTuneState(DISABLE);
	Wheel_ResetStep();	
    Usprintf_Test("Test All Function Right Wheel End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_MainBrush(void)
{
	uint32_t debug_time,check_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(g_test.all_function,0,0);
	MainBrush_SetVoltage(MAINBRUSH_VOLTAGE);
    MainBrush_SetVoltageControl(ENABLE);
	Usprintf_Test("Test All Function MainBrush Start\n");			

	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("brush current:%d pwm:%d\n",MainBrush_GetCurrent(),*g_mainbrush.p_pwm);
		}		
		if( g_test.all_function == TEST_AF_BRUSH)
    	{			
			if(Timer_Get_DiffNow(check_time) > TIME_3S)
			{
				if((MainBrush_GetCurrent() < TEST_MAINBRUSH_L_LIMIT)||(MainBrush_GetCurrent() > TEST_MAINBRUSH_H_LIMIT))/*normally:370ma*/
				{
					temp_result = TEST_RESULT_FAIL;
					g_test.error_state = 1;
					g_test.error_data = MainBrush_GetCurrent();							
				}
				if(*g_mainbrush.p_pwm > 90)
				{				
					temp_result = TEST_RESULT_FAIL;	
					g_test.error_state = 2;	
					g_test.error_data = *g_mainbrush.p_pwm;									
				}	
				else
				{
					temp_result = TEST_RESULT_SUCCESSFUL;
				}							
			}				
		}
    	if( g_test.all_function == TEST_AF_BRUSH_PLUG)
		{
			if((MainBrush_GetCurrent() > TEST_MAINBRUSH_OVER_LIMIT))
			{
				temp_result = TEST_RESULT_SUCCESSFUL;											  			
			}		
			if(Button_CleanKeyEvent())
			{
				g_test.error_state = 1;		
				g_test.error_data = MainBrush_GetCurrent();			
				temp_result = TEST_RESULT_FAIL;												
			}				
		}		
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	MainBrush_SetVoltage(0);
    MainBrush_SetVoltageControl(DISABLE);
	*g_mainbrush.p_pwm = 0;
    Usprintf_Test("Test All Function MainBrush End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_Vacuum(void)
{
	uint32_t debug_time,check_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test All Function Vacuum Start\n");	
	CodeShow_SetData(g_test.all_function,0,0);	
    Vacuum_SetSpeed(VAC_SPEED_NORMAL);
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("vac current:%d BLDC_PWM:%d \n",Vacuum_GetCurrent(),*g_vacuum.p_pwm);
		}	
		if(Timer_Get_DiffNow(check_time) > TIME_3S)
		{
			if(Vacuum_IsFail())
			{
				Vacuum_ResetFail();
				temp_result = TEST_RESULT_FAIL;	
				g_test.error_state = 3;				
			}			
			else if(Vacuum_GetCurrent() < VACUUM_STALL_NOLOAD || Vacuum_GetCurrent() > VACUUM_STALL_LIMIT)//1000mA
			{				
				temp_result = TEST_RESULT_FAIL;
				g_test.error_state = 1;	
				g_test.error_data = Vacuum_GetCurrent();				
			}
			else if(*g_vacuum.p_pwm > 90)
			{
				temp_result = TEST_RESULT_FAIL;	
				g_test.error_state = 2;	
				g_test.error_data = *g_vacuum.p_pwm;					
			}
			else
			{
				temp_result = TEST_RESULT_SUCCESSFUL;
			}			
		}	
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Vacuum_SetSpeed(0);
	*g_vacuum.p_pwm = 0;	
    Usprintf_Test("Test All Function Vacuum End\n");	
	return temp_result;
}

TestResult_t Test_AllFunction_Pumper(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t debug_time,test_time = Timer_GetTime(),check_time = Timer_GetTime();
	uint8_t tank_status=0,clean_status = 0,dirty_status = 0,pump_status = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test All Function Pump Start\n");	
	CodeShow_SetData(g_test.all_function,0,0);
	Pump_SetVoltageControl(ENABLE);
    Pump_SetVoltage(PUMP_VOLTAGE);
	Pump_SetState(ENABLE);
    g_watertank.state = ENABLE;
	while(Test_AllFunction_SwitchChoice())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("water state  clean: %d dirt: %d tank:%d pump:%d clean ad:%d dirty ad:%d\n",clean_status,dirty_status,tank_status,pump_status,g_watertank.adc.clean);
		}		
		if(Timer_Get_DiffNow(test_time) >= TIME_10S)
		{
			if(clean_status == 0x03)	
			{
				Pump_SetState(DISABLE);	
			}
			if(g_pump.cnt == 0)
			{
				g_test.error_state = 1;
				temp_result = TEST_RESULT_FAIL;	
			}
		}
		if(Timer_Get_DiffNow(check_time) > TIME_1S)
		{
			if(g_pump.cnt != 0)
			{
				pump_status = 1;	
			}

			if(g_watertank.adc.clean > WATERTANK_CLEAN_H_LIMIT)
			{
				clean_status |= 0X01;	
			}
			if(g_watertank.adc.clean < WATERTANK_CLEAN_L_LIMIT)
			{
				clean_status |= 0X02;
			}

			if(g_watertank.adc.dirt > WATERTANK_DIRTY_H_LIMIT)
			{
				dirty_status |= 0X01;
			}

			if(g_watertank.adc.dirt < WATERTANK_DIRTY_L_LIMIT)
			{
				dirty_status |= 0X02;
			}
		}

		if(WaterTank_IsInstall())
		{
			tank_status |= 0X01;
		}
		else
		{
			tank_status |= 0X02;	
		}

		if(pump_status && (clean_status == 0x03) && (dirty_status == 0x03) && (tank_status == 0x03))
		{
			temp_result = TEST_RESULT_SUCCESSFUL;	
		}

		if(Button_CleanKeyEvent())
		{					
			if(!pump_status)
			{
				g_test.error_state = 4;	
				g_test.error_data = g_pump.cnt;
			}
			else if (clean_status != 0x03)
			{
				g_test.error_state = 2;
				g_test.error_data = g_watertank.adc.clean;
			}
			else if (dirty_status != 0x03)
			{
				g_test.error_state = 3;
				g_test.error_data = g_watertank.adc.dirt;
			}
			else if (tank_status != 0x03)
			{
				g_test.error_state = 1;
				g_test.error_data = WaterTank_IsInstall();
			}
			temp_result = TEST_RESULT_FAIL;	
		}

		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Pump_SetVoltageControl(DISABLE);
    Pump_SetVoltage(0);
	Pump_SetState(DISABLE);
    g_watertank.state = DISABLE;
    Usprintf_Test("Test All Function Pump End\n");
	return temp_result;
}

TestResult_t Test_AllFunction_Charging(void)
{
	uint8_t charge_mode = CHARGE_MODE_CC;
	TestResult_t temp_result = TEST_RESULT_NONE;
  	int16_t charging_current = 0;
	uint16_t battery_voltage = 0;
	int16_t target_current = 50;
	uint16_t temp_pwm = 10,pwm_buffer = 0;
	uint8_t  judge_state = 0;
	TestCharger_t  charger_state = TEST_CHARGE_DOCK;
	uint32_t adjust_current_time,debug_time,charging_time;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	
	CodeShow_SetData(g_test.all_function,0,0);

	Usprintf_Test("Test All Function Charge Start\n");	

	while(Test_AllFunction_SwitchChoice())
	{
		switch(charger_state)
		{
			case TEST_CHARGE_DOCK:
					if(Charge_IsAdapterInput())
					{
						charger_state = TEST_CHARGE_INIT;
					}
					if(Remote_IsRemoteEvent(REMOTE_SIGNAL_NAV))
					{
						g_test.error_data = g_charge.voltage;
						g_test.error_state = 1;
						temp_result = TEST_RESULT_FAIL;			
					}						
			break;
			case TEST_CHARGE_INIT:
					Speaker_Voice(SPK_CHARGING_START);
					Button_WaitAllKeyUp();
					System_StoreBaselineAdc();
					Charge_ConfigurationIn();
					Display_All_Off();
					Display_SetBatteryState(LED_MODE_BREATH,LED_STATE_ORANGE); 
					*g_charge.p_pwm = 0;
					temp_pwm = 1;
					charge_mode = CHARGE_MODE_CC;
					charger_state = TEST_CHARGE_ING;
					Charge_SetCharging(TRUE);
					charging_time = Timer_GetTime();
					if((Battery_GetVoltage()> 1300) && (Battery_GetVoltage() <= 1400))	
					{
						judge_state = 1;	 
					}	
					else if((Battery_GetVoltage()> 1400) && (Battery_GetVoltage() <= 1450))
					{
						judge_state = 2;		 
					}	 
					else if((Battery_GetVoltage()> 1450) && (Battery_GetVoltage() <= 1500))
					{
						judge_state = 3;		 	 
					}			
					else if((Battery_GetVoltage()> 1500) && (Battery_GetVoltage() <= 1550))
					{
						judge_state = 4;		 	 
					}	
					else if((Battery_GetVoltage()> 1550) && (Battery_GetVoltage() <= 1600))
					{
						judge_state = 5;		 	 
					}		 
					else if(Battery_GetVoltage()> 1600)
					{
						judge_state = 6;		 	 
					}		
			break;
			case TEST_CHARGE_ING:
					charging_current = g_battery.capacity.current;
					battery_voltage = Battery_GetVoltage();

					if(Timer_IsTimeReach(&debug_time,TIME_1S))
					{
						Usprintf_Test("temp_pwm:%d charging_current:%d b:%d\n",temp_pwm,charging_current,battery_voltage);	
					}

					if(Timer_Get_DiffNow(charging_time) >= TIME_20S)
					{
						g_test.error_state = 5;
						temp_result = TEST_RESULT_FAIL;							
					}

					if(!Charge_IsAdapterInput())
					{
						g_test.error_state = 1;
						temp_result = TEST_RESULT_FAIL;
					}
					if(charging_current >= (TEST_CHARGE_TARGET_CURRENT*6))
					{
						g_test.error_state = 2;
						temp_result = TEST_RESULT_FAIL;							
					}
					if(battery_voltage > CHARGE_BATTERY_ERROR_VOLTAGE)
					{
						g_test.error_state = 4;
						temp_result = TEST_RESULT_FAIL;			
					}					
					if(Timer_IsTimeReach(&adjust_current_time,TIME_100MS))
					{
						if(battery_voltage > 1100)
						{
							target_current++;
							if(target_current > CHARGE_TARGET_CURRENT)target_current = CHARGE_TARGET_CURRENT;
						}
						else			
						{
							target_current = 50;
						}
					}
					if(charging_current >= (TEST_CHARGE_TARGET_CURRENT - 5))
					{
						Usprintf_Test("arrive target current,judge_state:%d p:%d ,c:%d\n",judge_state,temp_pwm,charging_current); 
						pwm_buffer = temp_pwm;
						switch(judge_state)
						{
							case 1 :  
											if((pwm_buffer > 150)  && (pwm_buffer < 270))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 												
											}
											break;
							case 2 :
											if((pwm_buffer > 170)  && (pwm_buffer < 310))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 
											}							
											break;					
							case 3 :
											if((pwm_buffer > 190)  && (pwm_buffer < 330))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 
											}							
											break;					
							case 4 :
											if((pwm_buffer > 220)  && (pwm_buffer < 360))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 
											}							
											break;
							case 5 :
											if((pwm_buffer > 250)  && (pwm_buffer < 400))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 
											}							
											break;
							case 6 :
											if((pwm_buffer > 280)  && (pwm_buffer < 460))
											{
												temp_result = TEST_RESULT_SUCCESSFUL; 
											}
											else
											{
												temp_result = TEST_RESULT_FAIL; 
											}							
											break;						
							default:break;
						}
						if(temp_result == TEST_RESULT_FAIL)
						{
							g_test.error_state = 3;	
							g_test.error_data = pwm_buffer;
						}
										
					}					
					switch(charge_mode)
					{
						case CHARGE_MODE_CV: 						
							if(battery_voltage > CHARGE_BATTERY_FULL_VOLTAGE)
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
							if(charging_current < CHARGE_FINISH_CURRENT)
							{
								charge_mode=CHARGE_MODE_FINISH;																
							}
							break;						
						case CHARGE_MODE_CC:	
							if(charging_current > target_current)
							{
								if(temp_pwm < 2)temp_pwm = 2;
								temp_pwm -= 2;
							}
							if(charging_current < target_current)
							{
								if(battery_voltage <= CHARGE_BATTERY_FULL_VOLTAGE)
								{
									temp_pwm++;
								}
								if(temp_pwm > CHARGE_PWM_RANGE)temp_pwm = CHARGE_PWM_RANGE;
							}									
							if(battery_voltage >= CHARGE_BATTERY_FULL_VOLTAGE)
							{
								charge_mode = CHARGE_MODE_CV;
								temp_result = TEST_RESULT_SUCCESSFUL;
							}
							break;
						case CHARGE_MODE_FINISH:					
							temp_pwm = 0;																		
							break;														
						default:charge_mode = CHARGE_MODE_FINISH;
						break;
					}		
					*g_charge.p_pwm = temp_pwm;					
			break;

			default:break;
		}								
			
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
 	*g_charge.p_pwm = 0;
  	Charge_ConfigurationOut();
	Usprintf_Test("Test All Function Charge End\n");	
	return temp_result;
}

TestWifiVersion_t Test_WifiVersion(void)
{
    TestWifiVersion_t reval = TEST_WIFI_VERSION_NONE;

    #ifdef ALICLOUD
    static uint8_t try_cnt = 0;
    static uint8_t g_wifi_aliyun_version[4] = {1,1,0,7};
    
    if(g_ali.wifi_sysinfo.is_receive)
    {
        uint8_t i,is_right = 1;
        for (i = 0;i < 4;i++)
        {
            if (g_ali.wifi_sysinfo.version[i] > g_wifi_aliyun_version[i])
            {
                Usprintf_Test("version is valid\n");
                is_right = 1;
                break; 
            }
            else if (g_ali.wifi_sysinfo.version[i] < g_wifi_aliyun_version[i])
            {
                Usprintf_Test("version is invalid\n");
                is_right = 0;												
                break; 
            }	
        }
        if(is_right)
        {
            g_ali.factory_test_mode = 1;
            g_ali.function_time = Timer_GetTime();
            g_ali.factorytest_seqnum = 0;
            g_ali.factorytest_ack_ok = 0;
            AliCloud_SetMsg(ALI_ATTRID_FACTORY_TEST);
            try_cnt = 0;
            reval = TEST_WIFI_VERSION_PASS;
            Usprintf_Test("version test ok---->function test\n");
        }
        else
        {
            reval = TEST_WIFI_VERSION_ERROR;
            Usprintf_Test("version test error\n");
        }										
    }
    else
    {
        if(Timer_IsTimeReach(&g_ali.get_mac_time,TIME_1S))
        {
            AliCloud_GetMsg(ALI_ATTRID_WIFI_MAC);	
            try_cnt++;
            if(try_cnt >= 5)
            {
                try_cnt = 0;
                reval = TEST_WIFI_VERSION_UART_ERROR;
                Usprintf_Test("usart test error\n");												
            }
        }	
    }	
    #endif

    #ifdef TUYA


    #endif

    return reval;
}

TestWifiFunction_t Test_WifiFunction(void)
{
    TestWifiFunction_t reval = TEST_WIFI_FUNCTION_NONE;

    #ifdef ALICLOUD
    static uint8_t try_cnt = 0;

    if (g_ali.factorytest_ack_ok == 1)
    {
        reval = TEST_WIFI_FUNCTION_PASS;
        Usprintf_Test("wifi test ok\n");		
    }	
    else
    {
        if(Timer_IsTimeReach(&g_ali.function_time,TIME_10S))
        {
            AliCloud_SetMsg(ALI_ATTRID_FACTORY_TEST);
            try_cnt++;
            if(try_cnt >= 3)
            {
                try_cnt = 0;
                reval = TEST_WIFI_FUNCTION_ERROR;
                Usprintf_Test("function test error\n");												
            }												
        }
    }
    #endif

    #ifdef TUYA


    #endif

    return reval;
}

void Test_Mobility(void)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test_Mobility Start\n");
	while(Mode_GetMode()==MODE_TEST)
	{
		switch(g_test.mobility)
		{
			case TEST_MOBILITY_WIFI:
									Test_MobilityResult_Handler(Test_Mobility_Wifi());	
			break;
			case TEST_MOBILITY_GYRO:
									Test_MobilityResult_Handler(Test_Mobility_Gyro());	
			break;
			case TEST_MOBILITY_MAINBRUSH:
									Test_MobilityResult_Handler(Test_Mobility_MainBrush());	
			break;
			case TEST_MOBILITY_VACUUM:
									Test_MobilityResult_Handler(Test_Mobility_Vacuum());
			break;
			case TEST_MOBILITY_RCON:
									Test_MobilityResult_Handler(Test_Mobility_Rcon());
			break;
			case TEST_MOBILITY_OBS:
									Test_MobilityResult_Handler(Test_Mobility_Obs());
			break;
			case TEST_MOBILITY_BUMPER:
									Test_MobilityResult_Handler(Test_Mobility_Bumper());	
			break;
			case TEST_MOBILITY_CLIFF:
									Test_MobilityResult_Handler(Test_Mobility_Cliff());	
			break;
			case TEST_MOBILITY_PUMPER:
									Test_MobilityResult_Handler(Test_Mobility_Pumper());	
			break;			
			default:break;
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Usprintf_Test("Test_Mobility end\n");
}

void Test_MobilityResult(void)
{
	Gyro_End();
	while(Mode_GetMode()==MODE_TEST)
	{	
		if(Button_CleanKeyEvent())
		{
			g_test.mobility = TEST_MOBILITY_WIFI;
			Speaker_Voice(SPK_DY);
			break;
		}
		osDelay(20);
	}
}

void Test_MobilityResult_Handler(uint8_t result)
{
	Button_WaitAllKeyUp();
	Usprintf_Test("mobility test result :%d\n",result);
	if(result == TEST_RESULT_FAIL)
	{
		Speaker_Voice(SPK_DON);
		Display_SetCleanState(LED_MODE_STATIC,LED_STATE_RED);
		Display_SetWarnKeyState(LED_STATE_RED);
		Robot_MotorsSetState(DISABLE);	
		g_power.on_set_wifi_state(OFF);
		g_robot.update_position = 0;
		g_cliff.stop = 0;
		Wifi_SetFactoryTestMode(0);				
	}
	else if(result == TEST_RESULT_SUCCESSFUL)
	{
		Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
	}
	else
	{
		Robot_MotorsSetState(DISABLE);
		Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
	}					
	
	switch(result)
	{
		case TEST_RESULT_NONE:
			CodeShow_SetData(g_test.mobility,0,0);
			Test_MobilityResult();	
		break;
		case TEST_RESULT_FAIL:
			CodeShow_SetData(g_test.mobility,g_test.error_state,g_test.error_data);
			Test_MobilityResult();	
			g_test.error_state = 0;
			g_test.error_data = 0;
			Display_SetWarnKeyState(LED_STATE_OFF);
		break;
		case TEST_RESULT_SUCCESSFUL:
			if(g_test.mobility == TEST_MOBILITY_PUMPER)	
			{	
				Robot_MotorsSetState(DISABLE);						
				CodeShow_SetData(0,0,8888);
				Test_MobilityResult();	
			}	
			else
			{
				g_test.mobility++;	
			}						
		break;								
		default:break;
	}
	Button_WaitAllKeyUp();
}

TestResult_t Test_Mobility_Wifi(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t check_time = Timer_GetTime();
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test_Mobility_Wifi Start\n");	
	g_power.on_set_wifi_state(ON);
	CodeShow_SetData(g_test.mobility,0,0);
	Robot_MotorsSetState(ENABLE);
	Rcon_ResetStatus();
	g_pump.cnt = 0;
	Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
	Wifi_TestStart();
	while(Test_Mobility_Stop())
	{
		if(Timer_Get_DiffNow(check_time) >= TIME_2S)
		{								
			Wifi_SendFactoryCmd();
			temp_result = TEST_RESULT_SUCCESSFUL;
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Usprintf_Test("Test_Mobility_Wifi End\n");	
	return temp_result;
}

TestResult_t Test_Mobility_Gyro(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	int16_t target_angle = 900;	
	uint8_t turn_cnt = 0;
	uint8_t is_gyro_ready = 0;
	uint32_t start_time = Timer_GetTime(),headcourse_time = Timer_GetTime();
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Mobility Gyro Start\n")
	CodeShow_SetData(g_test.mobility,0,0);
	Gyro_Start();
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);
	while(Test_Mobility_Stop())
	{
		if(Gyro_IsError())
		{
			g_test.error_state = 1;
			g_test.error_data = 0;				
		}

		if(Gyro_IsReady())
		{
			if(!is_gyro_ready)
			{
				is_gyro_ready = TRUE;	
				Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
				g_robot.update_position = ENABLE;
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
				Action_SetMove(MOVE_ACT_HANDLER);															
				headcourse_time = Timer_GetTime();	
			}			
		}

		if(is_gyro_ready)
		{
			switch(Action_GetMove())
			{						
				case MOVE_ACT_HEAD2COURCE:																	
					if(Action_WaitForHeadToCourseEnded()) 
					{																	
						if((Wheel_GetLeftStep() <= 900)||(Wheel_GetLeftStep() >= 1800))//1440
						{
							temp_result = TEST_RESULT_FAIL;
							g_test.error_state = 3;	
							g_test.error_data = Wheel_GetLeftStep();															
						}
					}
					if(Timer_Get_DiffNow(headcourse_time) > TIME_5S)
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 2;	
						g_test.error_data = 0;																	
					}
					
					break;
				case MOVE_ACT_DECELERATE:			
					Action_WaitForDecelerateEnded();				
					break;																																	
				case MOVE_ACT_HANDLER:					
					Action_ListSwitch();
					break;				
				case MOVE_ACT_STATIC: 															
					turn_cnt++;
					if(turn_cnt > 3)
					{
						turn_cnt = 0;
						temp_result = TEST_RESULT_SUCCESSFUL;
						break;
					}
					target_angle = target_angle + 900;
					if(target_angle > 2700)
					{
						target_angle = 0;
					}
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);	
					headcourse_time = Timer_GetTime();
					break;		
				default:break;						
			}
		}
			
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}			
	  	vTaskDelayUntil(&xLastWakeTime,50/portTICK_RATE_MS);
	}
	Usprintf_Test("Mobility Gyro End\n")
	return temp_result;
}

TestResult_t Test_Mobility_MainBrush(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test Mobility MainBrush \n");	
	CodeShow_SetData(g_test.mobility,0,0);
	while(Test_Mobility_Stop())
	{
		if((MainBrush_GetCurrent() < TEST_MAINBRUSH_L_LIMIT)||(MainBrush_GetCurrent() > TEST_MAINBRUSH_H_LIMIT))
		{
			temp_result = TEST_RESULT_FAIL;
			g_test.error_state = 1;
			g_test.error_data = MainBrush_GetCurrent();							
		}
		if(*g_mainbrush.p_pwm > 90)
		{					
			temp_result = TEST_RESULT_FAIL;	
			g_test.error_state = 2;	
			g_test.error_data = *g_mainbrush.p_pwm;									
		}	
		else
		{
			temp_result = TEST_RESULT_SUCCESSFUL;
		}			
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	return temp_result;
}

TestResult_t Test_Mobility_Vacuum(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test Mobility Vacuum \n");	
	CodeShow_SetData(g_test.mobility,0,0);
	while(Test_Mobility_Stop())
	{
		if(Vacuum_IsFail())
		{
			Vacuum_ResetFail();
			temp_result = TEST_RESULT_FAIL;	
			g_test.error_state = 3;				
		}			
		else if(Vacuum_GetCurrent() < VACUUM_STALL_NOLOAD || Vacuum_GetCurrent() > VACUUM_STALL_LIMIT)//1000mA
		{				
			temp_result = TEST_RESULT_FAIL;
			g_test.error_state = 1;	
			g_test.error_data = Vacuum_GetCurrent();				
		}
		else if(*g_vacuum.p_pwm > 90)
		{
			temp_result = TEST_RESULT_FAIL;	
			g_test.error_state = 2;	
			g_test.error_data = *g_vacuum.p_pwm;					
		}
		else
		{
			temp_result = TEST_RESULT_SUCCESSFUL;
		}			
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	return temp_result;
}

TestResult_t Test_Mobility_Rcon(void)
{
	uint8_t rcon_status = 0,status = 0;
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Mobility Rcon Test\n");	
	CodeShow_SetData(g_test.mobility,0,0);
	while(Test_Mobility_Stop())
	{
		rcon_status = Rcon_GetStatus();	
		//charger
		if( (rcon_status&RCON_FL_HOME_L) || (rcon_status & RCON_FL_HOME_R))
		{
			status |= 0x01;
		}
		if((rcon_status&RCON_FR_HOME_L) || (rcon_status & RCON_FR_HOME_R))
		{
			status |= 0x02;		
		}		
		//vw
		if((rcon_status&RCON_FL_VW_N) || (rcon_status&RCON_FL_VW_T))
		{
			status |= 0x01;
		}
		if((rcon_status&RCON_FR_VW_N) || (rcon_status&RCON_FR_VW_T))
		{
			status |= 0x02;		
		}
		
		if(status == 0x03)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;		
		}
		else if(!(status & 0x01))
		{
			g_test.error_state = 1;	
			temp_result = TEST_RESULT_FAIL;		
		}
		else if(!(status & 0x02))
		{
			g_test.error_state = 2;	
			temp_result = TEST_RESULT_FAIL;		
		}			
																		
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	return temp_result;
}

TestResult_t Test_Mobility_Obs(void)
{
	uint32_t debug_time,check_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t target_position_arrive = 0,target_angle_arrive = 0;
	Point16_t target_cell;
	uint8_t l_status = 0,f_status = 0,r_status = 0,l_wall_status = 0,r_wall_status = 0;
	uint16_t obs_l_low = 5000,obs_f_low = 5000,obs_r_low = 5000,wall_l_low = 5000,wall_r_low = 5000;
	uint16_t obs_l_high = 0,obs_f_high = 0,obs_r_high = 0,wall_l_high = 0,wall_r_high = 0;	
	g_robot.update_position = 1;
	g_gyro.is_ready = 1;
	target_cell.x = 7;//8
	target_cell.y = 0;
	CodeShow_SetData(g_test.mobility,0,0);
	Map_NormalInitialize();
	Wheel_SetCount(0,0);
	Pump_SetStartControl(ENABLE);
	Action_MoveToPoint(Map_CellToPoint(target_cell),20);
	Movement_PidSetThinkNextPoint(FALSE);
	Heading_Reset_Integral(HEADING_INTEGRAL_MOVETOPOSITION);
	if(g_robot.type == ROBOT_W410)
	{
		g_obs.state = ENABLE;
		g_wall.state = ENABLE;
	}	
	Usprintf_Test("Test Mobility Obs \n");
	while(Test_Mobility_Stop())
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_MOVE2POINT:
										if(Action_GetArriveTarget())
										{
											Action_ResetArriveTarget();
											Action_Static();
											target_position_arrive = 1;
											Usprintf_Test("Arrive Target Position,Start Checking obs\n");
										}
			break;
			case MOVE_ACT_HEAD2COURCE:
										if(Action_GetHead2CourseArrive())	
										{
											Action_ResetHead2CourseArrive();
											Action_Static();
											target_angle_arrive++;
											Usprintf_Test("Arrive Target Angle,Continue Turning,target_angle_arrive:%d\n",target_angle_arrive);
										}	
			break;															
			case MOVE_ACT_STATIC:
										if(target_position_arrive)
										{
											target_position_arrive = 0;	
											g_code_show.enable = DISABLE;									
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,900,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);											
											Action_SetMove(MOVE_ACT_HANDLER);									
										}
										if(target_angle_arrive == 1)
										{										
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,0,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);											
											Action_SetMove(MOVE_ACT_HANDLER);													
										}
										if(target_angle_arrive == 2)
										{									
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,2700,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);											
											Action_SetMove(MOVE_ACT_HANDLER);													
										}
										if(target_angle_arrive == 3)
										{										
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,0,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);											
											Action_SetMove(MOVE_ACT_HANDLER);													
										}										
										if(target_angle_arrive == 4)
										{
											temp_result = TEST_RESULT_FAIL;	
											if(l_status != 0x03)
											{
												g_test.error_state = 1;
												g_test.error_data = g_obs.adc.left;		
											}
											else if(f_status != 0x03)
											{
												g_test.error_state = 2;		
												g_test.error_data = g_obs.adc.front;	
											}
											else if(r_status != 0x03)
											{
												g_test.error_state = 3;	
												g_test.error_data = g_obs.adc.right;		
											}
											else if(l_wall_status != 0x03)
											{
												g_test.error_state = 4;	
												g_test.error_data = g_wall.adc.left;		
											}
											else if(r_wall_status != 0x03)
											{
												g_test.error_state = 5;	
												g_test.error_data = g_wall.adc.right;		
											}
											else
											{
												temp_result = TEST_RESULT_SUCCESSFUL;	
											}											
										}
			break;
			case MOVE_ACT_HANDLER:								
										Action_ListSwitch();
			break;			

			default:break;
		}
		if(Timer_Get_DiffNow(check_time) > TIME_100MS)
		{
			if(g_robot.type == ROBOT_W410)
			{
				if(g_obs.adc.left <= TEST_OBS_L_LIMIT)l_status |= 0x01;	
				if(g_obs.adc.left >= TEST_OBS_H_LIMIT)l_status |= 0x02;	
				if(g_obs.adc.front <= TEST_OBS_L_LIMIT)f_status |= 0x01;	
				if(g_obs.adc.front >= TEST_OBS_H_LIMIT)f_status |= 0x02;	
				if(g_obs.adc.right <= TEST_OBS_L_LIMIT)r_status |= 0x01;	
				if(g_obs.adc.right >= TEST_OBS_H_LIMIT)r_status |= 0x02;			
			}
			else
			{
				if(g_obs.adc.left <= TEST_OBS_L_LIMIT/2)l_status |= 0x01;	
				if(g_obs.adc.left >= TEST_OBS_H_LIMIT/2)l_status |= 0x02;	
				if(g_obs.adc.front <= TEST_OBS_L_LIMIT/2)f_status |= 0x01;	
				if(g_obs.adc.front >= TEST_OBS_H_LIMIT/2)f_status |= 0x02;	
				if(g_obs.adc.right <= TEST_OBS_L_LIMIT/2)r_status |= 0x01;	
				if(g_obs.adc.right >= TEST_OBS_H_LIMIT/2)r_status |= 0x02;				
			}
			if(g_obs.adc.left < obs_l_low)obs_l_low = g_obs.adc.left;	
			if(g_obs.adc.front < obs_f_low)obs_f_low = g_obs.adc.front;	
			if(g_obs.adc.right < obs_r_low)obs_r_low = g_obs.adc.right;	
			if(g_obs.adc.left > obs_l_high)obs_l_high = g_obs.adc.left;	
			if(g_obs.adc.front > obs_f_high)obs_f_high = g_obs.adc.front;	
			if(g_obs.adc.right > obs_r_high)obs_r_high = g_obs.adc.right;	

			if(Math_Diff_int(obs_l_low,obs_l_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_l_high > obs_l_low)
				{
					l_status = 0x03;
				}				
			}
			if(Math_Diff_int(obs_f_low,obs_f_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_f_high > obs_f_low)
				{
					f_status = 0x03;
				}					
			}
			
			if(Math_Diff_int(obs_r_low,obs_r_high) >= TEST_OBS_DIF_VALUE)
			{
				if(obs_r_high > obs_r_low)
				{
					r_status = 0x03;
				}					
			}			
			if(g_wall.adc.left <= TEST_WALL_L_LIMIT)l_wall_status |= 0x01;	
			if(g_wall.adc.left >= TEST_WALL_H_LIMIT)l_wall_status |= 0x02;	
			if(g_wall.adc.right <= TEST_WALL_L_LIMIT)r_wall_status |= 0x01;	
			if(g_wall.adc.right >= TEST_WALL_H_LIMIT)r_wall_status |= 0x02;

			if(g_wall.adc.left < wall_l_low)wall_l_low = g_obs.adc.left;	
			if(g_wall.adc.right < wall_r_low)wall_r_low = g_obs.adc.right;	
			if(g_wall.adc.left > wall_l_high)wall_l_high = g_obs.adc.left;	
			if(g_wall.adc.right > wall_r_high)wall_r_high = g_obs.adc.right;

			if(Math_Diff_int(wall_l_low,wall_l_high) >= TEST_WALL_DIF_VALUE)
			{
				if(wall_l_high > wall_l_low)
				{
					l_wall_status = 0x03;
				}				
			}

			if(Math_Diff_int(wall_r_low,wall_r_high) >= TEST_WALL_DIF_VALUE)
			{
				if(wall_r_high > wall_r_low)
				{
					r_wall_status = 0x03;
				}					
			}			
		}
		if(Timer_IsTimeReach(&debug_time,TIME_100MS))
		{
			Usprintf_Test("obs:(%d,%d,%d)wall(%d %d)\n",g_obs.adc.left,g_obs.adc.front,g_obs.adc.right,g_wall.adc.left,g_wall.adc.right);			
		}		
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	if(g_robot.type == ROBOT_W410)
	{
		g_obs.state = DISABLE;
		g_wall.state = DISABLE;
	}
	g_code_show.enable = ENABLE;
	return temp_result;
}

TestResult_t Test_Mobility_Bumper(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t l_status = 0,r_status = 0,turn_cnt = 0;
	Usprintf_Test("Mobility Bumper Test\n");
	CodeShow_SetData(g_test.mobility,0,0);
	if(Bumper_GetTrigStatus()&BUMPER_TRIG_LEFT)
	{
		temp_result = TEST_RESULT_FAIL;
		g_test.error_state = 1;
		g_test.error_data = g_adc_value.l_bumper;		
	}
	else if(Bumper_GetTrigStatus()&BUMPER_TRIG_RIGHT)
	{
		temp_result = TEST_RESULT_FAIL;
		g_test.error_state = 2;
		g_test.error_data = g_adc_value.r_bumper;		
	}	
	else
	{
		l_status |= 0x02;
		r_status |= 0x02;
		turn_cnt = 1;
		Action_ListClear();
		Action_ListAdd(MOVE_ACT_TURN_LEFT,800,TURN_SPEED,TURN_SPEED);	
		Action_ListAdd(MOVE_ACT_FORWARD,1000,FORWARD_SPEED,FORWARD_SPEED);	
		Action_SetMove(MOVE_ACT_HANDLER);	
	}	
	while(Test_Mobility_Stop())
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_FORWARD:
				if(Action_WaitForMoveEnded())
				{
					if(turn_cnt == 1)
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 2;
						g_test.error_data = g_adc_value.r_bumper;											
					}
					else
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 1;
						g_test.error_data = g_adc_value.l_bumper;											
					}	
					break;									
				}
				if(Bumper_GetTrigStatus()&BUMPER_TRIG_LEFT)
				{
					l_status |= 0x01;	
				}
				if(Bumper_GetTrigStatus()&BUMPER_TRIG_RIGHT)
				{
					r_status |= 0x01;											  
				}	
				if(Bumper_GetTrigStatus())
				{	
					if(turn_cnt == 1)
					{
						Action_ListClear();
						Action_ListAdd(MOVE_ACT_BACK,600,BACK_SPEED,BACK_SPEED);
						Action_ListAdd(MOVE_ACT_TURN_RIGHT,1600,TURN_SPEED,TURN_SPEED);																									
						Action_ListAdd(MOVE_ACT_FORWARD,1000,FORWARD_SPEED,FORWARD_SPEED);	
						Action_SetMove(MOVE_ACT_HANDLER);
					}																													
					else
					{
						Action_ListClear();
						Action_ListAdd(MOVE_ACT_BACK,600,BACK_SPEED,BACK_SPEED);																								
						Action_ListAdd(MOVE_ACT_STATIC,0,0,0);	
						Action_SetMove(MOVE_ACT_HANDLER);																						
					}
					turn_cnt++;																				  
				}								  								  
			break;		
			case MOVE_ACT_BACK:
				if(Action_WaitForMoveEnded())
				{
					if(Bumper_GetTrigStatus()&BUMPER_TRIG_LEFT)
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 1;
						g_test.error_data = g_adc_value.l_bumper;		
					}
					else if(Bumper_GetTrigStatus()&BUMPER_TRIG_RIGHT)
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 2;
						g_test.error_data = g_adc_value.r_bumper;		
					}										
				}
			break;	
			case MOVE_ACT_TURN_LEFT:
				Action_WaitForMoveEnded();
			break;	
			case MOVE_ACT_TURN_RIGHT:
				Action_WaitForMoveEnded();
			break;																							
			case MOVE_ACT_STATIC:
				temp_result = TEST_RESULT_FAIL;	
				if(l_status != 0x03)
				{
					g_test.error_state = 1;
					g_test.error_data = g_adc_value.l_bumper;		
				}
				else if(r_status != 0x03)
				{
					g_test.error_state = 2;	
					g_test.error_data = g_adc_value.r_bumper;	
				}
				else
				{
					temp_result = TEST_RESULT_SUCCESSFUL;	
				}
			break;
			case MOVE_ACT_HANDLER:								
				Action_ListSwitch();
			break;			

			default:break;
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	return temp_result;
}

TestResult_t Test_Mobility_Cliff(void)
{
	uint8_t cliff_trigger_status = 0;
	uint32_t check_time = Timer_GetTime(),debug_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t cliff_l = 0,cliff_r = 0,cliff_f = 0;
	uint8_t target_position_arrive = 0;
	Point16_t robot_cell,target_cell;
	PathListCell_t temp_pathlist;
	int32_t target_angle;
	robot_cell = Map_GetRobotCell();
	Usprintf_Test("Mobility Cliff Test\n");
	CodeShow_SetData(g_test.mobility,0,0);
	target_cell.x = robot_cell.x - 2;//3
	target_cell.y = robot_cell.y;

	target_angle = Map_GetRobotToPointAngle(Map_CellToPoint(target_cell));
	Action_SetTargetPoint(Map_CellToPoint(target_cell));	
    Action_ListClear();
    Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
    Action_ListAdd(MOVE_ACT_MOVE2POINT,MAX_DISTANCE,MOVETOPOINT_SPEED,MOVETOPOINT_SPEED);
    Action_SetMove(MOVE_ACT_HANDLER);	
	temp_pathlist.cell_pos.x = target_cell.x;
	temp_pathlist.cell_pos.y = -8;
	temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
	PathList_AddNewPoint(temp_pathlist);	

	temp_pathlist.cell_pos.x = 2;//0
	temp_pathlist.cell_pos.y = -8;
	temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
	PathList_AddNewPoint(temp_pathlist);	

	if(g_robot.type == ROBOT_W410)
	{
		g_cliff.state = ENABLE;
	}
	else 
	{
		ObsMsg_SetMode(OBSMSG_ENABLE);
	}	

	Cliff_ForwardStopEnable();
	while(Test_Mobility_Stop())
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_MOVE2POINT:
										if(Action_GetArriveTarget())
										{
											Action_ResetArriveTarget();
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,1800,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
											Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
											Action_SetMove(MOVE_ACT_HANDLER);	
											target_position_arrive = 1;
											Usprintf_Test("Target Position Arrive\n");										
										}
			break;
			case MOVE_ACT_HEAD2COURCE:
										Action_WaitForHeadToCourseEnded();	
			break;
			case MOVE_ACT_FORWARD:
										if(Action_WaitForMoveEnded())
										{	
											Usprintf_Test("Over Step\n");	
											Action_Static();	
										}	
										cliff_trigger_status = Cliff_GetDetectiontProcess_Result();
										if(cliff_trigger_status)
										{
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_BACK,600,BACK_SPEED,BACK_SPEED);											
											if(cliff_trigger_status&CLIFF_TRIG_LEFT)
											{
												Action_ListAdd(MOVE_ACT_TURN_LEFT,800,TURN_SPEED,TURN_SPEED);	
												Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
												cliff_l = 1;
												Usprintf_Test("Left Cliff Trig\n");	
											}
											else if(cliff_trigger_status&CLIFF_TRIG_FRONT)
											{
												Action_ListAdd(MOVE_ACT_TURN_LEFT,800,TURN_SPEED,TURN_SPEED);	
												Action_ListAdd(MOVE_ACT_FORWARD,2000,FORWARD_SPEED,FORWARD_SPEED);
												cliff_f = 1;
												Usprintf_Test("Front Cliff Trig\n");	
											}
											else if(cliff_trigger_status&CLIFF_TRIG_RIGHT)
											{
												Action_ListAdd(MOVE_ACT_TURN_RIGHT,1600,TURN_SPEED,TURN_SPEED);
												Action_ListAdd(MOVE_ACT_FORWARD,2000,FORWARD_SPEED,FORWARD_SPEED);
												cliff_r = 1;
												Usprintf_Test("Right Cliff Trig\n");		
											}
											Action_SetMove(MOVE_ACT_HANDLER);																						
										}
			break;
			case MOVE_ACT_BACK:
										Action_WaitForMoveEnded();
			break;	
			case MOVE_ACT_TURN_LEFT:
										Action_WaitForMoveEnded();
			break;	
			case MOVE_ACT_TURN_RIGHT:
										Action_WaitForMoveEnded();
			break;					
			case MOVE_ACT_STATIC:
										if(target_position_arrive)
										{
											target_position_arrive = 0;	
											g_code_show.enable = 0;									
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_FORWARD,4*CELL_COUNT_MUL,FORWARD_SPEED,FORWARD_SPEED);											
											Action_SetMove(MOVE_ACT_HANDLER);	
											Usprintf_Test("MoveForward-->\n");										
										}
										else
										{
											if(!cliff_l)
											{
												temp_result = TEST_RESULT_FAIL;
												g_test.error_state = 1;
												g_test.error_data = g_cliff.adc.left;
											}
											else if(!cliff_f)
											{
												temp_result = TEST_RESULT_FAIL;
												g_test.error_state = 2;
												g_test.error_data = g_cliff.adc.front;												
											}
											else if(!cliff_r)
											{
												temp_result = TEST_RESULT_FAIL;
												g_test.error_state = 3;
												g_test.error_data = g_cliff.adc.right;												
											}	
											else
											{
												Usprintf_Test("Cliff test ok\n");		
												temp_result = TEST_RESULT_SUCCESSFUL;
											}										
										}											
			break;
			case MOVE_ACT_HANDLER:								
										Action_ListSwitch();
			break;			

			default:break;
		}
		if(Timer_IsTimeReach(&debug_time,TIME_100MS))
		{
			Usprintf_Test("cliff:(%d,%d,%d)\n",g_cliff.adc.left,g_cliff.adc.front,g_cliff.adc.right);			
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	g_code_show.enable = 1;	
	g_cliff.stop = 0;
	if(g_robot.type == ROBOT_W410)g_cliff.state = DISABLE;
	return temp_result;
}

TestResult_t Test_Mobility_Pumper(void)
{
	uint32_t check_time = Timer_GetTime(),debug_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	uint8_t target_position_arrive = 0;
	Point16_t robot_cell,target_cell;
	PathListCell_t temp_pathlist;
	int32_t target_angle;

	robot_cell = Map_GetRobotCell();
	Usprintf_Test("Mobility Pump Test\n");
	CodeShow_SetData(g_test.mobility,0,0);
	target_cell.x = robot_cell.x + 3;
	target_cell.y = robot_cell.y;

	target_angle = Map_GetRobotToPointAngle(Map_CellToPoint(target_cell));
	Action_SetTargetPoint(Map_CellToPoint(target_cell));	
    Action_ListClear();
    Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
    Action_ListAdd(MOVE_ACT_MOVE2POINT,MAX_DISTANCE,MOVETOPOINT_SPEED,MOVETOPOINT_SPEED);
    Action_SetMove(MOVE_ACT_HANDLER);	

	temp_pathlist.cell_pos.x = target_cell.x;
	temp_pathlist.cell_pos.y = 0;
	temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
	PathList_AddNewPoint(temp_pathlist);	

	temp_pathlist.cell_pos.x = 0;
	temp_pathlist.cell_pos.y = 0;
	temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
	PathList_AddNewPoint(temp_pathlist);	

	g_watertank.state = ENABLE;

	while(Test_Mobility_Stop())
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_MOVE2POINT:
										if(Action_GetArriveTarget())
										{
											Action_ResetArriveTarget();
											Action_ListClear();
											Action_ListAdd(MOVE_ACT_HEAD2COURCE,0,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
											Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
											Action_SetMove(MOVE_ACT_HANDLER);	
											target_position_arrive = 1;
											Usprintf("Target Position Arrive\n");										
										}
			break;
			case MOVE_ACT_HEAD2COURCE:
										Action_WaitForHeadToCourseEnded();	
			break;					
			case MOVE_ACT_STATIC:
										if(target_position_arrive)
										{
											target_position_arrive = 0;	
											if(g_pump.cnt == 0)
											{
												g_test.error_state = 3;
												g_test.error_data = 0;
												temp_result = TEST_RESULT_FAIL;
											}
											else if(g_watertank.adc.clean > WATERTANK_CLEAN_H_LIMIT)
											{
												g_test.error_state = 1;
												g_test.error_data = g_watertank.adc.clean;
												temp_result = TEST_RESULT_FAIL;												
											}
											else if(g_watertank.adc.dirt < WATERTANK_DIRTY_L_LIMIT)
											{
												g_test.error_state = 2;
												g_test.error_data = g_watertank.adc.dirt;
												temp_result = TEST_RESULT_FAIL;													
											}
											else
											{
												temp_result = TEST_RESULT_SUCCESSFUL;		
											}																				
										}											
			break;
			case MOVE_ACT_HANDLER:								
										Action_ListSwitch();
			break;			

			default:break;
		}
		if(Timer_IsTimeReach(&debug_time,TIME_100MS))
		{
			Usprintf_Test("water:(%d,%d)\n",g_watertank.adc.clean,g_watertank.adc.dirt);			
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	return temp_result;
}

uint8_t Test_Mobility_Stop(void)
{
	uint8_t reval = 1;
	if(Button_CleanKeyEvent())
	{
		reval = 0;
		Usprintf("%s(%d):test button stop\n",__FUNCTION__, __LINE__);
	}

	Wifi_TestCycle();		
	
	return reval;
}

void Test_Wifi(void)
{
	TestWifi_t wifi_state = TEST_WIFI_INIT;
	uint32_t start_time = Timer_GetTime();
	TestWifiVersion_t version_result = TEST_WIFI_VERSION_NONE;
	TestWifiFunction_t function_result = TEST_WIFI_FUNCTION_NONE;
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CTRL_WIFI_ON;
	CodeShow_SetData(TEST_CHOICE_WIFI,0,0);
	Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);	
	Usprintf_Test("Test_Wifi start\n");
	while(Test_Stop())
	{		
		switch(wifi_state)
		{
			case TEST_WIFI_INIT:
									if(Timer_Get_DiffNow(start_time) >= TIME_2S)
									{
										Wifi_TestInit();
										wifi_state = TEST_WIFI_VERSION;
									}
			break;
			case TEST_WIFI_VERSION:
									version_result = Test_WifiVersion();
									
									switch(version_result)
									{
										case TEST_WIFI_VERSION_PASS:
																								wifi_state = TEST_WIFI_FUNCTION;
										break;
										case TEST_WIFI_VERSION_UART_ERROR:
																								temp_result = TEST_RESULT_FAIL;
																								g_test.error_state = 2;											
										break;
										case TEST_WIFI_VERSION_ERROR:
																								temp_result = TEST_RESULT_FAIL;
																								g_test.error_state = 1;											
										break;										
										default:break;									
									}									
			break;
			case TEST_WIFI_FUNCTION:								
									function_result = Test_WifiFunction();
									
									switch(function_result)
									{
										case TEST_WIFI_FUNCTION_PASS:
																								temp_result = TEST_RESULT_SUCCESSFUL;
										break;
										case TEST_WIFI_FUNCTION_ERROR:
																								temp_result = TEST_RESULT_FAIL;
																								g_test.error_state = 3;									
										break;									
										default:break;									
									}				
			break;
			default:break;
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	g_power.on_set_wifi_state(OFF);
	Usprintf_Test("Test_Wifi End\n");
	if(temp_result == TEST_RESULT_SUCCESSFUL)
	{
		Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
	}
	Test_ResultHandler(TEST_CHOICE_WIFI,temp_result);
}

void Test_Aging(void)
{
	uint32_t debug_time,wheel_dir_change_time = Timer_GetTime(),start_time = Timer_GetTime(),check_time = Timer_GetTime(),pump_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();	

	Robot_MotorsSetState(ENABLE);
	Action_MoveForward(FORWARD_SPEED,FORWARD_SPEED,MAX_DISTANCE,MOVE_ACT_FORWARD);
	g_watertank.state = ENABLE;
	g_pump.cnt = 0;
	CodeShow_SetData(TEST_CHOICE_AGING,0,0);
	Usprintf_Test("Test_Aging Start\n")

	while(Test_Stop())
	{	
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf_Test("wheel:%d %d brush:%d vac:%d\n",Wheel_GetLeftCurrent(),Wheel_GetRightCurrent(),MainBrush_GetCurrent(),Vacuum_GetCurrent());
		}

		if(Timer_Get_DiffNow(start_time) > TIME_3MIN)
		{
			temp_result = TEST_RESULT_SUCCESSFUL;
		}

		if(Timer_Get_DiffNow(check_time) >= TIME_3S)
		{
			if((MainBrush_GetCurrent() < TEST_MAINBRUSH_L_LIMIT)||(MainBrush_GetCurrent() > TEST_MAINBRUSH_H_LIMIT))
			{
				g_test.error_state = 1;
				g_test.error_data = MainBrush_GetCurrent();
				temp_result = TEST_RESULT_FAIL;
				Speaker_Voice(SPK_CHECK_BRUSH);
			}

			if(Vacuum_GetCurrent() < VACUUM_STALL_NOLOAD || Vacuum_GetCurrent() > VACUUM_STALL_LIMIT)
			{
				g_test.error_state = 2;
				g_test.error_data = Vacuum_GetCurrent();
				temp_result = TEST_RESULT_FAIL;			
			}

			if(Wheel_GetLeftCurrent() > TEST_WHEEL_H_LIMIT)
			{
				g_test.error_state = 3;
				g_test.error_data = Wheel_GetLeftCurrent();
				temp_result = TEST_RESULT_FAIL;	
				Speaker_Voice(SPK_CHECK_LEFT_WHEEL);			
			}

			if(Wheel_GetRightCurrent() > TEST_WHEEL_H_LIMIT)
			{
				g_test.error_state = 4;
				g_test.error_data = Wheel_GetRightCurrent();
				temp_result = TEST_RESULT_FAIL;	
				Speaker_Voice(SPK_CHECK_RIGHT_WHEEL);		
			}
		}

		if(Timer_Get_DiffNow(pump_time) > TIME_20S)
		{

			if(g_watertank.adc.clean > WATERTANK_CLEAN_H_LIMIT)
			{
				g_test.error_state = 5;
				g_test.error_data = g_watertank.adc.clean;
				temp_result = TEST_RESULT_FAIL;		
				Speaker_Voice(SPK_CHECK_CLEAN_TANK);					
			}
			
			if(g_watertank.adc.dirt < WATERTANK_DIRTY_L_LIMIT)
			{
				g_test.error_state = 6;
				g_test.error_data = g_watertank.adc.dirt;
				temp_result = TEST_RESULT_FAIL;		
				Speaker_Voice(SPK_CHECK_DIRTY_TANK);				
			}

			if(g_pump.cnt == 0)
			{
				g_test.error_state = 7;
				g_test.error_data = 0;
				temp_result = TEST_RESULT_FAIL;					
			}			
		}

		if(Timer_IsTimeReach(&wheel_dir_change_time,TIME_20S))	
		{
			if(Wheel_GetDir() == WHEEL_DIR_BACKWARD)
			{
				Wheel_SetDir(WHEEL_DIR_FORWARD);
			}
			else
			{
				Wheel_SetDir(WHEEL_DIR_BACKWARD);
			}
			check_time = Timer_GetTime();
		}	
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}			
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Usprintf_Test("Test_Aging End\n")
	Robot_MotorsSetState(DISABLE);
	Action_Static();
	g_watertank.state = DISABLE;
	Test_ResultHandler(TEST_CHOICE_AGING,temp_result);	
}

void Test_Pump(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t start_time = Timer_GetTime();
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	CodeShow_SetData(TEST_CHOICE_PUMP,0,0);
	Pump_SetVoltageControl(ENABLE);
    Pump_SetVoltage(PUMP_VOLTAGE);
	Pump_SetState(ENABLE);
	g_pump.cnt = 0;
	Usprintf_Test("Test_Pump Start\n")
	while(Test_Stop())
	{
		if(Timer_Get_DiffNow(start_time) > TIME_10S)
		{
			if(g_pump.cnt == 0)
			{
				g_test.error_state = 1;
				g_test.error_data = 0;
				temp_result = TEST_RESULT_FAIL;				
			}
			else
			{
				temp_result = TEST_RESULT_SUCCESSFUL;
			}						
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,50/portTICK_RATE_MS);
	}
	Usprintf_Test("Test_Pump End\n")
	Pump_SetVoltageControl(DISABLE);
    Pump_SetVoltage(0);
	Pump_SetState(DISABLE);
	Test_ResultHandler(TEST_CHOICE_PUMP,temp_result);
}

void Test_Gyro(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	int16_t target_angle = 900;	
	uint8_t turn_cnt = 0;
	uint32_t start_time = Timer_GetTime(),headcourse_time = Timer_GetTime();
	uint8_t is_gyro_ready = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf_Test("Test_Gyro Start\n");
	CodeShow_SetData(TEST_CHOICE_GYRO,0,0);
	Gyro_Start();
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);	
	while(Test_Stop())
	{
		if(Gyro_IsError())
		{
			g_test.error_state = 1;
			g_test.error_data = 0;				
		}

		if(Gyro_IsReady())
		{
			if(!is_gyro_ready)
			{
				is_gyro_ready = TRUE;	
				Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
				Action_SetMove(MOVE_ACT_HANDLER);															
				headcourse_time = Timer_GetTime();
			}			
		}
		if(is_gyro_ready)
		{
			switch(Action_GetMove())
			{						
				case MOVE_ACT_HEAD2COURCE:																	
					if(Action_WaitForHeadToCourseEnded()) 
					{																	
						if((Wheel_GetLeftStep() <= 800)||(Wheel_GetLeftStep() >= 2000))//1440
						{
							temp_result = TEST_RESULT_FAIL;
							g_test.error_state = 3;	
							g_test.error_data = Wheel_GetLeftStep();															
						}
					}
					if(Timer_Get_DiffNow(headcourse_time) > TIME_5S)
					{
						temp_result = TEST_RESULT_FAIL;
						g_test.error_state = 2;	
						g_test.error_data = 0;																	
					}
					if(Timer_Get_DiffNow(headcourse_time) > TIME_1S)
					{
						if(Wheel_GetLeftCurrent() > 500)
						{
							temp_result = TEST_RESULT_FAIL;
							g_test.error_state = 4;	
							g_test.error_data = Wheel_GetLeftCurrent();
						}
						if(Wheel_GetRightCurrent() > 500)
						{
							temp_result = TEST_RESULT_FAIL;
							g_test.error_state = 5;	
							g_test.error_data = Wheel_GetRightCurrent();															
						}														
					}						
					break;
				case MOVE_ACT_DECELERATE:			
					Action_WaitForDecelerateEnded();				
					break;																																	
				case MOVE_ACT_HANDLER:					
					Action_ListSwitch();
					break;				
				case MOVE_ACT_STATIC: 															
					turn_cnt++;
					if(turn_cnt > 11)
					{
						turn_cnt = 0;
						temp_result = TEST_RESULT_SUCCESSFUL;
						break;
					}
					target_angle = target_angle + 900;
					if(target_angle > 2700)
					{
						target_angle = 0;
					}
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);	
					headcourse_time = Timer_GetTime();
					break;		
				default:break;						
			}
		}

		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}			
	  	vTaskDelayUntil(&xLastWakeTime,50/portTICK_RATE_MS);
	}
	Usprintf_Test("Test_Gyro End\n");
	Gyro_End();
	Action_Static();
	Test_ResultHandler(TEST_CHOICE_GYRO,temp_result);
}

void Test_Bumper(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint32_t check_time = Timer_GetTime();
	uint8_t turn_dir = 0,trig_cnt = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("Test_Bumper Start\n");
	CodeShow_SetData(TEST_CHOICE_BUMPER,0,0);
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_FORWARD,TEST_BUMPER_FORWARD_DISTANCE,TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
	Action_SetMove(MOVE_ACT_HANDLER);
	while(Test_Stop())
	{
		switch (Action_GetMove())
		{
			case MOVE_ACT_FORWARD:
				Wheel_SetSpeed(TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
				if(Action_WaitForMoveEnded())
				{
					temp_result = TEST_RESULT_FAIL;
					g_test.error_state = 1;
				}
				if(Bumper_GetTrigStatus())
				{
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_BACK,TEST_BUMPER_BACK_DISTANCE,TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
					if(turn_dir)
					{
						Action_ListAdd(MOVE_ACT_TURN_LEFT,TEST_BUMPER_TURN_DISTANCE,TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
					}
					else
					{
						Action_ListAdd(MOVE_ACT_TURN_RIGHT,TEST_BUMPER_TURN_DISTANCE,TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
					}										
					Action_ListAdd(MOVE_ACT_FORWARD,TEST_BUMPER_FORWARD_DISTANCE,TEST_BUMPER_SPEED,TEST_BUMPER_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
					turn_dir = 1 - turn_dir;
				}
			break;
			case MOVE_ACT_TURN_LEFT:
				Action_WaitForMoveEnded();	
			break;
			case MOVE_ACT_TURN_RIGHT:
				Action_WaitForMoveEnded();	
			break;
			case MOVE_ACT_BACK:
				Action_WaitForMoveEnded();	
			break;
			case MOVE_ACT_HANDLER:
				Action_ListSwitch();		
			break;
			default:
			break;
		}
		if(Timer_IsTimeReach(&check_time,TIME_1S))
		{
			if(Bumper_GetTrigStatus())
			{
				trig_cnt++;
				if(trig_cnt >= 10)
				{
					trig_cnt = 0;
					temp_result = TEST_RESULT_FAIL;
					g_test.error_state = 2;
					g_test.error_data = Bumper_GetTrigStatus();
					Usprintf("Bumper Trig Always--->error\n");
				}
			}
			else
			{
				trig_cnt = 0;
			}			
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Usprintf("Test_Bumper end\n");
	Action_Static();
	Test_ResultHandler(TEST_CHOICE_BUMPER,temp_result);
}

void Test_Wheel(void)
{
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("Test_Wheel Start\n");
	CodeShow_SetData(TEST_CHOICE_WHEEL,0,0);
	Action_MoveForward(FORWARD_SPEED,FORWARD_SPEED,MAX_DISTANCE,MOVE_ACT_FORWARD);
	while(Test_Stop())
	{
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Usprintf("Test_Wheel End\n");
	Action_Static();
	Test_ResultHandler(TEST_CHOICE_WHEEL,temp_result);
}

void Test_Vacuum(void)
{
	uint32_t debug_time,check_time = Timer_GetTime(),start_time = Timer_GetTime();
	TestResult_t temp_result = TEST_RESULT_NONE;
	uint8_t vacuum_state = 1;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("Test_Vacuum Start\n");
	CodeShow_SetData(TEST_CHOICE_VACUUM,0,0);
	Vacuum_SetSpeed(VAC_SPEED_NORMAL);	
	while(Test_Stop())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			Usprintf("vac current:%d BLDC_PWM:%d \n",Vacuum_GetCurrent(),*g_vacuum.p_pwm);
		}

		if(Timer_IsTimeReach(&start_time,(TIME_1MIN*1)))
		{
			vacuum_state = 1 - vacuum_state;
			if(vacuum_state)
			{
				Vacuum_SetSpeed(VAC_SPEED_NORMAL);
				check_time = Timer_GetTime();
			}
			else
			{
				Vacuum_SetSpeed(0);
			}
		}

		if((Timer_Get_DiffNow(check_time) > TIME_3S) && vacuum_state)
		{
			if(Vacuum_IsFail())
			{
				Vacuum_ResetFail();
				temp_result = TEST_RESULT_FAIL;	
				g_test.error_state = 3;				
			}			
			else if(Vacuum_GetCurrent() < VACUUM_STALL_NOLOAD || Vacuum_GetCurrent() > VACUUM_STALL_LIMIT)//1000mA
			{				
				temp_result = TEST_RESULT_FAIL;
				g_test.error_state = 1;	
				g_test.error_data = Vacuum_GetCurrent();				
			}
			else if(*g_vacuum.p_pwm > 90)
			{
				temp_result = TEST_RESULT_FAIL;	
				g_test.error_state = 2;	
				g_test.error_data = *g_vacuum.p_pwm;					
			}			
		}	
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
	Usprintf("Test_Vacuum End\n");
	Vacuum_SetSpeed(0);	
	Test_ResultHandler(TEST_CHOICE_VACUUM,temp_result);
}

void Test_Speaker(void)
{
	SPK_t spk = SPK_DY;
	TestChange_t temp_change_type = TEST_CHANGE_NONE;	
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("Test_Speaker Start\n");
	CodeShow_SetData(TEST_CHOICE_SPEAKER,0,0);
	while(Test_Stop())
	{
		temp_change_type = Test_ChangeStep();
		if(temp_change_type == TEST_CHANGE_LAST)
		{
			if(spk > SPK_DY)
			{
				spk--;
				Speaker_Voice(spk);
			}
		}
		if(temp_change_type == TEST_CHANGE_NEXT)
		{
			if(spk < SPK_ENGLISH)
			{
				spk++;
				Speaker_Voice(spk);
			}			
		}
		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
	Usprintf("Test_Speaker End\n");	
	Test_ResultHandler(TEST_CHOICE_SPEAKER,temp_result);	
}

void Test_Speacial(void)
{
	uint8_t show_number = 0;
	int16_t show_data = 0;
	uint8_t showcode_enable = 0;
	uint32_t debug_time = 0;
	TestChange_t temp_change_type = TEST_CHANGE_NONE;	
	TestResult_t temp_result = TEST_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("Test_Speacial Start\n");
	CodeShow_SetData(TEST_CHOICE_SPEACAIL,0,0);
	g_cliff.state = ENABLE;
	g_obs.state = ENABLE;
	g_wall.state = ENABLE;
	g_watertank.state = ENABLE;

	while(Test_Stop())
	{
		if(Timer_IsTimeReach(&debug_time,TIME_1S))
		{
			showcode_enable = 1 - showcode_enable;
			g_code_show.enable = showcode_enable;
			Usprintf_Test("obs:(%d,%d,%d)wall(%d %d)cliff(%d %d %d) water(%d %d)\n",g_obs.adc.left,g_obs.adc.front,g_obs.adc.right,g_wall.adc.left,g_wall.adc.right,g_cliff.adc.left,g_cliff.adc.front,g_cliff.adc.right,g_watertank.adc.clean,g_watertank.adc.dirt);			
		}

		temp_change_type = Test_ChangeStep();
		if(temp_change_type == TEST_CHANGE_LAST)
		{
			if(show_number > 1)
			{
				show_number--;
			}
		}
		if(temp_change_type == TEST_CHANGE_NEXT)
		{
			if(show_number < 20)
			{
				show_number++;
			}			
		}

		switch(show_number)
		{
			case 1:
					show_data = g_obs.adc.left;	
			break;
			case 2:
					show_data = g_obs.adc.front;	
			break;
			case 3:
					show_data = g_obs.adc.right;
			break;
			case 4:
					show_data = g_wall.adc.left;
			break;
			case 5:
					show_data = g_wall.adc.right;
			break;
			case 6:
					show_data = g_cliff.adc.left;		
			break;
			case 7:
					show_data = g_cliff.adc.front;
			break;	
			case 8:
					show_data = g_cliff.adc.right;
			break;
			case 9:
					show_data = g_watertank.adc.clean;
			break;
			case 10:
					show_data = g_watertank.adc.dirt;
			break;																				
			default:break;
		}

		CodeShow_SetData(TEST_CHOICE_SPEACAIL,show_number,show_data);

		if(temp_result != TEST_RESULT_NONE)
		{
			break;	
		}		
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}	
	Usprintf("Test_Speacial End\n");	
	Test_ResultHandler(TEST_CHOICE_SPEACAIL,temp_result);	
}

uint8_t Test_Stop(void)
{
	uint8_t reval = 1;
	if(Button_CleanKeyEvent())
	{
		reval = 0;
	}
	return reval;
}

void Test_ResultHandler(uint8_t test_choice,TestResult_t result)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();		
	if(result == TEST_RESULT_FAIL)
	{
		CodeShow_SetData(test_choice,g_test.error_state,g_test.error_data);
		Display_SetCleanState(LED_MODE_STATIC,LED_STATE_RED);
		Display_SetWarnKeyState(LED_STATE_RED);
		Usprintf("function:%d fail:%d\n",test_choice,g_test.error_state);	
	}
	else
	{
		result = TEST_RESULT_SUCCESSFUL;
		Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
		Display_SetWarnKeyState(LED_STATE_OFF);
		CodeShow_SetData(0,0,8888);
		Usprintf("function :%d pass\n",test_choice);		
	}
	
	while(Mode_GetMode()==MODE_TEST)
	{
		if(Button_CleanKeyEvent())
		{
			Speaker_Voice(SPK_DY);
			break;
		}			
		vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
	}
	Button_WaitAllKeyUp();
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_OFF);
	Display_SetWarnKeyState(LED_STATE_OFF);
	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
}





















