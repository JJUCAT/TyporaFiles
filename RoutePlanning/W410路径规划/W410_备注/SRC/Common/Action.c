#include "Include.h"

Action_t g_action;


void Action_Init(void)
{
	memset(&g_action,0,sizeof(g_action));	
	g_action.mov_act = MOVE_ACT_STATIC;
}

void Action_SetHead2CourseArrive(void)
{
	g_action.head2course_arrive = 1;
}

void Action_ResetHead2CourseArrive(void)
{
	g_action.head2course_arrive = 0;
}

uint8_t Action_GetHead2CourseArrive(void)
{
	return g_action.head2course_arrive;
}

void Action_SetTurnDirection(TurnDirection_t turn)
{
	g_action.turn_dir = turn;
}

TurnDirection_t Action_GetTurnDirection(void)
{
    return g_action.turn_dir;
}

void Action_SetArriveTarget(void)
{
	g_action.target_point_arrive = 1;
}

void Action_ResetArriveTarget(void)
{
	g_action.target_point_arrive = 0;
}

uint8_t Action_GetArriveTarget(void)
{
	return g_action.target_point_arrive;
}

void Action_SetTargetPoint(Point32_t point)
{
	g_action.target_point = point;
}

Point32_t Action_GetTargetPoint(void)
{
	return g_action.target_point;
}

void Action_SetTargetAngle(int16_t angle)
{
	g_action.target_angle = angle;
}

int16_t Action_GetTargetAngle(void)
{
	return g_action.target_angle ;
}

void Action_SetMove(MoveAct_t act)
{
	g_action.mov_act = act;
}

MoveAct_t Action_GetMove(void)
{
	return g_action.mov_act;
}

void Action_SetStartTime(uint32_t time)
{
	g_action.start_time = time;
}

uint32_t Action_GetStartTime(void)
{
	return g_action.start_time;
}

void Action_SetHead2CourseStuck(uint8_t state)
{
	g_action.is_head2course_stuck = state;
}

uint8_t Action_GetHead2CourseStuck(void)
{
	return g_action.is_head2course_stuck;
}

void Action_HeadToCourse(uint8_t speed, int16_t angle)
{
	Usprintf_Action("Action_HeadToCourse speed:%d angle:%d\n",speed,angle);
	int16_t diff = 0;
	static int32_t	angle_turned = 0;
	HeadToCourseDir_t headtocourse_dir = HEADTOCOURSE_DIR_NONE;
	diff = angle - Gyro_GetAngle(0);

	while (diff >= 1800) 
	{	
		diff = diff - 3600;
	}

	while (diff <= (-1800)) 
	{
		diff = diff + 3600;
	}

	if (((diff <= 1800 && diff >= 1700) || (diff >= -1800 && diff <= -1700))) 
	{
		if (diff <= 1800 && diff >= 1700) 
		{
			if (angle_turned < 0) 
			{
				headtocourse_dir = HEADTOCOURSE_DIR_LEFT;
				angle_turned += diff;
			} 
			else 
			{
				headtocourse_dir = HEADTOCOURSE_DIR_RIGHT;
				angle_turned += (diff - 3600);
			}
		} 
		else 
		{
			if (angle_turned > 0) 
			{
				headtocourse_dir = HEADTOCOURSE_DIR_RIGHT;
				angle_turned += diff;
			} 
			else 
			{
				headtocourse_dir = HEADTOCOURSE_DIR_LEFT;
				angle_turned += (3600 + diff);
			}
		}
	} 
	else 
	{
		if ((diff >= 0) && (diff <= 1800)) 
		{	
			headtocourse_dir = HEADTOCOURSE_DIR_LEFT;
		} 
		else if ((diff <= 0) && (diff >= (-1800))) 
		{
			headtocourse_dir = HEADTOCOURSE_DIR_RIGHT;
		}
		angle_turned += diff;
	}
	if(headtocourse_dir == HEADTOCOURSE_DIR_LEFT)
	{
		Wheel_SetDir(WHEEL_DIR_LEFT);
		Action_SetTurnDirection(TURN_DIRECTION_LEFT);
	}
  	else
	{
		Wheel_SetDir(WHEEL_DIR_RIGHT);
		Action_SetTurnDirection(TURN_DIRECTION_RIGHT);
	}
	Action_SetMove(MOVE_ACT_HEAD2COURCE);
	Action_SetTargetAngle(angle);	
	Action_ResetHead2CourseArrive();
	Wheel_SetTargetSpeed(speed,speed);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Mobility_SetCheckState(DISABLE);
	Action_SetStartTime(Timer_GetTime());
	Heading_Reset_Integral(HEADING_INTEGRAL_HEAD2COURCE);
	Action_SetHead2CourseStuck(FALSE);
	Rcon_ResetStatus();
}

