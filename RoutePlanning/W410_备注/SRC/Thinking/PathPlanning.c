 #include "Include.h"
volatile uint8_t g_pathplanning_cleaned_rate = 0;

PathPlanningLane_t PathPlanning_IsLaneCleaned(Point16_t *p_lane_pos)
{
	uint8_t lane_max_flag = 0,lane_min_flag = 0;
	uint16_t x_offset = 0;
	uint16_t lane_min = 0, lane_max = 0;
	int16_t temp_x = p_lane_pos->x;
	int16_t temp_y = p_lane_pos->y;	

	if(temp_y >= g_map.y_edg_max)return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;
	if(temp_y <= g_map.y_edg_min)return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;
	
	if(g_map.boundary.arrive_y)
	{
		if(temp_y >= g_map.y_adjust_max)return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;  
		if(temp_y <= g_map.y_adjust_min)return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;  	
	}	
	while(1)
	{
		x_offset++; 
		if(!lane_max_flag)
		{
			temp_x = p_lane_pos->x + x_offset;
			if(Map_IsCellBlocked(temp_x, temp_y))
			{
				lane_max_flag = 1;
			}
			if(temp_x >= (g_map.x_edg_max - 1))
			{	
				lane_max_flag = 1;
			}
			if(g_map.boundary.arrive_x)
			{
				if(temp_x >= (g_map.x_adjust_max - 1))
				{
					lane_max_flag = 1;
				}
			}
			if(!lane_max_flag)
			{
				if(Map_IsCellUncleanLaneAccessible(temp_x,temp_y))
				{
					if(Map_IsCellUncleanLaneAccessible(temp_x - 1, temp_y))
					{
						lane_max = x_offset;
					}		
				}
			}
		}		
		if(!lane_min_flag)
		{
			temp_x = p_lane_pos->x - x_offset;
			if(Map_IsCellBlocked(temp_x, temp_y))
			{
				lane_min_flag = 1;
			}
			
			if(temp_x <= (g_map.x_edg_min + 1))
			{
				lane_min_flag = 1;
			}
			if(g_map.boundary.arrive_x)
			{
				if(temp_x <= (g_map.x_adjust_min + 1))
				{
					lane_min_flag = 1;
				}
			}
			if(!lane_min_flag)	
			{
				if(Map_IsCellUncleanLaneAccessible(temp_x,temp_y))
				{
					if(Map_IsCellUncleanLaneAccessible(temp_x + 1, temp_y))
					{
						lane_min = x_offset;
					}			
				}
			}	
		}
		if(lane_max_flag&&lane_min_flag)
		{
			break;
		}
	}

	if((lane_min == 0)&&(lane_max == 0))//p_lane_pos 这点左右都是障碍
	{
		return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;
	}

	if((lane_min != 0) && (lane_max != 0))//p_lane_pos 这点两侧（line）有未清扫区域，返回最远的点为目标
	{
		if(lane_min > lane_max)
		{			
			p_lane_pos->x -= lane_min;	
		}
		else
		{
			p_lane_pos->x += lane_max;	
		}
		return PATHPLANNING_LANE_BOTH_SIDE_UNCLEAN;			
	}
	
    if(Heading_GetCurrentDir() == HEADING_DIR_WEST)//p_lane_pos 这点只有一侧有未清扫区域，另一侧障碍，返回未清扫的末点为目标
    {
        if(lane_min != 0)
        {
            p_lane_pos->x -= lane_min;
            return PATHPLANNING_LANE_ONE_SIDE_UNCLEAN;
        }
        if(lane_max != 0)
        {
            p_lane_pos->x += lane_max;
            return PATHPLANNING_LANE_ONE_SIDE_UNCLEAN;
        }
    }
    else
    {
        if(lane_max != 0)
        {
            p_lane_pos->x += lane_max;
            return PATHPLANNING_LANE_ONE_SIDE_UNCLEAN;
        }
        if(lane_min != 0)
        {
            p_lane_pos->x -= lane_min;
            return PATHPLANNING_LANE_ONE_SIDE_UNCLEAN;
        }
    }

    return PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;
}

