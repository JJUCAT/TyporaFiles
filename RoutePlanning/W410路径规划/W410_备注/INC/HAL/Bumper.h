#ifndef __BUMPER_H
#define __BUMPER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef uint8_t (*Bumper_IsTrig)(void);

typedef enum
{
	BUMPER_TRIG_NO    = 0,
	BUMPER_TRIG_LEFT  = 0x01,
	BUMPER_TRIG_RIGHT = 0x02,
	BUMPER_TRIG_ALL   = 0x03,
}BumperTrig_t;

typedef struct 
{
	uint8_t trig_cnt;
	Bumper_IsTrig on_left_trig;
	Bumper_IsTrig on_right_trig;
}Bumper_t;


extern Bumper_t g_bumper;

void Bumper_Init(void);
BumperTrig_t Bumper_GetTrigStatus(void);
uint8_t Bumper_IsStuck(void);

#endif







