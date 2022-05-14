 #include "Include.h"
CleanMode_t g_clean_mode;
void Mode_Init(void)
{
	g_clean_mode.mode = MODE_NONE;
	g_clean_mode.selection = MODE_NAVIGATION;
}
void Mode_SetMode(Mode_t mode)
{
	g_clean_mode.mode = mode;
}
Mode_t Mode_GetMode(void)
{
	return g_clean_mode.mode;
}

void Mode_SelectionCycle(void)
{
	switch(g_clean_mode.selection)
	{
		case MODE_NAVIGATION:
								g_clean_mode.selection = MODE_SPOT;
		break;
		case MODE_SPOT:
								g_clean_mode.selection = MODE_WALL;
		break;
		case MODE_WALL:
								g_clean_mode.selection = MODE_AREA;		
		break;
		case MODE_AREA:
								g_clean_mode.selection = MODE_NAVIGATION;	
		break;
		default:break;				
	}
}





