uint8_t PathPlanning_NormalClean(Point16_t start_cell)
{
	//if(!Math_IsEven(start_cell.y))return 0;
	PathPlanningLane_t status = PATHPLANNING_LANE_BOTH_SIDE_BLOCKED;
	Point16_t next_pos  = start_cell;
	PathPlanningState_t temp_state = PATHPLANNING_STATE_NOTARGET;
	PathListCell_t temp_pathlist ;
	int16_t lane_dir = 2;
	uint8_t lane_dir_up = 1,lane_dir_down = 1;
	Point16_t robot_pos = Map_GetRobotCell();
	
	Usprintf_PathPlanning("pathplanning NormalClean StartPos(%d,%d)\n",start_cell.x,start_cell.y);
	
	(robot_pos.y > Map_GetLevelCell().y)?(lane_dir = -2):(lane_dir = 2);
	
	PathList_Clear();						
	status = PathPlanning_IsLaneCleaned(&next_pos);
	if(status != PATHPLANNING_LANE_BOTH_SIDE_BLOCKED)//该 line 还有未清扫区域
	{
		temp_pathlist.cell_pos = next_pos;
		temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
		PathList_AddNewPoint(temp_pathlist);
		return 1;
	}
    
    //该 line 已经没有可清扫区域，机器要移动到其他行！！！
	next_pos = start_cell;
    
    //lane_dir 和 lane_dir_up ，lane_dir_down 似乎是代码优化速度的结果，不必纠结于变量名

	if(g_map.boundary.arrive_y)
	{
		if((next_pos.y + lane_dir) >= g_map.y_adjust_max)//当机器位置 y < y0 时候有效	
		{
			lane_dir_up = 0;		
		}
		if((next_pos.y + lane_dir) <= g_map.y_adjust_min)//当机器位置 y > y0 时候有效	
		{
			lane_dir_up = 0;		
		}	
		if((next_pos.y - lane_dir) >= g_map.y_adjust_max)//当机器位置 y > y0 时候有效	
		{
			lane_dir_down = 0;		
		}
		if((next_pos.y - lane_dir) <= g_map.y_adjust_min)//当机器位置 y < y0 时候有效	
		{
			lane_dir_down = 0;		
		}		
	}

	if((next_pos.y + lane_dir) >= g_map.y_edg_max)//当机器位置 y < y0 时候有效
	{
		lane_dir_up = 0;		
	}
	if((next_pos.y + lane_dir) <= g_map.y_edg_min)//当机器位置 y > y0 时候有效
	{
		lane_dir_up = 0;		
	}	
	if((next_pos.y - lane_dir) >= g_map.y_edg_max)//当机器位置 y > y0 时候有效
	{
		lane_dir_down = 0;		
	}
	if((next_pos.y - lane_dir) <= g_map.y_edg_min)//当机器位置 y < y0 时候有效
	{
		lane_dir_down = 0;		
	}
    
	//当机器位置 y > 起始点 y0 时候，lane_dir = -2，换行优先检查已清扫的那一侧，
    //因为过来的那侧可能有障碍物导致那行有漏扫
    //O ：表示已清扫，X：表示障碍物，S：表示起点，_：表示未经过的区域
    //
    // O O O O O O O O O O O O O O O O X X
    // _ _ _ _ _ _ _ _ _ X X X X O O O X X
    // _ _ _ _ _ _ _ _ _ X X X X O O O X X
    // S O O O O O O O O O O O O O O O X X
	if(Map_IsCellUncleanAccessible(next_pos.x,next_pos.y + lane_dir)&&lane_dir_up)
	{
		next_pos.y += lane_dir;
		temp_pathlist.cell_pos = next_pos;
		temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
		PathList_AddNewPoint(temp_pathlist);
		temp_state = PATHPLANNING_STATE_NORMAL_CLEAN;		
	}
	else if(Map_IsCellUncleanAccessible(next_pos.x,next_pos.y - lane_dir)&&lane_dir_down)//往 y 轴负方向移动
	{
		next_pos.y -= lane_dir;
		temp_pathlist.cell_pos = next_pos;
		temp_pathlist.status = PATHLIST_NORMAL_CLEAN;
		PathList_AddNewPoint(temp_pathlist);
		temp_state = PATHPLANNING_STATE_NORMAL_CLEAN;
	}

	if(temp_state == PATHPLANNING_STATE_NORMAL_CLEAN)return 1;

	return 0;
}

