#ifndef __PATHPLANNING_H
#define __PATHPLANNING_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum
{
	PATHPLANNING_LANE_BOTH_SIDE_BLOCKED = 0,
	PATHPLANNING_LANE_ONE_SIDE_UNCLEAN,
	PATHPLANNING_LANE_BOTH_SIDE_UNCLEAN,
}PathPlanningLane_t;// line 的左右两侧区域的清扫情况

typedef enum
{
	PATHPLANNING_STATE_NOTARGET = 0,
	PATHPLANNING_STATE_NORMAL_CLEAN,
	PATHPLANNING_STATE_UNREACHABLE,
	PATHPLANNING_STATE_ARRIVE,
}PathPlanningState_t;

extern volatile uint8_t g_pathplanning_cleaned_rate;

uint8_t PathPlanning_IsSpotLaneCleaned(uint8_t idx ,Point16_t start_cell, int8_t dir);
//检查坐标点所在的路径(平行 x 轴，且宽度为一个机身)的清扫情况
//return @arg 0:所在路径左右被障碍物挡住
//return @arg 1:所在路径一侧未清扫，p_lane_pos->x 为最远距离的 x 坐标
//return @arg 1:所在路径两侧未清扫，p_lane_pos->x 为最远距离的 x 坐标
PathPlanningLane_t PathPlanning_IsLaneCleaned(Point16_t *p_lane_pos);
uint8_t PathPlanning_NormalClean(Point16_t start_cell);//获取正常清扫时候的路径终点
uint8_t PathPlanning_ShortestPath(Point16_t robot_cell,uint8_t first_line_width);
uint8_t PathPlanning_ShortestCreate(Point16_t robot_cell,uint8_t first_line_width);
void PathPlanning_CreatePathPoint(Point16_t robot_cell);
PathPlanningState_t PathPlanning_Cleaning(Point16_t start_cell);
PathPlanningState_t PathPlanning_BackHome(Point16_t start_cell);
PathPlanningState_t PathPlanning_WallFind(Point16_t start_cell,Mode_t mode);
void PathPlanning_Spot(Point16_t start_cell);




#endif





