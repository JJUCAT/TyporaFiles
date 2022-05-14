#ifndef __ALIGN_H
#define __ALIGN_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define ALIGN_SMOOTH_ANGLE_MAX 			4
#define ALIGN_CURVE_ANGLE_CNT           4
#define ALIGN_CURVE_ANGLE_MAX           (ALIGN_CURVE_ANGLE_CNT + 1)
#define ALIGN_ANGLE_DELTA               50
#define ALIGN_ANGLE_CURVE               50
#define ALIGN_ANGLE_CORRECT_MIN         50
typedef enum
{
    ALIGN_CORRECT_LINE_NONE = 0,
	ALIGN_CORRECT_LINE_SMOOTH,
	ALIGN_CORRECT_LINE_CURVE,
}AlignCorrectLine_t;


typedef struct
{
	uint8_t smooth_angle_cnt;	
	uint8_t smooth_angle_idx;
	int16_t smooth_angle[ALIGN_SMOOTH_ANGLE_MAX];
	uint8_t curve_angle_cnt;
	uint8_t curve_angle_idx;
	int16_t curve_angle[ALIGN_CURVE_ANGLE_CNT];
	uint8_t map;
}Align_t;

void Align_Init(void);
void Align_ResetSmoothAngleCnt(void);
void Align_CollectSmoothAngle(int16_t angle);
void Align_CollectCurveAngle(Point32_t point);
void Align_ResetCurveAngleCnt(void);
uint8_t Align_GetCurveAngleCnt(void);
uint8_t Align_CorrectLine(AlignCorrectLine_t type);
uint8_t Algin_Correct_Map(AlignCorrectLine_t type);
int16_t Align_CalculateSmoothAngle(void);
int16_t Align_GetAngleRight(int16_t angle);
void Align_RotateMap(int16_t angle_offset);
uint8_t Align_IsAlignMap(void);
void Align_SetAlignMap(void);
void Align_ResetAlginMap(void);




#endif




