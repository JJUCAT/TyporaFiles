 #include "Include.h"
Gyro_t g_gyro;
             
volatile uint8_t g_gyro_send_data_array[15] = 
											{0xaa, 0x00, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00,
											0x00, 0x00, 0x00, 0x00, 0x00};

void Gyro_Init(void)
{
	memset(&g_gyro,0,sizeof(g_gyro));
	g_gyro.on_gyro_usart_send = USART1_DMA_String;
}

void Gyro_Cal(void)
{
	if(Timer_IsTimeReach(&g_gyro.cal_time,TIME_20S))
	{
		Gyro_Calibration_Cmd(ENABLE);
		vTaskDelay(1000/portTICK_RATE_MS);//600
		Gyro_Calibration_Cmd(DISABLE);
		Gyro_ResetTime();
	}
}

uint8_t Gyro_IsNeedCalculate(void)
{
	uint8_t reval = 0;

	if(Timer_Get_DiffNow(g_gyro.cal_time) >= TIME_2MIN)
	{
		reval = 1;		
	}

	return reval;
}

void Gyro_ResetTime(void)
{
	g_gyro.cal_time = Timer_GetTime();
	Usprintf("Reset Gyro Cal Time\n");
}

uint16_t Gyro_GetAngle(uint8_t id) 
{
	return (g_gyro.angle[id] - g_gyro.offset + 3600) % 3600;
}

void Gyro_CalibrationEnableCmd(void)
{
	g_gyro_send_data_array[2] = 0x55;
	g_gyro_send_data_array[3] = 0x55;
	g_gyro_send_data_array[4] = 0x55;
	g_gyro_send_data_array[14] = 0xff;

	g_gyro.on_gyro_usart_send(15, (char *) g_gyro_send_data_array);
}

void Gyro_ResetCmd(void)
{
	g_gyro_send_data_array[2] = 0xff;
	g_gyro_send_data_array[3] = 0xff;
	g_gyro_send_data_array[4] = 0xff;
	g_gyro_send_data_array[14] = 0Xfd;

	g_gyro.on_gyro_usart_send(15, (char *) g_gyro_send_data_array);  
}

void Gyro_On_Cmd(void)
{
	g_gyro_send_data_array[2] = 0xee;
	g_gyro_send_data_array[3] = 0xee;
	g_gyro_send_data_array[4] = 0xee;
	g_gyro_send_data_array[14] = 0xca;

	g_gyro.on_gyro_usart_send(15, (char *) g_gyro_send_data_array);
}

void Gyro_ResetCalibrationValue(void) 
{
	g_gyro.calibration = 255;

	g_gyro.offset = g_gyro.angle[0];
	g_gyro.odometer = 0;
	g_gyro.time = 0;

	g_gyro.xacc_offset = g_gyro.xacc;
	g_gyro.yacc_offset = g_gyro.yacc;
	g_gyro.zacc_offset = g_gyro.zacc;
}

uint8_t Gryo_GetUpdateFlag(void)
{
	return g_gyro.update_flag;
}

uint8_t Gyro_IsUpdated(void) 
{
	if (g_gyro.update_flag) 
	{
		g_gyro.update_flag = 0;
		return 1;
	}
	return 0;
}
void Gyro_ResetUpdateFlag(void)
{
	g_gyro.update_flag = 0;
}


void Gyro_Cmd(FunctionalState state) 
{
	if (state != DISABLE)
	{
		Gyro_On_Cmd();
		Gyro_ResetCalibrationValue();
		g_gyro.xacc_offset = g_gyro.yacc_offset = g_gyro.zacc_offset = SHRT_MAX;
	} 
	else 
	{
		Gyro_ResetCmd();
		g_gyro.xacc_offset = g_gyro.yacc_offset = g_gyro.zacc_offset = SHRT_MAX;
	}
	Gyro_ResetUpdateFlag();
	g_gyro.is_ready = 0;
}

