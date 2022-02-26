#include "Include.h"
Map_t g_map;


Map_GetAngle on_map_get_angle;
Map_RealTimeMonitorSendData on_map_realtime_monitor_send_data;
Map_WifiRealMapSendPoint on_map_wifi_realmap_send_point;

void Map_Init(void)
{
	memset(&g_map,0,sizeof(g_map));
	on_map_get_angle = Gyro_GetAngle;
	on_map_realtime_monitor_send_data = RealTimeMonitor_SendData;
	on_map_wifi_realmap_send_point = Wifi_AddRealMapToSend;
	g_map.p_clean_cur_cnt = &g_cleancycle_count.cur_cnt;
}

void Map_SetLevelCell(Point16_t cell)
{
	g_map.level_cell = cell;
}

Point16_t Map_GetLevelCell(void)
{
	return g_map.level_cell;
}

void Map_EditLimit(MapLimit_t limit)
{
	g_map.limit |= limit;
}

void Map_ResetLimit(MapLimit_t limit)
{
	g_map.limit &= ~limit;
}

uint8_t Map_IsLimit(MapLimit_t limit)
{
	if(g_map.limit & limit)
	{
		return 1;
	}
	return 0;
}

uint8_t Map_GetLimit(void)
{
	return g_map.limit;
}

int32_t Map_CellToCount(int16_t cell) 
{
	return cell * CELL_COUNT_MUL;
}

int16_t Map_CountToCell(double count) 
{
	if(count < -CELL_COUNT_MUL_1_2)
	{
		return (count + CELL_COUNT_MUL_1_2) / CELL_COUNT_MUL - 1;
	} 
	else 
	{
		return (count + CELL_COUNT_MUL_1_2) / CELL_COUNT_MUL;
	}
}

Point32_t Map_CellToPoint( Point16_t cell ) 
{
	Point32_t pnt;
	pnt.x = Map_CellToCount(cell.x);
	pnt.y = Map_CellToCount(cell.y);
	return pnt;
}

Point16_t Map_PointToCell( Point32_t pnt ) 
{
	Point16_t cell;
	cell.x = Map_CountToCell(pnt.x);
	cell.y = Map_CountToCell(pnt.y);
	return cell;
}

int16_t Map_GetRobotCellX(void) 
{
	return Map_CountToCell(g_map.count_x);
}

int16_t Map_GetRobotCellY(void) 
{
	return Map_CountToCell(g_map.count_y);
}

Point16_t Map_GetRobotCell(void)
{
  Point16_t temp_robot;
  temp_robot.x = Map_GetRobotCellX();
  temp_robot.y = Map_GetRobotCellY();
  return temp_robot;
}

void Map_SetRobotCell(int16_t x,int16_t y)
{
  Map_SetRobotCount(Map_CellToCount(x),Map_CellToCount(y));
}

int32_t Map_GetRobotCountX(void) 
{
	return (int32_t)round(g_map.count_x);
}

int32_t Map_GetRobotCountY(void) 
{
	return (int32_t)round(g_map.count_y);
}

Point32_t Map_GetRobotCount(void)
{
	Point32_t temp;
	temp.x = (int32_t)round(g_map.count_x);
	temp.y = (int32_t)round(g_map.count_y);
	return temp;
}

void Map_SetRobotCount(double x, double y) 
{
	g_map.count_x = x;
	g_map.count_y = y;
}

void Map_CountUpdate(double d_x, double d_y) 
{
	g_map.count_x += d_x;
	g_map.count_y += d_y;
}

uint8_t Map_IsTwoCellEqual(Point16_t start_cell,Point16_t exit_cell)
{	
	if(start_cell.x != exit_cell.x)return 0;
	if(start_cell.y != exit_cell.y)return 0;
	return 1;
}

void Map_UpdatePreRobotCell(Point16_t robot_cell)
{
	if(Map_IsTwoCellEqual(robot_cell,g_map.pre_robot_cell))
	{
		g_map.robot_static_cnt++;	
	}
	g_map.pre_robot_cell = robot_cell;
}

