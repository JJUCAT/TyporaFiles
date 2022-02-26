#ifndef __SPOT_H
#define __SPOT_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define SPOT_RANGE 4

typedef enum
{
	SPOT_DIR_NONE = 0,
	SPOT_DIR_CW = -1,
	SPOT_DIR_CCW = 1,
	SPOT_DIR_IN	= 0,
	SPOT_DIR_OUT = 1,
}SpotDir_t;

typedef struct 
{
	SpotDir_t dir_outin;
	SpotDir_t dir_cw;
	uint8_t lane_idx; 	
}Spot_t;

extern Spot_t g_spot;

void Spot_Init(void);
Point16_t Spot_GetNextLanePos(Point16_t start_cell,uint8_t dir,uint8_t step);
uint8_t Spot_IsOnLane(uint8_t idx,Point16_t robot_cell); 
uint16_t Spot_GetPointLaneIdx(Point16_t point); 
void Spot_SetDirCw(SpotDir_t dir);
SpotDir_t Spot_GetDirCw(void);
void Spot_SetDirOutin(SpotDir_t dir);
SpotDir_t Spot_GetDirOutin(void);
void Spot_SetLaneIdx(uint8_t idx);
uint8_t Spot_GetLaneIdx(void);







#endif



































