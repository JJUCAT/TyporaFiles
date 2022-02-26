#ifndef __ERRORCODE_H
#define __ERRORCODE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum
{
	ERROR_NONE				=0x00,
	ERROR_BUMPER 			=0x01,
	ERROR_OBS				  =0x02,
	ERROR_WALL				=0x03,
	ERROR_CLIFF				=0x04,
	ERROR_PICK_UP			=0x05,
	ERROR_MOBILITY 			=0x06,
	ERROR_L_BRUSH			=0x07,
	ERROR_R_BRUSH			=0x08,
	ERROR_S_BRUSH			=0x09,
	ERROR_LEFT_WHEEL		=0x0A,
	ERROR_RIGHT_WHEEL		=0x0B,
	ERROR_MAINBRUSH			=0x0C,
	ERROR_VAC				   =0x0D,
	ERROR_WATER_PUMP		=0x0E,
	ERROR_AIR_PUMP     		=0x0F,
	ERROR_DUSTBIN			=0x10,
	ERROR_TANK				=0X11,	
	ERROR_STRAINER			=0X12,
	ERROR_BATTERY			=0X13,
	ERROR_GYRO				=0X14,
	ERROR_RADAR				=0X15,
	ERROR_CAMERA			=0X16,
	ERROR_STUCK				=0X17,
	ERROR_MOUSE				=0X18,
	ERROR_OTHER				=0X19,
	ERROR_LOW_LIGHT			=0X1A,
	ERROR_CLEAN_TANK		=0X1B,
	ERROR_DIRT_TANK			=0X1C,
	ERROR_LOW_BATTERY		=0X1D,
	ERROR_PUMP				=0X1F,	
	ERROR_END				=0X20,
}Error_t;

extern volatile Error_t g_error_code;

Error_t Error_GetCode(void);
void Error_SetCode(Error_t code);
void Error_ResetCode(void);


#endif





