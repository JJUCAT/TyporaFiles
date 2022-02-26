/**
  ******************************************************************************
  * @file    AI Cleaning Robot
  * @author  ILife Team Dxsong
  * @version V1.0
  * @date    17-Nov-2011
  * @brief   Basical Movement functions
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2011 ILife CO.LTD</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "Include.h"

Movement_t g_movement;

void Movement_Init(void)
{
	memset(&g_movement,0,sizeof(g_movement));	
	g_movement.base_spd = 100;
}

void Movement_Pid(void)
{
	switch(Action_GetMove())
	{
		case MOVE_ACT_FOLLOW_WALL_LEFT:
															Movement_PidLeftWall();
															break;
		case MOVE_ACT_FOLLOW_WALL_RIGHT:
															Movement_PidRightWall();
															break;	
		case MOVE_ACT_GYRO_FORWARD:
															Movement_PidGyroForward();
															break;
		case MOVE_ACT_MOVE2POINT:
															Movement_PidMoveToPoint();
															break;
		case MOVE_ACT_HEAD2COURCE:
															Movement_PidHeadToCourse();
															break;
		case MOVE_ACT_FOLLOW_BOUNDARY:
															Movement_PidMoveBoundary(); 
															break;															
		default:
															break;
	}  
}

void Movement_PidLeftWall(void)
{
	int32_t left_wall_speed = 0, right_wall_speed = 0,base_speed = 0;
	int32_t wall_proportion = 0;
	int32_t wall_delta = 0;
	static int32_t wall_previous = 0;
	int32_t wall_distance = Wall_GetDistance();

	wall_proportion = Wall_GetLeftAdcValue();
	if(Wall_UpdateBuffer(wall_proportion)) 
	{
		if(Wheel_GetLeftStep() >= WALL_SMOOTH_DISTANCE)
		{
			Action_ListWallForward(WALLDIR_EAST_LEFT);
		}			
	}
	wall_proportion = wall_proportion * 100 / wall_distance;
	wall_proportion -= 100;
	wall_delta = wall_proportion - wall_previous;

	base_speed = BASE_SPD + g_movement.base_spd/5;

	if(OBS_FrontClose())base_speed = OBS_SLOW_SPEED;

	left_wall_speed = base_speed + wall_proportion / 7 + wall_delta / 5;
	right_wall_speed = base_speed - wall_proportion / 7 - wall_delta / 5;

	if (right_wall_speed > WALL_ADJUST_TO_SPEED) 
	{
	left_wall_speed = 5;
	}
	Wheel_TuneRightSpeedDir(&right_wall_speed,15);	

	wall_previous = wall_proportion;

	right_wall_speed = Math_LimitingValue(right_wall_speed, WALL_ADJUST_TO_SPEED);
	left_wall_speed = Math_LimitingValue(left_wall_speed, WALL_ADJUST_TO_SPEED);

	Wheel_SetTargetSpeed(left_wall_speed,right_wall_speed);	
}

void Movement_PidRightWall(void)
{
	int32_t left_wall_speed = 0, right_wall_speed = 0,base_speed = 0;
	int32_t wall_proportion = 0;
	int32_t wall_delta = 0;
	int32_t wall_distance = Wall_GetDistance();
	static int32_t wall_previous = 0;
		
	wall_proportion = Wall_GetRightAdcValue();

	if(Wall_UpdateBuffer(wall_proportion)) 
	{
		if(Wheel_GetRightStep() >= WALL_SMOOTH_DISTANCE)
		{
			Action_ListWallForward(WALLDIR_EAST_RIGHT);	
		}			
	}

	wall_proportion = wall_proportion * 100 / wall_distance;
	wall_proportion -= 100;
	wall_delta = wall_proportion - wall_previous;

	base_speed = BASE_SPD + g_movement.base_spd/5;

	if(OBS_FrontClose())base_speed = OBS_SLOW_SPEED;

	right_wall_speed = base_speed + wall_proportion / 7 + wall_delta / 5;
	left_wall_speed = base_speed - wall_proportion / 7 - wall_delta / 5;	

	if (left_wall_speed > WALL_ADJUST_TO_SPEED) 
	{
		right_wall_speed = 5;
	}
	Wheel_TuneLeftSpeedDir(&left_wall_speed,15);	
	wall_previous = wall_proportion;
	right_wall_speed = Math_LimitingValue(right_wall_speed, WALL_ADJUST_TO_SPEED);
	left_wall_speed = Math_LimitingValue(left_wall_speed, WALL_ADJUST_TO_SPEED);

	Wheel_SetTargetSpeed(left_wall_speed,right_wall_speed);	
}

void Movement_PidGyroForward(void)
{	
	int32_t rotate_angle = 0;
	int16_t angle_diff = 0;
	static int16_t gyro_angle_buffer = 0;
	static uint8_t integration_cycle = 0;
	int32_t integration_value = 0;
	uint8_t move_speed_base = 20;
	int32_t left_speed = 0,right_speed = 0;	
	
	rotate_angle = Action_GetTargetAngle() - Gyro_GetAngle(0);
											
	rotate_angle = Math_RoundAngle(rotate_angle);																
																		
	angle_diff = Math_Diff_int(Gyro_GetAngle(0),gyro_angle_buffer);
	if(angle_diff > 1800)angle_diff = 3600 - angle_diff;
		
	integration_cycle++;
	if (integration_cycle > 10) 
	{
		integration_cycle = 0;																	
		integration_value += rotate_angle;
		if (integration_value > 150) 
		{
			integration_value = 150;
		} 
		else if (integration_value < -150) 
		{
			integration_value = -150;
		}
	}																
																																	
	if(OBS_IsNear())move_speed_base = OBS_SLOW_SPEED;
	else move_speed_base = BASE_SPD + g_movement.base_spd/5;
	
	left_speed = move_speed_base - rotate_angle / 8 - integration_value / 150; 
	right_speed = move_speed_base + rotate_angle / 8 + integration_value / 150; 

	Wheel_TuneLeftSpeedDir(&left_speed,PID_BACK_SPEED);
	Wheel_TuneRightSpeedDir(&right_speed,PID_BACK_SPEED);
							
	left_speed = Math_LimitingValue(left_speed, PID_ADJUST_TOP_SPEED);
	right_speed = Math_LimitingValue(right_speed, PID_ADJUST_TOP_SPEED);
	
	Wheel_SetTargetSpeed(left_speed,right_speed);	

	gyro_angle_buffer = Gyro_GetAngle(0);	
}

void Movement_PidMoveToPoint(void)
{
	static int32_t pre_rotate_angle = 0;
	int32_t left_speed = 0,right_speed = 0;
	int32_t rotate_angle = 0,delta_angle = 0;	
	static uint16_t integration_cycle = 0;
	static int32_t integration_value = 0;
	uint8_t move_speed_base = 20;
	uint8_t robot_slow_flag = 0;
	Point32_t target_point = Action_GetTargetPoint();
	Point16_t target_cell = Map_PointToCell(target_point);
	Point16_t robot_cell = Map_GetRobotCell();	
		
	if(Action_GetArriveTarget()) return ;	
	
	if(Map_GetTwoPointsDistance(Map_GetRobotCount(),target_point) <= (MAP_ROBOT_SIZE*CELL_COUNT_MUL))
	{
		rotate_angle = Math_Course2Dest(Map_GetRobotCellX(),Map_GetRobotCellY(),target_cell.x,target_cell.y);
	}
	else
	{		
		rotate_angle = Math_Course2Dest(Map_GetRobotCountX(),Map_GetRobotCountY(),target_point.x,target_point.y);	
	}

	rotate_angle = rotate_angle - Gyro_GetAngle(0);
	
	rotate_angle = Math_RoundAngle(rotate_angle);

	delta_angle = rotate_angle - pre_rotate_angle;

	if(!PathList_GetCnt())
	{
		if(Map_RobotCloseToPosCount(target_point))
		{
			if(g_movement.think_next_point)
			{
				target_cell = Map_PointToCell(target_point);
				PathPlanning_NormalClean(target_cell);	
				//Usprintf("Next Target Cell:%d %d\n",target_cell.x,target_cell.y);									
			}
			/*
			else
			{
				Action_SetArriveTarget();
			}*/
		}			
		if(Math_Abs_int(rotate_angle) > 1350)
		{
			Action_SetArriveTarget();
		}
		if(Map_IsTwoPointsNear(Map_GetRobotCount(),target_point))
		{
			Action_SetArriveTarget();																				
		}
		if(Math_Abs_int(Heading_Get_Integral(HEADING_INTEGRAL_MOVETOPOSITION)) > 3600)
		{
			Heading_Reset_Integral(HEADING_INTEGRAL_MOVETOPOSITION);
			Action_SetArriveTarget();
		}
	}
	else
	{
		if(Map_RobotCloseToPosCount(target_point))
		{
      		if(g_movement.set_robot_size_clean)Map_SetCellRobotSizeClean(target_cell.x,target_cell.y);
			if(PathList_GetNextPathListPoint(&target_point))
			{
				target_cell = Map_PointToCell(target_point);
				Usprintf("Next Target Cell:%d %d\n",target_cell.x,target_cell.y);
				Action_SetTargetPoint(target_point);
				Heading_Reset_Integral(HEADING_INTEGRAL_MOVETOPOSITION);
			}
			else
			{
				Action_SetArriveTarget();																
			}
		}			
	}	
	pre_rotate_angle = rotate_angle;																																
	integration_cycle++;
	if (integration_cycle > 10) 
	{
		integration_cycle = 0;
		integration_value += rotate_angle;
		if (integration_value > 150) 
		{
			integration_value = 150;
		} 
		else if (integration_value < -150) 
		{
			integration_value = -150;
		}
	}
	robot_slow_flag = 0;
	if(OBS_IsNear())robot_slow_flag |=1;
	
	if(Map_IsRobotNearPosCount(target_point))robot_slow_flag |=1;		
	
	if(robot_slow_flag)move_speed_base = OBS_SLOW_SPEED;
	else move_speed_base = BASE_SPD + g_movement.base_spd/5;

	left_speed = move_speed_base - rotate_angle / 5 - integration_value / 150 - delta_angle/3; 
	right_speed = move_speed_base + rotate_angle / 5 + integration_value / 150 + delta_angle/3; 

	//Usprintf("spd:%d %d rotate_angle:%d integration_value:%d delta_angle:%d\n",left_speed,right_speed,rotate_angle,integration_value,delta_angle);

	Wheel_TuneLeftSpeedDir(&left_speed,PID_BACK_SPEED);
	Wheel_TuneRightSpeedDir(&right_speed,PID_BACK_SPEED);
							
	left_speed = Math_LimitingValue(left_speed, PID_ADJUST_TOP_SPEED);
	right_speed = Math_LimitingValue(right_speed, PID_ADJUST_TOP_SPEED);
	
	

	Wheel_SetTargetSpeed(left_speed,right_speed);	
}

