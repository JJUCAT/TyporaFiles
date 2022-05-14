#ifndef __ALICLOUD_H
#define __ALICLOUD_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


extern AliStruct_t g_ali;
extern AliCloud_MapGetCell on_ali_get_cell;
extern AliCloud_SetPowerState on_set_power_state;

void AliCloud_Cycle(void);
void AliCloud_CommProcess(void);
void AliCloud_Init(void);
void AliCloud_Reset(void);
void AliCloud_ResetParameters(void);
void AliCloud_GetMsg(AliAttrIdType_t attr_id);
void AliCloud_SetMsg(AliAttrIdType_t attr_id);
void AliCloud_ReportMsg(AliAttrIdType_t attr_id);
void AliCloud_ReportErrorMsg(AliEventIdType_t event_id);
void AliCloud_InsertAck(AliPacketStruct_t *rec_packet);
void AliCloud_SetWifiState(AliWifiState_t state);
void AliCloud_CheckStationStartState(void);
void AliCloud_CheckParameters(void);
void AliCloud_CheckWorkMode(void);
void AliCloud_CheckBattery(void);
void AliCloud_CheckError(void);
void AliCloud_RefreshCloud(void);
void AliCloud_NavCleanStart(void);
void AliCloud_NavCleanEnd(void);
void AliCloud_ResetStartNav(void);
uint32_t AliCloud_GetCurrentUtc(void);

void AliCloud_AddRealMapToSend(Point16_t point);
void AliCloud_RealMapReprotCycle(void);
void AliCloud_TimerCycle(void);
void AliCloud_FindingRobotCycle(void);

void AliCloud_CompressionOfRun_WithType(AliHistoryCompression_t type);
void AliCloud_SendHistoryMap(uint32_t start_time,uint16_t cleaned_area,uint16_t cleaned_time);
void AliCloud_MapHistoryReprotCycle(void);
uint8_t AliCloud_IsMapHistoryBusy(void);
uint8_t AliCloud_IsParameterUpdate(void);
void AliCloud_UpdateRobot(Point16_t cell,int16_t angle);
void AliCloud_UpdateRealMapEdg(int16_t x_min,int16_t x_max,int16_t y_min,int16_t y_max);
void AliCloud_UpdateWorkMode(void);
void AliCloud_UpdateCleanDir(void);
void AliCloud_UpdateCleanCycle(void);
void AliCloud_UpdateResetFactory(void);
void AliCloud_UpdateWifiState(void);
void AliCloud_UpdateTimeZone(void);
void AliCloud_UpdateVoiceMute(void);
void AliCloud_UpdateMainBrush(void);
void AliCloud_UpdateWaterFlow(void);
void AliCloud_UpdateMoveSpeed(void);
void AliCloud_UpdateUnbanding(void);
void AliCloud_UpdateTimeCalculation(void);
void AliCloud_UpdateStates(void);
void AliCloud_CheckWifiDisplayState(void);
void AliCloud_SaveState(void);
#endif












