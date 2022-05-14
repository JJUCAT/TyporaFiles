#include "Include.h"

TouringResult_t WallFollow(Point32_t start_cnt, WallDir_t wall_dir, WallFollow_t wallfollow_state, Touring_t touring)
{
	TouringResult_t touring_result = TOURING_RESULT_NONE, arrived_flag = TOURING_RESULT_NONE;
	uint8_t temp_exception_state = 0;
	uint8_t wall_mode_is_find_obstacle = 0;
	uint8_t is_find_straight_wall = 0;
	BumperTrig_t bumper_trigger_status = BUMPER_TRIG_NO, bumper_buffer = BUMPER_TRIG_NO;
	ObsTrig_t obs_trigger_status = OBS_TRIG_NONE;
	CliffTrig_t cliff_trigger_status = CLIFF_TRIG_NO, cliff_buffer = CLIFF_TRIG_NO;
	Point32_t robot_pos_buffer = Map_GetRobotCount();
	Point16_t start_cell = Map_GetRobotCell();
	MoveAct_t turn_dir = MOVE_ACT_TURN_LEFT;
	uint8_t new_cell = 0, new_cell_cnt = 0;
	Point16_t cur_cell, pre_cell = Map_GetRobotCell(),spot_start_cell;
	uint8_t wall_cell_cnt = 0;
	uint8_t spot_current_lane = 0, spot_target_lane = 0;
	uint8_t near_home_cnt = 0;
	uint16_t bumper_turn_angle = WALLFOLLOW_TURN_ANGLE;
	uint32_t search_charger_time = 0, out_trap_time = 0, wall_start_t = 0, round_stop_time = 0;
	uint32_t temp_rcon_status = 0, wall_rcon_status = 0, pre_wall_rcon_status = 0;
	uint8_t round_stop_flag = 0;
	int32_t target_angle = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

	Usprintf("---------WallFollow Start-----------\n");
	Moving_SetType(MOVING_WALLFOLLOW);
	Map_Filter();
	Wall_SetDynamicState(DISABLE);
	Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
	Wall_ResetDistance();
	Action_ListClear();
	Rcon_ResetStatus();
	search_charger_time = Timer_GetTime();
	switch (wallfollow_state)
	{
	case WALLFOLLOW_FIND:
		Usprintf("WALLFOLLOW_FIND\n");
		wall_start_t = Timer_GetTime();
		Align_ResetSmoothAngleCnt();
		Align_ResetCurveAngleCnt();
		Action_ListAdd(MOVE_ACT_GYRO_FORWARD, Gyro_GetAngle(0), GYROFORWARD_SPEED, GYROFORWARD_SPEED);
		break;
	case WALLFOLLOW_MODE:
		Usprintf("WALLFOLLOW_MODE\n");
		wall_start_t = Timer_GetTime();
		Action_ListAdd(MOVE_ACT_GYRO_FORWARD, Gyro_GetAngle(0), GYROFORWARD_SPEED, GYROFORWARD_SPEED);
		break;

	case WALLFOLLOW_TRAPPED:
		Usprintf("WALLFOLLOW_TRAPPED\n");
		Action_ListAdd(MOVE_ACT_TURN_LEFT, WALLFOLLOW_TURN_ANGLE, TURN_SPEED, TURN_SPEED);
		Action_ListAdd(MOVE_ACT_FORWARD, MAX_DISTANCE, FORWARD_SPEED, FORWARD_SPEED);
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT, 0, 0, 0);
		xSemaphoreTake(g_binary_wallcalculate_found, 0);
		out_trap_time = Timer_GetTime();
		bumper_turn_angle = WALLFOLLOW_TRAP_TURN_ANGLE;
		break;
	case WALLFOLLOW_NEXTLANE:
		Usprintf("WALLFOLLOW_NEXTLANE\n");
		if ((wall_dir == WALLDIR_EAST_LEFT) || (wall_dir == WALLDIR_WEST_LEFT))
		{
			Action_ListAdd(MOVE_ACT_TURN_RIGHT, WALLFOLLOW_TURN_ANGLE, TURN_SPEED, TURN_SPEED);
			Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
			Action_ListAdd(MOVE_ACT_STRAIGHT,STRAIGHT_DISTANCE,STRAIGHT_SPEED,STRAIGHT_SPEED);
			Action_ListAdd(MOVE_ACT_FOLLOW_WALL_LEFT, 0, 0, 0);
		}
		else
		{
			Action_ListAdd(MOVE_ACT_TURN_LEFT, WALLFOLLOW_TURN_ANGLE, TURN_SPEED, TURN_SPEED);
			Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
			Action_ListAdd(MOVE_ACT_STRAIGHT,STRAIGHT_DISTANCE,STRAIGHT_SPEED,STRAIGHT_SPEED);
			Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT, 0, 0, 0);
		}
		break;
	case WALLFOLLOW_SPOT: 
		spot_start_cell = Map_GetRobotCell();
		spot_current_lane = Math_GetMax(Math_Absolute(spot_start_cell.x),Math_Absolute(spot_start_cell.y));
		spot_target_lane =  spot_current_lane + (Spot_GetDirOutin() == SPOT_DIR_OUT ?1:-1);
		Action_ListAdd(MOVE_ACT_TURN_LEFT,WALLFOLLOW_TURN_ANGLE,TURN_SPEED,TURN_SPEED);
		Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT,0,0,0);	
		break;
	case WALLFOLLOW_CHARGE:
		Usprintf("WALLFOLLOW_CHARGE\n");
		cur_cell = Map_GetRobotCell();
		Action_ListAdd(MOVE_ACT_TURN_ROUND, 7200, STATION_TURN_SPEED, STATION_TURN_SPEED);
		Action_ListAdd(MOVE_ACT_FORWARD, MAX_DISTANCE, WALL_FOLLOW_SPEED, WALL_FOLLOW_SPEED);
		Map_BoundarySetRange(cur_cell, 10);
		if (g_robot.type == ROBOT_W410)
			g_obs.state = DISABLE;
		break;
	default:
		break;
	}
	Action_SetMove(MOVE_ACT_HANDLER);
	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		cur_cell = Map_GetRobotCell();
		new_cell = Map_IsReach_NewCell(cur_cell, &pre_cell) ? 1 : 0;
		if (new_cell)
		{
			if (wall_cell_cnt < 250)
				wall_cell_cnt++;
			if (new_cell_cnt < 250)
				new_cell_cnt++;
			if (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_LEFT || Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT)
			{
				Touring_UpdateMapWall(wall_dir);
			}
		}
		switch (Action_GetMove())
		{
		case MOVE_ACT_GYRO_FORWARD:
		case MOVE_ACT_FOLLOW_BOUNDARY:
		case MOVE_ACT_FOLLOW_WALL_LEFT:
		case MOVE_ACT_FOLLOW_WALL_RIGHT:
			break;
		case MOVE_ACT_STRAIGHT:
			Action_WaitForMoveEnded();
			break;
		case MOVE_ACT_DECELERATE:
			Action_WaitForDecelerateEnded();
			break;
		case MOVE_ACT_TURN_RIGHT:
			Action_WaitForMoveEnded();
			if(Cliff_ContinueTrig())
			{
				Error_SetCode(ERROR_CLIFF);
			}				
			break;
		case MOVE_ACT_TURN_LEFT:
			Action_WaitForMoveEnded();
			if(Cliff_ContinueTrig())
			{
				Error_SetCode(ERROR_CLIFF);
			}				
			break;
		case MOVE_ACT_HEAD2COURCE:
			Action_WaitForHeadToCourseEnded();
			break;
		case MOVE_ACT_FORWARD:
			Action_WaitForMoveEnded();
			if (wallfollow_state == WALLFOLLOW_CHARGE)
			{
				temp_rcon_status = Rcon_GetStatus();
				if (temp_rcon_status & RCON_FL_HOME_L)
				{
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_TURN_RIGHT, 600, TURN_SPEED, TURN_SPEED);
					Action_ListAdd(MOVE_ACT_STATIC, 0, 0, 0);
					Action_SetMove(MOVE_ACT_HANDLER);
					arrived_flag = TOURING_RESULT_FIND_CHARGE;
				}
				else if (temp_rcon_status & RCON_FL_HOME_R)
				{
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_TURN_LEFT, 600, TURN_SPEED, TURN_SPEED);
					Action_ListAdd(MOVE_ACT_STATIC, 0, 0, 0);
					Action_SetMove(MOVE_ACT_HANDLER);
					arrived_flag = TOURING_RESULT_FIND_CHARGE;
				}
			}
			break;
		case MOVE_ACT_BACK:
			if (Action_WaitForMoveEnded())
			{
				if (bumper_buffer || cliff_buffer)
				{
					if (bumper_buffer)
					{
						Touring_UpdateMapBumper(bumper_buffer);
						bumper_buffer = BUMPER_TRIG_NO;
					}
					if (cliff_buffer)
					{
						Touring_UpdateMapCliff(cliff_buffer);
						cliff_buffer = CLIFF_TRIG_NO;
					}
					if (wallfollow_state == WALLFOLLOW_FIND)
					{
						Align_CollectCurveAngle(Map_GetRobotCount());
						if (Align_CorrectLine(ALIGN_CORRECT_LINE_CURVE))
						{
							is_find_straight_wall = 1;
						}
					}
				}
				cliff_trigger_status = Cliff_GetTrigStatus();
				if (cliff_trigger_status)
					Error_SetCode(ERROR_CLIFF);
				if (Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)
					Error_SetCode(ERROR_PICK_UP);
			}
			break;
		case MOVE_ACT_TURN_ROUND:
			Action_WaitForMoveEnded();
			if (!round_stop_flag)
			{
				temp_rcon_status = Rcon_GetStatus();
				if ((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_L))
				{
					round_stop_flag = 1;
				}
				else if ((temp_rcon_status & RCON_FL_HOME_R) && (temp_rcon_status & RCON_FR_HOME_R))
				{
					round_stop_flag = 1;
				}
				else if ((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_R))
				{
					round_stop_flag = 1;
				}
				if (round_stop_flag)
				{
					round_stop_time = Timer_GetTime();
					Wheel_SetTargetSpeed(0, 0);
					Rcon_ResetStatus();
				}
			}
			else
			{
				if (Timer_Get_DiffNow(round_stop_time) >= TIME_2S)
				{
					temp_rcon_status = Rcon_GetStatus();
					if (((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_L)) || ((temp_rcon_status & RCON_FL_HOME_R) && (temp_rcon_status & RCON_FR_HOME_R)) || ((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_R)))
					{
						Action_Static();
						arrived_flag = TOURING_RESULT_FIND_CHARGE;
						Usprintf("Find station signal\n");
					}
					else
					{
						round_stop_flag = 0;
						Wheel_SetTargetSpeed(STATION_TURN_SPEED, STATION_TURN_SPEED);
					}
				}
			}
			break;
		case MOVE_ACT_HANDLER:
			Action_ListSwitch();
			break;
		case MOVE_ACT_STATIC:
			Action_ListSwitch();
			if (arrived_flag)
			{
				Gyro_Cal();
				touring_result = arrived_flag;
			}
			break;
		default:
			Action_SetMove(MOVE_ACT_STATIC);
			break;
		}

		if (Action_GetMove() != MOVE_ACT_BACK)
		{
			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_LEFT) || (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) ||
				(Action_GetMove() == MOVE_ACT_GYRO_FORWARD) || (Action_GetMove() == MOVE_ACT_STRAIGHT))
			{
				obs_trigger_status = OBS_GetTrigStatus();
				OBS_IsTrigLimit(&obs_trigger_status);
				if (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_LEFT)
				{
					obs_trigger_status &= ~OBS_TRIG_LEFT;
				}
				if (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT)
				{
					obs_trigger_status &= ~OBS_TRIG_RIGHT;
				}
			}
			else
			{
				obs_trigger_status = OBS_TRIG_NONE;
			}
			if (wallfollow_state == WALLFOLLOW_CHARGE)
			{
				obs_trigger_status = OBS_TRIG_NONE;
			}
			if (obs_trigger_status)
			{
				turn_dir = ((wall_dir == WALLDIR_WEST_LEFT) || (wall_dir == WALLDIR_EAST_LEFT)) ? MOVE_ACT_TURN_RIGHT : MOVE_ACT_TURN_LEFT;
				Wheel_Stop();
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
				Action_ListAdd(turn_dir, TURN_ANGLE_OBS, TURN_SPEED, TURN_SPEED);
				Action_ListStraightAndWallFollow(MOVE_ACT_DECELERATE, 200, wall_dir);
				if ((obs_trigger_status & OBS_TRIG_LEFT))
				{
					Touring_UpdateMapObs(OBS_TRIG_LEFT);
				}
				if (obs_trigger_status & OBS_TRIG_FRONT)
				{
					Touring_UpdateMapObs(OBS_TRIG_FRONT);
				}
				if ((obs_trigger_status & OBS_TRIG_RIGHT))
				{
					Touring_UpdateMapObs(OBS_TRIG_RIGHT);
				}
			}

			bumper_trigger_status = Bumper_GetTrigStatus();
			if (bumper_trigger_status)
			{
				Usprintf("Wall Follow Bumper Event\n");
				bumper_buffer = bumper_trigger_status;
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_BACK, 100, BACK_SPEED, BACK_SPEED);
				Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
				turn_dir = ((wall_dir == WALLDIR_WEST_LEFT) || (wall_dir == WALLDIR_EAST_LEFT)) ? MOVE_ACT_TURN_RIGHT : MOVE_ACT_TURN_LEFT;
				if (turn_dir == MOVE_ACT_TURN_LEFT)
				{
					if (bumper_trigger_status & BUMPER_TRIG_LEFT)
					{
						Wall_ResetDistance();
						Action_ListAdd(turn_dir, bumper_turn_angle, TURN_SPEED, TURN_SPEED);
					}
					else
					{
						Action_ListAdd(turn_dir, bumper_turn_angle, TURN_SPEED, TURN_SPEED);
					}
				}
				else
				{
					if (bumper_trigger_status & BUMPER_TRIG_RIGHT)
					{
						Wall_ResetDistance();
						Action_ListAdd(turn_dir, bumper_turn_angle, TURN_SPEED, TURN_SPEED);
					}
					else
					{
						Action_ListAdd(turn_dir, bumper_turn_angle, TURN_SPEED, TURN_SPEED);
					}
				}
				Action_ListStraightAndWallFollow(MOVE_ACT_DECELERATE, STRAIGHT_DISTANCE, wall_dir);
				Wall_DistanceDecrease();
				if ((wallfollow_state == WALLFOLLOW_CHARGE) || (Station_GetStartState() && (wallfollow_state == WALLFOLLOW_TRAPPED)))
				{
					if (!arrived_flag)
					{
						wall_rcon_status = Rcon_GetStatus();
						temp_rcon_status = wall_rcon_status | pre_wall_rcon_status;
						if (((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_R)) || ((temp_rcon_status & RCON_FL_HOME_L) && (temp_rcon_status & RCON_FR_HOME_R)) || ((wall_rcon_status & RCON_FL_HOME_L) && (wall_rcon_status & RCON_FR_HOME_L)) || ((wall_rcon_status & RCON_FL_HOME_R) && (wall_rcon_status & RCON_FR_HOME_R)))
						{
							Usprintf("Wall Find charger\n");
							Action_ListBackToStop(200);
							arrived_flag = TOURING_RESULT_FIND_CHARGE;
						}
						pre_wall_rcon_status = wall_rcon_status;
						Rcon_ResetStatus();
					}
				}
			}
			cliff_trigger_status = Cliff_GetTrigStatus();
			if (cliff_trigger_status)
			{
				cliff_buffer = cliff_trigger_status;
				Action_ListClear();
				switch (cliff_trigger_status)
				{
				case CLIFF_TRIG_LEFT:
					Action_ListAdd(MOVE_ACT_BACK, 800, BACK_SPEED, BACK_SPEED / 4);
					break;
				case CLIFF_TRIG_RIGHT:
					Action_ListAdd(MOVE_ACT_BACK, 800, BACK_SPEED / 4, BACK_SPEED);
					break;
				case CLIFF_TRIG_FRONT:
					Action_ListAdd(MOVE_ACT_BACK, 300, BACK_SPEED, BACK_SPEED);
					break;
				default:
					break;
				}
				Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
				turn_dir = ((wall_dir == WALLDIR_WEST_LEFT) || (wall_dir == WALLDIR_EAST_LEFT)) ? MOVE_ACT_TURN_RIGHT : MOVE_ACT_TURN_LEFT;

				if (turn_dir == MOVE_ACT_TURN_LEFT)
				{
					if (cliff_trigger_status & CLIFF_TRIG_RIGHT)
					{
						Action_ListAdd(turn_dir, TURN_ANGLE_CLIFF, TURN_SPEED, TURN_SPEED);
					}
					else
					{
						Action_ListAdd(turn_dir, TURN_ANGLE_CLIFF, TURN_SPEED, TURN_SPEED);
					}
				}
				else
				{
					if (cliff_trigger_status & CLIFF_TRIG_LEFT)
					{
						Action_ListAdd(turn_dir, TURN_ANGLE_CLIFF, TURN_SPEED, TURN_SPEED);
					}
					else
					{
						Action_ListAdd(turn_dir, TURN_ANGLE_CLIFF, TURN_SPEED, TURN_SPEED);
					}
				}
				Action_ListStraightAndWallFollow(MOVE_ACT_DECELERATE, 200, wall_dir);
				if (Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)
				{
					Action_Static();
					Error_SetCode(ERROR_PICK_UP);
					Usprintf("Pick Up Event\n");
				}
			}
			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_LEFT) || (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) || (Action_GetMove() == MOVE_ACT_FOLLOW_BOUNDARY) || (Action_GetMove() == MOVE_ACT_GYRO_FORWARD) || (Action_GetMove() == MOVE_ACT_STRAIGHT) || (Action_GetMove() == MOVE_ACT_FORWARD))
			{
				if (wallfollow_state != WALLFOLLOW_MODE)
				{
					if (Map_BoundaryReach())
					{
						Usprintf("Boundary Reach\n");
						Action_Static();
						arrived_flag = TOURING_RESULT_BOUNDARY;
					}
				}
			}
		}
		switch (wallfollow_state)
		{
		case WALLFOLLOW_FIND:
			if (Action_GetMove() != MOVE_ACT_GYRO_FORWARD)
			{
				if (Timer_Get_DiffNow(wall_start_t) >= TIME_40S)
				{
					Usprintf("Robot Search Wall For 40s,Stop and Touring\n");
					is_find_straight_wall = 1;
				}
			}

			if ((Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) > 3600) || (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) < -3600))
			{
				Usprintf("Robot Circling\n");
				is_find_straight_wall = 1;
			}
			if (new_cell)
			{
				if (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT)
				{
					Align_CollectSmoothAngle(Gyro_GetAngle(0));
					if (Align_CorrectLine(ALIGN_CORRECT_LINE_SMOOTH))
					{
						Usprintf("Robot Find Smooth Line\n");
						is_find_straight_wall = 1;
					}
				}
			}
			temp_rcon_status = Rcon_GetStatus();
			if ((temp_rcon_status & RCON_FR_HOME_L) || (temp_rcon_status & RCON_FR_HOME_R))
			{
				is_find_straight_wall = 1;
				Usprintf("charge detect\n");
			}
			if (is_find_straight_wall)
			{
				is_find_straight_wall = 0;
				Action_Static();
				arrived_flag = TOURING_RESULT_FINDWALL;
			}
			break;
		case WALLFOLLOW_TRAPPED:
			if (new_cell)
			{
				xSemaphoreGive(g_binary_wallcalculate_start);
			}
			if (Timer_Get_DiffNow(out_trap_time) > TIME_6MIN) 
			{
				if (touring == TOURING_ING)
				{
					Action_Static();
					arrived_flag = TOURING_RESULT_TRAP;
					Usprintf("Touring Wall Out Trap but 6 MIN\n");
				}
				else if (touring == TOURING_BACK_HOME)
				{
					if (Station_GetStartState())
					{
						Action_Static();
						arrived_flag = TOURING_RESULT_TIME_OUT;
						Usprintf("Back Home Wall Out Trap Time Out\n");
					}
					else
					{
						Mode_SetMode(MODE_USERINTERFACE);
						Usprintf("Back Home Wall Out Trap To Userinterface\n");
					}					
				}
				else if (touring == TOURING_FIND_CHARGER)
				{
					Action_Static();
					arrived_flag = TOURING_RESULT_TIME_OUT;
					Usprintf("Find Charger Wall Out Trap but 6 MIN\n");
				}
			}
			if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) > 4800)
			{
				Usprintf("Wall Out Trap but Robot circling\n");
				Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_FORWARD, MAX_DISTANCE, FORWARD_SPEED, FORWARD_SPEED);
				Action_SetMove(MOVE_ACT_HANDLER);
			}
			if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) < -7800)
			{
				Usprintf("Wall Out Trap,Back to Userinterface!\n");
				Error_SetCode(ERROR_STUCK);
				Mode_SetMode(MODE_USERINTERFACE);
			}
			if (xSemaphoreTake(g_binary_wallcalculate_found, 0) == pdPASS)
			{
				Action_ListDecelerateToStop();
				Usprintf("Wall Out Trap,Shortest Way Found\n");
				arrived_flag = TOURING_RESULT_ARRIVED;
			}
			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) || (Action_GetMove() == MOVE_ACT_GYRO_FORWARD) || (Action_GetMove() == MOVE_ACT_STRAIGHT) || (Action_GetMove() == MOVE_ACT_FORWARD))
			{
				if (Map_BoundaryNear())
				{
					Usprintf("Boundary Near\n");
					Wheel_Stop();
					Action_SetTargetPoint(Map_BoundaryGetPoint());
					target_angle = Map_GetRobotToPointAngle(Map_BoundaryGetPoint());
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_HEAD2COURCE, target_angle, HEAD2COURSE_SPEED, HEAD2COURSE_SPEED);
					Action_ListAdd(MOVE_ACT_FOLLOW_BOUNDARY, target_angle, BOUNDARY_SPEED, BOUNDARY_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
				}
			}
			break;
		case WALLFOLLOW_NEXTLANE:
			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_LEFT) || (Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) || (Action_GetMove() == MOVE_ACT_FOLLOW_BOUNDARY) ||
				(Action_GetMove() == MOVE_ACT_STRAIGHT) || (Action_GetMove() == MOVE_ACT_FORWARD))
			{
				if (Map_BoundaryNear())
				{
					Usprintf("Boundary Near\\n");
					Action_Static();
					arrived_flag = TOURING_RESULT_ARRIVED;
				}
				if (Action_GetMove() != MOVE_ACT_STRAIGHT)
				{
					if (cur_cell.y != start_cell.y)
					{
						if (Math_IsEven(cur_cell.y))
						{
							Usprintf("New Lane\n");
							if (g_robot.type == ROBOT_W400)
							{
								Action_ListWallOffEdgeStop(wall_dir);
							}
							else
							{
								Action_Static();
							}
							arrived_flag = TOURING_RESULT_ARRIVED;
						}
					}
					if (cur_cell.y == start_cell.y)
					{
						if (Math_Diff_int(cur_cell.x, start_cell.x) >= 2)
						{
							if (Heading_GetHeading8() == HEADING_DIR_NORTH || Heading_GetHeading8() == HEADING_DIR_SOUTH)
							{
								Usprintf("Back Lane\n");
								if (g_robot.type == ROBOT_W400)
								{
									Action_ListWallOffEdgeStop(wall_dir);
								}
								else
								{
									Action_Static();
								}
								arrived_flag = TOURING_RESULT_ARRIVED;
							}
						}
					}
				}
			}
			if (!arrived_flag)
			{
				temp_rcon_status = Rcon_GetStatus();
				if ((wall_dir == WALLDIR_WEST_LEFT) || (wall_dir == WALLDIR_EAST_LEFT))
				{
					if ((temp_rcon_status & RCON_FL_HOME_L) || (temp_rcon_status & RCON_FL_HOME_R))
					{
						if (g_robot.type == ROBOT_W400)
						{
							Action_ListWallOffEdgeStop(wall_dir);
						}
						else
						{
							Action_Static();
						}
						arrived_flag = TOURING_RESULT_ARRIVED;
					}
				}
				else
				{
					if ((temp_rcon_status & RCON_FR_HOME_L) || (temp_rcon_status & RCON_FR_HOME_R))
					{
						if (g_robot.type == ROBOT_W400)
						{
							Action_ListWallOffEdgeStop(wall_dir);
						}
						else
						{
							Action_Static();
						}
						arrived_flag = TOURING_RESULT_ARRIVED;
					}
				}
			}
			break;
		case WALLFOLLOW_MODE:
			if ((Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) >= 3600) || (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) <= -3600))
			{
				if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) < -3600)
				{
					if (new_cell)
					{
						if (Map_GetTwoCellsDistance(cur_cell,Map_GetHomeCell()) <= 3)
						{
							near_home_cnt++;
							if (near_home_cnt >= 3)
							{
								near_home_cnt = 0;
								if (!arrived_flag)
								{
									Action_Static();
									arrived_flag = TOURING_RESULT_WALL_BACK_START;
									Usprintf("Wall Follow For One Circle,back start point\n");	
								}								
							}
						}
					}
				}
				if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) > 4800)
				{
					Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_TURN_LEFT, 1800, TURN_SPEED, TURN_SPEED);
					Action_ListAdd(MOVE_ACT_DECELERATE, 0, 0, 0);
					Action_ListAdd(MOVE_ACT_FORWARD, MAX_DISTANCE, FORWARD_SPEED, FORWARD_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
					Usprintf("Wall Follow Island\n");
				}
			}
			if (Timer_Get_DiffNow(wall_start_t) >= (TIME_1MIN * 10))
			{	
				if (!arrived_flag)
				{
					Action_Static();
					arrived_flag = TOURING_RESULT_TIME_OUT;
					Usprintf("Wall Follow For 10 min\n");
				}
			}
			if (obs_trigger_status || bumper_trigger_status || cliff_trigger_status)
			{
				if (!wall_mode_is_find_obstacle)
				{
					wall_mode_is_find_obstacle = 1;
					Map_SetHomeCell(cur_cell.x, cur_cell.y);
					Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
				}
			}
			break;
		case WALLFOLLOW_SPOT:
			if (Spot_IsOnLane(spot_target_lane,Map_GetRobotCell()))
			{
				Usprintf("Spot Arrive Target Lane\n");
				Action_Static();
				touring_result = TOURING_RESULT_ARRIVED;
			}
			if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) < -3600)
			{
				if (Spot_GetDirOutin() == SPOT_DIR_IN)
				{
					Usprintf("Spot wall circle\n");
					Mode_SetMode(MODE_USERINTERFACE);
				}
				else
				{
					Usprintf("Spot wall circle\n");
					Spot_SetDirOutin(SPOT_DIR_IN);					
					Action_Static();
					touring_result = TOURING_RESULT_ARRIVED;
				}
			}
			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) || (Action_GetMove() == MOVE_ACT_GYRO_FORWARD) || (Action_GetMove() == MOVE_ACT_STRAIGHT) || (Action_GetMove() == MOVE_ACT_FORWARD))
			{
				if (Map_BoundaryNear())
				{
					Usprintf("Boundary Near\n");
					Wheel_Stop();
					Action_SetTargetPoint(Map_BoundaryGetPoint());
					target_angle = Map_GetRobotToPointAngle(Map_BoundaryGetPoint());
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_HEAD2COURCE, target_angle, HEAD2COURSE_SPEED, HEAD2COURSE_SPEED);
					Action_ListAdd(MOVE_ACT_FOLLOW_BOUNDARY, target_angle, BOUNDARY_SPEED, BOUNDARY_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
				}
			}			
			break;
		case WALLFOLLOW_CHARGE:
			if (wall_dir == WALLDIR_WEST_LEFT)
			{
				if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) < -4800)
				{
					Usprintf("Robot circling\n");
					Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
					Action_Static();
					arrived_flag = TOURING_RESULT_ARRIVED;
				}
			}
			else
			{
				if (Heading_Get_Integral(HEADING_INTEGRAL_WALLFOLLOW) > 4800)
				{
					Usprintf("Robot circling\n");
					Heading_Reset_Integral(HEADING_INTEGRAL_WALLFOLLOW);
					Action_Static();
					arrived_flag = TOURING_RESULT_ARRIVED;
				}
			}

			if ((Action_GetMove() == MOVE_ACT_FOLLOW_WALL_RIGHT) || (Action_GetMove() == MOVE_ACT_GYRO_FORWARD) || (Action_GetMove() == MOVE_ACT_STRAIGHT) || (Action_GetMove() == MOVE_ACT_FORWARD))
			{
				if (Map_BoundaryNear())
				{
					Usprintf("Boundary Near\n");
					Wheel_Stop();
					Action_SetTargetPoint(Map_BoundaryGetPoint());
					target_angle = Map_GetRobotToPointAngle(Map_BoundaryGetPoint());
					Action_ListClear();
					Action_ListAdd(MOVE_ACT_HEAD2COURCE, target_angle, HEAD2COURSE_SPEED, HEAD2COURSE_SPEED);
					Action_ListAdd(MOVE_ACT_FOLLOW_BOUNDARY, target_angle, BOUNDARY_SPEED, BOUNDARY_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
				}
			}

			if (!arrived_flag)
			{
				if (Timer_Get_DiffNow(search_charger_time) > TIME_3MIN)
				{
					Action_Static();
					arrived_flag = TOURING_RESULT_TIME_OUT;
					Usprintf("Wall Charge,Time Out\n");
				}
			}

			break;
		default:
			break;
		}
		
		temp_exception_state = Exception_GetState();
		if (temp_exception_state)
		{
			new_cell_cnt = 0;
			Exception_ResetState();
			Exception_Handler(temp_exception_state);
			if (Mode_GetMode() == MODE_WALL)
			{
				Action_ListClear();
				Action_ListStraightAndWallFollow(MOVE_ACT_DECELERATE, 200, wall_dir);
			}
			else
			{
				Action_ListDecelerateToStop();
			}
			arrived_flag = TOURING_RESULT_OBSTACLE;
		}

		if(Battery_IsLow())
		{	
			if(Error_GetCode() != ERROR_LOW_BATTERY)
			{
				Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
				Error_SetCode(ERROR_LOW_BATTERY);
			}						
		}

		if (Error_GetCode())
		{
			if ((Mode_GetMode() == MODE_WALL) || (Mode_GetMode() == MODE_SPOT))
			{
				Usprintf("Error\n");
				Mode_SetMode(MODE_USERINTERFACE);
			}
			if ((Error_GetCode() == ERROR_CLEAN_TANK) || (Error_GetCode() == ERROR_DIRT_TANK) || (Error_GetCode() == ERROR_LOW_BATTERY))
			{
				if(Error_GetCode() == ERROR_CLEAN_TANK ||(Error_GetCode() == ERROR_DIRT_TANK))
				{
					if(g_robot.type == ROBOT_W400)
					{
						Display_SetWaterWarnState(LED_STATE_RED);	
					}
					else
					{
						Display_SetWaterWarnState(LED_STATE_GREEN);
					}					
				}				
				if (touring == TOURING_ING || touring == TOURING_FIND_WALL)
				{
					Action_Static();
					arrived_flag = TOURING_RESULT_GO_HOME;
					Usprintf("Go Home\n");
				}
			}
			else
			{
				Usprintf("Error\n");
				Mode_SetMode(MODE_USERINTERFACE);
			}
		}

		if (touring == TOURING_ING || touring == TOURING_FIND_WALL)
		{
			if (Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
			{
				Usprintf("Remote go home\n");
				Action_Static();
				arrived_flag = TOURING_RESULT_GO_HOME;
			}
		}

		if (touring == TOURING_BACK_HOME)
		{
			if(Touring_IsBackhomeTimeout())
			{
				Mode_SetMode(MODE_USERINTERFACE);
				Usprintf("back home time out\n");
			}
		}


		if (touring_result != TOURING_RESULT_NONE)
		{
			break;
		}
		Event_Handler();
		vTaskDelayUntil(&xLastWakeTime, 10 / portTICK_RATE_MS);
	}
	Usprintf("---------WallFollow End-----------\n");
	return touring_result;
}
