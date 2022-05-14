#include "Include.h"

Pump_t g_pump;

void Pump_Init(void)
{
	memset(&g_pump,0,sizeof(g_pump));
	g_pump.set_state = Hw_Pump_SetState;
	g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;
	g_pump.pre_cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;
	g_pump.p_battery_voltage = &g_battery.voltage;
	g_pump.p_pwm = Hw_Pump_Pwm();
}

void Pump_SetVoltage(int32_t voltage)
{
	g_pump.voltage = voltage;
}

void Pump_SetVoltageControl(FunctionalState state)
{
	g_pump.voltage_control = state;
}

void Pump_SetStartControl(FunctionalState state)
{
	g_pump.start_control = state;
}

void Pump_Process(void)
{
	static uint16_t pump_time_counter = 0,max_t = 0;
	uint16_t pump_on_t = 85;	
	if(g_pump.state == ENABLE)
	{		
		if(g_pump.max)
		{		
			max_t++;
			if(max_t >= 60000)//6s
			{
				max_t = 0;
				g_pump.max = 0;
			}
		}
		else
		{
			pump_on_t = 85;
		}		
		
		pump_time_counter++;			
		if(pump_time_counter >= g_pump.cycle_cnt)
		{
			pump_time_counter = 0;
			g_pump.set_state(ON);
			return;
		}
		if(pump_time_counter == pump_on_t)
		{
			g_pump.set_state(OFF);			
		}
	}
	else
	{
		pump_time_counter=0;
		g_pump.set_state(OFF);		
	}
}

void Pump_VoltageControl(void)
{
	if(!g_pump.voltage_control)return;

	int32_t temp_battery_voltage = 0, temp_pump_target_power = 0;
	temp_battery_voltage =	*g_pump.p_battery_voltage;

	temp_pump_target_power =(uint8_t)(g_pump.voltage/temp_battery_voltage);
	if(temp_pump_target_power > 100)temp_pump_target_power = 100;
	
	if(g_pump.temp_power < temp_pump_target_power)g_pump.temp_power++;
	else g_pump.temp_power--;	
	
  	if(temp_pump_target_power == 0)g_pump.temp_power = 0;
	if(g_pump.temp_power < 0)g_pump.temp_power=0;
	if(g_pump.max)g_pump.temp_power = 100;
	*g_pump.p_pwm = g_pump.temp_power;	
}

uint8_t Pump_GetSpeed(void)
{
	uint8_t reval = 0;
	if(g_pump.cycle_cnt >= PUMP_WATER_FLOW_LEVEL_HIGH)
	{
		reval = (PUMP_WATER_FLOW_LEVEL_lOW - g_pump.cycle_cnt)/10;
	}
	return reval;
}

void Pump_SetState(FunctionalState state)
{
	g_pump.state = state;
}

FunctionalState Pump_GetState(void)
{
	return g_pump.state;
}










