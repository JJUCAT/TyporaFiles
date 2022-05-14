#ifndef __CLIFF_H
#define __CLIFF_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define CLIFF_LIMIT          50//80

typedef void (*Cliff_PowerCtrl)(uint8_t state);
typedef void (*Cliff_SetWheelStop)(void);
typedef uint8_t (*Cliff_IsWheelForward)(void);
typedef struct
{
	volatile uint16_t left;
	volatile uint16_t front;
	volatile uint16_t right;
	volatile uint16_t temp_left;
	volatile uint16_t temp_front;
	volatile uint16_t temp_right;	
}CliffAdc_t;

typedef enum
{
	CLIFF_TRIG_NO    = 0,
	CLIFF_TRIG_LEFT  = 0x01,
	CLIFF_TRIG_RIGHT = 0x02,
	CLIFF_TRIG_FRONT = 0x04,
	CLIFF_TRIG_ALL   = 0x07,
}CliffTrig_t;

typedef struct
{
	FunctionalState state;
	CliffTrig_t result;
	CliffAdc_t adc;
	uint8_t stop;
	uint8_t l_trig_cnt;
	uint8_t f_trig_cnt;
	uint8_t r_trig_cnt;
	uint32_t check_time;
	uint8_t all_trig_cnt;
	Cliff_PowerCtrl on_power_ctrl;	
	Cliff_SetWheelStop on_set_wheel_stop;
	Cliff_IsWheelForward on_is_wheel_forward;
}Cliff_t;

extern Cliff_t g_cliff;

void Cliff_Init(void);
void Cliff_ForwardStopEnable(void);
void Cliff_DetectionProcess(void);
uint8_t Cliff_IsTrigStop(void);
uint8_t Cliff_IsPickUp(void);
CliffTrig_t Cliff_GetDetectiontProcess_Result(void);
CliffTrig_t Cliff_GetTrigStatus(void);
uint8_t Cliff_ContinueTrig(void);





#endif














