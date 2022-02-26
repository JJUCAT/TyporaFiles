/*
 * @Author: your name
 * @Date: 2020-06-29 11:39:32
 * @LastEditTime: 2020-08-08 16:46:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: \工程调试\SRC\TouchPad.c
 */ 

#include "Include.h"

Button_t g_button;

void Button_Init(void)
{
	g_button.on_clean_trig = Hw_Button_CleanTrig;
	g_button.on_key_1_trig = Hw_Button_LeftKeyTrig;
	g_button.on_key_2_trig = Hw_Button_RightKeyTrig;
}

uint8_t Button_GetCleanStatus(void)
{
	if(g_button.on_clean_trig())
	{
		return 1;
	}
	return 0;
}

uint8_t Button_CleanKeyEvent(void)
{
	static uint8_t key_state = 0;
 	uint8_t reval = 0;

	if(Button_GetCleanStatus())
	{
		if(!key_state)
		{
			reval = 1;
		}
		key_state=1;
	}
	else
	{
		key_state=0;
	}
	return reval;
}
uint8_t Button_CleanKeyEventTime(void)
{
 	uint8_t reval = 0;

	if(Button_CleanEventTime() >= TIME_50MS)
	{	
		reval = 1;
	}
	
	return reval;
}
uint32_t Button_CleanEventTime(void)
{
	static uint8_t key_state = 0;
	static uint32_t key_start_time = 0;
 	uint32_t reval = 0;

	if(Button_GetCleanStatus())
	{
		if(!key_state)
		{
			key_start_time = Timer_GetTime();			
		}
		key_state=1;
	}
	else
	{				
		key_state=0;
	}
	if(key_state)
	{
		reval = Timer_Get_DiffNow(key_start_time);		
	}
	return reval;
}

uint8_t Button_KeysEvent(void)
{
	static uint32_t key_start_time = 0;	
	static ButtonValue_t button_value;
	static uint8_t key_flag = 1;
	uint8_t reval = 0;

	if(g_button.on_key_1_trig()||g_button.on_key_2_trig())
	{
		if(g_button.on_key_1_trig() && key_flag)
		{
			key_flag = 0;
			button_value = BUTTON_VALUE_ONE;
			if(button_value == BUTTON_WIFI_CHOICE_VALUE)key_start_time = Timer_GetTime();	
		}
		if(g_button.on_key_2_trig() && key_flag)
		{		
			key_flag = 0;
			button_value = BUTTON_VALUE_TWO;
			if(button_value == BUTTON_WIFI_CHOICE_VALUE)key_start_time = Timer_GetTime();			
		}
	}
	else
	{
		key_flag = 1;
		if(button_value)
		{
			reval = button_value;			
			button_value = BUTTON_VALUE_NONE;
		}
	}

	if(button_value == BUTTON_WIFI_CHOICE_VALUE)
	{
		if(Timer_Get_DiffNow(key_start_time) >= TIME_3S)
		{
			button_value = BUTTON_VALUE_NONE;
			reval =  BUTTON_VALUE_WIFI;
		}					
	}
	return reval;
}

void Button_WaitCleanUp(void)
{
	uint16_t key_break_cnt = 0;	
	while(Button_GetCleanStatus())
	{
		key_break_cnt++;
		if(key_break_cnt >= 1000)break;
		vTaskDelay(20/portTICK_RATE_MS);
	}		
}

void Button_WaitAllKeyUp(void)
{	
	uint16_t key_break_cnt = 0;	
	while(g_button.on_clean_trig()||g_button.on_key_1_trig()||g_button.on_key_2_trig())
	{
		key_break_cnt++;
		if(key_break_cnt >= 1000)break;
		vTaskDelay(20/portTICK_RATE_MS);
	}	
}

uint8_t Button_StartUp(void)
{
	uint8_t reval = 0;
	uint32_t button_time = 0;
	uint8_t button_start = 0;
	while(Button_GetCleanStatus())
	{	
		button_time = Button_CleanEventTime();
		if(button_time >= TIME_500MS)
		{
			if(!button_start)
			{
				button_start = 1;
				Speaker_Voice(SPK_DY);
				reval = 1;
			}
		}
	}
	return reval;
}




