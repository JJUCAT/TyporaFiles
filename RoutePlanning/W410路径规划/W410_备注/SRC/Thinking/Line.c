 #include "Include.h"

LineList_t g_line_list;

Line_GetTargetCnt on_line_get_target_cnt;

Line_ReadTargetPnt on_line_read_target_pnt;

Line_StoreTargetPnt on_line_store_target_pnt;

void Line_Init(void)
{
	memset(&g_line_list,0,sizeof(g_line_list));
	on_line_get_target_cnt = TargetList_ReadCnt;
	on_line_read_target_pnt = TargetList_ReadTarget;
	on_line_store_target_pnt = TargetList_StoreCurrentTargetPoint;	
}

void Line_Reset(void)
{
	g_line_list.line_cnt = 0;
	g_line_list.line_level = 0;
}

uint16_t Line_GetLineCnt(void)
{
	return g_line_list.line_cnt;
}

void Line_EditLine(Line_t l,uint16_t idx)
{
	if (idx >= LINE_CNT)return;
	g_line_list.line[idx] = l;
}

void Line_DeleteLine(uint16_t idx)
{
	uint16_t i=0;
	for(i = idx; i < (g_line_list.line_cnt - 1); i++)
	{
		g_line_list.line[i] = g_line_list.line[i + 1];
		g_line_list.line[i].idx--;
	}
	g_line_list.line_cnt--;
}

Line_t Line_ReadLine(uint16_t idx)
{
	return g_line_list.line[idx];
}

uint8_t Line_IsLineEnd(int16_t x,int16_t y,uint8_t width)
{
	uint8_t reval = 0;
	if(Map_IsLimit(MAP_LIMIT_CLEAN))//line 只算已清扫过的区域
	{
		if(!Map_IsCellCleaned(x, y))
		{
			reval = 1;	
		}
		if(!Map_IsCellCleaned(x, y + width))
		{
			reval = 1;	
		}
		if(!Map_IsCellCleaned(x, y - width))
		{
			reval = 1;	
		}
	}

	if(Map_IsCellBlocked(x, y))
	{
		reval = 1;	
	}
	if(Map_IsCellBlocked(x, y + width))
	{
		reval = 1;	
	}
	if(Map_IsCellBlocked(x, y - width))
	{
		reval = 1;	
	}

	if(x > g_map.x_edg_max + 1)
	{
		reval = 1;
	}
	if(x < g_map.x_edg_min - 1)
	{
		reval = 1;
	}

	if(g_map.boundary.arrive_x)
	{
		if(x > g_map.x_adjust_max + 1)
		{
			reval = 1;
		}
		if(x < g_map.x_adjust_min - 1)
		{
			reval = 1;
		}	
	}
	if(g_map.boundary.arrive_y)
	{
		if(y > g_map.y_adjust_max + 1)
		{
			reval = 1;
		}
		if( y + width > g_map.y_adjust_max + 1)
		{
			reval = 1;
		}

		if(y < g_map.y_adjust_min - 1)
		{
			reval = 1;
		}

		if(y - width < g_map.y_adjust_min - 1)
		{
			reval = 1;
		}	
	}	
	if(y > g_map.y_edg_max + 1)
	{
		reval = 1;
	}
	if( y + width > g_map.y_edg_max + 1)
	{
		reval = 1;
	}

	if(y < g_map.y_edg_min - 1)
	{
		reval = 1;
	}

	if(y - width < g_map.y_edg_min - 1)
	{
		reval = 1;
	}
	
	return reval;
}

Line_t Line_FindOneLine(Point16_t start_cell, uint8_t line_width)
{
	Line_t ret_val;
	ret_val.y = start_cell.y;
	int16_t temp_xs = start_cell.x, temp_xe = start_cell.x;
	uint8_t low_flag = 1, high_flag = 1;
	
	ret_val.xs = temp_xs;
	ret_val.xe = temp_xe;
	while(low_flag || high_flag)
	{
		if(low_flag)
		{
			temp_xs--;
			if(Line_IsLineEnd(temp_xs,ret_val.y,line_width))
			{
				low_flag = 0;	
			}	
			if(low_flag)ret_val.xs = temp_xs;
		}
		if(high_flag)
		{
			temp_xe++;
			if(Line_IsLineEnd(temp_xe,ret_val.y,line_width))
			{
				high_flag = 0;	
			}
			if(high_flag)ret_val.xe = temp_xe; 
		}
	}	
	ret_val.level = g_line_list.line_level;
	return ret_val;
}

