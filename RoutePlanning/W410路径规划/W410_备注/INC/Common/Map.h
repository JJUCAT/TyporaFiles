#ifndef __MAP_H
#define __MAP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define MAP_DIMENSION		    160//地图直径(uint:dm)
#define MAP_SIZE			    (MAP_DIMENSION / CELL_SIZE)//地图直径(uint:cell)
#define MAP_LIMIT_LOW			(-MAP_SIZE/2)//地图坐标最小值(uint:cell)
#define MAP_LIMIT_HIGH			(MAP_SIZE/2)//地图坐标最大值(uint:cell)
#define MAP_LIMIT				(MAP_SIZE/2)//实际运行中限制的地图直径
#define MAP_ROBOT_SIZE			(3)//机身直径(uint:cell)

typedef enum 
{
	MAP_LIMIT_CLEAN = 0X01,
} MapLimit_t;

typedef enum 
{
	MAP_BOUNDARY_DIR_NONE = 0,
	MAP_BOUNDARY_DIR_EAST = 1,
	MAP_BOUNDARY_DIR_WEST = 2,
	MAP_BOUNDARY_DIR_SOUTH = 4,
	MAP_BOUNDARY_DIR_NORTH = 8,
}MapBoundary_Dir_t;

typedef struct
{
	int16_t east;//地图东边(右)边界坐标(uint:cell)
	int16_t west;//地图西边(左)边界坐标(uint:cell)
	int16_t north;//地图北边(上)边界坐标(uint:cell)
	int16_t south;//地图南边(下)边界坐标(uint:cell)
	MapBoundary_Dir_t dir;//机器当前穿越的边界
	uint8_t arrive_x;//到了地图 x 轴边界
	uint8_t arrive_y;//到了地图 y 轴边界
}MapBoundary_t;



typedef struct 
{
	uint8_t data[MAP_SIZE+1][(MAP_SIZE + 1) / 4];//内存上的地图，坐标是正的
	uint8_t limit;//清扫中的地图限制
	Point16_t pre_robot_cell;//上次更新位置时刻的机器坐标(uint:cell)
	uint8_t robot_static_cnt;//检查到机器静止的计数
	double count_x;//边轮脉冲表示的机器当前 x 坐标(uint:count)
	double count_y;//边轮脉冲表示的机器当前 y 坐标(uint:count)
	double relative_sin;//机器当前航向角的 sin 值
	double relative_cos;//机器当前航向角的 cos 值
	int16_t x_min;//机器在地图中经过的 x 坐标最小值(uint:cell)
	int16_t x_max;//机器在地图中经过的 x 坐标最大值(uint:cell)
	int16_t y_min;//机器在地图中经过的 y 坐标最小值(uint:cell)
	int16_t y_max;//机器在地图中经过的 y 坐标最大值(uint:cell)
	int16_t x_adjust_min;//实际运行中限制的边界 x 坐标最小值(uint:cell)
	int16_t x_adjust_max;//实际运行中限制的边界 x 坐标最大值(uint:cell)
	int16_t y_adjust_min;//实际运行中限制的边界 y 坐标最小值(uint:cell)
	int16_t y_adjust_max;//实际运行中限制的边界 y 坐标最大值(uint:cell)
	int16_t x_edg_min;//地图边界 x 坐标最小值(uint:cell)
	int16_t x_edg_max;//地图边界 x 坐标最大值(uint:cell)
	int16_t y_edg_min;//地图边界 y 坐标最小值(uint:cell)
	int16_t y_edg_max;//地图边界 y 坐标最大值(uint:cell)
	MapBoundary_t boundary;//地图边界，由上面 adjust_xxx 坐标调整得来(uint:cell)
	Point16_t home_cell;//充电座坐标(uint:cell)
	Point16_t level_cell;//清扫起点或者找到墙后的点，地图起点(uint:cell)
	volatile uint8_t *p_clean_cur_cnt;//@LMR???已清扫的次数，洗地机会洗两次地
}Map_t;

extern Map_t g_map;

typedef void (*Map_WifiRealMapSendPoint)(Point16_t point);
extern 	Map_WifiRealMapSendPoint on_map_wifi_realmap_send_point;//wifi 地图数据发送接口
typedef void (*Map_RealTimeMonitorSendData)(int32_t x, int32_t y, int32_t angle, uint8_t type);
extern 	Map_RealTimeMonitorSendData on_map_realtime_monitor_send_data;//调试用的地图数据发送接口
typedef uint16_t (*Map_GetAngle)(uint8_t idx);
extern Map_GetAngle on_map_get_angle;//获取航向角(uint:0.1°)


