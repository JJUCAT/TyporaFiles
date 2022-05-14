#include "Include.h"

Hw_Display_t g_hw_display;
uint8_t g_hw_display_array[12] = {0x00,0x40,0xc0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0};
void (*on_hw_display_int)(void);

void Hw_Display_Init(void)
{
	memset(&g_hw_display,0,sizeof(g_hw_display));
    on_hw_display_int	=  Hw_Display_IntProcess;
}

void Hw_Display_ALL_On(void)
{
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data1 |= LED_WARN_KEY_W400|LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400;
		g_hw_display.data2 |= LED_WARN_WATER|LED_CLEAN_RED|LED_CLEAN_GREEN|LED_WIFI_W400;
		g_hw_display.data3 |= LED_MODE_NAV|LED_MODE_AREA|LED_MODE_SPOT|LED_MODE_WALL|LED_MODE_KEY;
	}
	else
	{
		g_hw_display.data1 |= LED_CLEAN_TIME|LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410;
		g_hw_display.data2 |= LED_WARN_KEY_W410|LED_WARN_WATER;
		g_hw_display.data3 |= LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN;	
		g_hw_display.data4 |= LED_WIFI_W410|LED_CLEAN|LED_AREA;	
	}	
	g_hw_display.brightness = 7;
}

void Hw_Display_IntProcess(void)
{
	if(g_hw_display.data_bit_idx==0)
	{
		g_hw_display.data_buffer = g_hw_display_array[g_hw_display.array_idx];
		TM1618_STB_LOW;
	}
	if(g_hw_display.data_bit_cycle == 0)
	{
		if(g_hw_display.data_buffer&0x01)
		{          
			TM1618_DIO_HIGH;
		}
		else
		{
			TM1618_DIO_LOW;
		}
		TM1618_CLK_LOW;
		g_hw_display.data_bit_idx ++;
		g_hw_display.data_bit_cycle =1;
	}
	else
	{
		TM1618_CLK_HIGH;
		g_hw_display.data_buffer >>= 1;
		if(g_hw_display.data_bit_idx > 7)
		{
			g_hw_display.data_bit_idx = 0;
			g_hw_display.array_idx++;
			if(g_hw_display.array_idx > 11)
			{
				Hw_Display_KeyIntProcessStart();
				TM1618_STB_HIGH;
				return;
			}
			if((g_hw_display.array_idx == 1) || (g_hw_display.array_idx == 2) || (g_hw_display.array_idx == 11))
			{
				TM1618_STB_HIGH;
			}
		}
		g_hw_display.data_bit_cycle = 0;
	}
}

void Hw_Display_KeyTm1618IntProcess(void)
{
    if(g_hw_display.data_bit_idx == 0)
	{
		g_hw_display.data_buffer = 0x42;
		g_hw_display.button_buffer = 0;
		TM1618_STB_LOW;
	}
	
	if(g_hw_display.data_bit_cycle == 0)
	{
		if(g_hw_display.array_idx == 0)
		{
			if(g_hw_display.data_buffer & 0x01)
			{          
				TM1618_DIO_HIGH;
			}
			else
			{
				TM1618_DIO_LOW;
			}
		}
		else
		{
			if(TM1618_DIO_IS_HIGH)g_hw_display.button_buffer = g_hw_display.button_buffer|0x80;
			g_hw_display.button_buffer>>=1;
		}
		TM1618_CLK_LOW;
		g_hw_display.data_bit_idx ++;
		g_hw_display.data_bit_cycle =1;
	}
	else
	{
		TM1618_CLK_HIGH;
		g_hw_display.data_buffer >>= 1;
		if(g_hw_display.data_bit_idx > 7)
		{
			g_hw_display.data_bit_idx = 0;
			g_hw_display.array_idx++;
			if(g_hw_display.array_idx > 1)
			{
				TM1618_STB_HIGH;
				Hw_Display_IntConfig(0);
				g_hw_display.button_final_value =  g_hw_display.button_buffer;
				return;
			}
			TM1618_DIO_INPUT;
		}
		g_hw_display.data_bit_cycle = 0;
	}
	
}


void Hw_Display_KeyIntProcessStart(void)
{
	Hw_Display_IntConfig(0);
	on_hw_display_int	=  Hw_Display_KeyTm1618IntProcess; 
	g_hw_display.button_buffer = 0;
	Hw_Display_IntConfig(1);
}