uint8_t PathPlanning_ShortestPath(Point16_t robot_cell,uint8_t first_line_width)
{
	uint8_t reval = 0;
	/*
	if(Math_IsEven(robot_cell.y))
	{
		reval = PathPlanning_ShortestCreate(robot_cell,first_line_width);
	}
	else
	{
		robot_cell.y = robot_cell.y + 1;
		reval = PathPlanning_ShortestCreate(robot_cell,first_line_width);
		if(!reval)
		{
			robot_cell.y = robot_cell.y - 2;
			reval = PathPlanning_ShortestCreate(robot_cell,first_line_width);
		}
	}
	*/
	reval = PathPlanning_ShortestCreate(robot_cell,first_line_width);
	return reval;
}

uint8_t PathPlanning_ShortestCreate(Point16_t robot_cell,uint8_t first_line_width)
{
	Usprintf_PathPlanning("pathplanning start cell:%d %d\n",robot_cell.x,robot_cell.y);
	Line_t temp_line;
	Line_Reset();
	PathPoint_ClearAllPoints();
	PathList_Clear();
	temp_line = Line_FindOneLine(robot_cell,first_line_width);

	for(uint8_t i = 0;i< TargetList_ReadCnt();i++)
	{
		if(Line_IsCellOnLine(TargetList_ReadTarget(i),temp_line))
		{
			PathPoint_AddOnePoint(robot_cell); 
			PathPoint_AddOnePoint(TargetList_ReadTarget(i));
			Usprintf_PathPlanning("pathplanning target cell:%d %d on first line\n",TargetList_ReadTarget(i).x,TargetList_ReadTarget(i).y);	
			return 1;
		}		
	}

	temp_line.level = g_line_list.line_level;
	Line_AddLine(temp_line);
	if(Line_FindAllLines())
	{
		Line_SortLines();//已经有连续相邻相接的 lines 了
		PathPlanning_CreatePathPoint(robot_cell);
		return 1;
	}
	return 0;
}
//此时的 lines 列表，是只有连续相邻相接的，能从起点 line 抵达目标点 line 的列表，计算生成从起点到目标点的路径
void PathPlanning_CreatePathPoint(Point16_t robot_cell)
{
	int16_t i = 0, j = 0, idx_buffer = 0, length_buffer = 1000, pathline_mark_cnt = 0;
	uint8_t line_low_flag = 1, line_high_flag = 1, current_xm_save_flag = 0, next_xm_save_flag = 0;
	int16_t current_x = 0;
	uint8_t line_queue = 0;
	Line_t current_line, next_line;
	Point16_t pos_cell_buffer;
	
	Usprintf_PathPlanning("robot_cell:%d %d\n",robot_cell.x,robot_cell.y)

	for(i = 0; i < Line_GetLineCnt(); i++)
	{
		current_line = Line_ReadLine(i);
		current_line.level = current_line.xe - current_line.xs;
		current_line.state = 0;
		Line_EditLine(current_line,i);
	}
	
	while(pathline_mark_cnt < Line_GetLineCnt())//更新 lines 的 xm 坐标，且筛选后的 line 至少长 3 个栅格，可容纳机器纵向移动
	{
		if(line_queue == 0)//get robot line
		{
			line_queue = 1;
			idx_buffer = 0;
			current_line = Line_ReadLine(idx_buffer);
			current_x = robot_cell.x;			
		}
		else if(line_queue == 1)//get last line
		{
			line_queue = 2;
			idx_buffer = Line_GetLineCnt()-1;
			current_line = Line_ReadLine(idx_buffer);
			current_x = TargetList_GetCurrentTargetPoint().x;
		}
		else
		{
			// find the shortest line 
			length_buffer =  1000;
			for(i=1;i<(Line_GetLineCnt()-1);i++)
			{
				current_line = Line_ReadLine(i);
				if(!current_line.state)
				{
					if(length_buffer > current_line.level)
					{
						idx_buffer = i;
						length_buffer = current_line.level;
					}
				}
			}
			current_line = Line_ReadLine(idx_buffer);
			current_x = (current_line.xs + current_line.xe) / 2;
		}
		
		i = idx_buffer;
		j = i;
		//get shortest line's middle x point and store to Current_X
		current_line.state = 1;//表示该 line 的 xm 已确认
		current_line.xm = current_x;
		Line_EditLine(current_line,i);
		pathline_mark_cnt++;

		/*
        find the nearby lines which have space for Current_X 
        extent the Current_X vertically if the next line have a 3 cells space for Current_X
        */
        //lines 是至少有 3 个栅格宽度的，可容纳机器纵向移动
		line_low_flag = 1;
		line_high_flag = 1;
		while(line_low_flag || line_high_flag)
		{
			if(line_high_flag)
			{
				if(i >= Line_GetLineCnt() - 1)
				{
					line_high_flag = 0;
				}
				else
				{
					i++;
					current_line = Line_ReadLine(i);
                    //line 长度至少大于 3 个栅格，且未确认 xm 值
					if(((current_x - current_line.xs) > 0) && ((current_line.xe - current_x ) > 0) && (!current_line.state))
					{
						current_line.state = 1;//表示该 line 的 xm 已确认
						current_line.xm = current_x;
						Line_EditLine(current_line,i);
						pathline_mark_cnt++;
					}
					else//该路径抵达不了目标点???
					{
						 line_high_flag = 0;
					}
				}
			}
			if(line_low_flag)
			{
				if(j==0)
				{
					line_low_flag = 0;
				}
				else 
				{
					j--;
					current_line = Line_ReadLine(j);
                    //line 长度至少大于 3 个栅格，且未确认 xm 值
					if(((current_x - current_line.xs) > 0) && ((current_line.xe - current_x ) > 0) && (!current_line.state))
					{
						current_line.state = 1;
						current_line.xm = current_x;
						Line_EditLine(current_line,j);
						pathline_mark_cnt++;
					}
					else//该路径抵达不了目标点???
					{
						line_low_flag = 0;
					}
				}
			}
		}
	}

	PathPoint_AddOnePoint(robot_cell); 
	pos_cell_buffer = robot_cell;
	current_line = Line_ReadLine(0);
	
	if(robot_cell.x != current_line.xm)
	{
		pos_cell_buffer.x = current_line.xm;
		pos_cell_buffer.y = current_line.y;
		PathPoint_AddOnePoint(pos_cell_buffer);							
	}

	for(i = 0; i < (Line_GetLineCnt() - 1); i++)//根据 lines 的 xm 调整 pathpoint 的位置，添加到 pathpoints 列表中
	{
		current_line = Line_ReadLine(i);
		next_line =  Line_ReadLine(i+1);
		if(current_line.xm != next_line.xm)
		{
			if(((current_line.xm - next_line.xs)>0) && ((next_line.xe - current_line.xm) > 0))//the Next Line is Include the current Line x middle 
			{
				current_xm_save_flag = 0;
				pos_cell_buffer.y = next_line.y;
			}
			else
			{
				current_xm_save_flag = 1;
				pos_cell_buffer.y = current_line.y;
			}
			pos_cell_buffer.x = current_line.xm;
			PathPoint_AddOnePoint(pos_cell_buffer);											
			if(((next_line.xm - current_line.xs) > 0) && ((current_line.xe - next_line.xm) > 0))//the Current Line is Include the next Line x middle 
			{
				next_xm_save_flag = 0;
				pos_cell_buffer.y = current_line.y;
			}
			else
			{
				next_xm_save_flag = 1;
				pos_cell_buffer.y = next_line.y;				
			}
			pos_cell_buffer.x = next_line.xm;
			PathPoint_AddOnePoint(pos_cell_buffer);	
			/*
				check if cross lines 
				if both two point are saved means these 2 lines cross 
						1. line a -------
						   line b		---------
						2. line a  	---------
                           line b --------
				so need to insert 2 turn points between those 2 saved points
			*/
			if(next_xm_save_flag && current_xm_save_flag)//need to insert 2 point 
			{
				if(current_line.xe > next_line.xe)
				{
					pos_cell_buffer.x = (current_line.xs + next_line.xe) / 2;
				}
				else
				{
					pos_cell_buffer.x = (current_line.xe + next_line.xs) / 2;
				}
				pos_cell_buffer.y = current_line.y;
				PathPoint_InsertOnePoint(pos_cell_buffer, PathPoint_GetPointCnt() - 1);
				pos_cell_buffer.y = next_line.y;
				PathPoint_InsertOnePoint(pos_cell_buffer, PathPoint_GetPointCnt() - 1);
			}
		}
	}	
		
	current_line = Line_ReadLine(Line_GetLineCnt() - 1);
	pos_cell_buffer.x = current_line.xm;
	pos_cell_buffer.y = current_line.y;

	if(!Map_IsTwoCellEqual(pos_cell_buffer, PathPoint_ReadLastPoint()))//添加最后 line 的中点
	{
		PathPoint_AddOnePoint(pos_cell_buffer);		
	}

	if(!Map_IsTwoCellEqual(PathPoint_ReadLastPoint(),TargetList_GetCurrentTargetPoint()))//添加目标点
	{
		PathPoint_AddOnePoint(TargetList_GetCurrentTargetPoint());
	}
}

