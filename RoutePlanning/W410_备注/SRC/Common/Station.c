
 #include "Include.h"
Station_t g_station;


void Station_Init(void)
{
	memset(&g_station,0,sizeof(g_station));
}

void Station_SetStartState(uint8_t state)
{
    g_station.start_state = state;
}

uint8_t Station_GetStartState(void)
{
    return g_station.start_state;
}

void Station_SetMoveState(uint8_t state)
{
    g_station.move_state = state;
}

uint8_t Station_GetMoveState(void)
{
    return g_station.move_state;
}