uint8_t Line_IsCellOnLine(Point16_t cell,Line_t line)
{
	if(cell.y == line.y)
	{
		if((cell.x >= line.xs) && (cell.x <= line.xe))return 1;
	}
	if(cell.y == (line.y + 1))
	{
		if((cell.x >= line.xs) && (cell.x <= line.xe))return 1;
	}
	if(cell.y == (line.y - 1))
	{
		if((cell.x >= line.xs) && (cell.x <= line.xe))return 1;
	}		
	return 0;
}

uint8_t Line_IsLineExist(Line_t line)
{
	uint16_t i=0;
	for(i = 0; i < g_line_list.line_cnt; i++)
	{
		if((line.xs == g_line_list.line[i].xs) && (line.xe == g_line_list.line[i].xe) && (line.y == g_line_list.line[i].y))
		{
			return 1;
		}
	}
	return 0;
}

void Line_AddLine(Line_t line)
{
	if (g_line_list.line_cnt + 1 >= LINE_CNT) 
	{
		return;
	}
	if(Line_IsLineExist(line))
	{
		return;
	}
	g_line_list.line_cnt++;
	line.idx = g_line_list.line_cnt - 1;
	g_line_list.line[g_line_list.line_cnt - 1] = line;
}

void Line_DeleteSameLevel(Line_t * line)
{
	uint16_t temp_this_idx=0;
	for(temp_this_idx = (g_line_list.line_cnt - 1) ; temp_this_idx > 0; temp_this_idx--)//列表中只有最后的 line 带有目标点，其余都是没目标点的
	{
		if(Line_ReadLine(temp_this_idx).idx != line->idx)
		{
			if(Line_ReadLine(temp_this_idx).level == line->level)
			{
				if(Line_ReadLine(temp_this_idx).idx < line->idx)line->idx--;
				Line_DeleteLine(temp_this_idx);
			}
		}
	}
}

Line_t Line_GetNearLine(Line_t current_line)//找相邻相接且 level 低一级的 lines
{
	uint16_t break_cnt = 1000;
	uint16_t temp_this_idx = 0;
	Line_t temp_line;
	temp_this_idx = (current_line.idx - 1);
	while(break_cnt--)
	{
		temp_line = Line_ReadLine(temp_this_idx);
		if(temp_line.level == (current_line.level - 1))
		{
			if(Math_Abs_int(temp_line.y - current_line.y) == 1)
			{
				if(temp_line.xs <= current_line.xs)
				{
					if(temp_line.xe >= current_line.xs)return Line_ReadLine(temp_this_idx);
				}
				if(temp_line.xs >= current_line.xs)
				{
					if(temp_line.xs <= current_line.xe)return Line_ReadLine(temp_this_idx);
				}
			}
		}
		if(temp_this_idx == 0)break;
		temp_this_idx--;
	}
	return current_line;
}

void Line_SortLines(void)//清除多余的同 level 的 lines,只剩下能通往目标点的 lines
{
	uint16_t break_cnt = 10000;
	Line_t line_buffer;
	line_buffer = Line_ReadLine(g_line_list.line_cnt-1);//此时的 lines 中只有最后一条 line 包含目标点
	while(break_cnt--)//清除多余同 level 的 lines
	{
		Line_DeleteSameLevel(&line_buffer);
		line_buffer = Line_GetNearLine(line_buffer);
		if(line_buffer.level==0)break;
	}
}