PathPlanningState_t PathPlanning_Cleaning(Point16_t start_cell)
{
	uint8_t clean_rate = 0;
	clean_rate++;
	Map_IsRobotNotMoving();
	Usprintf_PathPlanning("PathPlanning_Cleaning\n");
	if(PathPlanning_NormalClean(start_cell) == 1)
	{
		return PATHPLANNING_STATE_NORMAL_CLEAN;
	}	
	if(TargetList_CreateList() == 0)//清扫完成
	{
		return PATHPLANNING_STATE_NOTARGET;		
	}
	TargetList_Rearrange();	
	Map_EditLimit(MAP_LIMIT_CLEAN);
	if(PathPlanning_ShortestPath(start_cell,1))
	{
		PathPoint_AddPathPointToPathList(1);	
		Map_ResetLimit(MAP_LIMIT_CLEAN);
		return PATHPLANNING_STATE_NORMAL_CLEAN;
	}

	g_pathplanning_cleaned_rate = (Line_GetAcreage() * 100 / Map_GetCellAcreage());
	clean_rate = g_pathplanning_cleaned_rate;

	Map_ResetLimit(MAP_LIMIT_CLEAN);
	if(PathPlanning_ShortestPath(start_cell,1))
	{
		PathPoint_AddPathPointToPathList(1);	
		return PATHPLANNING_STATE_NORMAL_CLEAN;
	}	

	clean_rate = (Line_GetAcreage() * 100 / Map_GetCellAcreage());

    return PATHPLANNING_STATE_UNREACHABLE;
}

