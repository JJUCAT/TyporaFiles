 #include "Include.h"
TargetList_t g_target_list;

void TargetList_Init(void)
{
	memset(&g_target_list,0,sizeof(g_target_list));
}

void TargetList_Clear(void)
{
	g_target_list.cnt = 0;
}

uint8_t TargetList_ReadCnt(void)
{
	return g_target_list.cnt;
}

Point16_t TargetList_ReadTarget(uint8_t idx)
{
	if(idx < TargetList_ReadCnt())
	{
		return g_target_list.list[idx];
	}
	return (Point16_t){0,0};
}

void TargetList_DeleteTarget(uint8_t idx)
{
	uint8_t i=0;
	for(i = idx; i < (TargetList_ReadCnt()-1); i++)//后面的点往前挪
	{
		g_target_list.list[i] = g_target_list.list[i + 1];
	}
	g_target_list.cnt--;
}  

void TargetList_AddTarget(int16_t x,int16_t y)
{
	if(g_target_list.cnt >= TARGET_LIST_AMOUNT)
	{
		return ;
	}
	g_target_list.list[g_target_list.cnt].x = x;
	g_target_list.list[g_target_list.cnt].y = y;
	g_target_list.cnt++;
}

uint8_t TargetList_IsCellTargetable(int16_t x,int16_t y)
{
	int8_t val = 0, i = 1, j = 1;
	CellState_t temp_cell = CELL_STATE_UNCLEAN;
	if(!Map_IsCellUncleanWithCleanCnt(x,y))return 0;	
	if(!Map_IsCellAccessible(x,y))return 0;
	if(!Map_IsCellInMapEdg(x,y))return 0;
	if(x >= g_map.x_edg_max)return 0;
	if(x <= g_map.x_edg_min)return 0;	
	if(y >= g_map.y_edg_max)return 0;
	if(y <= g_map.y_edg_min)return 0;

	if(g_map.boundary.arrive_x)	
	{
		if(x >= g_map.x_adjust_max)return 0;
		if(x <= g_map.x_adjust_min)return 0;
	}
	if(g_map.boundary.arrive_y)	
	{
		if(y >= g_map.y_adjust_max)return 0;
		if(y <= g_map.y_adjust_min)return 0;
	}
	
	for(i = 1;i >=-1;i--)
	{
		for(j = 1; j >=-1;j--)
		{
			temp_cell = Map_GetCell(x + i, y + j);
			if(temp_cell == CELL_STATE_UNCLEAN)val++;
			else if (g_cleancycle_count.cur_cnt == 2 && temp_cell == CELL_STATE_CLEANED)val++;
			else if(temp_cell >= CELL_STATE_BLOCKED)
			{
				return 0;
			}
		}
	}
	if(val > 1)return 1;
	return 0;					
}

uint8_t TargetList_CreateList(void)
{
	int16_t x = 0,y = 0;
	uint8_t ret_val = 0;
	Point16_t robot_cell = Map_GetRobotCell();
	TargetList_Clear();

	for(y = g_map.y_min; y <= g_map.y_max; y++)
	{
		for(x = g_map.x_min; x <= g_map.x_max; x++)//先按 x(lane) 添加
		{
			if(g_cleancycle_count.cur_cnt == 1)
			{
				if(Map_GetCell(x,y)!= CELL_STATE_CLEANED)
				{
					continue;	
				}	
			}
			if(g_cleancycle_count.cur_cnt == 2)
			{
				if(Map_GetCell(x,y)!= CELL_STATE_CLEANED_TWICE)
				{
					continue;	
				}	
			}
            //检查已经清扫的 cell 的上下左右，把未清扫的 cell 添加到目标点
			if(Map_IsCellAccessible(x,y) && Map_IsCellInMapEdg(x,y))
			{
				if(TargetList_IsCellTargetable(x, y + 1))
				{
					if(!Map_IsCellBesideBlocked(x, y + 1))
					{
						if((robot_cell.x != x)||(robot_cell.y != (y + 1)))
						{
							/*
							if(Math_IsEven(y + 1))
							{
								TargetList_AddTarget(x, y + 1);
								ret_val = 1;
							}
							else
							{
								if((robot_cell.x != x)||(robot_cell.y != y))
								{
									TargetList_AddTarget(x, y);
									ret_val = 1;
								}							
							}
							*/
							TargetList_AddTarget(x, y + 1);
							ret_val = 1;							
						}
					}
				}
				if(TargetList_IsCellTargetable(x,y - 1))
				{
					if(!Map_IsCellBesideBlocked(x, y - 1))
					{
						if((robot_cell.x != x)||(robot_cell.y != (y - 1)))
						{
							/*
							if(Math_IsEven(y - 1))
							{
								TargetList_AddTarget(x, y - 1);
								ret_val = 1;
							}
							else
							{
								if((robot_cell.x != x)||(robot_cell.y != y))
								{
									TargetList_AddTarget(x, y);
									ret_val = 1;
								}								
							}
							*/
							TargetList_AddTarget(x, y - 1);
							ret_val = 1;											
						}
					}
				}
				if(TargetList_IsCellTargetable(x + 1, y))
				{
					if(!Map_IsCellBesideBlocked(x + 1, y))
					{
						if((robot_cell.x != (x + 1))||(robot_cell.y != y))
						{
							/*						
							if(Math_IsEven(y))
							{
								TargetList_AddTarget(x + 1, y);
								ret_val = 1;
							}
							else
							{
								if(TargetList_IsCellTargetable(x + 1, y - 1))
								{
									if((robot_cell.x != (x + 1))||(robot_cell.y != (y - 1)))
									{
										TargetList_AddTarget(x + 1, y  - 1);
										ret_val = 1;
									}									
								}
								else if(TargetList_IsCellTargetable(x + 1, y + 1))
								{
									if((robot_cell.x != (x + 1))||(robot_cell.y != (y + 1)))
									{
										TargetList_AddTarget(x + 1, y  + 1);
										ret_val = 1;
									}
								}
							}
							*/
							TargetList_AddTarget(x + 1, y);
							ret_val = 1;	
						}
					}

				}
				if(TargetList_IsCellTargetable(x - 1,y))
				{
					if(!Map_IsCellBesideBlocked(x - 1, y))
					{
						if((robot_cell.x != (x - 1))||(robot_cell.y != y))
						{
							/*
							if(Math_IsEven(y))
							{
								TargetList_AddTarget(x - 1, y);
								ret_val = 1;
							}
							else
							{
								if(TargetList_IsCellTargetable(x - 1, y - 1))
								{
									if((robot_cell.x != (x - 1))||(robot_cell.y != (y - 1)))
									{
										TargetList_AddTarget(x - 1, y  - 1);
										ret_val = 1;
									}									
								}
								else if(TargetList_IsCellTargetable(x - 1, y + 1))
								{
									if((robot_cell.x != (x - 1))||(robot_cell.y != (y + 1)))
									{
										TargetList_AddTarget(x - 1, y  + 1);
										ret_val = 1;
									}
								}								
							}
							*/
							TargetList_AddTarget(x - 1, y);
							ret_val = 1;						
						}
					}
				}								
			}
							
		}
	}
	return ret_val;
}

