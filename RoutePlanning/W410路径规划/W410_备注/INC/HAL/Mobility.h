#ifndef __MOBILITY_H
#define __MOBILITY_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef struct 
{
    FunctionalState check_state;
    uint8_t trig_cnt;
    uint8_t error_cnt;    
    uint32_t step;
    uint8_t error_flag;
}Mobility_t;

extern Mobility_t g_mobility;

void Mobility_Init(void);
uint8_t Mobility_GetTrigCnt(void);
void Mobility_ResetTrigCnt(void);
void Mobility_TrigCntIncrease(void);
void Mobility_SetCheckState(FunctionalState state);
FunctionalState Mobility_GetCheckState(void);
void Mobility_SetStep(uint32_t steps);
void Mobility_ResetStep(void);
uint32_t Mobility_GetStep(void);
uint8_t Mobility_IsError(void);
void Mobility_SetError(void);
void Mobility_ResetError(void);


#endif