void Action_MoveToPoint(Point32_t point,uint32_t speed)
{	
    Usprintf_Action("MoveToPoint\n");
	Action_ResetArriveTarget();
	Action_SetTargetPoint(point);	
	Wheel_SetTargetSpeed(speed,speed);
    Wheel_SetTargetStep(MAX_DISTANCE,MAX_DISTANCE);
	Action_SetMove(MOVE_ACT_MOVE2POINT);
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	OBS_SetDynamicState(ENABLE);
	Mobility_SetCheckState(ENABLE);
	Wheel_ResetStep();
}

void Action_MoveWithBoundary(void)
{	
    Usprintf_Action("MoveWithBoundary\n");
	Action_SetMove(MOVE_ACT_FOLLOW_BOUNDARY);	
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	OBS_SetDynamicState(ENABLE);
	Mobility_SetCheckState(ENABLE);
	Wheel_ResetStep();
}

void Action_MoveForward(uint8_t l_speed, uint8_t r_speed, uint32_t step, MoveAct_t act)
{
	Usprintf_Action("Move forward\n");
	Wheel_SetTargetSpeed(l_speed, r_speed);
	Wheel_SetTargetStep(step,step);	
	Action_SetMove(act);
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Mobility_SetCheckState(ENABLE);
	OBS_SetDynamicState(ENABLE);	
}

void Action_GyroForward(int16_t angle)
{
	Usprintf_Action("Gyro forward\n");
	Action_SetTargetAngle(angle);
	Action_SetMove(MOVE_ACT_GYRO_FORWARD);
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Mobility_SetCheckState(ENABLE);
	OBS_SetDynamicState(ENABLE);	
}

void Action_WallFollow(MoveAct_t act)
{
	Usprintf_Action("wall follow\n");
	Wheel_ResetStep();
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Action_SetMove(act);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Mobility_SetCheckState(DISABLE);
}

void Action_Turn(MoveAct_t dir, uint8_t speed, uint32_t angle)
{
    Usprintf_Action("Turn\n");
	Wheel_SetTargetSpeed(speed,speed);
    Wheel_SetTargetStep(angle,angle);	
	Action_SetMove(dir);
	if(dir == MOVE_ACT_TURN_LEFT)
	{
		Wheel_SetDir(WHEEL_DIR_LEFT);
	}
	else
	{
		Wheel_SetDir(WHEEL_DIR_RIGHT);
	}
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Mobility_SetCheckState(DISABLE);
	OBS_SetDynamicState(DISABLE);
}

void Action_MoveBack(uint8_t l_speed, uint8_t r_speed, uint32_t step)
{
	Usprintf_Action("Action_MoveBack\n");
	Wheel_SetTargetSpeed(l_speed,r_speed);
	Wheel_SetTargetStep(step,step);	
	Wheel_SetDir(WHEEL_DIR_BACKWARD);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Action_SetMove(MOVE_ACT_BACK);
	Mobility_SetCheckState(DISABLE);
	OBS_SetDynamicState(DISABLE);
}

void Action_Deceleration(void)
{
	Usprintf_Action("Action_Deceleration\n");
	Wheel_SetTargetSpeed(0,0);
	Action_SetMove(MOVE_ACT_DECELERATE);
	Wheel_Configure(ENABLE);
	Wheel_SetTuneState(ENABLE);
	Wheel_ResetStep();
	Mobility_SetCheckState(DISABLE);
}

