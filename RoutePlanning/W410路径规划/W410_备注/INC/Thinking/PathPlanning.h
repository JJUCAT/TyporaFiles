#ifndef __PATHPLANNING_H
#define __PATHPLANNING_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum
{
	PATHPLANNING_LANE_BOTH_SIDE_BLOCKED = 0,
	PATHPLANNING_LANE_ONE_SIDE_UNCLEAN,
	PATHPLANNING_LANE_BOTH_SIDE_UNCLEAN,
}PathPlanningLane_t;// line �����������������ɨ���

typedef enum
{
	PATHPLANNING_STATE_NOTARGET = 0,
	PATHPLANNING_STATE_NORMAL_CLEAN,
	PATHPLANNING_STATE_UNREACHABLE,
	PATHPLANNING_STATE_ARRIVE,
}PathPlanningState_t;

extern volatile uint8_t g_pathplanning_cleaned_rate;

uint8_t PathPlanning_IsSpotLaneCleaned(uint8_t idx ,Point16_t start_cell, int8_t dir);
//�����������ڵ�·��(ƽ�� x �ᣬ�ҿ��Ϊһ������)����ɨ���
//return @arg 0:����·�����ұ��ϰ��ﵲס
//return @arg 1:����·��һ��δ��ɨ��p_lane_pos->x Ϊ��Զ����� x ����
//return @arg 1:����·������δ��ɨ��p_lane_pos->x Ϊ��Զ����� x ����
PathPlanningLane_t PathPlanning_IsLaneCleaned(Point16_t *p_lane_pos);
uint8_t PathPlanning_NormalClean(Point16_t start_cell);//��ȡ������ɨʱ���·���յ�
uint8_t PathPlanning_ShortestPath(Point16_t robot_cell,uint8_t first_line_width);
uint8_t PathPlanning_ShortestCreate(Point16_t robot_cell,uint8_t first_line_width);
void PathPlanning_CreatePathPoint(Point16_t robot_cell);
PathPlanningState_t PathPlanning_Cleaning(Point16_t start_cell);
PathPlanningState_t PathPlanning_BackHome(Point16_t start_cell);
PathPlanningState_t PathPlanning_WallFind(Point16_t start_cell,Mode_t mode);
void PathPlanning_Spot(Point16_t start_cell);




#endif





