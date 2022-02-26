#ifndef __CELL_H
#define __CELL_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define CELL_SIZE					(1)//一个栅格大小(uint:dm)
#define CELL_SIZE_2					(2 * CELL_SIZE)
#define CELL_SIZE_3					(3 * CELL_SIZE)
#define CELL_COUNT_MUL				681//栅格转脉冲(uint:count/cell)
#define CELL_COUNT_MUL_1_2			(CELL_COUNT_MUL/2)
#define CELL_COUNT_MUL_3_2			(3*CELL_COUNT_MUL/2)
#define CELL_COUNT_MUL_2			(CELL_COUNT_MUL*2)
#define CELL_COUNT_MUL_3			(CELL_COUNT_MUL*3)
#define CELL_COUNT_MUL_1_4			(CELL_COUNT_MUL/4)

typedef enum 
{
  CELL_STATE_UNCLEAN  = 0,
  CELL_STATE_CLEANED = 1,
  CELL_STATE_CLEANED_TWICE = 2,
  CELL_STATE_BLOCKED = 3,
} CellState_t;//栅格状态


#endif




