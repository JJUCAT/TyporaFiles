#ifndef __PUMP_H
#define __PUMP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define PUMP_VOLTAGE                    100000
#define PUMP_WATER_FLOW_LEVEL_lOW	    (1150 + 500) 
#define PUMP_WATER_FLOW_LEVEL_NORMAL	(1150) 
#define PUMP_WATER_FLOW_LEVEL_HIGH	    (1150 - 500)

typedef void(*Pump_SetStatus)(uint8_t state);

typedef struct
{
    FunctionalState start_control;
    FunctionalState voltage_control;
    FunctionalState state;
    uint8_t max;
    uint16_t cycle_cnt;
    uint16_t pre_cycle_cnt;
    int32_t temp_power;
    int32_t voltage;
    int32_t cnt;
    volatile uint16_t *p_battery_voltage;
    volatile uint32_t *p_pwm;
    Pump_SetStatus set_state;
}Pump_t;

extern Pump_t g_pump;
void Pump_Init(void);
void Pump_Process(void);
void Pump_SetVoltage(int32_t voltage);
void Pump_VoltageControl(void);
void Pump_SetVoltageControl(FunctionalState state);
void Pump_SetStartControl(FunctionalState state);
uint8_t Pump_GetSpeed(void);
void Pump_SetState(FunctionalState state);
FunctionalState Pump_GetState(void);
#endif