void TargetList_Rearrange(void)
{
	uint16_t break_cnt = 10000;
	uint8_t same_line_flag = 0;
	uint8_t i = 0,j = 0;
	int16_t x = 0;
	Point16_t target_1,target_2;
	for(i = 0; i < (TargetList_ReadCnt() - 1);i++)
	{
		target_1 = TargetList_ReadTarget(i);
		j = i + 1;
		while(j < TargetList_ReadCnt())
		{
			same_line_flag = 1;
			if(target_1.y == TargetList_ReadTarget(j).y)
			{
				target_2 = TargetList_ReadTarget(j);

				for(x = target_1.x; x < target_2.x; x++)
				{
					if(Map_IsCellBlocked(x, target_1.y))break;
				}
				if(x == (target_2.x))//两个目标点之间无障碍，删除离机器当前位置最远的
				{
                    //无论删除当前点还是下一个点，后面的目标点都会往前挪
					if(Math_Diff_int(target_1.x,Map_GetRobotCell().x) < Math_Diff_int(target_2.x,Map_GetRobotCell().x))
					{
						TargetList_DeleteTarget(j);					
					}
					else
					{
						TargetList_DeleteTarget(i);
					}
					same_line_flag=0;
				}
			}
			if(same_line_flag)j++;
			break_cnt--;
			if(break_cnt == 0)break;
		}
	}
}

void TargetList_StoreCurrentTargetPoint(Point16_t target)
{
	g_target_list.current_point = target;
}

Point16_t TargetList_GetCurrentTargetPoint(void)
{
	return g_target_list.current_point;
}

uint8_t TargetList_AddHomeCell(void)
{
	uint8_t reval = 0;

	Point16_t home_cell = Map_GetHomeCell();	

	if(Map_IsCellAccessible(home_cell.x,home_cell.y))
	{
		TargetList_AddTarget(home_cell.x,home_cell.y);
		reval = 1;
	}
	return reval;
}

uint8_t TargetList_AddHomeCells(void)
{
	uint8_t reval = 0;
	uint8_t range = 6;

	Point16_t home_cell = Map_GetHomeCell();

	for(int8_t i = -range;i <= range;i++)
	{
		if(Map_IsCellAccessible(home_cell.x + i,home_cell.y))
		{						
			if((Map_GetCell(home_cell.x + i,home_cell.y) == CELL_STATE_CLEANED)||(Map_GetCell(home_cell.x + i,home_cell.y) == CELL_STATE_CLEANED_TWICE))
			{
				reval = 1;
				TargetList_AddTarget(home_cell.x + i,home_cell.y);
				Map_SetHomeCell(home_cell.x + i,home_cell.y);
				break;
			}			
		}
	}
	for(int8_t j = -(2*range);j <= (range*2);j++)
	{
		if(Map_IsCellAccessible(home_cell.x,home_cell.y + j))
		{		
			if((Map_GetCell(home_cell.x,home_cell.y + j) == CELL_STATE_CLEANED)||(Map_GetCell(home_cell.x ,home_cell.y + j) == CELL_STATE_CLEANED_TWICE))
			{
				reval = 1;
				TargetList_AddTarget(home_cell.x ,home_cell.y + j);
				Map_SetHomeCell(home_cell.x ,home_cell.y + j);
				break;
			}			
		}					
	}		
	return reval;
}
















