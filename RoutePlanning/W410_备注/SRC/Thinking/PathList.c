 #include "Include.h"
PathList_t g_pathlist;
PathList_RealTimeMonitorSendData pathlist_realtime_monitor_send_data;

void PathList_Init(void)
{
	memset(&g_pathlist,0,sizeof(g_pathlist));
	pathlist_realtime_monitor_send_data = RealTimeMonitor_SendData;
}

void PathList_AddNewPoint(PathListCell_t point)
{
	Point16_t robot_cell; 
	robot_cell = Map_GetRobotCell();
	if((point.cell_pos.x == robot_cell.x) && (point.cell_pos.y == robot_cell.y))
	{
		Usprintf_PathPlanning("not add robot cell to path list\n");
		return;
	}
	if(g_pathlist.list_cnt > PATHLIST_CNT_LENGTH)
	{
		Usprintf_PathPlanning("Path List Full!!\n");
		return;
	}
	g_pathlist.list[g_pathlist.list_input_idx] = point;
	g_pathlist.list_input_idx++;
	if(g_pathlist.list_input_idx > PATHLIST_CNT_LENGTH)g_pathlist.list_input_idx = 0;
	g_pathlist.list_cnt++;	
	#ifdef DEBUG_REALTIME_MONITOR
	pathlist_realtime_monitor_send_data(point.cell_pos.x,point.cell_pos.y,0,9);
	#endif
	Usprintf_PathPlanning("add new point:%d %d %d\n",g_pathlist.list_cnt,point.cell_pos.x,point.cell_pos.y);
}

uint8_t PathList_Out(PathListCell_t *p_out)
{
	if(g_pathlist.list_cnt == 0)
	{
		Usprintf("No Path Out!!\n");
		return 0;
	}
	*p_out = g_pathlist.list[g_pathlist.list_output_idx];
	g_pathlist.list_output_idx++;
	if(g_pathlist.list_output_idx > PATHLIST_CNT_LENGTH)g_pathlist.list_output_idx = 0;
	if(g_pathlist.list_cnt > 0)g_pathlist.list_cnt--;
	return 1;
}

uint8_t PathList_GetNextPathListPoint(Point32_t *point_result)
{
	if(g_pathlist.list_cnt == 0)
	{
		return 0;
	}
	*point_result = Map_CellToPoint(g_pathlist.list[g_pathlist.list_output_idx].cell_pos);
	g_pathlist.list_output_idx++;
	if(g_pathlist.list_output_idx > PATHLIST_CNT_LENGTH)g_pathlist.list_output_idx = 0;
	if(g_pathlist.list_cnt > 0)g_pathlist.list_cnt--;
	return 1;
}

PathListCell_t PathList_ReadLastPath(void)
{
	if(g_pathlist.list_input_idx == 0)return g_pathlist.list[PATHLIST_CNT_LENGTH];
	return g_pathlist.list[(g_pathlist.list_input_idx - 1)];
}

void PathList_Clear(void)
{
	g_pathlist.list_cnt = 0;
	g_pathlist.list_input_idx = 0;
	g_pathlist.list_output_idx = 0;
}

uint8_t PathList_GetCnt(void)
{
	return g_pathlist.list_cnt;
}






