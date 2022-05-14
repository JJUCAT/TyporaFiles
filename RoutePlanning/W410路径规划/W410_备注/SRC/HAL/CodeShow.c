#include "Include.h"

CodeShow_t g_code_show;


void CodeShow_Init(void)
{
    memset(&g_code_show,0,sizeof(g_code_show));
	g_code_show.on_codeshow_usart_send = USART2_DMA_String;	
	g_code_show.on_gpio_config = Hw_CodeShow_GpioConfig;
	g_code_show.on_timer_config = Hw_CodeShow_TimerConfig;
}

void CodeShow_SetState(FunctionalState state)
{
	g_code_show.enable = state;
}

void CodeShow_LoadData(volatile uint8_t *buffer,uint8_t step,uint16_t data,uint8_t code)
{
	uint8_t i = 0,sum = 0 ;
	uint8_t tx_idx = 0;
	uint32_t sensor_data=0;
	
	buffer[tx_idx++] = 0XAA;
	buffer[tx_idx++] = 0x55;
	buffer[tx_idx++] = 0x0D;
	
	for(i = 0;i<3;i++)
	{
	 buffer[tx_idx++]= 0xCC; 
	}
	buffer[tx_idx++]=step;
	
	sensor_data=(uint32_t)data;
	
	for(i=0;i<2;i++)
	{
		buffer[tx_idx++]=sensor_data&0xFF;
		sensor_data=sensor_data>>8;
	}			
	buffer[tx_idx++]=code;
	
	for(i = 0;i<4;i++)
		buffer[tx_idx++] = 0x00; 

	for(i=2;i<tx_idx;i++)
		sum += buffer[i];

	buffer[tx_idx] = sum;
}

void CodeShow_SetData(uint8_t step,uint16_t code,uint16_t data)
{
	g_code_show.step = step;
	g_code_show.code = code;
	g_code_show.data = data;
}

void CodeShow_ShowData(uint8_t step,uint16_t code,uint16_t data)
{
    CodeShow_LoadData(g_code_show.buffer,step,data,code); 
    g_code_show.on_codeshow_usart_send(20,(char *)g_code_show.buffer);	
}

uint32_t CodeShow_DataConversion(uint8_t step,uint8_t dir,uint8_t error,uint8_t error_data,uint16_t display_data)
{
	uint32_t temp1=0;	
	uint32_t temp2=0;
	
	temp2 = step;
	if(temp2<64)
	{
		temp1 += temp2<<25;
	}
	
	temp2 = dir;
	if(temp2==1)
	{
		temp1 += temp2<<24;
	}
	
	temp2 = error;
	if(temp2==1)
	{
		temp1 += temp2<<23;
	}
	
	temp2 = error_data;
	if(temp2<128)
	{
		temp1 += temp2<<16;
	}
	
	temp2 = display_data;
	if(temp2<65536)
	{
		temp1 += temp2;
	}
	
	temp2 = display_data;
	if((temp2&0x01)==0x01)
	{
		temp2 = 1;
		temp1 += temp2<<31;
	}	
	
	return temp1;
}

