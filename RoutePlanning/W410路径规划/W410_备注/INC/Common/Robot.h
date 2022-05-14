#ifndef __ROBOT_H
#define __ROBOT_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum
{
	ROBOT_NONE = 0,
	ROBOT_W400 = 1,
	ROBOT_W410 = 2,
}RobotType_t;

typedef struct 
{
	RobotType_t type;
	uint8_t update_position;
}Robot_t;

extern Robot_t g_robot;

void Robot_StructInit(void);
RobotType_t Robot_GetType(void);
void Robot_MotorsSetState(FunctionalState state);
void Robot_SensorsSetState(FunctionalState state);
void Robot_MotorsSetCheckState(FunctionalState state);
void Robot_Initialize(void);
void Robot_ReportError(Error_t error_val);
void Robot_ModeDisplay(void);
void Robot_Remote2Act(void);

void Robot_AddAct(MoveAct_t act,uint8_t is_wifi_remote);



#endif