void Gyro_Calibration_Cmd(FunctionalState state)
{
#ifdef GYRO_CALIBRATION
	if (state == ENABLE) 
	{
		g_gyro_send_data_array[2] = 0x55;
		g_gyro_send_data_array[3] = 0x55;
		g_gyro_send_data_array[4] = 0x55;
		g_gyro_send_data_array[14] = 0xff;
	} 
	else 
	{
		g_gyro_send_data_array[2] = 0x44;
		g_gyro_send_data_array[3] = 0x44;
		g_gyro_send_data_array[4] = 0x44;
		g_gyro_send_data_array[14] = 0xcc;
	}
	g_gyro.on_gyro_usart_send(15, (char *) g_gyro_send_data_array);
#endif
}

void Gyro_SetOffset(int16_t offset) 
{
	g_gyro.offset = (3600 + offset + g_gyro.offset) % 3600;
	Usprintf("Gyro Offset:%d\n",g_gyro.offset);
}

int16_t Gyro_GetOffset(void) 
{
	return g_gyro.offset;
}

void Gyro_SetAngle(int16_t theta) 
{
	g_gyro.offset = (g_gyro.angle[0] + 3600 - theta) % 3600;
}

void Gyro_ResetWithOffset(int16_t offset) 
{
	g_gyro.calibration = 255;

	g_gyro.offset = offset;
	g_gyro.odometer = 0;
	g_gyro.time = 0;

	g_gyro.xacc_offset = g_gyro.xacc;
	g_gyro.yacc_offset = g_gyro.yacc;
	g_gyro.zacc_offset = g_gyro.zacc;
}

uint8_t Gyro_GetCalibration(void) 
{
	return g_gyro.calibration;
}

void Gyro_Reset_With_Offset(int16_t offset)
{
	g_gyro.calibration = 255;
	g_gyro.offset = offset;
	g_gyro.odometer = 0;
	g_gyro.time = 0;

	g_gyro.xacc_offset = g_gyro.xacc;
	g_gyro.yacc_offset = g_gyro.yacc;
	g_gyro.zacc_offset = g_gyro.zacc;

	Usprintf("reset Gyro Offset:%d\n",g_gyro.offset);
}

uint8_t Gyro_ParseMsg(volatile uint8_t * msg)
{
	uint8_t c, checksum = 0;
	int16_t i;
	int32_t l;

	++g_gyro.time;

	for (c = 0; c < 12; ++c)
	{
		checksum += *(msg + c);
	}

	if (checksum != *(msg + 12))
	{
		return 0;
	}
	else
	{
		g_gyro.idx = *msg;
		g_gyro.rate[1] = g_gyro.rate[0];
		g_gyro.angle[1] = g_gyro.angle[0];
		g_gyro.rate[0] = (int16_t)((*(msg + 3) & 0xFF) | ((*(msg + 4) << 8) & 0xFF00)) * -1;

		i = (*(msg + 1) & 0xFF) | ((*(msg + 2) << 8) & 0xFF00);  //[3:4]   = Angle
		g_gyro.raw = i;
		l = (int32_t)i * -1;
		l = (l + 36000 + 5) % 36000;
		l /= 10;
		g_gyro.angle[0] = (uint16_t)l;
		g_gyro.odometer += g_gyro.angle[0] - g_gyro.angle[1];

		g_gyro.yacc = (*(msg + 5) & 0xFF) | ((*(msg + 6) << 8) & 0xFF00);  //[7:8]   = X Acceleration
		g_gyro.xacc = (*(msg + 7) & 0xFF) | ((*(msg + 8) << 8) & 0xFF00);  //[9:10]  = Y Acceleration
		g_gyro.xacc *= -1;
		g_gyro.zacc = (*(msg + 9) & 0xFF) | ((*(msg + 10) << 8) & 0xFF00);  //[11:12] = Z Acceleration
		g_gyro.zacc *= -1;

		g_gyro.calibration = (*(msg + 11) & 0xFF);

		if(g_gyro.xacc_offset == SHRT_MAX)
		{
			g_gyro.xacc_offset = g_gyro.xacc;
		}
		if(g_gyro.yacc_offset == SHRT_MAX)
		{
			g_gyro.yacc_offset = g_gyro.yacc;
		}
		if (g_gyro.zacc_offset == SHRT_MAX)
		{
			g_gyro.zacc_offset = g_gyro.zacc;
		}
		
		g_gyro.update_flag = 1;
		return 1;
	}
}