void Map_Init(void);
void Map_SetLevelCell(Point16_t cell);
Point16_t Map_GetLevelCell(void);
void Map_EditLimit(MapLimit_t limit);
void Map_ResetLimit(MapLimit_t limit);
uint8_t Map_IsLimit(MapLimit_t limit);
uint8_t Map_GetLimit(void);
int32_t Map_CellToCount(int16_t cell);//cell 转 count 
int16_t Map_CountToCell(double count);//count 转 cell
Point32_t Map_CellToPoint( Point16_t cell );//cell 坐标转 count 坐标
Point16_t Map_PointToCell( Point32_t pnt );//count 坐标转 cell 坐标
int16_t Map_GetRobotCellX(void);
int16_t Map_GetRobotCellY(void);
Point16_t Map_GetRobotCell(void);
void Map_SetRobotCell(int16_t x,int16_t y);//机器当前位置(uint:cell)
int32_t Map_GetRobotCountX(void);
int32_t Map_GetRobotCountY(void);  
Point32_t Map_GetRobotCount(void);//机器当前位置(uint:count)
void Map_SetRobotCount(double x, double y);
void Map_CountUpdate(double d_x, double d_y);
void Map_Adjust_Mapsize(int16_t x,int16_t y);
int16_t Map_CellToMap(int16_t data);//地图坐标转内存地图坐标
void Map_Reset(void);
uint8_t Map_GetMapArray(int16_t x,int16_t y);//通过内存地图坐标获取坐标点状态
void Map_SetMapArray(int16_t x,int16_t y,uint8_t data);//设置内存地图坐标点状态
void Map_SetCell(int16_t x, int16_t y, CellState_t value);//设置地图坐标点状态
CellState_t Map_GetCell(int16_t x, int16_t y);//通过地图坐标获取坐标点状态
void Map_BoundarySet(int16_t edg);
void Map_BoundarySetRange(Point16_t robot_cell,uint16_t range);//通过地图坐标获取坐标点状态
uint8_t Map_BoundaryReach(void);//更新边界状态
uint8_t Map_BoundaryNear(void);//即将穿过边界
Point32_t Map_BoundaryGetPoint(void);//获取地图边界交点坐标
Point32_t Map_BoundaryGetNextPoint(Point32_t current_target_point);
void Map_SetMapEdg(int16_t edg);
uint8_t Map_IsCellInMapEdg(int16_t x,int16_t y);//检查坐标点是否在边界内
uint8_t Map_IsCellUncleanWithCleanCnt(int16_t x,int16_t y);//检查坐标点是否未达到清扫次数
uint8_t Map_IsCellBlocked(int16_t x, int16_t y);
uint8_t Map_IsCellCleaned(int16_t x, int16_t y);
uint8_t Map_IsCellAccessible(int16_t x, int16_t y);//检查坐标点四周一圈是否有障碍物
uint8_t Map_IsCellUncleanLaneAccessible(int16_t x, int16_t y);//检查坐标点和 y 轴上下相邻点是否未清扫，且坐标点四周无障碍
uint8_t Map_IsCellBetweenBlockedCells(int16_t x,int16_t y);
uint8_t Map_IsCellBesideBlocked(int16_t x, int16_t y);//检查坐标点上下左右是否有障碍物
uint8_t Map_IsCellUncleanAccessible(int16_t x, int16_t y);
uint8_t Map_IsBlockUncleanAccessible(int16_t x, int16_t y);
void Map_Filter(void);
uint32_t Map_GetCellAcreage(void);
uint8_t Map_IsReach_NewCell(Point16_t cur_cell,Point16_t *pre_cell);
uint8_t Map_IsUncleanCellsOnFront(void);
void Map_NormalInitialize(void);
void Map_AreaInitialize(void);
uint8_t Map_IsTwoCellEqual(Point16_t start_cell,Point16_t exit_cell);
void Map_UpdatePreRobotCell(Point16_t robot_cell);
uint8_t Map_IsRobotNotMoving(void);
uint8_t Map_IsRobotNearPosCount(Point32_t point);
uint8_t Map_RobotCloseToPosCount(Point32_t point);
uint8_t Map_IsTwoPointsNear(Point32_t pos_a,Point32_t pos_b);
void Map_SetHomeCell(int16_t x,int16_t y);
Point16_t Map_GetHomeCell(void);
void Map_SetCellCleaned(int16_t x, int16_t y);
void Map_SetCellRobotSizeClean(int16_t x, int16_t y);
void Map_SetBlockedByOffset(int16_t offset_lat,int16_t offset_long);
int32_t Map_GetRobotToPointAngle(Point32_t point);
uint16_t Map_GetTwoCellsDistance(Point16_t start_cell,Point16_t exit_cell);
uint32_t Map_GetTwoPointsDistance(Point32_t start_point,Point32_t exit_point);
void Map_Rotate_Point(Point32_t origin,Point32_t input,Point32_t *output,int16_t theta);
void Map_Rotate_Cell(Point16_t origin,Point16_t input,Point16_t *output,int16_t theta);
Point16_t Map_GetRelativeByCell(Point16_t target_cell,int16_t heading, int16_t offset_lat, int16_t offset_long);
void Map_ClearSpotCleanCells(int8_t idx); 
void Map_ResetMap(int16_t x, int16_t y);
#endif













