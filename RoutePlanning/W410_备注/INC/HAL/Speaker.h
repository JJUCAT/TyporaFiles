#ifndef __SPEAKER_H
#define __SPEAKER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void (*Speaker_SetDataIoState)(uint8_t state);
typedef uint8_t (*Speaker_GetBusyIoState)(void);
typedef void (*Speaker_SetPower)(uint8_t state);
typedef enum
{
	SPK_DY					= 0,
	SPK_DON					,
	SPK_AUTO_CLEAN_START	,
	SPK_AREA_CLEAN_START	,
	SPK_CLEAN_FINISH		,
	SPK_LOW_POWER			,
	SPK_CHARGING_START		,
	SPK_NO_WATER_TANK		,
	SPK_CHECK_CLEAN_TANK	,
	SPK_CHECK_DIRTY_TANK	,
	SPK_CHECK_LEFT_WHEEL	,
	SPK_CHECK_RIGHT_WHEEL	,
	SPK_CHECK_BRUSH			,
	SPK_CHECK_BUMPER		,
	SPK_CHECK_CLIFF			,
	SPK_CHECK_MOBILITY		,
	SPK_PICK_UP				,
	SPK_STUCK				,
	SPK_WIFI_CONNECTING		,
	SPK_WIFI_CONNECTED		,
	SPK_VOICE_DISABLE		,
	SPK_VIOCE_ENABLE		,
	SPK_MEDIUM_WATERFLOW	,
	SPK_SMALL_WATERFLOW		,
	SPK_MAX_WATERFLOW		,
	SPK_REGULAR_CLEANING	,
	SPK_DEEP_CLEANING		,
	SPK_ONE_TIME_CLEANING	,
	SPK_TWO_TIME_CLEANING	,
	SPK_FIND_ROBOT			,		
    SPK_PRESS_BOTTON		,
	SPK_SPOT_MODE			,
	SPK_START_GO_HOME		,		
	SPK_ENGLISH				,  
}SPK_t;

typedef enum 
{
	SPK_OLD_DY = 0,
	SPK_OLD_DON,
	SPK_OLD_SELECT_MODE,
	SPK_OLD_CLEAN_START,
	SPK_OLD_CLEAN_FINISH,
	SPK_OLD_LOW_POWER,
	SPK_OLD_CHARGING_START,
	SPK_OLD_CHARGING_FINISH,
	SPK_OLD_NO_WATER_TANK,
	SPK_OLD_CHECK_CLEAN_TANK,
	SPK_OLD_CHECK_DIRTY_TANK,
	SPK_OLD_CHECK_LEFT_WHEEL,
	SPK_OLD_CHECK_RIGHT_WHEEL,
	SPK_OLD_CHECK_BRUSH,
	SPK_OLD_CHECK_BUMPER,
	SPK_OLD_CHECK_CLIFF,
	SPK_OLD_CHECK_MOBILITY,
	SPK_OLD_PICK_UP,
	SPK_OLD_WIFI_CONNECTING,
	SPK_OLD_WIFI_CONNECTED,
	SPK_OLD_FAN_ABNORMAL,
	SPK_OLD_GYRO_ABNORMAL,	
  	SPK_OLD_STUCK_C,
  	SPK_OLD_STUCK_E = 0x14,	
}SpeakrOld_t;


typedef struct 
{
	uint8_t length;
	uint8_t idx;
	uint8_t sound;
	uint8_t sound_temp;
	uint8_t v_on;
	uint8_t out_cnt; 
	uint8_t cycle_cnt;
	uint8_t busy_flag;
	uint8_t check_flag;	
	uint8_t enable;
	uint8_t finish;
	uint16_t check_cnt;
	uint8_t power;
	uint32_t power_time;
	Speaker_SetDataIoState on_set_data_io_state;
	Speaker_GetBusyIoState on_get_busy_io_state;
	Speaker_SetPower on_set_power_state;
}Speaker_t;

extern Speaker_t g_speaker;
void Speaker_Init(void);
void Speaker_Initialize(void);
void Speaker_SetPowerState(uint8_t state);
uint8_t Speaker_GetPowerState(void);
void Speaker_Voice(uint8_t output_val);
void Speaker_InterruptHandler(void);
uint8_t Speaker_IsChecking(void);
uint32_t Speaker_GetPowerTime(void);
void Speaker_RefreshPowerTime(void);
#endif