void Action_Static(void)
{
	Usprintf_Action("Action_Static\n");
    Wheel_Stop();
	Action_SetMove(MOVE_ACT_STATIC);
	Mobility_SetCheckState(DISABLE);
	Wheel_SetTuneState(ENABLE);
}

uint8_t Action_WaitForMoveEnded(void)
{
	if((Wheel_GetLeftStep() >= Wheel_GetLeftTargetStep()) || (Wheel_GetRightStep() >= Wheel_GetRightTargetStep()))
	{
		Action_SetMove(MOVE_ACT_HANDLER);
		return 1;
	}
	return 0;
}

uint8_t Action_WaitForHeadToCourseEnded(void)
{  
	if(Action_GetHead2CourseArrive())	
	{
		Action_ResetHead2CourseArrive();
		Action_SetMove(MOVE_ACT_HANDLER);	
		return 1;																
	}
	return 0;	
}

uint8_t Action_WaitForDecelerateEnded(void)
{
	if((Wheel_GetLeftSpeed() <= 10)&&(Wheel_GetRightSpeed() <= 10))
	{		
		Action_SetMove(MOVE_ACT_HANDLER);
        return 1;
	}
    return 0;
}

void Action_ListAdd(MoveAct_t act_mode, int32_t act_step, uint8_t l_speed, uint8_t r_speed)
{
	g_action.mov_list[g_action.mov_list_input_idx].state = act_mode;
	g_action.mov_list[g_action.mov_list_input_idx].step = act_step;
	g_action.mov_list[g_action.mov_list_input_idx].l_speed = l_speed;
	g_action.mov_list[g_action.mov_list_input_idx].r_speed = r_speed;
	
	g_action.mov_list_input_idx++;
	if(g_action.mov_list_input_idx > ACTLIST_LENGTH)g_action.mov_list_input_idx = 0;
	g_action.mov_list_cnt++;
}

Move_t Action_ListGetNext(void)
{
	Move_t act_buffer;
	if(g_action.mov_list_cnt)
	{
		g_action.mov_list_cnt--;
		act_buffer = g_action.mov_list[g_action.mov_list_output_idx];
		g_action.mov_list[g_action.mov_list_output_idx].state = MOVE_ACT_STATIC;
		g_action.mov_list_output_idx++;
		if(g_action.mov_list_output_idx > ACTLIST_LENGTH)g_action.mov_list_output_idx = 0;
	}
	else
	{
		act_buffer.state = MOVE_ACT_STATIC;
	}
	return act_buffer;
}

void Action_ListClear(void)
{
	g_action.mov_list_output_idx = 0;
	g_action.mov_list_input_idx = 0;
	g_action.mov_list_cnt = 0;
}

uint8_t Action_ListGetCnt(void)
{
	return  g_action.mov_list_cnt;
}