Point16_t Map_GetRelativeByCell(Point16_t target_cell,int16_t heading, int16_t offset_lat, int16_t offset_long) 
{
	int32_t temp_x, temp_y;
	Point16_t point;
	
	if(heading < 0)heading =  Math_NormalAngle(heading);
	
	if(heading != 3600) 
	{
		if(heading == 0) 
		{
			g_map.relative_sin = 0;
			g_map.relative_cos = 1;
		} 
		else if(heading == 900) 
		{
			g_map.relative_sin = 1;
			g_map.relative_cos = 0;
		} 
		else if(heading == 1800) 
		{
			g_map.relative_sin = 0;
			g_map.relative_cos = -1;
		} 
		else if(heading == 2700) 
		{
			g_map.relative_sin = -1;
			g_map.relative_cos = 0;
		} 
		else 
		{
			g_map.relative_sin = sin(Math_Deg2Rad(heading, 10));
			g_map.relative_cos = cos(Math_Deg2Rad(heading, 10));
		}
	}

	 temp_x = Map_CellToCount(target_cell.x) + (int32_t)( (((double)offset_long * g_map.relative_cos * CELL_COUNT_MUL) -
	                                      ((double)offset_lat	* g_map.relative_sin * CELL_COUNT_MUL) ) / CELL_SIZE );
	
	 temp_y = Map_CellToCount(target_cell.y) + (int32_t)( ( ((double)offset_long * g_map.relative_sin * CELL_COUNT_MUL) +
	                                      ((double)offset_lat *	g_map.relative_cos * CELL_COUNT_MUL) ) / CELL_SIZE );
	 
	 point.x = Map_CountToCell(temp_x);
	 point.y = Map_CountToCell(temp_y);
	 return point;
}

void Map_Adjust_Mapsize(int16_t x,int16_t y)
{
	if(x < g_map.x_min) 
	{
		g_map.x_min = x;
	} 
	else if(x > g_map.x_max) 
	{
		g_map.x_max = x;
	}
	if(y < g_map.y_min) 
	{
		g_map.y_min = y;
	} 
	else if(y > g_map.y_max) 
	{
		g_map.y_max = y;
	}
}

int16_t Map_CellToMap(int16_t data)
{
	data = data + MAP_SIZE / 2;
	return data;
}

void Map_Reset(void)
{
	uint8_t c, d;
	for(c = 0; c < MAP_SIZE; ++c) 
	{
		for(d = 0; d < (MAP_SIZE + 1) / 4; ++d) 
		{
			g_map.data[c][d] = 0;
		}
	}
}

uint8_t Map_GetMapArray(int16_t x,int16_t y)
{
	uint8_t map_val=0;
	map_val = g_map.data[x][y/4];
	map_val >>= (y%4)*2;
	map_val &= 0x03;
	return map_val;
}

void Map_SetMapArray(int16_t x,int16_t y,uint8_t data)
{
	uint8_t pos = 0;
	pos = (y%4)*2;
	data <<= pos;
	g_map.data[x][y/4] &= ~(0x03<<pos); 
	g_map.data[x][y/4] |= data;
}

void Map_SetCell(int16_t x, int16_t y, CellState_t value) 
{
	Point16_t wifi_point;
	#ifdef DEBUG_REALTIME_MONITOR
	Point16_t real_time_monitor_cell;
	real_time_monitor_cell.x = x;
	real_time_monitor_cell.y = y;
	#endif
	wifi_point.x = x;
	wifi_point.y = y;
	Map_Adjust_Mapsize(x,y);
	x = Map_CellToMap(x);
	y = Map_CellToMap(y);
	if(x >= 0 && x <= MAP_SIZE && y >= 0 && y <= MAP_SIZE) 
	{
		if(Map_GetMapArray(x,y) != value) 
		{
			Map_SetMapArray(x,y,value);
		}
		#ifdef DEBUG_REALTIME_MONITOR
		on_map_realtime_monitor_send_data(real_time_monitor_cell.x,real_time_monitor_cell.y,0,(uint8_t)value);
		#endif
		#ifdef REALMAP_TO_WIFI_ENABLE
		on_map_wifi_realmap_send_point(wifi_point);
		#endif			
	}
}

CellState_t Map_GetCell(int16_t x, int16_t y) 
{
	CellState_t val = CELL_STATE_UNCLEAN;
	x = Map_CellToMap(x);
	y = Map_CellToMap(y);
	if(x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE) 
	{
		val = (CellState_t)Map_GetMapArray(x,y);
	}
	return val;
}

