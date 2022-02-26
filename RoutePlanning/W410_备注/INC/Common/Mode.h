#ifndef __MODE_H
#define __MODE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum 
{
	MODE_NONE = 0,
	MODE_USERINTERFACE,
	MODE_WALL,		
	MODE_CHARGE,						
	MODE_SLEEP,					
	MODE_TEST,				
	MODE_REMOTE,		
	MODE_SPOT,				
	MODE_NAVIGATION,			
	MODE_AREA,				
	MODE_GO_HOME,       	             										
}Mode_t;

typedef struct 
{
	Mode_t mode;
	uint16_t selection;
}CleanMode_t;

extern CleanMode_t g_clean_mode;
void Mode_Init(void);
void Mode_SetMode(Mode_t mode);
Mode_t Mode_GetMode(void);
void Mode_SelectionCycle(void);






#endif















