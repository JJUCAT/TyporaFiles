#include "Include.h"

#ifdef DEBUG_ENABLE
char g_debug_fifo[DEBUG_SIZE][DEBUG_LENGTH] = {0};
uint8_t g_fifo_idx = 0;
char outString[256];
#endif

void Debug_PrintString(char *p_data)
{
#ifdef DEBUG_ENABLE	
	uint8_t fifo_ptr = 0;
	Usart_Queue_TypeDef temp_queue_uart;
	
	while(*p_data)//copy text to buffer
	{
		g_debug_fifo[g_fifo_idx][fifo_ptr] = *p_data;
		p_data++;
		fifo_ptr++;
		if(fifo_ptr >= DEBUG_LENGTH - 1)break;
	}
	
	temp_queue_uart.idx = g_fifo_idx;
	temp_queue_uart.length = fifo_ptr;

	xQueueSend(g_queue_usart, &temp_queue_uart, 1 / portTICK_RATE_MS);
	g_fifo_idx++;
	if(g_fifo_idx >= (DEBUG_SIZE))g_fifo_idx = 0;
	
	if(uxQueueMessagesWaiting(g_queue_usart) >= (DEBUG_SIZE-5))//queue buffer full,wait for usart send
	{
		vTaskDelay(2/ portTICK_RATE_MS);
	}
#endif	
}


void Debug_PrintStringByLength(char *p_data,uint8_t length)
{
#ifdef DEBUG_ENABLE	
	uint8_t i = 0;
	uint8_t fifo_ptr = 0;
	Usart_Queue_TypeDef temp_queue_uart;
	
	for(i = 0;i < length; i++)
	{
		g_debug_fifo[g_fifo_idx][fifo_ptr] = *p_data;
		p_data++;
		fifo_ptr++;
		if(fifo_ptr >= DEBUG_LENGTH - 1)break;
	}
	
	temp_queue_uart.idx = g_fifo_idx;
	temp_queue_uart.length = fifo_ptr;

	xQueueSend(g_queue_usart, &temp_queue_uart, 1 / portTICK_RATE_MS);
	g_fifo_idx++;
	if(g_fifo_idx >= (DEBUG_SIZE))g_fifo_idx = 0;
	
	if(uxQueueMessagesWaiting(g_queue_usart) >= (DEBUG_SIZE-2))//queue buffer full,wait for usart send
	{
		vTaskDelay(140 / portTICK_RATE_MS);
	}
#endif	
}

void Debug_Cycle(void)
{
	static uint32_t pre_time = 0;
	//uint8_t flash_data = 0;
	//static uint8_t flash_state = 0;
	if(Timer_IsTimeReach(&pre_time,TIME_1S))
	{	
		//Usprintf("display data 1:%x\n",g_hw_display.data1);
		//Usprintf("g_obsmsg_length:%d g_obsmsg_update:%d\n",g_obsmsg.length,g_obsmsg.update);
		//Usprintf("g_left_speed_cnt:%d g_right_speed_cnt :%d pwm:%d %d\n",g_left_speed_cnt,g_right_speed_cnt,*g_wheel.p_pwm_left,*g_wheel.p_pwm_right);
		//Usprintf("rcon status:%x\n",Rcon_GetStatus());
		//Usprintf("bumper:%d %d water:%d %d wheel:%d %d charge:%d system:%d vac:%d obs:%d battery:%d\n",g_adc_value.l_bumper,g_adc_value.r_bumper,g_adc_value.clean,g_adc_value.dirt,g_adc_value.left_wheel,g_adc_value.right_wheel,g_adc_value.charger,g_adc_value.system,g_adc_value.vac,g_adc_value.obs_wall,g_adc_value.battery);
		//Usprintf("obs:(%d,%d,%d)wall(%d %d)cliff (%d,%d,%d)water(%d,%d)bumper(%d,%d)\n",g_obs_adc.left,g_obs_adc.front,g_obs_adc.right,g_wall_adc.left,g_wall_adc.right,g_cliff_adc.left,g_cliff_adc.front,g_cliff_adc.right,g_watertank_adc.clean,g_watertank_adc.dirt,g_adc_value.l_bumper,g_adc_value.r_bumper);	
		//Usprintf("g_front_obs_trig_val:%d\n",g_front_obs_trig_val);
		//Usprintf("Battery_GetVoltage():%d level:%d\n",Battery_GetVoltage(),Hw_Battery_Voltage2Level(g_battery_voltage))
		//Usprintf("g_batterycapacity.cost:%d BATTERY_CAPACITY_MAX :%d level:%d\n",g_batterycapacity.cost,BATTERY_CAPACITY_MAX,g_batterycapacity.level);//330*66/4096/10
		//Usprintf("Wheel current:%d %d\n",Wheel_GetLeftCurrent(),Wheel_GetRightCurrent());		
		//Usprintf("gyro angle:%d gyro update:%d\n",Gyro_GetAngle(0),Gryo_GetUpdateFlag());
		//Usprintf("Charge voltage:%d Battery current:%d\n",Charge_GetVoltage(),g_batterycapacity.current);
		//Usprintf("MainBrush PWM:%d baseline:%d adc:%d\n",*g_mainbrush.p_pwm,g_mainbrush_baseline,g_adc_value.mainbrush);
		//Usprintf("sensors state:%d %d %d\n",g_obs_state,g_cliff_state,g_watertank_state);
		//Usprintf("clean:%d temp clean:%d\n",g_adc_value.clean,g_watertank_temp_cln_dirt_ad.clean);
		//Usprintf("wheel dir:%d %d %d\n",Wheel_GetDir(),Wheel_GetLeftDir(),Wheel_GetRightDir());
		//Usprintf("motors current:%d %d %d %d %d\n",g_left_wheel_current,g_right_wheel_current,g_mainbrush_current,g_vacuum_current,g_system_current);	
		//Usprintf("system :%d %d\n",g_system_baseline,g_adc_value.system);
		//Usprintf("charge voltage:%d adc:%d battery voltage:%d adc:%d\n",Charge_GetVoltage(),g_adc_value.charger,Battery_GetVoltage(),g_adc_value.battery);
		//Usprintf("cliff:%d %d %d\n",g_cliff.adc.left,g_cliff.adc.front,g_cliff.adc.right);g_cliff.state = ENABLE;
		//Usprintf("display key value:%d\n",g_hw_display.button_final_value);
		//Usprintf("rcon status:%x\n",Rcon_GetStatus());Rcon_ResetStatus();
		//Usprintf("obsmag real_fps:%d\n",g_obsmsg.real_fps);ObsMsg_SetMode(OBSMSG_ENABLE);
		//Usprintf("wall:%d %d\n",g_wall.adc.left,g_wall.adc.right);ObsMsg_SetMode(OBSMSG_ENABLE);g_obs.state = ENABLE;
		/*
		flash_state = 1 - flash_state;
		if(flash_state)
		{
			Flash_Unlock();
			if(Flash_ProgramByte(FLASH_START_ADDRESS,5) == FLASH_COMPLETE)
			{
				Usprintf("flash write data:%d\n",5);	
			}
			Flash_Lock();
		}
		else
		{
			flash_data = Flash_ReadByte(FLASH_START_ADDRESS);	
			Usprintf("flash read data:%d\n",flash_data);
		}	
		*/
	}
}