void CodeShow_Cycle(void)
{
	#ifndef CODESHOW_ENABLE
	return;	
	#endif
	static uint32_t send_time;
    if(!g_code_show.enable)return;
	
	if(g_robot.type == ROBOT_W400)
	{
		if(Timer_IsTimeReach(&send_time,TIME_1S))//TIME_200MS TIME_1S
		{	
			if(g_obsmsg.cur_mode == OBSMSG_ENABLE)	
			{
				CodeShow_ShowData(g_code_show.step,g_code_show.code,g_code_show.data);
			}				
		}
	}
	else if(g_robot.type == ROBOT_W410)
	{
		switch(g_code_show.state)
		{
			case CODESHOW_ILDE:
								if(Timer_IsTimeReach(&send_time,TIME_1S))
								{
									g_code_show.is_working = 1;
									g_code_show.state = CODESHOW_INIT_START;						
								}														
			break;

			case CODESHOW_INIT_START:	
								g_code_show.on_gpio_config(TRUE);								
								g_code_show.delay_time = Timer_GetTime();
								g_code_show.state = CODESHOW_INIT_DELAY;
								if((g_code_show.code == 0)&&(g_code_show.data == 0))
								{
									g_code_show.send_data = CodeShow_DataConversion(g_code_show.step,0,0,g_code_show.code,g_code_show.data);
								}
								else if(g_code_show.data == 8888)
								{
									g_code_show.send_data = CodeShow_DataConversion(g_code_show.step,0,0,g_code_show.code,g_code_show.data);
								}
								else
								{
									g_code_show.send_data = CodeShow_DataConversion(g_code_show.step,0,1,g_code_show.code,g_code_show.data);
								}													
			break;
			case CODESHOW_INIT_DELAY:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_INIT_DELAY_T)
								{
									g_code_show.state = CODESHOW_SEND_START_INIT;	
								}
			break;
			case CODESHOW_SEND_START_INIT:
								g_code_show.on_timer_config(ON);
								g_code_show.delay_time = Timer_GetTime();
								g_code_show.state = CODESHOW_SEND_START_WAIT;
			break;
			case CODESHOW_SEND_START_WAIT:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_START_INIT_T)
								{
									g_code_show.state = CODESHOW_SEND_START_END;	
									g_code_show.delay_time = Timer_GetTime();
									g_code_show.on_timer_config(OFF);
								}							
			break;
			case CODESHOW_SEND_START_END:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_START_END_T)
								{								
									g_code_show.send_data_cnt = 32;
									if((g_code_show.send_data&0x80000000) == 0x80000000)
									{
										g_code_show.state = CODESHOW_SEND_DATA_H_START;	
									}
									else
									{
										g_code_show.state = CODESHOW_SEND_DATA_L_START;	
									}															
								}									
			break;
			case CODESHOW_SEND_DATA_H_START:
								g_code_show.on_timer_config(ON);	
								g_code_show.delay_time = Timer_GetTime();
								g_code_show.state = CODESHOW_SEND_DATA_H_WAIT;						
			break;

			case CODESHOW_SEND_DATA_H_WAIT:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_H_BIT_INIT_T)
								{
									g_code_show.state = CODESHOW_SEND_DATA_H_END;	
									g_code_show.delay_time = Timer_GetTime();
									g_code_show.on_timer_config(OFF);	
								}
			break;
			case CODESHOW_SEND_DATA_H_END:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_H_BIT_END_T)
								{
									g_code_show.send_data = g_code_show.send_data<<1;
									g_code_show.send_data_cnt--;
									if(g_code_show.send_data_cnt)
									{
										if((g_code_show.send_data&0x80000000) == 0x80000000)
										{
											g_code_show.state = CODESHOW_SEND_DATA_H_START;	
										}
										else
										{
											g_code_show.state = CODESHOW_SEND_DATA_L_START;	
										}									
									}
									else
									{
										g_code_show.state = CODESHOW_END;
									}									
								}								
			break;
			case CODESHOW_SEND_DATA_L_START:
								g_code_show.on_timer_config(ON);	
								g_code_show.delay_time = Timer_GetTime();
								g_code_show.state = CODESHOW_SEND_DATA_L_WAIT;						
			break;

			case CODESHOW_SEND_DATA_L_WAIT:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_L_BIT_INIT_T)
								{
									g_code_show.state = CODESHOW_SEND_DATA_L_END;	
									g_code_show.delay_time = Timer_GetTime();
									g_code_show.on_timer_config(OFF);	
								}
			break;
			case CODESHOW_SEND_DATA_L_END:
								if(Timer_Get_DiffNow(g_code_show.delay_time) >= CODESHOW_L_BIT_END_T)
								{
									g_code_show.send_data = g_code_show.send_data<<1;
									g_code_show.send_data_cnt--;
									if(g_code_show.send_data_cnt)
									{
										if((g_code_show.send_data&0x80000000) == 0x80000000)
										{
											g_code_show.state = CODESHOW_SEND_DATA_H_START;
										}
										else
										{
											g_code_show.state = CODESHOW_SEND_DATA_L_START;	
										}									
									}
									else
									{
										g_code_show.state = CODESHOW_END;
									}									
								}								
			break;		
			case CODESHOW_END:
								g_code_show.on_gpio_config(FALSE);
								g_code_show.on_timer_config(OFF);							
								g_code_show.state = CODESHOW_ILDE;
								g_code_show.is_working = 0;
			break;
			default:break;
		}		
	}
}

