PathPlanningState_t PathPlanning_BackHome(Point16_t start_cell)
{
	if(Map_GetTwoCellsDistance(start_cell,Map_GetHomeCell())<=1)
	{
		return PATHPLANNING_STATE_ARRIVE;
	}
	Map_IsRobotNotMoving();
	Usprintf_PathPlanning("PathPlanning_BackHome\n");
	TargetList_Clear();	 
	if(TargetList_AddHomeCell())
	{
		Map_EditLimit(MAP_LIMIT_CLEAN);
		if(PathPlanning_ShortestPath(start_cell,1))
		{
		  Usprintf_PathPlanning("home cell with clean cells\n");
		  PathPoint_AddPathPointToPathList(1);
		  Map_ResetLimit(MAP_LIMIT_CLEAN);	  
		  return PATHPLANNING_STATE_NORMAL_CLEAN;
		}
		Map_ResetLimit(MAP_LIMIT_CLEAN);	
		if(PathPlanning_ShortestPath(start_cell,1))
		{
		  Usprintf_PathPlanning("home cell with all cells\n");
		  PathPoint_AddPathPointToPathList(1); 
		  return PATHPLANNING_STATE_NORMAL_CLEAN;
		}		
	}
	
	TargetList_Clear();	

	if(TargetList_AddHomeCells())
	{
		Map_EditLimit(MAP_LIMIT_CLEAN);
		if(PathPlanning_ShortestPath(start_cell,1))
		{
		  Usprintf_PathPlanning("home cells with clean cells\n");
		  PathPoint_AddPathPointToPathList(1);
		  Map_ResetLimit(MAP_LIMIT_CLEAN);	  
		  return PATHPLANNING_STATE_NORMAL_CLEAN;
		}
		Map_ResetLimit(MAP_LIMIT_CLEAN);
		if(PathPlanning_ShortestPath(start_cell,1))
		{
		  Usprintf_PathPlanning("home cells with all cells\n");
		  PathPoint_AddPathPointToPathList(1); 
		  return PATHPLANNING_STATE_NORMAL_CLEAN;
		}		
	}
	return PATHPLANNING_STATE_UNREACHABLE;
}