void Hw_Display_IntConfig(uint8_t state)
{
	if(state)
	{
		TM1618_STB_LOW;
		TIMER_INT_ENABLE; 
		g_hw_display.array_idx = 0;
		g_hw_display.data_bit_idx = 0;
		g_hw_display.data_bit_cycle = 0;
		TM1618_DIO_OUTPUT;
		return;
	}
	else
	{
		TIMER_INT_DISABLE;  
		g_hw_display.array_idx = 10;
		return;
	}
}

void Hw_Display_DataInit(uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4,uint8_t Light)
{
	Hw_Display_IntConfig(0);
	/* copy data to output array*/
	g_hw_display_array[3] =  data1&0xff; 
	data1 >>= 8;
	g_hw_display_array[4] =  data1&0xff;
	
	g_hw_display_array[5] =  data2&0xff;
	data2 >>= 8;
	g_hw_display_array[6] =  data2&0xff;
	
	g_hw_display_array[7] =  data3&0xff;
	data3 >>= 8;
	g_hw_display_array[8] =  data3&0xff;
	
	g_hw_display_array[9] =  data4&0xff;
	data4 >>= 8;
	g_hw_display_array[10] =  data4&0xff;
	
	g_hw_display_array[11] =  Light|0x88;
	
	on_hw_display_int	=  Hw_Display_IntProcess; 
	Hw_Display_IntConfig(1);
}

void Hw_Display_IntHandler(void)
{
	 on_hw_display_int();
} 

void Hw_Display_Process(void)
{
	Hw_Display_DataInit(g_hw_display.data1,g_hw_display.data2,g_hw_display.data3,g_hw_display.data4,g_hw_display.brightness);
}

void Hw_Display_CleanLedOff(void)
{
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data2 &= ~(LED_CLEAN_RED|LED_CLEAN_GREEN);
	}  
    if(g_robot.type == ROBOT_W410)g_hw_display.data4 &= ~LED_CLEAN;  
}

void Hw_Display_CleanLedRed(void)
{
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data2 &= ~(LED_CLEAN_RED|LED_CLEAN_GREEN);
	}  
    if(g_robot.type == ROBOT_W400)g_hw_display.data2 |= LED_CLEAN_RED;
}

void Hw_Display_CleanLedGreen(void)
{
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data2 &= ~(LED_CLEAN_RED|LED_CLEAN_GREEN);
	}  
    if(g_robot.type == ROBOT_W400)
    {
        g_hw_display.data2 |= LED_CLEAN_GREEN;
    }
    else
    {
        g_hw_display.data4 |= LED_CLEAN;
    }
}

void Hw_Display_CleanLedOrange(void)
{
	if(g_robot.type == ROBOT_W400)
	{
		g_hw_display.data2 &= ~(LED_CLEAN_RED|LED_CLEAN_GREEN);
	}  
    if(g_robot.type == ROBOT_W400)g_hw_display.data2 |= LED_CLEAN_GREEN|LED_CLEAN_RED;  
}

void Hw_Display_AreaLedOff(void)
{
    g_hw_display.data4 &= ~LED_AREA;
}

void Hw_Display_AreaLedGreen(void)
{
    g_hw_display.data4 |= LED_AREA;
}

void Hw_Display_WifiLedOff(void)
{
    if(g_robot.type == ROBOT_W400)
    {								
        g_hw_display.data2 &= ~LED_WIFI_W400;
    }
    else
    {
        g_hw_display.data4 &= ~LED_WIFI_W410;
    }    
}

void Hw_Display_WifiLedGreen(void)
{
    if(g_robot.type == ROBOT_W400)
    {
        g_hw_display.data2 |= LED_WIFI_W400;
    }
    else
    {
        g_hw_display.data4 |= LED_WIFI_W410;
    }    
}

void Hw_Display_BatteryLedOff(void)
{
	if(g_robot.type == ROBOT_W410)
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410);	
	}
	else
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400);	
	}    
}

void Hw_Display_BatteryLedRed(void)
{
	if(g_robot.type == ROBOT_W410)
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410);	
	}
	else
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400);	
	}    
    if(g_robot.type == ROBOT_W410)
    {
        g_hw_display.data1 |= LED_BATTERY_RED_W410;
    }
    else
    {
        g_hw_display.data1 |= LED_BATTERY_RED_W400;
    }    
}

void Hw_Display_BatteryLedGreen(void)
{
	if(g_robot.type == ROBOT_W410)
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410);	
	}
	else
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400);	
	}    

    if(g_robot.type == ROBOT_W410)
    {
        g_hw_display.data1 |= LED_BATTERY_GREEN_W410;
    }
    else
    {
        g_hw_display.data1 |= LED_BATTERY_GREEN_W400;
    }    
}

