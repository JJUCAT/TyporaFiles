#ifndef __MAINBRUSH_H
#define __MAINBRUSH_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define MAINBRUSH_BASE_VOLTAGE      54000
#define MAINBRUSH_VOLTAGE         	87000
#define MAINBRUSH_SPOT_VOLTAGE	    120000

#define MAINBRUSH_STALL_LIMIT    	  (uint16_t)920    
#define MAINBRUSH_STALL_NOLOAD        (uint16_t)0    

typedef uint16_t (*MainBrush_GetAdc)(void);

typedef struct 
{
    FunctionalState voltage_control;   
    int32_t voltage;  
    int32_t adjust_voltage;
    int32_t temp_voltage;
    int16_t baseline;
    int16_t current;
    uint16_t stall_cnt;
    volatile uint16_t *p_battery_voltage;   
    MainBrush_GetAdc on_get_adc;   
    volatile uint32_t *p_pwm;
}MainBrush_t;

extern MainBrush_t g_mainbrush;

void MainBrush_Init(void);
int32_t MainBrush_GetVoltage(void);
void MainBrush_SetVoltage(int32_t voltage);
void MainBrush_SetVoltageControl(FunctionalState state);
void MainBrush_StoreBaseline(void);
void MainBrush_VoltageControl(void);
uint8_t MainBrush_GetSpeed(void);
void MainBrush_SetCurrent(int16_t current);
int16_t MainBrush_GetCurrent(void);
void MainBrush_SetPWM(uint8_t pwm);
uint8_t MainBrush_CheckCurrent(void);
void MainBrush_CurrentDetectionCycle(float adc2current);

#endif