PathPlanningState_t PathPlanning_WallFind(Point16_t start_cell,Mode_t mode)
{
	if(mode == MODE_GO_HOME)
	{
		TargetList_Clear();		
		if(!TargetList_AddHomeCells())
		{
			return PATHPLANNING_STATE_UNREACHABLE;
		}
	}
	else if(TargetList_CreateList() == 0)
	{
		return PATHPLANNING_STATE_NOTARGET;
	}
	Usprintf_PathPlanning("PathPlanning_WallFind\n");
	Map_ResetLimit(MAP_LIMIT_CLEAN);
	if(PathPlanning_ShortestPath(Map_GetRobotCell(),1))
	{
		PathPoint_AddPathPointToPathList(1);
		return PATHPLANNING_STATE_NORMAL_CLEAN;
	}
	return PATHPLANNING_STATE_UNREACHABLE;
}

uint8_t PathPlanning_IsSpotLaneCleaned(uint8_t idx ,Point16_t start_cell, int8_t dir)
{
    uint16_t angle = 0;
    double tan_result = 0;
    uint16_t angle_cnt = 0;
    int16_t angle_32 = 0;
    int32_t x = idx, y = idx , px = start_cell.x, py = start_cell.y;
    uint8_t ret_val = 0;

    PathPoint_ClearAllPoints();

    PathPoint_AddOnePoint(start_cell);

    angle = Math_Course2Dest(0,0,start_cell.x,start_cell.y)/10;

    for(angle_cnt = 0;angle_cnt < 361;angle_cnt ++)
    {	 
        angle += dir;
        if(angle > 359)angle = 0;
        else if(angle <= 0 )angle = 360;
        
        
        angle_32 = angle;

        if( angle == 45)
        {
            tan_result = 1;
        }
        else if( angle == 135)
        {
            tan_result = -1;
        }
        else if( angle == 225)
        {
            tan_result = 1;
        }
        else if( angle == 315)
        {
            tan_result = -1;
        }
        else 
        {
            tan_result = tan(Math_Deg2Rad(angle,1));
            if(tan_result == 0)continue;
        }
        
        if((angle_32 <= 45) || (angle_32 > 315))
        {
            x = idx;
            y = x * tan_result;
        }
        else if((angle_32 > 45) && (angle_32 <= 135))
        {
            y = idx;
            x = y / tan_result;
        }
        else if((angle_32 > 135) && (angle_32 <= 225))
        {
            x = -idx;
            y = x * tan_result;
        }
        else if((angle_32 > 225) && (angle_32 <= 315))
        {
            y = -idx;
            x = y / tan_result;
        }
        if(!Map_IsCellAccessible(x, y))
        {
            break;
        }
        if((x != px) || (y != py))
        {
            if(angle_cnt > 180)
            {
                if(x == start_cell.x)
                {
                    if(Map_IsCellAccessible(x, y))
                    {
                        if(Math_Diff_int(y,start_cell.y) < (SPOT_RANGE - 1))
                        {
                            PathPoint_AddOnePoint((Point16_t){x,y});
                            ret_val |= 0x01;
                            break;
                        }					
                    }
                }
                if(y == start_cell.y)
                {
                    if(Map_IsCellAccessible(x, y))
                    {
                        if(Math_Diff_int(x,start_cell.x) < (SPOT_RANGE - 1))
                        {
                            PathPoint_AddOnePoint((Point16_t){x,y});
                            ret_val |= 0x01;
                            break;
                        }												
                    }
                }
            }			
            if(Map_IsCellUncleanAccessible(x, y))
            {
                PathPoint_AddOnePoint((Point16_t){x,y});
                ret_val |= 0x01;
            }	         					
        }
        
        px = x;
        py = y;
    }
    PathPoint_SortPoints();
    return ret_val;
}