void Action_ListSwitch(void)
{
	volatile Move_t mov_list_buffer;

	mov_list_buffer = Action_ListGetNext();
	switch(mov_list_buffer.state)
	{				
		case MOVE_ACT_FORWARD:									Action_MoveForward(mov_list_buffer.l_speed,mov_list_buffer.r_speed,mov_list_buffer.step,MOVE_ACT_FORWARD);
																break;
		case MOVE_ACT_GYRO_FORWARD:								Action_GyroForward(mov_list_buffer.step); 
																break;
		case MOVE_ACT_MOVE2POINT:								Action_MoveToPoint(Action_GetTargetPoint(),(mov_list_buffer.l_speed + mov_list_buffer.r_speed)/2);
																break;	
		case MOVE_ACT_DOCKING:									Action_MoveForward(mov_list_buffer.l_speed,mov_list_buffer.r_speed,mov_list_buffer.step,MOVE_ACT_DOCKING);		
																break;																
		case MOVE_ACT_FOLLOW_BOUNDARY:						    Action_MoveWithBoundary();
																break;
		case MOVE_ACT_FOLLOW_WALL_LEFT:                         Action_WallFollow(mov_list_buffer.state);
																break;
		case MOVE_ACT_FOLLOW_WALL_RIGHT:                        Action_WallFollow(mov_list_buffer.state);																
																break;		
		case MOVE_ACT_BACK:					                    Action_MoveBack(mov_list_buffer.l_speed,mov_list_buffer.r_speed,mov_list_buffer.step);
																break;
		case MOVE_ACT_TURN_LEFT:		                        Action_Turn(mov_list_buffer.state,mov_list_buffer.l_speed,mov_list_buffer.step);
																break;
		case MOVE_ACT_TURN_RIGHT:		                        Action_Turn(mov_list_buffer.state,mov_list_buffer.l_speed,mov_list_buffer.step);
																break;
		case MOVE_ACT_TURN_ROUND:		                        Action_Turn(mov_list_buffer.state,mov_list_buffer.l_speed,mov_list_buffer.step);
																break;		
		case MOVE_ACT_STRAIGHT:			                        Action_MoveForward(mov_list_buffer.l_speed,mov_list_buffer.r_speed,mov_list_buffer.step,MOVE_ACT_STRAIGHT);
																break;
		case MOVE_ACT_HEAD2COURCE:	                            Action_HeadToCourse((mov_list_buffer.l_speed + mov_list_buffer.r_speed)/2,mov_list_buffer.step);
													 			break;
		case MOVE_ACT_DECELERATE:		                        Action_Deceleration();
																break;
		case MOVE_ACT_STATIC:				                    Action_Static();
																break;
		default:												Action_Static();
																break;
	}
}

void Action_ListBackToStop(uint16_t step)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_BACK,step,BACK_SPEED,BACK_SPEED);
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListDecelerateToStop(void)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void ActList_BackSideToStop(CliffTrig_t cliff_status)
{
	uint16_t back_distance = 800;
	Action_ListClear();
	if(cliff_status&CLIFF_TRIG_LEFT)cliff_status = CLIFF_TRIG_LEFT;
	if(cliff_status&CLIFF_TRIG_RIGHT)cliff_status = CLIFF_TRIG_RIGHT;
	if(cliff_status&CLIFF_TRIG_FRONT)cliff_status = CLIFF_TRIG_FRONT;
	switch(cliff_status)
	{
		case CLIFF_TRIG_LEFT:
								Action_ListAdd(MOVE_ACT_BACK,back_distance,BACK_SPEED,BACK_SPEED/4);
		break;
		case CLIFF_TRIG_RIGHT:
								Action_ListAdd(MOVE_ACT_BACK,back_distance,BACK_SPEED/4,BACK_SPEED);
		break;
		case CLIFF_TRIG_FRONT:
								Action_ListAdd(MOVE_ACT_BACK,back_distance/2,BACK_SPEED,BACK_SPEED);
		break;
		default:
		break;
	}	
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListBackLeftAndRight(void)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_BACK,CHARGE_BUMPER_BACK_DITANCE,BACK_SPEED,BACK_SPEED);
	Action_ListAdd(MOVE_ACT_TURN_LEFT,CHARGE_BUMPER_TURN_ANGLE,DOCKING_TURN_SPEED,DOCKING_TURN_SPEED);																	
	Action_ListAdd(MOVE_ACT_TURN_RIGHT,CHARGE_BUMPER_TURN_ANGLE*2,DOCKING_TURN_SPEED,DOCKING_TURN_SPEED);
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListBackAndWall(MoveAct_t act)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_BACK,CHARGE_BUMPER_BACK_DITANCE,BACK_SPEED,BACK_SPEED);
	if(act == MOVE_ACT_FOLLOW_WALL_LEFT)
	{
		Action_ListAdd(MOVE_ACT_TURN_RIGHT,CHARGE_BUMPER_TURN_ANGLE/2,DOCKING_TURN_SPEED,DOCKING_TURN_SPEED);		
	}
	else
	{
		Action_ListAdd(MOVE_ACT_TURN_LEFT,CHARGE_BUMPER_TURN_ANGLE/2,DOCKING_TURN_SPEED,DOCKING_TURN_SPEED);	
	}	
	Action_ListAdd(act,MAX_DISTANCE,DOCKING_SPEED,DOCKING_SPEED); 
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListWallOffEdgeStop(WallDir_t wall_dir)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	if((wall_dir == WALLDIR_EAST_LEFT) || (wall_dir==WALLDIR_WEST_LEFT))
	{
		Action_ListAdd(MOVE_ACT_BACK,200,BACK_SPEED,0);
		Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
		Action_ListAdd(MOVE_ACT_BACK,400,0,BACK_SPEED);
		Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
		Action_ListAdd(MOVE_ACT_STRAIGHT,600,STRAIGHT_SPEED,0);
	}
	else
	{
		Action_ListAdd(MOVE_ACT_BACK,200,0,BACK_SPEED);
		Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
		Action_ListAdd(MOVE_ACT_BACK,400,BACK_SPEED,0);
		Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
		Action_ListAdd(MOVE_ACT_STRAIGHT,600,0,STRAIGHT_SPEED);
	}
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListWallStraightAndForward(MoveAct_t decelerate, uint32_t act_step)
{
	if(decelerate == MOVE_ACT_DECELERATE)Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STRAIGHT,act_step,STRAIGHT_SPEED,STRAIGHT_SPEED);
	Action_ListAdd(MOVE_ACT_FORWARD,MAX_DISTANCE,FORWARD_SPEED,FORWARD_SPEED);
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListStraightAndWallFollow(MoveAct_t decelerate, uint32_t act_step,WallDir_t wall_dir)
{
	if(decelerate == MOVE_ACT_DECELERATE)Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	Action_ListAdd(MOVE_ACT_STRAIGHT,act_step,STRAIGHT_SPEED,STRAIGHT_SPEED);
	if(wall_dir == WALLDIR_EAST_LEFT || wall_dir == WALLDIR_WEST_LEFT)
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_LEFT,0,0,0);	
	}
	else
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT,0,0,0);
	}
	Action_SetMove(MOVE_ACT_HANDLER);
}