void Map_SetMapEdg(int16_t edg)
{
	g_map.x_edg_min = -edg;
	g_map.x_edg_max = edg;	
	g_map.y_edg_min = -edg;
	g_map.y_edg_max = edg;		
}

uint8_t Map_IsCellInMapEdg(int16_t x,int16_t y)
{
	uint8_t reval = 1;
	if(x <= (g_map.x_edg_min + 2)) reval = 0;
	if(x >= (g_map.x_edg_max - 2)) reval = 0;
	if(y <= (g_map.y_edg_min + 2)) reval = 0;
	if(y >= (g_map.y_edg_max - 2)) reval = 0;
	if(g_map.boundary.arrive_x)
	{
		if(x >= (g_map.x_adjust_max - 2))
		{
			reval = 0;
		}
		if(x <= (g_map.x_adjust_min + 2))
		{
			reval = 0;
		}		
	}	
	if(g_map.boundary.arrive_y)
	{
		if(y >= (g_map.y_adjust_max - 2))
		{
			reval = 0;
		}
		if(y <= (g_map.y_adjust_min + 2))
		{
			reval = 0;
		}		
	}	
	return reval;
}

uint8_t Map_IsCellUncleanWithCleanCnt(int16_t x,int16_t y)
{
    uint8_t reval = 0;

	if(Map_GetCell(x,y) == CELL_STATE_UNCLEAN)
	{
		reval = 1;
	}
	if(*g_map.p_clean_cur_cnt == 2)
	{
		if(Map_GetCell(x,y) == CELL_STATE_CLEANED)
		{
			reval = 1;    
		}
	}
    return reval;
}

uint8_t Map_IsCellBlocked(int16_t x, int16_t y)
{
	uint8_t retval = 0;
	CellState_t cs;

	cs = Map_GetCell(x, y);
	if (cs == CELL_STATE_BLOCKED)retval = 1;
	return retval;
}

uint8_t Map_IsCellCleaned(int16_t x, int16_t y)
{
	uint8_t retval = 0;
	CellState_t cs;

	cs = Map_GetCell(x, y);
	if (cs == CELL_STATE_CLEANED || cs == CELL_STATE_CLEANED_TWICE)retval = 1;
	return retval;
}

uint8_t Map_IsCellAccessible(int16_t x, int16_t y)
{
	int8_t i = 0,j = 0;
	for(i = -1;i <= 1;i++)
	{
		for(j = -1;j <= 1;j++)
		{
			if(Map_GetCell(x + i, y + j) >= CELL_STATE_BLOCKED)
			{	
				return 0;
			}
		}	
	}		
	return 1;
}

uint8_t Map_IsCellUncleanAccessible(int16_t x, int16_t y)
{
	uint8_t retval = 0, count = 0;
    if(!Map_IsCellAccessible(x, y))return 0;

    if(Map_IsCellUncleanWithCleanCnt(x,y))count++;

	if (count >= 1)retval = 1;

	return retval;
}

uint8_t Map_IsCellUncleanLaneAccessible(int16_t x, int16_t y)
{
	uint8_t retval = 0, count = 0;
    if(!Map_IsCellAccessible(x, y))return 0;

    if(Map_IsCellUncleanWithCleanCnt(x,y))count++;

	if(Map_IsCellInMapEdg(x,y + 1))
	{
		if(Map_IsCellUncleanWithCleanCnt(x,y + 1))count++;
	}
	if(Map_IsCellInMapEdg(x,y - 1))
	{
		if(Map_IsCellUncleanWithCleanCnt(x,y - 1))count++;	
	}
    	
	if (count >= 1)retval = 1;

	return retval;
}

uint8_t Map_IsCellBesideBlocked(int16_t x, int16_t y)
{
	uint8_t retval = 0;
	CellState_t cs;

	cs = Map_GetCell(x + 1, y);
	if (cs == CELL_STATE_BLOCKED)retval = 1;

	cs = Map_GetCell(x - 1, y);
	if (cs == CELL_STATE_BLOCKED)retval = 1;

	cs = Map_GetCell(x, y + 1);
	if (cs == CELL_STATE_BLOCKED)retval = 1;

	cs = Map_GetCell(x, y - 1);
	if (cs == CELL_STATE_BLOCKED)retval = 1;

	return retval;
}

