#ifndef __TARGETLIST_H
#define __TARGETLIST_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define TARGET_LIST_AMOUNT 		100



typedef struct 
{
    Point16_t list[TARGET_LIST_AMOUNT];//Ԥ����Ŀ������꼯��(uint:cell)
    uint8_t cnt;//���˶��ٸ�Ŀ���
    Point16_t current_point;//Ҫ�����Ŀ�������
}TargetList_t;

extern TargetList_t g_target_list;
void TargetList_Init(void);
void TargetList_Clear(void);
uint8_t TargetList_ReadCnt(void);
Point16_t TargetList_ReadTarget(uint8_t idx);
void TargetList_DeleteTarget(uint8_t idx);
void TargetList_AddTarget(int16_t x,int16_t y);
uint8_t TargetList_IsCellTargetable(int16_t x,int16_t y);
uint8_t TargetList_CreateList(void);
void TargetList_Rearrange(void);
void TargetList_StoreCurrentTargetPoint(Point16_t target);
Point16_t TargetList_GetCurrentTargetPoint(void);
uint8_t TargetList_AddHomeCell(void);
uint8_t TargetList_AddHomeCells(void);






#endif

