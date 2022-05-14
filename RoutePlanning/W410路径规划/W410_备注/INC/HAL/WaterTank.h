#ifndef __WATERTANK_H
#define __WATERTANK_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define WATERTANK_ERROR_TIME_LIMIT      15
#define WATERTANK_CLEAN_L_LIMIT 		2500
#define WATERTANK_CLEAN_H_LIMIT 		3000
#define WATERTANK_DIRTY_L_LIMIT 		1500
#define WATERTANK_DIRTY_H_LIMIT 		2000

typedef uint8_t (*WaterTank_GetTankState)(void);
typedef void (*WaterTank_SetDriverState)(uint8_t state);
typedef struct
{
	volatile uint16_t clean;
	volatile uint16_t dirt;
}WaterAdc_t;

typedef struct
{
	FunctionalState state;
	FunctionalState check_state;
	WaterAdc_t adc;
	WaterAdc_t temp_cln_dirt_ad;
	uint8_t clean_error_cnt;
	uint8_t dirt_error_cnt;	
	WaterTank_GetTankState on_get_tank_state;
	WaterTank_SetDriverState on_set_clean_driver_state;
	WaterTank_SetDriverState on_set_dirty_driver_state;
}WaterTank_t;

extern WaterTank_t g_watertank;

void WaterTank_Init(void);
void WaterTank_SetCheckState(FunctionalState state);
FunctionalState WaterTank_GetCheckState(void);
uint8_t WaterTank_IsInstall(void);
void WaterTank_CheckProcess(FunctionalState state);
void WaterTank_ResetErrorCnt(void);
#endif


