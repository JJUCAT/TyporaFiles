#include "Include.h"
MainBrush_t g_mainbrush;

void MainBrush_Init(void)
{
	memset(&g_mainbrush,0,sizeof(g_mainbrush));
	g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;	
	g_mainbrush.p_battery_voltage = &g_battery.voltage;
	g_mainbrush.on_get_adc = Hw_MainBrush_GetAdc;
	g_mainbrush.p_pwm = Hw_MainBrush_Pwm();
}

int32_t MainBrush_GetVoltage(void)
{
	return g_mainbrush.voltage;
}

void MainBrush_SetVoltage(int32_t voltage)
{
	g_mainbrush.voltage = voltage;
}

void MainBrush_SetVoltageControl(FunctionalState state)
{
	g_mainbrush.voltage_control = state;
}

void MainBrush_StoreBaseline(void)
{
	uint8_t i = 0;
	int32_t brush_adc_sum = 0;
	for(i = 0; i < 10; i++)
	{
		brush_adc_sum += g_mainbrush.on_get_adc();
		delay(20);
	}
	g_mainbrush.baseline = brush_adc_sum / 10;
}

void MainBrush_VoltageControl(void)
{
	if(!g_mainbrush.voltage_control)return;

	int32_t temp_battery_voltage = 0, temp_brush_target_power = 0;

	temp_battery_voltage =	*g_mainbrush.p_battery_voltage;

	temp_brush_target_power = (uint8_t)(g_mainbrush.voltage/temp_battery_voltage);	 
	if(temp_brush_target_power > 100)temp_brush_target_power = 100;
	
	if(g_mainbrush.temp_voltage < temp_brush_target_power)g_mainbrush.temp_voltage++;
	else g_mainbrush.temp_voltage--;		

	if(temp_brush_target_power == 0)g_mainbrush.temp_voltage = 0;
  	if(g_mainbrush.temp_voltage < 0)g_mainbrush.temp_voltage=0;
	*g_mainbrush.p_pwm = g_mainbrush.temp_voltage;
}

uint8_t MainBrush_CheckCurrent(void) 
{
  	int16_t temp_current = 0;
	
	temp_current = MainBrush_GetCurrent();
	if(temp_current < MAINBRUSH_STALL_NOLOAD)
	{
		if(*g_mainbrush.p_pwm > 10)g_mainbrush.stall_cnt++;
	}
	else if(temp_current > MAINBRUSH_STALL_LIMIT)
	{
		g_mainbrush.stall_cnt += 4;
	}
	else
	{
		g_mainbrush.stall_cnt = 0;
	}	
	
	if(g_mainbrush.stall_cnt > 200)
	{
		Usprintf("brush Error %d\n",temp_current);
		g_mainbrush.stall_cnt = 0;
		return 1;
	}
	return 0;	
}

void MainBrush_SetPWM(uint8_t pwm)
{
  *g_mainbrush.p_pwm = pwm;
}

void MainBrush_SetCurrent(int16_t current)
{
	g_mainbrush.current = current;
}

int16_t MainBrush_GetCurrent(void)
{
	return g_mainbrush.current;
}

uint8_t MainBrush_GetSpeed(void)
{
	uint8_t reval = 0;

	if(g_mainbrush.adjust_voltage >= MAINBRUSH_BASE_VOLTAGE)
	{
		reval = 100*(g_mainbrush.adjust_voltage - MAINBRUSH_BASE_VOLTAGE)/(MAINBRUSH_SPOT_VOLTAGE - MAINBRUSH_BASE_VOLTAGE);
	}
	return reval;
}

void MainBrush_CurrentDetectionCycle(float adc2current)
{
	static uint32_t mainbrush_current_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	mainbrush_current_sum += g_mainbrush.on_get_adc();
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		mainbrush_current_sum /= 8;
		mainbrush_current_sum = (mainbrush_current_sum - g_mainbrush.baseline) * adc2current;
		g_mainbrush.current = mainbrush_current_sum;
		mainbrush_current_sum = 0;
	}
}












