#include "Include.h"

Hc4051_t g_hc4051;

void Hc4051_Init(void)
{
	memset(&g_hc4051,0,sizeof(g_hc4051));	
	g_hc4051.on_set_a0_state = Hw_Hc4051_SetChannelA0State;
	g_hc4051.on_set_a1_state = Hw_Hc4051_SetChannelA1State;
	g_hc4051.on_set_a2_state = Hw_Hc4051_SetChannelA2State;
}


void Hc4051_SetChannel(void) 
{	
	switch(g_hc4051.channel)
	{
		case 0:g_hc4051.on_set_a2_state(OFF);g_hc4051.on_set_a1_state(OFF);g_hc4051.on_set_a0_state(OFF);			
			break;		
		case 1:g_hc4051.on_set_a2_state(OFF);g_hc4051.on_set_a1_state(OFF);g_hc4051.on_set_a0_state(ON);	
			break;		
		case 2:g_hc4051.on_set_a2_state(OFF);g_hc4051.on_set_a1_state(ON);g_hc4051.on_set_a0_state(OFF);	
			break;
		case 3:g_hc4051.on_set_a2_state(OFF);g_hc4051.on_set_a1_state(ON);g_hc4051.on_set_a0_state(ON);	
			break;
		case 4:g_hc4051.on_set_a2_state(ON);g_hc4051.on_set_a1_state(OFF);g_hc4051.on_set_a0_state(OFF);	
			break;
		case 5:g_hc4051.on_set_a2_state(ON);g_hc4051.on_set_a1_state(OFF);g_hc4051.on_set_a0_state(ON);	
			break;
		case 6:g_hc4051.on_set_a2_state(ON);g_hc4051.on_set_a1_state(ON);g_hc4051.on_set_a0_state(OFF);	
			break;
		case 7:g_hc4051.on_set_a2_state(ON);g_hc4051.on_set_a1_state(ON);g_hc4051.on_set_a0_state(ON);	
			break;		
		default:g_hc4051.on_set_a2_state(OFF);g_hc4051.on_set_a1_state(OFF);g_hc4051.on_set_a0_state(OFF);			
			break;
	}
}

uint8_t Hc4051_GetChannel(void) 
{
	return g_hc4051.channel;
}

uint8_t Hc4051_ChannelCycle(void) 
{
	g_hc4051.channel++;
	if(g_hc4051.channel>= 8)
	{
		g_hc4051.channel = 0;		
		return 1;
	}		
	return 0;
}

void Hc4051_ResetChannel(void) 
{
	g_hc4051.channel = 0;
}