void Action_ListWallSlipStraightAndForward(MoveAct_t action)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	if(action == MOVE_ACT_TURN_RIGHT)
	{
		Action_ListAdd(MOVE_ACT_STRAIGHT,300,STRAIGHT_SPEED,STRAIGHT_SPEED - 5);
	}	
	else if(action == MOVE_ACT_TURN_LEFT)
	{
		Action_ListAdd(MOVE_ACT_STRAIGHT,300,STRAIGHT_SPEED - 5,STRAIGHT_SPEED);
	}
	Action_ListAdd(MOVE_ACT_FORWARD,MAX_DISTANCE,FORWARD_SPEED,FORWARD_SPEED);
	Action_SetMove(MOVE_ACT_HANDLER);		
}

void Action_ListWallTurnSlip(MoveAct_t action,WallDir_t wall_dir)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
	if(action == MOVE_ACT_TURN_RIGHT)
	{
		Action_ListAdd(MOVE_ACT_STRAIGHT,100,STRAIGHT_SPEED,STRAIGHT_SPEED - 5);
	}	
	else if(action == MOVE_ACT_TURN_LEFT)
	{
		Action_ListAdd(MOVE_ACT_STRAIGHT,100,STRAIGHT_SPEED - 5,STRAIGHT_SPEED);
	}
	if((wall_dir == WALLDIR_EAST_LEFT)||(wall_dir == WALLDIR_WEST_LEFT))
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_LEFT,0,0,0);	
	}
	else
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT,0,0,0);	
	}
	Action_SetMove(MOVE_ACT_HANDLER);		
}

void Action_ListWallForward(WallDir_t wall_dir)
{
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_FORWARD,200,FORWARD_SPEED + 3,FORWARD_SPEED);
	if(wall_dir == WALLDIR_EAST_RIGHT || wall_dir == WALLDIR_WEST_RIGHT)
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_RIGHT,0,0,0);
	}
	else
	{
		Action_ListAdd(MOVE_ACT_FOLLOW_WALL_LEFT,0,0,0);
	}
	Action_SetMove(MOVE_ACT_HANDLER);	
}









