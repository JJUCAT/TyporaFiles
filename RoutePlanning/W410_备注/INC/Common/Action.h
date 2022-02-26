#ifndef __ACTION_H
#define __ACTION_H

#include <stdint.h>
#include "Hw_SysInitialize.h"
#define ACTLIST_LENGTH	 20


#define MAX_DISTANCE 	0x1ffffffe
#define STATION_BACK_DISTANCE        4000
#define CHARGE_BUMPER_BACK_DITANCE   100
#define CHARGE_BUMPER_TURN_ANGLE     400
#define WALLFOLLOW_TURN_ANGLE 		 700//600
#define WALLFOLLOW_TRAP_TURN_ANGLE	 150
#define MOVETOPOINT_BUMPER_DISTANCE  400
#define HEAD2COURSE_STUCK_BACK_DISTANCE  3000
#define TURN_ANGLE_OBS      		 300
#define TURN_ANGLE_CLIFF      		 600
#define STRAIGHT_DISTANCE			 350

#define PID_BACK_SPEED			5//5//15
#define PID_ADJUST_TOP_SPEED	45//35
#define BACK_SPEED		        20
#define DOCKING_TURN_SPEED      10
#define DOCKING_SPEED           15
#define STRAIGHT_SPEED		    20
#define FORWARD_SPEED		    20
#define TURN_SPEED		    	20
#define WALL_FOLLOW_SPEED		20
#define WALL_ADJUST_TO_SPEED    30
#define GYROFORWARD_SPEED       20
#define MOVETOPOINT_SPEED       20
#define BOUNDARY_SPEED       	20
#define HEAD2COURSE_SPEED       25
#define OBS_SLOW_SPEED       	10
#define EXCEPTION_SPEED			15
#define STATION_TURN_SPEED		10
#define SLOW_SPEED				10
#define REMOTE_SPEED			15

typedef enum 
{
	MOVE_ACT_NONE= 0,
	MOVE_ACT_STATIC,
	MOVE_ACT_TURN_LEFT,
	MOVE_ACT_TURN_RIGHT,
	MOVE_ACT_FORWARD,
	MOVE_ACT_BACK,
	MOVE_ACT_DECELERATE,
	MOVE_ACT_TURN,
	MOVE_ACT_STRAIGHT,
	MOVE_ACT_HEAD2COURCE,
	MOVE_ACT_MOVE2POINT,
	MOVE_ACT_HANDLER,
	MOVE_ACT_TURN_ROUND,	
	MOVE_ACT_FOLLOW_WALL_LEFT,
	MOVE_ACT_FOLLOW_WALL_RIGHT,
	MOVE_ACT_FOLLOW_BOUNDARY,
	MOVE_ACT_GYRO_FORWARD,
	MOVE_ACT_DOCKING,
}MoveAct_t;

typedef struct
{
	MoveAct_t state;
	int32_t step;
	uint16_t l_speed;
	uint16_t r_speed;
}Move_t;


typedef enum 
{
    TURN_DIRECTION_NONE = 0,
    TURN_DIRECTION_LEFT,
    TURN_DIRECTION_RIGHT,
}TurnDirection_t;

typedef enum
{
	WALLDIR_NONE = 0,
	WALLDIR_EAST_LEFT,
	WALLDIR_EAST_RIGHT,
	WALLDIR_WEST_LEFT,
	WALLDIR_WEST_RIGHT,
}WallDir_t;

typedef enum
{
	HEADTOCOURSE_DIR_NONE = 0,
	HEADTOCOURSE_DIR_LEFT,
	HEADTOCOURSE_DIR_RIGHT,
}HeadToCourseDir_t;

typedef struct
{
	uint8_t head2course_arrive;
	uint8_t target_point_arrive;
	TurnDirection_t turn_dir;
	int16_t target_angle;
	Point32_t target_point;
	MoveAct_t mov_act;
	Move_t mov_list[ACTLIST_LENGTH + 1];
	uint8_t mov_list_input_idx;
	uint8_t mov_list_output_idx;
	uint8_t mov_list_cnt;
	uint32_t start_time;
	uint8_t is_head2course_stuck;
}Action_t;

extern Action_t g_action;
void Action_Init(void);
void Action_SetTurnDirection(TurnDirection_t turn);
TurnDirection_t Action_GetTurnDirection(void);
void Action_SetHead2CourseArrive(void);
void Action_ResetHead2CourseArrive(void);
uint8_t Action_GetHead2CourseArrive(void);
void Action_SetArriveTarget(void);
void Action_ResetArriveTarget(void);
uint8_t Action_GetArriveTarget(void);
void Action_SetTargetPoint(Point32_t point);
Point32_t Action_GetTargetPoint(void);
void Action_SetTargetAngle(int16_t angle);
int16_t Action_GetTargetAngle(void);
void Action_SetMove(MoveAct_t act);
MoveAct_t Action_GetMove(void);
void Action_SetStartTime(uint32_t time);
uint32_t Action_GetStartTime(void);
void Action_SetHead2CourseStuck(uint8_t state);
uint8_t Action_GetHead2CourseStuck(void);
void Action_HeadToCourse(uint8_t speed, int16_t angle);
void Action_MoveToPoint(Point32_t point,uint32_t speed);
void Action_MoveWithBoundary(void);
void Action_MoveForward(uint8_t l_speed, uint8_t r_speed, uint32_t step, MoveAct_t act);
void Action_GyroForward(int16_t angle);
void Action_WallFollow(MoveAct_t act);
void Action_Turn(MoveAct_t dir, uint8_t speed, uint32_t angle);
void Action_MoveBack(uint8_t l_speed, uint8_t r_speed, uint32_t step);
void Action_Deceleration(void);
void Action_Static(void);
uint8_t Action_WaitForMoveEnded(void);
uint8_t Action_WaitForHeadToCourseEnded(void);
uint8_t Action_WaitForDecelerateEnded(void);
void Action_ListAdd(MoveAct_t act_mode, int32_t act_step, uint8_t l_speed, uint8_t r_speed);
Move_t Action_ListGetNext(void);
void Action_ListClear(void);
uint8_t Action_ListGetCnt(void);
void Action_ListSwitch(void);
void Action_ListBackToStop(uint16_t step);
void ActList_BackSideToStop(CliffTrig_t cliff_status);
void Action_ListDecelerateToStop(void);
void Action_ListBackLeftAndRight(void);
void Action_ListBackAndWall(MoveAct_t act);
void Action_ListWallOffEdgeStop(WallDir_t wall_dir);
void Action_ListWallStraightAndForward(MoveAct_t decelerate, uint32_t act_step);
void Action_ListStraightAndWallFollow(MoveAct_t decelerate, uint32_t act_step,WallDir_t wall_dir);
void Action_ListWallSlipStraightAndForward(MoveAct_t action);
void Action_ListWallTurnSlip(MoveAct_t action,WallDir_t wall_dir);
void Action_ListWallForward(WallDir_t wall_dir);

#endif