void PathPlanning_Spot(Point16_t start_cell)
{
    int8_t idx = 0;
    PathListCell_t  temp_pathlist;
    Point16_t temp_start_cell;
    uint8_t step = 1;
        
    PathPoint_ClearAllPoints();
    PathList_Clear();

    temp_pathlist.cell_pos = start_cell;
    temp_pathlist.status = PATHLIST_SHORT;

    PathList_AddNewPoint(temp_pathlist);
    idx = Math_GetMax(Math_Absolute(start_cell.x),Math_Absolute(start_cell.y));

    temp_start_cell = start_cell;

    if(idx >= SPOT_RANGE)
    {
        if(Spot_GetDirOutin() == SPOT_DIR_OUT)
        {
            Spot_SetDirOutin(SPOT_DIR_IN);		
        }
    }

    if(Spot_GetDirOutin() == SPOT_DIR_OUT)
    {
        for(;idx < SPOT_RANGE;idx +=1)
        {
            if(PathPlanning_IsSpotLaneCleaned(idx,temp_start_cell,Spot_GetDirCw()))
            {
                PathPoint_AddPathPointToPathList(0);
                temp_start_cell = Spot_GetNextLanePos(PathList_ReadLastPath().cell_pos,1,step);	
            }
            else
            {
                temp_start_cell = Spot_GetNextLanePos(start_cell,1,step);	
            }					
        }
        idx++;
    }

    Map_ClearSpotCleanCells(idx);

    for(;idx > 0;idx -=1)
    {
        if(PathPlanning_IsSpotLaneCleaned(idx,  temp_start_cell, Spot_GetDirCw()))
        {
            PathPoint_AddPathPointToPathList(0);
            temp_start_cell = Spot_GetNextLanePos(PathList_ReadLastPath().cell_pos,0,step);		
        }
        else
        {
            temp_start_cell = Spot_GetNextLanePos(start_cell,0,step);	
        }
    }
}