void Movement_PidHeadToCourse(void)
{
	static uint32_t check_stuck_time;
	int32_t diff = 0;
	
	if(Action_GetHead2CourseArrive()) return ;	

	if(Timer_Get_DiffNow(Action_GetStartTime()) >= TIME_3S)
	{
		if(Timer_IsTimeReach(&check_stuck_time,TIME_3S))
		{
			Usprintf("headtocourse angle:%d\n",Heading_Get_Integral(HEADING_INTEGRAL_HEAD2COURCE));
			if(Math_Abs_int(Heading_Get_Integral(HEADING_INTEGRAL_HEAD2COURCE)) <= 400)
			{
				Action_SetHead2CourseStuck(TRUE);
				Usprintf("head2course stuck,%d\n",Heading_Get_Integral(HEADING_INTEGRAL_HEAD2COURCE));
			}
		}
	}
	else
	{
		check_stuck_time = Timer_GetTime();	
	}

	diff = Math_Diff_int(Action_GetTargetAngle(), Gyro_GetAngle(0));
	diff = diff > 1800 ? 3600 - diff : diff;
	diff = Math_Abs_int(diff);
	if (diff < 20) 
	{
		Action_SetHead2CourseArrive();
	}
	else if((diff < 100))
	{
		diff /= 10;
		if(diff > 15)diff = HEAD2COURSE_SPEED;
		if(diff < 7)diff = 7;
		Wheel_SetRightTargetSpeed(diff);
		Wheel_SetLeftTargetSpeed(diff);
	}			
}

