#include "Include.h"

System_t g_system;

void System_Init(void)
{
	memset(&g_system,0,sizeof(g_system));
	g_system.on_set_power_state = Hw_System_SetPowerState;
	g_system.on_get_adc = Hw_System_GetAdc;
}

int32_t System_GetBaselineAdc(void)
{
	int32_t sys_voltage_sum = 0;
	uint8_t temp = 0;
	g_system.on_set_power_state(ON);
	delay(100);
	for(temp = 0; temp < 10; temp++)
	{
		delay(10);
		sys_voltage_sum += g_system.on_get_adc();
	}
	g_system.on_set_power_state(OFF);
	sys_voltage_sum /= 10;
	return sys_voltage_sum;
}

void System_SetBaseLineAdc(int32_t baseline)
{
	g_system.baseline = baseline;
}

void System_StoreBaselineAdc(void)
{
	g_system.baseline = System_GetBaselineAdc();
}

void System_UpdateBaselineAdc(void)
{
	g_system.baseline = System_GetBaselineAdc();
}

void System_SetCurrent(int16_t current)
{
	g_system.current = current;
}

int16_t System_GetCurrent(void)
{
	return g_system.current;
}

void System_CurrentDetectionCycle(float adc2current)
{
	static uint32_t system_current_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	system_current_sum += g_system.on_get_adc();
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		system_current_sum /= 8;
		if(system_current_sum > g_system.baseline)
		{
			system_current_sum = (uint32_t)((system_current_sum - g_system.baseline) * adc2current);
		}
		else
		{
			system_current_sum = 0;	
		}	
		System_SetCurrent(system_current_sum);
		system_current_sum = 0;
	}
}



