uint8_t Map_IsCellBetweenBlockedCells(int16_t x,int16_t y)
{
	if(Map_IsCellBlocked(x,y))return 0;
	if(Map_IsCellBlocked(x + 1,y)&&Map_IsCellBlocked(x - 1,y))return 1;
	if(Map_IsCellBlocked(x,y + 1)&&Map_IsCellBlocked(x,y - 1))return 1;
	return 0;
}

uint8_t Map_IsBlockUncleanAccessible(int16_t x, int16_t y)
{
	uint8_t retval = 0, count = 0;
	int8_t i = 0,j = 0;	
	
	if(!Map_IsCellAccessible(x, y))return 0;

	for(i = -1;i <= 1;i++)
	{
		for(j = -1;j <= 1;j++)
		{
			if (Map_IsCellUncleanWithCleanCnt(x + i, y + j))
			{	
				count++;	
			}			
		}	
	}			
	if (count >= 1)retval = 1;
	
	return retval;
}

void Map_BoundarySet(int16_t edg)
{
	g_map.boundary.west	=-edg;
	g_map.boundary.east = edg;	
	g_map.boundary.south =-edg;
	g_map.boundary.north = edg;		
}

uint8_t Map_BoundaryReach(void) 
{
	uint8_t reval = 0;
	
	if(!g_map.boundary.arrive_x)
	{
		if((g_map.x_max - g_map.x_min) >= MAP_LIMIT)
		{
			g_map.x_adjust_min = g_map.x_min;
			g_map.x_adjust_max = g_map.x_max;
			g_map.boundary.east = g_map.x_adjust_max - 1;
			g_map.boundary.west = g_map.x_adjust_min + 1;				
		    g_map.boundary.arrive_x = 1;
			reval = 1;
		}
	}
	if(!g_map.boundary.arrive_y)
	{
		if((g_map.y_max - g_map.y_min) >= MAP_LIMIT)
		{
			g_map.y_adjust_min = g_map.y_min;
			g_map.y_adjust_max = g_map.y_max;	
			g_map.boundary.north = g_map.y_adjust_max - 1;
			g_map.boundary.south = g_map.y_adjust_min + 1;			
    		g_map.boundary.arrive_y = 1;
			reval = 1;
		}
	}
	return reval;
}

uint8_t Map_BoundaryNear(void) 
{
	uint8_t reval = 0;
	Point16_t robot_cell = Map_GetRobotCell();
	uint16_t robot_heading = on_map_get_angle(0);
	
	if(robot_cell.x >= g_map.boundary.east)
	{
		if((robot_heading < 800)||(robot_heading > 2800))
		{
			g_map.boundary.dir = MAP_BOUNDARY_DIR_EAST;
			reval = 1;			
		}
	}
	if(robot_cell.x <= g_map.boundary.west)
	{
		if((robot_heading > 1000)&&(robot_heading < 2600))
		{
			g_map.boundary.dir = MAP_BOUNDARY_DIR_WEST;
			reval = 1;			
		}
	}				

	if(robot_cell.y >= g_map.boundary.north)
	{
		if((robot_heading > 100)&&(robot_heading < 1700))
		{
			g_map.boundary.dir = MAP_BOUNDARY_DIR_NORTH;
			reval = 1;			
		}
	}
	if(robot_cell.y <= g_map.boundary.south)
	{
		if((robot_heading > 1900)&&(robot_heading < 3500))
		{
    		g_map.boundary.dir = MAP_BOUNDARY_DIR_SOUTH;
			reval = 1;			
		}
	}		

	return reval;
}

Point32_t Map_BoundaryGetPoint(void)
{
	Point16_t target_point;
		
	switch(g_map.boundary.dir)
	{
		case MAP_BOUNDARY_DIR_EAST:
												target_point.x = g_map.boundary.east;
												target_point.y = g_map.boundary.north;
		break;
		case MAP_BOUNDARY_DIR_WEST:
												target_point.x = g_map.boundary.west;
												target_point.y = g_map.boundary.south;																
		break;
		case MAP_BOUNDARY_DIR_SOUTH:
												target_point.x = g_map.boundary.east;
												target_point.y = g_map.boundary.south;																
		break;
		case MAP_BOUNDARY_DIR_NORTH:
												target_point.x = g_map.boundary.west;
												target_point.y = g_map.boundary.north;																
		break;															
		default:break;
	}			

	return Map_CellToPoint(target_point);
}

