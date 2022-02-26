#ifndef __CLEANCYCLECOUNT_H
#define __CLEANCYCLECOUNT_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef struct 
{
    uint8_t target_cnt;
    uint8_t cur_cnt;
    uint8_t set_cnt;
    uint8_t over_cnt;
    uint8_t pre_set_cnt;
}CleanCycleCount_t;

extern CleanCycleCount_t g_cleancycle_count;

void CleanCycleCount_Init(void);
void CleanCycleCount_Start(void);

uint8_t CleanCycleCount_IsArriveTargetCnt(void);







#endif





