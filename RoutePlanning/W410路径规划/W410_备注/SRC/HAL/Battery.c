#include "Include.h"

Battery_t g_battery;
	
void Battery_Init(void)
{
	memset(&g_battery,0,sizeof(g_battery));	
	g_battery.on_get_adc = Hw_Battery_GetAdc;
	g_battery.on_get_level = Hw_Battery_Voltage2Level;	
}


void Battery_CapacityInit(uint16_t baseline)
{
	uint8_t level = 0;
	level = g_battery.on_get_level(g_battery.voltage);
	g_battery.capacity.base = baseline;
	g_battery.capacity.level = level;
	g_battery.capacity.fading_offset = 0;	
	g_battery.capacity.cost = (100 - level)*BATTERY_CAPACITY_MAX/100;
	Usprintf("battery init level:%d\n",level);	
}

void Battery_ResetPreLevel(void)
{
	g_battery.pre_level = 0;
}

uint8_t Battery_IsLow(void)
{	
	if(g_battery.voltage < BATTERY_LOW_VOLTAGE)return 1;
	if(g_battery.capacity.level < BATTERY_LEVEL_LOW)return 1;
	return 0;
}

uint8_t Battery_IsWeak(void)
{
	if(g_battery.voltage <= BATTERY_WEAK_VOLTAGE)return 1;
	if(g_battery.on_get_level(g_battery.voltage) < BATTERY_LEVEL_RED)return 1;
	return 0;	
}

void Battery_ResetCapacity(void)
{
	g_battery.capacity.cost = 0;
}

void Battery_AddCapacityCurrent(uint16_t sys_current_adc)
{
	g_battery.capacity.temporary += sys_current_adc;
	
	if (sys_current_adc < g_battery.capacity.min)g_battery.capacity.min = sys_current_adc;
	
	if (sys_current_adc > g_battery.capacity.max)g_battery.capacity.max = sys_current_adc;
	
	g_battery.capacity.cnt++;
}

uint8_t Battery_IsChargeLow(void)
{	
	if(g_battery.capacity.level <= 20)return 1;
	return 0;
}

void Battery_SetVoltage(uint16_t voltage)
{
	g_battery.voltage = voltage;
}

uint16_t Battery_GetVoltage(void)
{	 	
	return g_battery.voltage;
}

uint16_t Battery_GetReChargeVoltage(void)
{	 	
	return g_battery.recharge_voltage;
}

void Battery_CapacityCalc(uint8_t is_charging,float adc_2_current)
{
	int32_t current_cnt,temp_current;

	current_cnt = g_battery.capacity.cnt;
	temp_current = (g_battery.capacity.temporary - (g_battery.capacity.min + g_battery.capacity.max))/(current_cnt - 2);
	
	g_battery.capacity.cnt = 0;
	g_battery.capacity.temporary = 0;
	g_battery.capacity.min = 4096;
	g_battery.capacity.max = 0;
	
	if(is_charging)
	{
		temp_current = (g_battery.capacity.base - temp_current)*adc_2_current;
		if (temp_current < 0)temp_current = 0;
		g_battery.capacity.current = temp_current;
		g_battery.capacity.cost -= (temp_current*current_cnt)/1000;
		if (g_battery.capacity.cost < 0)g_battery.capacity.cost = 0;		
	}
	else
	{
		temp_current = (temp_current - g_battery.capacity.base)*adc_2_current;
		if (temp_current < 0)temp_current = 0;
		g_battery.capacity.current = temp_current;		
		g_battery.capacity.cost += (temp_current*current_cnt)/1000;
	}
	
	if (g_battery.capacity.cost > BATTERY_CAPACITY_MAX)g_battery.capacity.cost = BATTERY_CAPACITY_MAX;		
	g_battery.capacity.level = (BATTERY_CAPACITY_MAX - g_battery.capacity.cost)*100/(BATTERY_CAPACITY_MAX + 1);//percent 1--100
	if(g_battery.capacity.cost == 0)g_battery.capacity.level  = 100;
	if (g_battery.capacity.level > 100)g_battery.capacity.level  = 100;
}

void Battery_AddCapacityCurrentCycle(uint16_t system_current_adc)
{
	g_battery.capacity_cnt++;
	if(g_battery.capacity_cnt>=20)
	{
		g_battery.capacity_cnt = 0;
		Battery_AddCapacityCurrent(system_current_adc);	
	}
}

void Battery_VolatgeDetectionCycle(float adc2volatge)
{
	static uint32_t battery_voltage_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	battery_voltage_sum += g_battery.on_get_adc();
	g_battery.recharge_voltage = g_battery.on_get_adc()* adc2volatge;
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		battery_voltage_sum /= 8;
		battery_voltage_sum = (uint32_t)(battery_voltage_sum * adc2volatge);
		Battery_SetVoltage(battery_voltage_sum);
		battery_voltage_sum = 0;
	}
}












