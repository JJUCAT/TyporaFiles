#include "Include.h"

Charge_t g_charge;

void Charge_Init(void)
{
	memset(&g_charge,0,sizeof(g_charge));
	g_charge.p_pwm = Hw_Charge_Pwm();
	g_charge.on_get_adc = Hw_Charge_GetAdc;
	g_charge.on_gpio_config = Hw_Charge_GpioConfig;	
	g_charge.on_timer_config = Hw_Charge_TimerConfig;
	g_charge.on_power_config = Hw_Charge_PowerConfig;
}

void Charge_SetFromSleep(uint8_t sleep)
{
    g_charge.from_sleep = sleep;
}

uint8_t Charge_IsFromSleep(void)
{
    return g_charge.from_sleep;
}

void Charge_SetMovStation(uint8_t state)
{
    g_charge.is_mov_station = state;
}

uint8_t Charge_IsMovStation(void)
{
    return g_charge.is_mov_station;
}

void Charge_SetCharging(uint8_t charging)
{
    g_charge.charging = charging;
}

uint8_t Charge_IsCharging(void)
{
    return g_charge.charging;
}

void Charge_SetVoltage(uint16_t voltage)
{
	g_charge.voltage = voltage;
}

uint16_t Charge_GetVoltage(void)
{
	return g_charge.voltage;
}

ChargerMode_t Charge_GetMode(void)
{
	return g_charge.mode;
}

void Charge_SetMode(ChargerMode_t mode)
{
	g_charge.mode = mode;
}

void Charge_VolatgeDetectionCycle(float adc2volatge)
{
	static uint32_t charge_voltage_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	charge_voltage_sum += g_charge.on_get_adc();
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		charge_voltage_sum /= 8;
		charge_voltage_sum = (uint32_t)(charge_voltage_sum * adc2volatge);
		Charge_SetVoltage(charge_voltage_sum);
		charge_voltage_sum = 0;
	}
}

uint8_t Charge_IsAdapterInput(void)
{
	
	if(g_charge.voltage >= CHARGE_INPUT_VOLTAGE)
	{
		return 1;
	}
	return 0;
}


void Charge_ConfigurationIn(void)
{
	g_charge.on_gpio_config(TRUE);
	g_charge.on_timer_config(TRUE);
	//g_charge.on_power_config(FALSE);
}

void Charge_ConfigurationOut(void)
{
    g_charge.on_gpio_config(FALSE);
    g_charge.on_timer_config(FALSE);
    g_charge.on_power_config(TRUE);
}











