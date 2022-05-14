#ifndef __TUYA_H
#define __TUYA_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

extern TuyaStruct_t g_tuya;

void Tuya_Init(void);
void Tuya_Cycle(void);
void Tuya_SetHistory(void);
void Tuya_AskGreenTime(void);
void Tuya_AskLocalTime(void);
void Tuya_SetWifiMode(unsigned char mode);
void Tuya_CheckWorkModeOnChange(void);
void Tuya_TimerHandle(void);
void Tuya_CheckWaterFlow(void);
void Tuya_CheckBatteryOnChange(void);
void Tuya_UpdateAllStates(void);
void Tuya_CheckError(void);
void Tuya_SaveState(void);
void Tuya_CheckSpeakerEnable(void);
uint8_t Tuya_GetErrorCode(uint8_t code);
void Tuya_NavCleanStart(void);
void Tuya_NavCleanEnd(void);
void Tuya_FindingRobotHandle(void);
void Tuya_CheckWifiDisplayState(void);
void Tuya_ParseData(void);
void Tuya_DataHandle(unsigned short offset);
static void Tuya_HeartBeatCheck(void);
static void Tuya_ProductInfoUpdate(void);
static void Tuya_GetMcuMode(void);
void Tuya_GetGreentime(unsigned char time[]);
void Tuya_GetLocaltime(unsigned char time[]);
static unsigned char Tuya_DataPointHandle(const unsigned char value[]);
unsigned char Tuya_DpDownloadHandle(unsigned char dpid,const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpdownloadModeHandle(const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpDownloadFindRobotHandle(const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpDownloadSpeakerHandle(const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpDownloadFactoryResetHandle(const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpDownloadWaterControlHandle(const unsigned char value[], unsigned short length);
static unsigned char Tuya_DpDownloadDirectionControlHandle(const unsigned char value[], unsigned short length);
void Tuya_SetMapStreamState(unsigned char state);
void Tuya_MapStreamWaitResult(void);
void Tuya_EditMapStreamState(unsigned char state);
void Tuya_MapStreamStateSwitch(void);
void Tuya_RealMapListReset(void);
uint8_t Tuya_RealMapListPush(Point16_t point);
uint8_t Tuya_RealMapListShift(Point16_t *point);
void Tuya_RealMapBusyListReset(void);
uint8_t Tuya_RealMapBusyListPush(Point16_t point);
uint8_t Tuya_RealMapBusyListShift(Point16_t *point);
void Tuya_AddRealMapToSend(Point16_t point);
void Tuya_UpdateRobotCell(void);
uint8_t Tuya_DpEnumUpdate(uint8_t dpid,uint8_t value);
uint8_t Tuya_DpValueUpdate(uint8_t dpid,uint32_t value);
uint8_t Tuya_DpFaultUpdate(uint8_t dpid,uint32_t value);
uint8_t Tuya_DpStringUpdate(uint8_t dpid,uint8_t *p_value,uint16_t len);
uint8_t Tuya_DpBoolUpdate(uint8_t dpid,uint8_t value);


















#endif


