#ifndef __VACUUM_H
#define __VACUUM_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define VAC_SPEED_MAX                 1550
#define VAC_SPEED_NORMAL              1350
#define VAC_SPEED_LOW              	  1000

#define VACUUM_STALL_LIMIT        (uint16_t)1500 
#define VACUUM_STALL_NOLOAD       (uint16_t)200 

typedef uint16_t (*Vacuum_GetAdc)(void);
typedef void (*Vacuum_SetPowerState)(uint8_t state);
typedef struct 
{
      int32_t speed;
      int32_t temp_pwm;
      int32_t pulse_cnt;
      uint8_t fail_flag;
      int16_t baseline;
      int16_t current;
      Vacuum_GetAdc on_get_adc;
      Vacuum_SetPowerState on_set_power_state;
      volatile uint32_t *p_pwm;
}Vacuum_t;

extern Vacuum_t g_vacuum;


void Vacuum_Init(void);
void Vacuum_TuneProcess(void);
void Vacuum_SetSpeed(uint32_t speed);
int32_t Vacuum_GetSpeed(void);
void Vacuum_StoreBaseline(void);
void Vacuum_CurrentDetectionCycle(float adc2current);
uint8_t Vacuum_IsFail(void);
void Vacuum_ResetFail(void);
uint8_t Vacuum_CheckCurrent(void) ;
int16_t Vacuum_GetCurrent(void);
#endif
