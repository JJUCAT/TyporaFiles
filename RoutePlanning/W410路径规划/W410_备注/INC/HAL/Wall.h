#ifndef __WALL_H
#define __WALL_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define WALL_DEFAULT_DISTANCE	300

typedef struct
{
	volatile uint16_t left;
	volatile uint16_t right;	
	volatile uint16_t temp_left;
	volatile uint16_t temp_right;	
}WallAdc_t;

typedef struct
{
	WallAdc_t adc;
	FunctionalState state;
	int16_t left_baseline;
	int16_t right_baseline;
	FunctionalState dynamic_state;
	int32_t distance;
	int16_t buffer[3];
}Wall_t;

extern Wall_t g_wall;


void Wall_Init(void);
void Wall_SetDynamicState(FunctionalState state);
FunctionalState Wall_GetDynamicState(void);
int16_t Wall_GetRightAdcValue(void);
int16_t Wall_GetLeftAdcValue(void);
void Wall_DynamicProcess(FunctionalState state);
int32_t Wall_GetDistance(void);
void Wall_SetDistance(int32_t dis);
void Wall_ResetDistance(void);
void Wall_DistanceDecrease(void);
uint8_t Wall_UpdateBuffer(int16_t value);
#endif































