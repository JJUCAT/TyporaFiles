#ifndef __OBS_H
#define __OBS_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void (*Obs_SetDriverState)(uint8_t state);

typedef struct
{
	volatile uint16_t left;
	volatile uint16_t front;
	volatile uint16_t right;
	volatile uint16_t temp_left;
	volatile uint16_t temp_front;
	volatile uint16_t temp_right;	
}ObsAdc_t;

typedef enum
{
	OBS_TRIG_NONE			= 0,
	OBS_TRIG_LEFT		= 0x01,
	OBS_TRIG_FRONT		= 0x02,
	OBS_TRIG_RIGHT		= 0x04,
	OBS_TRIG_ALL		= 0x07,
}ObsTrig_t;

typedef enum
{
	OBS_NO_STOP			= 0,
	OBS_LEFT_STOP    = 1,
	OBS_FRONT_STOP   =	2,
	OBS_RIGHT_STOP   =	4,
}ObsStop_t;


typedef struct
{
	FunctionalState state ;
	FunctionalState dynamic_state;	
	ObsAdc_t adc;
	int16_t left_trig_val;
	int16_t front_trig_val;
	int16_t right_trig_val;	
	Obs_SetDriverState on_set_driver_state;
}Obs_t;

extern Obs_t g_obs;


#define OBS_LEFT_TRIG_BIAS	300	
#define OBS_FRONT_TRIG_BIAS	300
#define OBS_RIGHT_TRIG_BIAS	300	
#define OBS_NEAR_TRIG 150

void OBS_Init(void);
void OBS_DynamicProcess(FunctionalState state);
void OBS_SetDynamicState(FunctionalState state);
FunctionalState OBS_GetDynamicState(void);
ObsTrig_t OBS_GetTrigStatus(void);
uint8_t OBS_FrontClose(void);
uint8_t OBS_IsNear(void);
void OBS_IsTrigLimit(ObsTrig_t *status);


#endif













