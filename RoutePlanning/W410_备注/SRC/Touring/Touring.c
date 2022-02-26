#include "Include.h"

volatile TouringBackHome_t g_touring_backhome;

void Touring_Start(void)
{
	Robot_MotorsSetState(ENABLE);
	Robot_SensorsSetState(ENABLE);
	CodeShow_SetState(DISABLE);
	Event_BatteryProcess();                               
	Robot_ModeDisplay();
	Error_ResetCode();
	Pump_SetStartControl(ENABLE);	
	BackStation();	
	Wifi_NavStart();
	Align_ResetAlginMap();	
	Map_NormalInitialize();	
	PathList_Clear();
	TargetList_Clear();
	PathPoint_ClearAllPoints();														      		
	CleanCycleCount_Start();
	Wheel_ResetCounts();
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);
	Gyro_Start();
	g_touring_backhome.start_time_init = 0;
}

void Touring_End(void)
{
	Robot_MotorsSetState(DISABLE);
	Robot_SensorsSetState(DISABLE);	
	Gyro_End();	
	Wifi_NavEnd();	   
}

void Touring_GoStation(void)
{
	Mode_SetMode(MODE_GO_HOME);
	PathList_Clear();		
	Pump_SetStartControl(DISABLE);
	Pump_SetState(DISABLE);	
	WaterTank_SetCheckState(DISABLE);
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);	
	if(g_robot.type == ROBOT_W410)g_obs.state = DISABLE;	
	Vacuum_SetSpeed(VAC_SPEED_LOW);
	MainBrush_SetVoltage(MAINBRUSH_BASE_VOLTAGE);
	Wifi_NavEnd();    
}

void Touring_GoStartPoint(void)
{
	PathList_Clear();	
	Pump_SetStartControl(DISABLE);
	Pump_SetState(DISABLE);
	Vacuum_SetSpeed(VAC_SPEED_LOW);
	MainBrush_SetVoltage(MAINBRUSH_BASE_VOLTAGE);
	Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);		
	if(Station_GetStartState())
	{
		Mode_SetMode(MODE_GO_HOME);
		Speaker_Voice(SPK_START_GO_HOME);
		if(g_robot.type == ROBOT_W410)g_obs.state = DISABLE;	
	}
	Wifi_NavEnd();	
	g_touring_backhome.start_time_init = 1;
	g_touring_backhome.start_time = Timer_GetTime();
}

uint8_t Touring_IsBackhomeTimeout(void)
{
	uint8_t reval = 0;
	if(g_touring_backhome.start_time_init)
	{
		if(Timer_Get_DiffNow(g_touring_backhome.start_time) >= TIME_10MIN)
		{
			reval = 1;
		}
	}
	return reval;
}

void Touring_ExploreStation(void)
{
	Map_NormalInitialize();
	Touring_GoStation();	
	Map_SetMapEdg(MAP_LIMIT/4);
	Map_BoundarySet(MAP_LIMIT/4 - 2);	
}

void Touring_GoBoundary(void)
{
	PathList_Clear();
	Gyro_Cal();
}

void Touring_MoveStop(void)
{
	PathList_Clear();
	Gyro_Cal();	
}

