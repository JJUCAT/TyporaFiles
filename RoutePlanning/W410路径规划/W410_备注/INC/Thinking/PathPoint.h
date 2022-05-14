#ifndef __PATHPOINT_H
#define __PATHPOINT_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define PATH_POINT_CNT	(300)

typedef struct
{
    Point16_t point_list[PATH_POINT_CNT + 1]; 
    uint16_t cnt;
}PathPoint_t;

extern PathPoint_t g_pathpoint;

void PathPoint_Init(void);
uint16_t PathPoint_GetPointCnt(void);
Point16_t PathPoint_ReadPoint(int16_t idx);
void PathPoint_AddOnePoint(Point16_t point);
void PathPoint_EditPoint(Point16_t point,uint16_t idx);
void PathPoint_AddPathPointToPathList(uint16_t start_idx);
Point16_t PathPoint_ReadLastPoint(void);
void PathPoint_ClearAllPoints(void);
void PathPoint_RemoveOnePoint(uint16_t idx);
void PathPoint_RemoveLastPoint(void);
void PathPoint_InsertOnePoint(Point16_t point,uint8_t idx);
void PathPoint_SortPoints(void);


#endif






