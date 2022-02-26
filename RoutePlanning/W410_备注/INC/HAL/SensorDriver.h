#ifndef __SENSORDRIVER_H
#define __SENSORDRIVER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void(*SensorAdcStart)(void);

typedef struct 
{
    uint8_t h_start;
    uint8_t l_start;
    uint8_t start;
    uint8_t cycle_cnt;
    uint8_t on;
    uint8_t adc;
    SensorAdcStart on_start_adc;
}SensorDriver_t;


extern SensorDriver_t g_sensordriver;





void SensorDriver_Init(void);
void SensorDriver_Process(FunctionalState cliff_state,FunctionalState obs_wall_state,FunctionalState cln_dirty_water_state,uint8_t code_show);
void SensorDriver_Start(void);

#endif