void Hw_Display_BatteryLedOrange(void)
{
	if(g_robot.type == ROBOT_W410)
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410);	
	}
	else
	{
		g_hw_display.data1 &= ~(LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400);	
	}    
    
    if(g_robot.type == ROBOT_W410)
    {
        g_hw_display.data1 |= LED_BATTERY_RED_W410|LED_BATTERY_GREEN_W410;	
    }
    else
    {
        g_hw_display.data1 |= LED_BATTERY_RED_W400|LED_BATTERY_GREEN_W400;
    }   
}

void Hw_Display_WaterFlowLedOff(void)
{
    g_hw_display.data3 &= ~(LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN);
}

void Hw_Display_WaterFlowLedRed(void)
{
    g_hw_display.data3 &= ~(LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN);
    g_hw_display.data3 |= LED_WATER_FLOW_RED;
}

void Hw_Display_WaterFlowLedGreen(void)
{
    g_hw_display.data3 &= ~(LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN);
    g_hw_display.data3 |= LED_WATER_FLOW_GREEN;
}

void Hw_Display_WaterFlowLedOrange(void)
{
    g_hw_display.data3 &= ~(LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN);
    g_hw_display.data3 |= LED_WATER_FLOW_RED|LED_WATER_FLOW_GREEN;
}

void Hw_Display_WarnKeyStateLedOff(void)
{
    if(g_robot.type == ROBOT_W400)
    {
        g_hw_display.data1 &= ~LED_WARN_KEY_W400;
    }
    else
    {
        g_hw_display.data2 &= ~LED_WARN_KEY_W410;
    }
}

void Hw_Display_WarnKeyStateLedRed(void)
{
    if(g_robot.type == ROBOT_W400)
    {
        g_hw_display.data1 |= LED_WARN_KEY_W400;
    }
    else
    {
        g_hw_display.data2 |= LED_WARN_KEY_W410;	
    }    
}

void Hw_Display_WaterWarnStateLedOff(void)
{
    g_hw_display.data2 &= ~LED_WARN_WATER;	  
}

void Hw_Display_WaterWarnStateLedRed(void)
{
    if(g_robot.type == ROBOT_W400)g_hw_display.data2 |= LED_WARN_WATER;
}

void Hw_Display_WaterWarnStateLedGreen(void)
{
    if(g_robot.type == ROBOT_W410)g_hw_display.data2 |= LED_WARN_WATER; 
}

void Hw_Display_ModeNavStateLedOff(void)
{
    g_hw_display.data3 &= ~LED_MODE_NAV;
}

void Hw_Display_ModeNavStateLedGreen(void)
{
    g_hw_display.data3 |= LED_MODE_NAV;
}

void Hw_Display_CleanCntStateLedOff(void)
{
    g_hw_display.data1 &= ~LED_CLEAN_TIME;		
}

void Hw_Display_CleanCntStateLedGreen(void)
{
    g_hw_display.data1 |= LED_CLEAN_TIME;
}

void Hw_Display_ModeAreaStateLedOff(void)
{
    g_hw_display.data3 &= ~LED_MODE_AREA;			
}

void Hw_Display_ModeAreaStateLedGreen(void)
{
    g_hw_display.data3 |= LED_MODE_AREA;
}

void Hw_Display_ModeSpotStateLedOff(void)
{
    g_hw_display.data3 &= ~LED_MODE_SPOT;			
}

void Hw_Display_ModeSpotStateLedGreen(void)
{
    g_hw_display.data3 |= LED_MODE_SPOT;
}

void Hw_Display_ModeWallStateLedOff(void)
{
    g_hw_display.data3 &= ~LED_MODE_WALL;		
}

void Hw_Display_ModeWallStateLedGreen(void)
{
    g_hw_display.data3 |= LED_MODE_WALL;
}

void Hw_Display_ModeKeyStateLedOff(void)
{
    g_hw_display.data3 &= ~LED_MODE_KEY;			
}

void Hw_Display_ModeKeyStateLedGreen(void)
{
    g_hw_display.data3 |= LED_MODE_KEY;
}

void Hw_Display_SetBrightness(uint8_t value)
{
	g_hw_display.brightness = value;
}

void Hw_Display_BrightnessHigh(void)
{
	g_hw_display.brightness = LED_BRIGHTNESS_HIGH_VALUE;
}















