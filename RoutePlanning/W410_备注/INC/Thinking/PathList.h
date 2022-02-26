#ifndef __PATH_H
#define __PATH_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define PATHLIST_CNT_LENGTH	40

typedef enum 
{
    PATHLIST_SHORT = 1,
    PATHLIST_NORMAL_CLEAN = 2,
    PATHLIST_SPOT_CLEAN = 3,
    PATHLIST_FIND_DOCKING_CELL = 4,
} PathListState_t;

typedef struct
{
	Point16_t cell_pos;
	PathListState_t status;
}PathListCell_t;

typedef struct
{
    PathListCell_t list[PATHLIST_CNT_LENGTH + 1];
    uint8_t list_cnt; 
    uint8_t list_input_idx; 
    uint8_t list_output_idx;
}PathList_t;

extern PathList_t g_pathlist;

typedef void (*PathList_RealTimeMonitorSendData)(int32_t x, int32_t y, int32_t angle, uint8_t type);
extern 	PathList_RealTimeMonitorSendData pathlist_realtime_monitor_send_data;



void PathList_Init(void);
void PathList_AddNewPoint(PathListCell_t point);//添加路径终点到路径列表
uint8_t PathList_Out(PathListCell_t *p_out);//获取最近的路径终点
uint8_t PathList_GetNextPathListPoint(Point32_t *point_result);//获取最近的路径终点
PathListCell_t PathList_ReadLastPath(void);
void PathList_Clear(void);
uint8_t PathList_GetCnt(void);

#endif








