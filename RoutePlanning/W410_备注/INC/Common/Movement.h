#ifndef __MOVEMENT_H
#define __MOVEMENT_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define BASE_SPD            18
#define WALL_SMOOTH_DISTANCE 300
typedef struct 
{
    uint8_t base_spd;
    uint8_t think_next_point;
    uint8_t set_robot_size_clean;
}Movement_t;

extern Movement_t g_movement;

void Movement_Init(void);
void Movement_Pid(void);
void Movement_PidLeftWall(void);
void Movement_PidRightWall(void);
void Movement_PidGyroForward(void);
void Movement_PidMoveToPoint(void);
void Movement_PidHeadToCourse(void);
void Movement_PidMoveBoundary(void);
void Movement_PidSetThinkNextPoint(uint8_t state);
void Movement_PidSetRobotSizeClean(uint8_t state);
#endif /* __Movement_H */

