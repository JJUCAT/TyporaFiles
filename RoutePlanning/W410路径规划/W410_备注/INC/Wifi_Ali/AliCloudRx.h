#ifndef __ALICLOUDRX_H
#define __ALICLOUDRX_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


extern AliStruct_t *p_ali_rx;
extern AliCloud_InsertTheAck on_ali_rx_insert_ack;

void AliCloudRx_Init(void);
void AliCloudRx_ParseMsg(char * data_fifo, uint16_t length);
void AliCloudRx_ParsePacket(char *rec_data);
void AliCloudRx_DecodePacket(AliPacketStruct_t  *packet,char *rec_data);
void AliCloudRx_ParseStreamSet(AliPacketStruct_t  *packet);
void AliCloudRx_ParseStreamAck(AliPacketStruct_t  *packet);
uint32_t AliCloudRx_GetUtcWithDay(AliTimeCalibration_t time_calibration,uint16_t time_zone);
void AliCloudRx_Parse_RxLink_TimeCalibration(char *data);
void AliCloudRx_Parse_RxLink_WorkMode(char *data);
void AliCloudRx_Parse_RxLink_CleanLoopCnt(char *data);
void AliCloudRx_Parse_RxLink_ResFactory(char *data);
void AliCloudRx_Parse_RxLink_WifiState(char *data);
void AliCloudRx_Parse_RxLink_Utc(char *data);
void AliCloudRx_Parse_RxLink_TimeZone(char *data);
void AliCloudRx_Parse_RxLink_WifiMac(char *data);
void AliCloudRx_Parse_RxLink_FactoryTest(char *data);
void AliCloudRx_Parse_RxLink_BeepNoDisturb(char *data);
void AliCloudRx_Parse_RxLink_MainBrushPower(char *data);
void AliCloudRx_Parse_RxLink_WaterControl(char *data);
void AliCloudRx_Parse_RxLink_WheelSpeed(char *data);
void AliCloudRx_Parse_RxLink_FindRobot(char *data);
void AliCloudRx_Parse_RxLink_RemoteDir(char *data);
void AliCloudRx_Parse_RxLink_Unbanding(char *data);

#endif