void Touring_UpdatePosition(uint16_t heading_0, int16_t heading_1, int16_t left, int16_t right) 
{
	double	dd, temp_rad;
	int16_t path_heading;
	Point16_t cell,cur_cell,target_cell;
	static Point16_t pre_cell;
	int16_t i;
	static uint32_t debug_time;
	if(!g_robot.update_position)
	{
		pre_cell.x = 0 ;
		pre_cell.y = 0 ; 
		return;
	}
	
	if(Moving_GetType() == MOVING_DOCKING)
	{
		return;
	}

	if (left == 0 && right == 0)return; 
	
	if ((heading_0 > heading_1) && ((heading_0 - heading_1) > 1800)) 
	{
		path_heading = (uint16_t)((heading_0 + heading_1 + 3600) >> 1) % 3600;
	} 
	else if ((heading_1 > heading_0) && ((heading_1 - heading_0) > 1800)) 
	{
		path_heading = (uint16_t)((heading_0 + heading_1 + 3600) >> 1) % 3600;
	} 
	else 
	{
		path_heading = (uint16_t)(heading_0 + heading_1) >> 1;
	}

	Wheel_SetCount(Wheel_GetLeftCount() - left,Wheel_GetRightCount() - right);

	dd = left + right;
	dd /= 2;

	temp_rad = Math_Deg2Rad(path_heading, 10);
	Map_CountUpdate(dd * cos(temp_rad), dd * sin(temp_rad));
	
	cur_cell = Map_GetRobotCell();
	
	if(Moving_GetType() == MOVING_MOVO2POINT)
	{
		target_cell = Map_PointToCell(Action_GetTargetPoint());		
		if(Math_Diff_int(target_cell.x,cur_cell.x) >= MAP_ROBOT_SIZE)
		{
			if(cur_cell.y != target_cell.y)
			{
				cur_cell.y = target_cell.y; 
			}
		}
		else if(Math_Diff_int(target_cell.y,cur_cell.y) >= MAP_ROBOT_SIZE)
		{
			if(cur_cell.x != target_cell.x)
			{
				cur_cell.x = target_cell.x; 
			}		
		}		
	}

	for(i = 1;i >= -1;i--)
	{
		cell = Map_GetRelativeByCell(cur_cell,heading_0,i*CELL_SIZE ,CELL_SIZE);

		Map_SetCellCleaned(cell.x,cell.y);				
	}

	if(Timer_IsTimeReach(&debug_time,TIME_1S))//show robot position
	{
		#ifdef DEBUG_REALTIME_MONITOR
		RealTimeMonitor_SendData(cur_cell.x,cur_cell.y,Gyro_GetAngle(0),7);
		if(g_cleancycle_count.cur_cnt == 1)
		{
			RealTimeMonitor_SendData(pre_cell.x,pre_cell.y,Gyro_GetAngle(0),1);
		}
		else
		{
			RealTimeMonitor_SendData(pre_cell.x,pre_cell.y,Gyro_GetAngle(0),2);
		}
		#else
		pre_cell.x++;
		pre_cell.y++;
		#endif
		pre_cell.x = cur_cell.x;
		pre_cell.y = cur_cell.y;
	}

	Wifi_UpdateRobot(cur_cell,heading_0);

	Wifi_UpdateRealMapEdg(g_map.x_min,g_map.x_max,g_map.y_min,g_map.y_max);		
}

void Touring_UpdateMapBumper(BumperTrig_t bumper)
{
	if((bumper & BUMPER_TRIG_RIGHT) && (bumper & BUMPER_TRIG_LEFT)) 
	{
		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);		
//		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*3);
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*3);				
	} 
	else if (bumper & BUMPER_TRIG_LEFT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*3);		
	} 
	else if (bumper & BUMPER_TRIG_RIGHT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);	
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*3);		
	}
}

void Touring_UpdateMapCliff(CliffTrig_t cliff)
{
	if (cliff & CLIFF_TRIG_LEFT) 
	{
		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
	} 
	if (cliff & CLIFF_TRIG_FRONT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
	}	
	if (cliff & CLIFF_TRIG_RIGHT) 
	{
		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
	}	
}

void Touring_UpdateMapCharge(void)
{
	Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
	Map_SetBlockedByOffset(0, CELL_SIZE*2);
	Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);
}

void Touring_UpdateMapObs(ObsTrig_t obs)
{	
	if(obs & OBS_TRIG_LEFT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*3);		
	} 
	if (obs & OBS_TRIG_FRONT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*2);
		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);	
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(CELL_SIZE, CELL_SIZE*3);
//		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*3);		
	}
	if (obs & OBS_TRIG_RIGHT) 
	{
		Map_SetBlockedByOffset(0, CELL_SIZE*2);
		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*2);
//		Map_SetBlockedByOffset(0, CELL_SIZE*3);
//		Map_SetBlockedByOffset(-CELL_SIZE, CELL_SIZE*3);		
	}	
}

void Touring_UpdateMapWall(WallDir_t dir)
{
	if((dir == WALLDIR_EAST_LEFT)||(dir == WALLDIR_WEST_LEFT))
	{
		Map_SetBlockedByOffset(2*CELL_SIZE,0);
		Map_SetBlockedByOffset(2*CELL_SIZE,CELL_SIZE);		
	}
	else
	{
		Map_SetBlockedByOffset(-2*CELL_SIZE,0);
		Map_SetBlockedByOffset(-2*CELL_SIZE,CELL_SIZE);		
	}
}

WallDir_t Touring_GetWallDir(Point16_t target_cell)
{
	WallDir_t ret_val = WALLDIR_NONE;
	if((Gyro_GetAngle(0) < 900)|| (Gyro_GetAngle(0) > 2700))
	{
		if(target_cell.y > Map_GetRobotCellY())
		{
			ret_val = WALLDIR_EAST_RIGHT;
		}
		else
		{
			ret_val = WALLDIR_EAST_LEFT;
		}
	}
	else 
	{
		if(target_cell.y > Map_GetRobotCellY())
		{
			ret_val = WALLDIR_WEST_LEFT;
		}
		else
		{
			ret_val = WALLDIR_WEST_RIGHT;
		}
	}
	return ret_val;
}

