uint8_t Gyro_IsReady(void)
{
	return g_gyro.is_ready;
}

uint8_t Gyro_IsError(void)
{
	return g_gyro.is_error;
}


void Gyro_Start(void)
{
	g_gyro.start_up = GYRO_STARTUP_START;	
}

void Gyro_End(void)
{
	g_gyro.start_up = GYRO_STARTUP_END;	
}

void Gyro_StartUpProcess(void)
{
	static uint32_t time = 0;
	switch(g_gyro.start_up)
	{
		case GYRO_STARTUP_NONE:

		break;
		case GYRO_STARTUP_START:
			g_gyro.try_cnt = 0;	
			g_gyro.is_ready = FALSE;
			g_gyro.is_error = FALSE;	
			g_gyro.start_up = GYRO_STARTUP_DISABLE;
		break;
		case GYRO_STARTUP_DISABLE:
			Gyro_Cmd(DISABLE);																	
			g_gyro.start_up = GYRO_STARTUP_ENABLE;
			time = Timer_GetTime();
			Usprintf("GYRO_STARTUP_DISABLE\n");			
		break;
		case GYRO_STARTUP_ENABLE:
			if(Timer_Get_DiffNow(time) > TIME_300MS)
			{
				Usprintf("GYRO_STARTUP_ENABLE\n");	
				Gyro_Cmd(ENABLE);	
				g_gyro.start_up = GYRO_STARTUP_WAIT_DATA;	
				time = Timer_GetTime();	
			}
		break;
		case GYRO_STARTUP_WAIT_DATA:
			if(Gryo_GetUpdateFlag())
			{	
				Usprintf("GYRO_STARTUP_WAIT_DATA\n");					
				g_gyro.start_up = GYRO_STARTUP_READY;
			}
			else
			{
				if(Timer_Get_DiffNow(time) > TIME_500MS)
				{
					g_gyro.start_up = GYRO_STARTUP_RESTART;									
				}									
			}				
		break;
		case GYRO_STARTUP_RESTART:
			g_gyro.try_cnt++;
			if(g_gyro.try_cnt >= 2)
			{
				g_gyro.try_cnt = 0;
				g_gyro.is_error = TRUE;	
				g_gyro.start_up = GYRO_STARTUP_NONE;	
				Usprintf("GYRO_STARTUP_NONE\n");						
			}
			else
			{
				g_gyro.start_up = GYRO_STARTUP_DISABLE;	
			}			
		break;
		case GYRO_STARTUP_READY:
			if(Timer_Get_DiffNow(time) >= TIME_3S)
			{
				if(Gyro_GetAngle(0) < 30 || Gyro_GetAngle(0) > 3570)
				{
					Gyro_ResetTime();	
					Gyro_ResetWithOffset(0);
					g_gyro.is_ready = TRUE;	
					g_gyro.start_up = GYRO_STARTUP_NONE;	
					Usprintf("GYRO_STARTUP_READY\n");
				}	
				else
				{
					g_gyro.start_up = GYRO_STARTUP_DISABLE;
				}
			}			
		break;	
		case GYRO_STARTUP_END:
			Gyro_Cmd(DISABLE);																	
			g_gyro.start_up = GYRO_STARTUP_NONE;
			Usprintf("GYRO_STARTUP_END\n");			
		break;		
		default:break;
	}	

}