uint8_t Line_FindNextLines(Line_t current_line)
{
	uint16_t target_idx = 0;
	Point16_t temp_pos_cell;
	Line_t temp_line;
	uint8_t y_up = 1,y_down = 1;
	temp_pos_cell.x = current_line.xs;
	temp_pos_cell.y = current_line.y + 1;
/*
	if(!Math_IsEven(temp_pos_cell.y))
	{
		temp_pos_cell.y = temp_pos_cell.y + 1;
	}
*/
	if(temp_pos_cell.y > g_map.y_edg_max + 1)
	{
		y_up = 0;
	}
	if(g_map.boundary.arrive_y)
	{
		if(temp_pos_cell.y > g_map.y_max + 1)
		{
			y_up = 0;
		}
	}	
	if(y_up)
	{
		for(temp_pos_cell.x = (current_line.xs + 1);temp_pos_cell.x <= (current_line.xe - 1);temp_pos_cell.x++)//当前行的末端能通过一个机身
		{
			if(Map_IsCellBlocked(temp_pos_cell.x,temp_pos_cell.y)||Map_IsCellBlocked(temp_pos_cell.x,temp_pos_cell.y+1))
			{
				continue;
			}
			if(temp_pos_cell.y + 1 > g_map.y_edg_max + 1)
			{
				continue;
			}
			if(g_map.boundary.arrive_y)
			{
				if(temp_pos_cell.y + 1  > g_map.y_adjust_max + 1)
				{
					continue;
				}
			}
			temp_line = Line_FindOneLine(temp_pos_cell,1);
			if(((temp_line.xe - temp_line.xs) >= 2)&&((temp_line.xe - current_line.xs) >= 2)&&((current_line.xe - temp_line.xs) >= 2)) 
			{				
				Line_AddLine(temp_line);
				for(target_idx = 0; target_idx < on_line_get_target_cnt(); target_idx++)
				{					
					if(Line_IsCellOnLine(on_line_read_target_pnt(target_idx),temp_line))
					{
						on_line_store_target_pnt(on_line_read_target_pnt(target_idx));
						return 1;
					}
				}
			}
			temp_pos_cell.x = temp_line.xe + 1;			
		}
	}

	temp_pos_cell.x = current_line.xs;
	temp_pos_cell.y = current_line.y - 1; 
/*
	if(!Math_IsEven(temp_pos_cell.y))
	{
		temp_pos_cell.y = temp_pos_cell.y - 1;
	}
*/
	if(temp_pos_cell.y < g_map.y_edg_min - 1)
	{
		y_down = 0;
	}
	if(g_map.boundary.arrive_y)
	{
		if(temp_pos_cell.y < g_map.y_adjust_min - 1)
		{
			y_down = 0;
		}
	}

	if(y_down)
	{
		for(temp_pos_cell.x = (current_line.xs + 1);temp_pos_cell.x <= (current_line.xe - 1);	temp_pos_cell.x++)
		{
			if(Map_IsCellBlocked(temp_pos_cell.x,temp_pos_cell.y)||Map_IsCellBlocked(temp_pos_cell.x,temp_pos_cell.y-1))
			{
				continue;
			}
			if(temp_pos_cell.y - 1 < g_map.y_edg_min - 1)
			{
				continue;
			}
			if(g_map.boundary.arrive_y)
			{
				if(temp_pos_cell.y - 1  < g_map.y_adjust_min - 1)
				{
					continue;
				}
			}
			temp_line = Line_FindOneLine(temp_pos_cell,1);			
			if(((temp_line.xe - temp_line.xs) >= 2)&&((temp_line.xe - current_line.xs) >= 2)&&((current_line.xe - temp_line.xs) >= 2)) 
			{
				Line_AddLine(temp_line);
				for(target_idx = 0; target_idx < on_line_get_target_cnt(); target_idx++)
				{
					if(Line_IsCellOnLine(on_line_read_target_pnt(target_idx),temp_line))
					{
						on_line_store_target_pnt(on_line_read_target_pnt(target_idx));
						return 1;
					}
				}
			}
			temp_pos_cell.x = temp_line.xe + 1;
		}
	}
	return 0;
}

uint8_t Line_FindAllLines(void)
{
	uint16_t pre_line_idx = 0, new_line_idx = 0, current_line_idx = 0;
	uint16_t break_cnt = 10000;
	uint8_t  pre_flag = 1;
	while(break_cnt--)	
	{
		pre_flag=1;//标志新的 line 大筛选
		current_line_idx = pre_line_idx;
		while(current_line_idx < new_line_idx)//在现有的所有 line 中找相邻相连且包含目标点的 line ，然后添加到 line 表中
		{
			if(pre_flag)
			{
				pre_line_idx = g_line_list.line_cnt;//记录本次大筛选的 line 数量
				pre_flag=0;
			}
			if(Line_FindNextLines(Line_ReadLine(current_line_idx)))//添加的新 line 和上一代的 line 相邻相接
			{
				return 1;
			}
			current_line_idx++;
		}
		
		new_line_idx = g_line_list.line_cnt;//上面操作可能添加了新的 line，line 表中数量增加
		g_line_list.line_level++;//标记新增加 line 是第几轮筛选的
		
		if(pre_line_idx == new_line_idx)//大筛选后并没有添加新的 line，结束筛选
		{		
			break;
		}
	}
	return 0;
}

uint32_t Line_GetAcreage(void)
{
	uint32_t acreage_temp=0;
	Line_t temp_line;
	uint16_t i=0;
	for(i=0;i< g_line_list.line_cnt;i++)
	{
		temp_line = Line_ReadLine(i);
		acreage_temp += (temp_line.xe - temp_line.xs + 1);
	}
	return acreage_temp;
}








