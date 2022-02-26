#ifndef __ALICLOUDTX_H
#define __ALICLOUDTX_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

extern AliStruct_t *p_ali_tx;
typedef void (*AliCloudTx_UsartSend)(uint16_t length,char *data);
extern AliCloudTx_UsartSend on_ali_tx_usart_send;
extern AliCloud_MapGetCell on_ali_tx_get_cell;

void AliCloudTx_Init(void);
void AliCloudTx_ListReset(void);
uint8_t AliCloudTx_IsListError(void);
uint8_t AliCloudTx_ListPush(AliTxMsgStruct_t msg);
uint8_t AliCloudTx_ListShift(AliTxMsgStruct_t *msg);
uint8_t AliCloudTx_GetListCnt(void);
uint16_t AliCloudTx_SendData(void);
uint16_t AliCloudTx_GetSize(void);
void AliCloudTx_ClearSize(void);
uint8_t AliCloudTx_AttrIdTpTypeId(AliAttrIdType_t attrid);
void AliCloudTx_InSertPayload(AliTxMsgStruct_t msg);
void AliCloudTx_PayloadData_Uint32_t(char *p_data,uint32_t data);
void AliCloudTx_PayloadData_Uint16_t(char *p_data,uint16_t data);
void AliCloudTx_PayloadData_Double_t(char *p_data,double data);
void AliCloudTx_RealMapListReset(void);
uint8_t AliCloudTx_RealMapListPush(Point16_t point);
uint8_t AliCloudTx_RealMapListShift(Point16_t *point);
void AliCloudTx_RealMapBusyListReset(void);
uint8_t AliCloudTx_RealMapBusyListPush(Point16_t point);
uint8_t AliCloudTx_RealMapBusyListShift(Point16_t *point);
void AliCloudTx_HistoryMapListReset(void);
uint8_t AliCloudTx_HistoryMapListPush(AliMapCompress_t input_data);
uint8_t AliCloudTx_HistoryMapListShift(AliMapCompress_t *output_data);
uint8_t AliCloudTx_HistoryMapListFull(void);


#endif



















