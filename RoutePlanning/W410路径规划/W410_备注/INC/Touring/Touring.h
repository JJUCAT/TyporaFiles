#ifndef __TOURING_H
#define __TOURING_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


typedef enum 
{
	TOURING_RESULT_NONE = 0,
	TOURING_RESULT_OBSTACLE,
	TOURING_RESULT_ARRIVED,
	TOURING_RESULT_GO_HOME,
	TOURING_RESULT_BOUNDARY,
	TOURING_RESULT_FINDWALL,	
	TOURING_RESULT_FIND_CHARGE,
	TOURING_RESULT_ARRIVE_CHARGE,  
	TOURING_RESULT_NO_CHARGE,  
	TOURING_RESULT_TIME_OUT,
	TOURING_RESULT_TRAP,
	TOURING_RESULT_CALCULATE_GYRO,
	TOURING_RESULT_WALL_BACK_START,
}TouringResult_t;


typedef enum
{
    TOURING_INIT = 0,
    TOURING_ING,		
    TOURING_FIND_WALL,
    TOURING_BACK_HOME,
    TOURING_FIND_CHARGER,
    TOURING_WALL_CHARGE,
    TOURING_DOCKING, 
	TOURING_WALL,
	TOURING_SPOT,
}Touring_t;

typedef struct 
{
	uint8_t start_time_init;
	uint32_t start_time;
}TouringBackHome_t;


void Touring_Start(void);
void Touring_End(void);
void Touring_GoStation(void);
void Touring_GoStartPoint(void);
void Touring_ExploreStation(void);
void Touring_GoBoundary(void);
void Touring_MoveStop(void);
void Touring_UpdatePosition(uint16_t heading_0, int16_t heading_1, int16_t left, int16_t right); 
void Touring_UpdateMapBumper(BumperTrig_t bumper);
void Touring_UpdateMapCliff(CliffTrig_t cliff);
void Touring_UpdateMapObs(ObsTrig_t obs);
void Touring_UpdateMapCharge(void);
void Touring_UpdateMapWall(WallDir_t dir);
WallDir_t Touring_GetWallDir(Point16_t target_cell);
uint8_t Touring_IsBackhomeTimeout(void);


#endif














