#ifndef __HEADING_H
#define __HEADING_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef struct
{
	int32_t previous;
	int32_t wallfollow;
	int32_t movetoposition;
	int32_t global;
	int32_t head2cource;
}Heading_Integral_t;

typedef enum
{
	HEADING_INTEGRAL_PREVIOUS = 0x01,
	HEADING_INTEGRAL_WALLFOLLOW = 0x02,
	HEADING_INTEGRAL_MOVETOPOSITION = 0x04,
	HEADING_INTEGRAL_GLOBAL = 0x08,
	HEADING_INTEGRAL_HEAD2COURCE = 0x10,
}Heading_Integral_Type_t;

typedef enum
{
	HEADING_DIR_EAST       = 0, 
	HEADING_DIR_NORTH_EAST = 450,
	HEADING_DIR_NORTH      = 900,
	HEADING_DIR_NORTH_WEST = 1350,
	HEADING_DIR_WEST       = 1800,
	HEADING_DIR_SOUTH_WEST = 2250,
	HEADING_DIR_SOUTH      = 2700,
	HEADING_DIR_SOUTH_EAST = 3150,
	HEADING_DIR_DIR_NONE   = 3600,
}HeadingDir_t;

typedef uint16_t (*Heading_GetAngle)(uint8_t idx);
extern Heading_GetAngle on_heading_get_angle;
extern volatile int16_t g_heading_target_angle;
extern Heading_Integral_t g_heading_integral;

void Heading_Init(void);
void Heading_Update_Integral(uint16_t cur_heading);
int32_t Heading_Get_Integral(Heading_Integral_Type_t type);
void Heading_Reset_Integral(Heading_Integral_Type_t type);
HeadingDir_t Heading_GetHeading4(uint16_t angle);
uint16_t Heading_GetHeading8(void);
uint16_t Heading_GetCurrentDir(void);

















#endif