Point32_t Map_BoundaryGetNextPoint(Point32_t current_target_point)
{
	Point16_t target_cell;
	Point16_t current_target_cell = Map_PointToCell(current_target_point);

	if(current_target_cell.x == g_map.boundary.east && current_target_cell.y == g_map.boundary.north)
	{
		target_cell.x = g_map.boundary.west;
		target_cell.y = g_map.boundary.north;		
	}
	else if(current_target_cell.x == g_map.boundary.west && current_target_cell.y == g_map.boundary.north)
	{
		target_cell.x = g_map.boundary.west;
		target_cell.y = g_map.boundary.south;		
	}
	else if(current_target_cell.x == g_map.boundary.west && current_target_cell.y == g_map.boundary.south)
	{
		target_cell.x = g_map.boundary.east;
		target_cell.y = g_map.boundary.south;			
	}			
	else if(current_target_cell.x == g_map.boundary.east && current_target_cell.y == g_map.boundary.south)
	{
		target_cell.x = g_map.boundary.east;
		target_cell.y = g_map.boundary.north;			
	}
	return Map_CellToPoint(target_cell);
}

void Map_BoundarySetRange(Point16_t robot_cell,uint16_t range)
{
	g_map.boundary.east = robot_cell.x + range;
	g_map.boundary.west = robot_cell.x - range;
	g_map.boundary.north = robot_cell.y + range;
	g_map.boundary.south = robot_cell.y - range;

	if(g_map.boundary.east > MAP_LIMIT_HIGH)g_map.boundary.east = MAP_LIMIT_HIGH;
	if(g_map.boundary.west < MAP_LIMIT_LOW)g_map.boundary.west = MAP_LIMIT_LOW;
	if(g_map.boundary.north > MAP_LIMIT_HIGH)g_map.boundary.north = MAP_LIMIT_HIGH;
	if(g_map.boundary.south < MAP_LIMIT_LOW)g_map.boundary.south = MAP_LIMIT_LOW;

	if(g_map.boundary.arrive_x)
	{
		if(g_map.boundary.east > (g_map.x_max - 2))g_map.boundary.east = g_map.x_max - 2;
		if(g_map.boundary.west < (g_map.x_min + 2))g_map.boundary.west = g_map.x_min + 2;
	}
	if(g_map.boundary.arrive_y)
	{
		if(g_map.boundary.north > (g_map.y_max - 2))g_map.boundary.north = g_map.y_max - 2;
		if(g_map.boundary.south < (g_map.y_min + 2))g_map.boundary.south = g_map.y_min + 2;		
	}
}

uint8_t Map_IsUncleanCellsOnFront(void)
{
	uint8_t reval = 0;

	Point16_t robot_cell = Map_GetRobotCell();

	Point16_t temp_cell;
	temp_cell = Map_GetRelativeByCell(robot_cell, on_map_get_angle(0),0,2);

    reval = Map_IsCellUncleanWithCleanCnt(temp_cell.x,temp_cell.y);

	temp_cell = Map_GetRelativeByCell(robot_cell, on_map_get_angle(0),0,3);

    reval = Map_IsCellUncleanWithCleanCnt(temp_cell.x,temp_cell.y);

	return reval;
}

void Map_NormalInitialize(void) 
{
	Map_Reset();
	Map_SetMapEdg(MAP_LIMIT);
	Map_BoundarySet(MAP_LIMIT - 2);
	Map_ResetLimit(MAP_LIMIT_CLEAN);    
	g_map.x_min = g_map.x_max = g_map.y_min = g_map.y_max = 0;
	g_map.boundary.arrive_x = 0,g_map.boundary.arrive_y = 0;
	g_map.x_adjust_min = 0,g_map.x_adjust_max = 0,g_map.y_adjust_min = 0,g_map.y_adjust_max = 0;
	g_map.boundary.dir = MAP_BOUNDARY_DIR_NONE;
	g_map.count_x = 0;
	g_map.count_y = 0;
	g_map.home_cell.x = 0;
	g_map.home_cell.y = 0;    
    g_map.robot_static_cnt = 0;
}

