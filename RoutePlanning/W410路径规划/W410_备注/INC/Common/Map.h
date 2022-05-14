#ifndef __MAP_H
#define __MAP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define MAP_DIMENSION		    160//��ͼֱ��(uint:dm)
#define MAP_SIZE			    (MAP_DIMENSION / CELL_SIZE)//��ͼֱ��(uint:cell)
#define MAP_LIMIT_LOW			(-MAP_SIZE/2)//��ͼ������Сֵ(uint:cell)
#define MAP_LIMIT_HIGH			(MAP_SIZE/2)//��ͼ�������ֵ(uint:cell)
#define MAP_LIMIT				(MAP_SIZE/2)//ʵ�����������Ƶĵ�ͼֱ��
#define MAP_ROBOT_SIZE			(3)//����ֱ��(uint:cell)

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
	int16_t east;//��ͼ����(��)�߽�����(uint:cell)
	int16_t west;//��ͼ����(��)�߽�����(uint:cell)
	int16_t north;//��ͼ����(��)�߽�����(uint:cell)
	int16_t south;//��ͼ�ϱ�(��)�߽�����(uint:cell)
	MapBoundary_Dir_t dir;//������ǰ��Խ�ı߽�
	uint8_t arrive_x;//���˵�ͼ x ��߽�
	uint8_t arrive_y;//���˵�ͼ y ��߽�
}MapBoundary_t;



typedef struct 
{
	uint8_t data[MAP_SIZE+1][(MAP_SIZE + 1) / 4];//�ڴ��ϵĵ�ͼ������������
	uint8_t limit;//��ɨ�еĵ�ͼ����
	Point16_t pre_robot_cell;//�ϴθ���λ��ʱ�̵Ļ�������(uint:cell)
	uint8_t robot_static_cnt;//��鵽������ֹ�ļ���
	double count_x;//���������ʾ�Ļ�����ǰ x ����(uint:count)
	double count_y;//���������ʾ�Ļ�����ǰ y ����(uint:count)
	double relative_sin;//������ǰ����ǵ� sin ֵ
	double relative_cos;//������ǰ����ǵ� cos ֵ
	int16_t x_min;//�����ڵ�ͼ�о����� x ������Сֵ(uint:cell)
	int16_t x_max;//�����ڵ�ͼ�о����� x �������ֵ(uint:cell)
	int16_t y_min;//�����ڵ�ͼ�о����� y ������Сֵ(uint:cell)
	int16_t y_max;//�����ڵ�ͼ�о����� y �������ֵ(uint:cell)
	int16_t x_adjust_min;//ʵ�����������Ƶı߽� x ������Сֵ(uint:cell)
	int16_t x_adjust_max;//ʵ�����������Ƶı߽� x �������ֵ(uint:cell)
	int16_t y_adjust_min;//ʵ�����������Ƶı߽� y ������Сֵ(uint:cell)
	int16_t y_adjust_max;//ʵ�����������Ƶı߽� y �������ֵ(uint:cell)
	int16_t x_edg_min;//��ͼ�߽� x ������Сֵ(uint:cell)
	int16_t x_edg_max;//��ͼ�߽� x �������ֵ(uint:cell)
	int16_t y_edg_min;//��ͼ�߽� y ������Сֵ(uint:cell)
	int16_t y_edg_max;//��ͼ�߽� y �������ֵ(uint:cell)
	MapBoundary_t boundary;//��ͼ�߽磬������ adjust_xxx �����������(uint:cell)
	Point16_t home_cell;//���������(uint:cell)
	Point16_t level_cell;//��ɨ�������ҵ�ǽ��ĵ㣬��ͼ���(uint:cell)
	volatile uint8_t *p_clean_cur_cnt;//@LMR???����ɨ�Ĵ�����ϴ�ػ���ϴ���ε�
}Map_t;

extern Map_t g_map;