void Movement_PidMoveBoundary(void)
{
	static int32_t pre_rotate_angle = 0;
	int32_t left_speed = 0,right_speed = 0;
	int32_t rotate_angle = 0,delta_angle = 0;
	static uint16_t integration_cycle = 0;
	static int32_t integration_value = 0;
	uint8_t move_speed_base = 20;
	uint8_t robot_slow_flag = 0;
	Point32_t target_point = Action_GetTargetPoint();
	
	if(Action_GetArriveTarget()) return ;

	if(Map_IsRobotNearPosCount(target_point))
	{
		target_point = Map_BoundaryGetNextPoint(target_point);
		Action_SetTargetPoint(target_point);
	}	
	rotate_angle = Math_Course2Dest(Map_GetRobotCountX(), Map_GetRobotCountY(), target_point.x, target_point.y) - Gyro_GetAngle(0);
	
	rotate_angle = Math_RoundAngle(rotate_angle);

	delta_angle = rotate_angle - pre_rotate_angle;

	integration_cycle++;
	if (integration_cycle > 10) 
	{
		integration_cycle = 0;
		integration_value += rotate_angle;
		if (integration_value > 150) 
		{
			integration_value = 150;
		} 
		else if (integration_value < -150) 
		{
			integration_value = -150;
		}
	}
	robot_slow_flag = 0;
	if(OBS_IsNear())robot_slow_flag |=1;
	
	if(Map_IsRobotNearPosCount(target_point))robot_slow_flag |=1;
	
	if(robot_slow_flag)move_speed_base = OBS_SLOW_SPEED;
	else move_speed_base = BASE_SPD + g_movement.base_spd/5;

	left_speed = move_speed_base - rotate_angle / 5 - integration_value / 150 - delta_angle/3; 
	right_speed = move_speed_base + rotate_angle / 5 + integration_value / 150 + delta_angle/3; 

	pre_rotate_angle = rotate_angle;

	Wheel_TuneLeftSpeedDir(&left_speed,PID_BACK_SPEED);
	Wheel_TuneRightSpeedDir(&right_speed,PID_BACK_SPEED);
							
	left_speed = Math_LimitingValue(left_speed, PID_ADJUST_TOP_SPEED);
	right_speed = Math_LimitingValue(right_speed, PID_ADJUST_TOP_SPEED);
	
	Wheel_SetTargetSpeed(left_speed,right_speed);	

}

void Movement_PidSetThinkNextPoint(uint8_t state)
{
	g_movement.think_next_point = state;
}

void Movement_PidSetRobotSizeClean(uint8_t state)
{
	g_movement.set_robot_size_clean = state;
}


