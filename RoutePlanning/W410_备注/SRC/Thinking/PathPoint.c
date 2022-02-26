 #include "Include.h"

PathPoint_t g_pathpoint;

void PathPoint_Init(void)
{
	memset(&g_pathpoint,0,sizeof(g_pathpoint));
}

Point16_t PathPoint_ReadPoint(int16_t idx)
{
	return g_pathpoint.point_list[idx];
}

void PathPoint_AddOnePoint(Point16_t point)
{
	if(g_pathpoint.cnt >= PATH_POINT_CNT)
	{
		return;
	}
	Usprintf_PathPlanning("pathplanning add point:%d %d\n",point.x,point.y);
	g_pathpoint.point_list[g_pathpoint.cnt] = point;
	g_pathpoint.cnt++;
}

void PathPoint_EditPoint(Point16_t point,uint16_t idx)
{
	if(idx >= PATH_POINT_CNT)
	{
		return;
	}
	g_pathpoint.point_list[idx] = point;
}

uint16_t PathPoint_GetPointCnt(void)
{
	return g_pathpoint.cnt;
}

void PathPoint_AddPathPointToPathList(uint16_t start_idx)
{
	uint16_t i = 0;
	PathListCell_t temp_pathlist;
	for(i = start_idx; i < g_pathpoint.cnt; i++)
	{
		temp_pathlist.cell_pos = PathPoint_ReadPoint(i);
		temp_pathlist.status = PATHLIST_SHORT;
		PathList_AddNewPoint(temp_pathlist);
	}
}

Point16_t PathPoint_ReadLastPoint(void)
{
	if(g_pathpoint.cnt == 0)return g_pathpoint.point_list[0];
	return g_pathpoint.point_list[g_pathpoint.cnt - 1];
}

void PathPoint_ClearAllPoints(void)
{
	uint16_t i=0;
	for(i = 0; i < (PATH_POINT_CNT + 1); i++)
	{
		g_pathpoint.point_list[i].x = 0;
		g_pathpoint.point_list[i].y = 0;
	}
	g_pathpoint.cnt = 0;
}

void PathPoint_RemoveOnePoint(uint16_t idx)
{
	uint16_t i=0;
	for(i = idx; i < (PathPoint_GetPointCnt() - 1); i++)
	{
		g_pathpoint.point_list[i] = g_pathpoint.point_list[i+1];
	}
	g_pathpoint.cnt--;
}

void PathPoint_RemoveLastPoint(void)
{
	if(g_pathpoint.cnt > 0)g_pathpoint.cnt--;
}

void PathPoint_InsertOnePoint(Point16_t point,uint8_t idx)
{
	uint16_t i = 0;
	for(i = PathPoint_GetPointCnt(); i > idx; i--)
	{
		g_pathpoint.point_list[i] = g_pathpoint.point_list[i-1];
	}
	g_pathpoint.point_list[idx] = point;
	g_pathpoint.cnt++;
}

void PathPoint_SortPoints(void)
{
	uint16_t i = 0;
	Point16_t point_1, point_2, point_3;
	
	while(i < (PathPoint_GetPointCnt() - 2)) 
	{
		point_1 = PathPoint_ReadPoint(i);
		point_2 = PathPoint_ReadPoint(i + 1);
		point_3 = PathPoint_ReadPoint(i + 2);
		if((point_1.x == point_2.x) && (point_2.x == point_3.x))
		{
			PathPoint_RemoveOnePoint(i + 1);
		}
		else if((point_1.y == point_2.y) && (point_2.y == point_3.y))
		{
			PathPoint_RemoveOnePoint(i + 1);
		}
		else
		{
			i++;
		}
	}	
}