typedef void (*Map_WifiRealMapSendPoint)(Point16_t point);
extern 	Map_WifiRealMapSendPoint on_map_wifi_realmap_send_point;//wifi ��ͼ���ݷ��ͽӿ�
typedef void (*Map_RealTimeMonitorSendData)(int32_t x, int32_t y, int32_t angle, uint8_t type);
extern 	Map_RealTimeMonitorSendData on_map_realtime_monitor_send_data;//�����õĵ�ͼ���ݷ��ͽӿ�
typedef uint16_t (*Map_GetAngle)(uint8_t idx);
extern Map_GetAngle on_map_get_angle;//��ȡ�����(uint:0.1��)


void Map_Init(void);
void Map_SetLevelCell(Point16_t cell);
Point16_t Map_GetLevelCell(void);
void Map_EditLimit(MapLimit_t limit);
void Map_ResetLimit(MapLimit_t limit);
uint8_t Map_IsLimit(MapLimit_t limit);
uint8_t Map_GetLimit(void);
int32_t Map_CellToCount(int16_t cell);//cell ת count 
int16_t Map_CountToCell(double count);//count ת cell
Point32_t Map_CellToPoint( Point16_t cell );//cell ����ת count ����
Point16_t Map_PointToCell( Point32_t pnt );//count ����ת cell ����
int16_t Map_GetRobotCellX(void);
int16_t Map_GetRobotCellY(void);
Point16_t Map_GetRobotCell(void);
void Map_SetRobotCell(int16_t x,int16_t y);//������ǰλ��(uint:cell)
int32_t Map_GetRobotCountX(void);
int32_t Map_GetRobotCountY(void);  
Point32_t Map_GetRobotCount(void);//������ǰλ��(uint:count)
void Map_SetRobotCount(double x, double y);
void Map_CountUpdate(double d_x, double d_y);
void Map_Adjust_Mapsize(int16_t x,int16_t y);
int16_t Map_CellToMap(int16_t data);//��ͼ����ת�ڴ��ͼ����
void Map_Reset(void);
uint8_t Map_GetMapArray(int16_t x,int16_t y);//ͨ���ڴ��ͼ�����ȡ�����״̬
void Map_SetMapArray(int16_t x,int16_t y,uint8_t data);//�����ڴ��ͼ�����״̬
void Map_SetCell(int16_t x, int16_t y, CellState_t value);//���õ�ͼ�����״̬
CellState_t Map_GetCell(int16_t x, int16_t y);//ͨ����ͼ�����ȡ�����״̬
void Map_BoundarySet(int16_t edg);
void Map_BoundarySetRange(Point16_t robot_cell,uint16_t range);//ͨ����ͼ�����ȡ�����״̬
uint8_t Map_BoundaryReach(void);//���±߽�״̬
uint8_t Map_BoundaryNear(void);//���������߽�
Point32_t Map_BoundaryGetPoint(void);//��ȡ��ͼ�߽罻������
Point32_t Map_BoundaryGetNextPoint(Point32_t current_target_point);
void Map_SetMapEdg(int16_t edg);
uint8_t Map_IsCellInMapEdg(int16_t x,int16_t y);//���������Ƿ��ڱ߽���
uint8_t Map_IsCellUncleanWithCleanCnt(int16_t x,int16_t y);//���������Ƿ�δ�ﵽ��ɨ����
uint8_t Map_IsCellBlocked(int16_t x, int16_t y);
uint8_t Map_IsCellCleaned(int16_t x, int16_t y);
uint8_t Map_IsCellAccessible(int16_t x, int16_t y);//������������һȦ�Ƿ����ϰ���
uint8_t Map_IsCellUncleanLaneAccessible(int16_t x, int16_t y);//��������� y ���������ڵ��Ƿ�δ��ɨ����������������ϰ�
uint8_t Map_IsCellBetweenBlockedCells(int16_t x,int16_t y);
uint8_t Map_IsCellBesideBlocked(int16_t x, int16_t y);//�����������������Ƿ����ϰ���
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













