#ifndef __LINE_H
#define __LINE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define LINE_CNT			(300)

typedef struct
{
    int16_t xs;//x ��ʼ����(uint:cell)
    int16_t xe;//x ��������(uint:cell)
    int16_t xm;//x �е�����(uint:cell)
    int16_t y;//y ����(uint:cell)
    uint16_t level;////�� Line_FindAllLines() ע��
    uint16_t idx;//�� LineList_t �� line[] �е��±�
    uint8_t state;
}Line_t;

typedef struct
{
    uint16_t line_level;//�� Line_FindAllLines() ע��
    uint16_t line_cnt;//����� line ��Ŀ
    Line_t line[LINE_CNT];
}LineList_t;

typedef uint8_t (*Line_GetTargetCnt)(void);

extern Line_GetTargetCnt on_line_get_target_cnt;

typedef Point16_t (*Line_ReadTargetPnt)(uint8_t idx);

extern Line_ReadTargetPnt on_line_read_target_pnt;

typedef void (*Line_StoreTargetPnt)(Point16_t pnt);

extern Line_StoreTargetPnt on_line_store_target_pnt;

extern LineList_t g_line_list;

void Line_Init(void);
void Line_Reset(void);
uint16_t Line_GetLineCnt(void);
void Line_DeleteLine(uint16_t idx);
void Line_EditLine(Line_t l,uint16_t idx);
Line_t Line_ReadLine(uint16_t idx);
uint8_t Line_IsLineEnd(int16_t x,int16_t y,uint8_t width);//�ж�������Ƿ������� line ��������
Line_t Line_FindOneLine(Point16_t start_cell, uint8_t line_width);//�������Ѱ��һ��line
uint8_t Line_IsCellOnLine(Point16_t cell,Line_t line);
uint8_t Line_IsLineExist(Line_t line);
void Line_AddLine(Line_t line);
Line_t Line_GetNearLine(Line_t current_line);
uint8_t Line_FindNextLines(Line_t current_line);
void Line_DeleteSameLevel(Line_t * line);
void Line_SortLines(void);
uint8_t Line_FindAllLines(void);
uint32_t Line_GetAcreage(void);







#endif






