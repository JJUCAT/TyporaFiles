#include "Include.h"

CleanCycleCount_t g_cleancycle_count;

void CleanCycleCount_Init(void)
{
	g_cleancycle_count.target_cnt = g_cleancycle_count.set_cnt = g_cleancycle_count.pre_set_cnt = 1;
	g_cleancycle_count.cur_cnt = 1;
	g_cleancycle_count.over_cnt = 0;
}

void CleanCycleCount_Start(void)
{
	g_cleancycle_count.target_cnt = g_cleancycle_count.set_cnt;
	g_cleancycle_count.cur_cnt = 1;
	g_cleancycle_count.over_cnt = 0;
	Usprintf("cycle cnt start target cnt:%d \n",g_cleancycle_count.target_cnt);	
}




uint8_t CleanCycleCount_IsArriveTargetCnt(void)
{
	g_cleancycle_count.over_cnt++;
	if(g_cleancycle_count.cur_cnt < g_cleancycle_count.target_cnt)
	{
		g_cleancycle_count.cur_cnt++;
		Usprintf("g_cleancycle_count cur_cnt:%d\n",g_cleancycle_count.cur_cnt);
		return 0;	
	}
	else if(g_cleancycle_count.cur_cnt == g_cleancycle_count.target_cnt)
	{
		return 1;
	}
	return 0;		
}








