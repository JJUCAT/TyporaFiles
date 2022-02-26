#ifndef __EXCEPTION_H
#define __EXCEPTION_H

#include <stdint.h>
#include "Hw_SysInitialize.h"



typedef enum 
{
	EXCEPTION_ERROR_L_WHEEL  	=0x01,
	EXCEPTION_ERROR_R_WHEEL 	=0x02,
	EXCEPTION_ERROR_OBS         =0x04,
	EXCEPTION_ERROR_MAINBRUSH   =0x08,
	EXCEPTION_ERROR_VACUUM   	=0x10,		
	EXCEPTION_ERROR_BUMPER      =0x20,
	EXCEPTION_ERROR_MOBILITY    =0x40,
	EXCEPTION_ERROR_PUMPER      =0x80,
}ExceptionError_t;

typedef struct 
{
	uint16_t state;
	FunctionalState motors_check_state;
	FunctionalState sensors_check_state;
}Exception_t;

extern Exception_t g_exception;
void Exception_Init(void);
void Exception_Cycle(void);
uint16_t Exception_GetState(void);
void Exception_ResetState(void);
void Exception_MotorsSetCheckState(FunctionalState state);
void Exception_MotorsCheckCycle(void);
void Exception_SensorsSetCheckState(FunctionalState state);
void Exception_SensorsCheckCycle(void);
void Exception_MobilityDetection(void);
void Exception_Handler(uint16_t state);

#endif






