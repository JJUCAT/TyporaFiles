 #include "Include.h"
Mobility_t g_mobility;

void Mobility_Init(void)
{
	memset(&g_mobility,0,sizeof(g_mobility));
}

uint8_t Mobility_GetTrigCnt(void)
{
	return g_mobility.trig_cnt;
}

void Mobility_ResetTrigCnt(void)
{
	g_mobility.trig_cnt = 0;
}

void Mobility_TrigCntIncrease(void)
{
	g_mobility.trig_cnt++;
}

void Mobility_SetCheckState(FunctionalState state)
{
	g_mobility.check_state = state;
}
FunctionalState Mobility_GetCheckState(void)
{
	return g_mobility.check_state;
}
void Mobility_SetStep(uint32_t steps)
{
  g_mobility.step = steps;
}
void Mobility_ResetStep(void)
{
  g_mobility.step = 0;
}
uint32_t Mobility_GetStep(void)
{
  return g_mobility.step;
}
uint8_t Mobility_IsError(void)
{
	return g_mobility.error_flag;
}
void Mobility_SetError(void)
{
	g_mobility.error_flag = 1;
}
void Mobility_ResetError(void)
{
	g_mobility.error_flag = 0;
}























