#include "Include.h"

Power_t g_power;

void Power_Init(void)
{
	g_power.on_set_3v3_5v_state = Hw_Power_Set_3V3_5V_State;
	g_power.on_set_battery_state = Hw_Power_Set_Battery_State;
	g_power.on_set_obs_state = Hw_Power_Set_Obs_State;
	g_power.on_set_wifi_state = Hw_Power_Set_Wifi_State;
	g_power.on_set_kill_vcc_state = Hw_Power_Set_Kill_Vcc_State;
}

void Power_StartUp(void)
{
	uint16_t cycle_cnt = 600;
	while(cycle_cnt--)
	{
		g_power.on_set_3v3_5v_state(ON);
		g_power.on_set_3v3_5v_state(OFF);
	}
}


void Power_EnableAll(void)
{
	g_power.on_set_3v3_5v_state(ON);
	g_power.on_set_wifi_state(ON);
	g_power.on_set_battery_state(ON);
	g_power.on_set_obs_state(ON);
}

void Power_DisableAll(void)
{
	g_power.on_set_3v3_5v_state(OFF);
	g_power.on_set_battery_state(OFF);
	g_power.on_set_wifi_state(OFF);
	g_power.on_set_obs_state(OFF);
}

void Power_KillAllVcc(void)
{	
	g_power.on_set_kill_vcc_state(ON);
	while(1);
}
