void Map_AreaInitialize(void)
{
	Point16_t temp_cell;

	temp_cell.x = 0;
	temp_cell.y = g_map.boundary.north - 2;
	
	Map_SetRobotCell(temp_cell.x,temp_cell.y);
	Map_SetHomeCell(temp_cell.x,temp_cell.y);

	g_map.y_max = g_map.boundary.north;
	g_map.y_min = 0;

	g_map.boundary.arrive_y = 1;

	g_map.y_adjust_max = g_map.boundary.north;
	g_map.y_adjust_min = 0;

	g_map.boundary.north = g_map.y_adjust_max - 2;
	g_map.boundary.south = g_map.y_adjust_min + 2;

}

uint8_t Map_IsReach_NewCell(Point16_t cur_cell,Point16_t *pre_cell)
{
	uint8_t retval = 0;
	
	if((pre_cell->x != cur_cell.x) || (pre_cell->y != cur_cell.y))
	{	
		retval = 1;
		*pre_cell = cur_cell;
	}	
	
	return retval;
}

uint32_t Map_GetCellAcreage(void)
{
	uint32_t acreage_temp = 0;
	int32_t x_sum = 0, y_sum = 0;
	int16_t mx,my;
	for(mx = g_map.x_min;mx < g_map.x_max;mx++)
	{
		for(my = g_map.y_min;my < g_map.y_max;my++)
		{
			if(*g_map.p_clean_cur_cnt == 1)
			{
				if(Map_GetCell(mx,my) == CELL_STATE_CLEANED)
				{
					acreage_temp++;
					x_sum += mx;
					y_sum += my;
				}
			}
			else
			{
				if(Map_GetCell(mx,my) == CELL_STATE_CLEANED_TWICE)
				{
					acreage_temp++;
					x_sum += mx;
					y_sum += my;
				}			
			}			
		}
	}
	return acreage_temp;
}

uint8_t Map_IsRobotNotMoving(void)
{
	Point16_t robot_cell = Map_GetRobotCell();	
	Point16_t temp_cell;
	uint8_t reval = 0;
	Map_UpdatePreRobotCell(robot_cell);
	if(g_map.robot_static_cnt >= 3)
	{
		g_map.robot_static_cnt = 0;
		temp_cell = Map_GetRelativeByCell(robot_cell, on_map_get_angle(0),0,2);
		Map_SetCell(temp_cell.x,temp_cell.y,CELL_STATE_BLOCKED);
		Usprintf("Robot is Not Moving\n");
		reval = 1;
	}
	return reval;
}

void Map_SetBlockedByOffset(int16_t offset_lat,int16_t offset_long)
{
	Point16_t offset;
	
	offset = Map_GetRelativeByCell(Map_GetRobotCell(), on_map_get_angle(0),offset_lat,offset_long);

	if(Map_GetCell(offset.x,offset.y) != CELL_STATE_BLOCKED)
	{
		Map_SetCell(offset.x,offset.y, CELL_STATE_BLOCKED);
	}		
}

void Map_Rotate_Point(Point32_t origin,Point32_t input,Point32_t *output,int16_t theta)
{
	double de_x = 0,de_y = 0;
	
	de_x = (double)(input.x - origin.x);
	de_y = (double)(input.y - origin.y);
	
	Math_Rotate(&de_x, &de_y, theta);
	
	output->x = origin.x+ (int16_t)round(de_x);
	output->y = origin.y + (int16_t)round(de_y);
}

void Map_Rotate_Cell(Point16_t origin,Point16_t input,Point16_t *output,int16_t theta)
{
	double de_x = 0,de_y = 0;
	
	de_x = (double)(input.x - origin.x);
	de_y = (double)(input.y - origin.y);
	
	Math_Rotate(&de_x, &de_y, theta);
	
	output->x = origin.x + (int16_t)round(de_x);
	output->y = origin.y + (int16_t)round(de_y);
}

void Map_SetCellRobotSizeClean(int16_t x, int16_t y)
{
	for(int8_t i = -1;i <= 1;i++)
	{
		for(int8_t j = -1;j <= 1;j++)
		{	
			Map_SetCellCleaned(x + i,y + j);		
		}
	}
}

void Map_SetCellCleaned(int16_t x, int16_t y)
{
	switch(*g_map.p_clean_cur_cnt)
	{
		case 1:
					if(Map_GetCell(x,y) != CELL_STATE_CLEANED)
					{
						Map_SetCell(x,y,CELL_STATE_CLEANED);
					}
		break;	
		case 2:
					if(Map_GetCell(x,y) != CELL_STATE_CLEANED_TWICE)
					{
						Map_SetCell(x,y,CELL_STATE_CLEANED_TWICE);
					}
		break;
		default:break;	
	}	
}

void Map_Filter(void)
{
	Point16_t robot_cell = Map_GetRobotCell();

	int8_t range = 6;

	int16_t temp_x,temp_y;

	for(temp_x = (robot_cell.x - range);temp_x <= (robot_cell.x + range);temp_x++)
	{
		for(temp_y = (robot_cell.y - range);temp_y <= (robot_cell.y + range);temp_y++)
		{
			if(Map_IsCellBetweenBlockedCells(temp_x,temp_y))
			{
				Map_SetCell(temp_x,temp_y, CELL_STATE_BLOCKED);
			}
		}
	}
}

void Map_SetHomeCell(int16_t x,int16_t y)
{
	g_map.home_cell.x= x;
	g_map.home_cell.y = y;
	Usprintf("set home cell:%d %d\n",x,y);
}

Point16_t Map_GetHomeCell(void)
{
	return g_map.home_cell;
}

uint8_t Map_IsTwoPointsNear(Point32_t pos_a,Point32_t pos_b)
{
	if (Math_Diff_int(pos_a.x, pos_b.x) < 150 && Math_Diff_int(pos_a.y, pos_b.y) < 150)return 1;
	return 0;
}

uint8_t Map_IsRobotNearPosCount(Point32_t point)
{
	Point32_t temp_robot_count = Map_GetRobotCount();
	if ((Math_Diff_int(temp_robot_count.x,point.x) < 400) 
		&& (Math_Diff_int(temp_robot_count.y,point.y) < 400))
	{
		return 1;
	}
	return 0;
}

uint8_t Map_RobotCloseToPosCount(Point32_t point)
{
	Point32_t temp_robot_cnt = Map_GetRobotCount();
	if ((Math_Diff_int(temp_robot_cnt.x,point.x) < CELL_COUNT_MUL_2) // CELL_COUNT_MUL_3_2
		&& (Math_Diff_int(temp_robot_cnt.y,point.y) < CELL_COUNT_MUL_2))//CELL_COUNT_MUL_3_2 
	{
		return 1;        
	}
	return 0;
}

uint16_t Map_GetTwoCellsDistance(Point16_t start_cell,Point16_t exit_cell)
{
	uint16_t d, e;
	
	d = start_cell.x - exit_cell.x;
	e = start_cell.y - exit_cell.y;
	d *= d;
	e *= e;

	return (uint16_t)round(sqrt(d + e));
}

uint32_t Map_GetTwoPointsDistance(Point32_t start_point,Point32_t exit_point)
{
	int32_t d, e;
	
	d = start_point.x - exit_point.x;
	e = start_point.y - exit_point.y;
	d *= d;
	e *= e;

	return (uint32_t)round(sqrt(d + e));
}

int32_t Map_GetRobotToPointAngle(Point32_t point)
{
	int32_t target_angle = 0;
	if(Map_GetTwoPointsDistance(Map_GetRobotCount(),point) <= (MAP_ROBOT_SIZE*CELL_COUNT_MUL))
	{
		target_angle = Math_Course2Dest(Map_GetRobotCellX(),Map_GetRobotCellY(),Map_CountToCell(point.x), Map_CountToCell(point.y));
	}
	else
	{
		target_angle = Math_Course2Dest(Map_GetRobotCountX(), Map_GetRobotCountY(), point.x, point.y);	
	}
	return target_angle;
}	

void Map_ClearSpotCleanCells(int8_t idx)
{
	int16_t x,y;
	for(x = -idx; x <= idx; x++)
	{
		for(y = -idx; y <= idx; y++)
		{
			if(Map_GetCell(x,y) == CELL_STATE_CLEANED)Map_SetCell(x,y,CELL_STATE_UNCLEAN);
		}
	}
}

void Map_ResetMap(int16_t x, int16_t y)
{
	for(int8_t i = -10;i <= 10;i++)
	{
		for(int8_t j = -10;j <= 10;j++)
		{	
			Map_SetCell(x + i,y + j,CELL_STATE_UNCLEAN);	
			osDelay(50);	
		}
	}
}



















