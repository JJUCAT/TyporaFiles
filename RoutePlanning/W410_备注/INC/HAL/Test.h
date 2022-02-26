#ifndef __TEST_H
#define __TEST_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define TEST_VERSION 4000
#define TEST_SUB_VERSION 10
#define TEST_OBS_VERSION 0006
#define TEST_CHARGE_TARGET_CURRENT 	100
#define TEST_OVER_POWER 1700
#define TEST_WHEEL_L_LIMIT 1 
#define TEST_WHEEL_H_LIMIT 120
#define TEST_WHEEL_PWM_LIMIT 45
#define TEST_OBS_L_LIMIT 400
#define TEST_OBS_H_LIMIT 500
#define TEST_WALL_L_LIMIT 150
#define TEST_WALL_H_LIMIT 200
#define TEST_BASEVOL_LIMIT_L 50
#define TEST_BASEVOL_LIMIT_H 200
#define TEST_SYSCURRENT_LIMIT_L 2
#define TEST_SYSCURRENT_LIMIT_H 500
#define TEST_MAINBRUSH_OVER_LIMIT	800
#define TEST_MAINBRUSH_H_LIMIT	500
#define TEST_MAINBRUSH_L_LIMIT	10
#define TEST_BATTREY_VOL_LIMIT_L 1390
#define TEST_BATTREY_VOL_LIMIT_H 1700
#define TEST_BUMPER_FORWARD_DISTANCE 10000
#define TEST_BUMPER_BACK_DISTANCE 400
#define TEST_BUMPER_TURN_DISTANCE 900
#define TEST_BUMPER_SPEED 40
#define TEST_OBS_DIF_VALUE 100
#define TEST_WALL_DIF_VALUE 100
typedef enum
{
	TEST_CHOICE_NONE = 0,
	TEST_CHOICE_AUTO,
	TEST_CHOICE_ALLFUNCTION,
	TEST_CHOICE_AGING,
	TEST_CHOICE_MOBILITY,	
	TEST_CHOICE_PUMP,	
	TEST_CHOICE_GYRO,	
	TEST_CHOICE_WIFI,				
	TEST_CHOICE_BUMPER,
	TEST_CHOICE_WHEEL,
	TEST_CHOICE_VACUUM,
	TEST_CHOICE_SPEAKER,
	TEST_CHOICE_SPEACAIL,
	TEST_CHOICE_END,
}TestChoice_t;

typedef enum
{	
	TEST_AF_NONE = 0,
	TEST_AF_LED,
	TEST_AF_OBS,
	TEST_AF_BUMPER,
	TEST_AF_CLIFF,
	TEST_AF_RCON,		
	TEST_AF_MOBSENSOR,	
	TEST_AF_LEFTWHEEL,	
	TEST_AF_LEFTWHEEL_PLUG,
	TEST_AF_RIGHTWHEEL,	
	TEST_AF_RIGHTWHEEL_PLUG,	
	TEST_AF_BRUSH ,
	TEST_AF_BRUSH_PLUG,
	TEST_AF_VACUUM,	
	TEST_AF_PUMPER,
	TEST_AF_CHARGE,
}TestAllFunction_t;	

typedef enum
{
	TEST_MOBILITY_WIFI = 0,
	TEST_MOBILITY_GYRO,
	TEST_MOBILITY_MAINBRUSH,
	TEST_MOBILITY_VACUUM,
	TEST_MOBILITY_RCON,
	TEST_MOBILITY_OBS,
	TEST_MOBILITY_BUMPER,
	TEST_MOBILITY_CLIFF,
	TEST_MOBILITY_PUMPER,
}TestMobility_t;

typedef enum 
{
	TEST_GYRO_NONE = 0,
	TEST_GYRO_INIT,
	TEST_GYRO_FINISH,
}TestGyro_t;


typedef enum
{
	TEST_AUTO_NONE = 0,
	TEST_AUTO_GRYO,
	TEST_AUTO_BUMPER,
	TEST_AUTO_BRUSH,
	TEST_AUTO_VAC,
	TEST_AUTO_PUMP ,
	TEST_AUTO_LWHEEL,
	TEST_AUTO_RWHEEL,	
	TEST_AUTO_MOB,
	TEST_AUTO_TANK,
	TEST_AUTO_CLEAN_TANK,
	TEST_AUTO_DIRTY_TANK,		
}TestAuto_t;

typedef enum 
{
	TEST_RESULT_NONE= 0,
	TEST_RESULT_FAIL,
	TEST_RESULT_SUCCESSFUL,
	TEST_RESULT_NEXT_CHOICE,
	TEST_RESULT_PREV_CHOICE,
}TestResult_t;


typedef enum 
{
	TEST_SELFCHECK_NONE = 0,
	TEST_SELFCHECK_BASE_VOL,
	TEST_SELFCHECK_RTC,
	TEST_SELFCHECK_BATTERY,
	TEST_SELFCHECK_STATIC_CURRENT,
	TEST_SELFCHECK_GYRO,
	TEST_SELFCHECK_OBS,
	TEST_SELFCHECK_END,
}TestSelfCheck_t;

typedef enum 
{
	TEST_CHANGE_NONE = 0,
	TEST_CHANGE_NEXT,
	TEST_CHANGE_LAST,	
}TestChange_t;
	
typedef enum 
{
	TEST_CHARGE_NONE = 0,
	TEST_CHARGE_DOCK,
	TEST_CHARGE_INIT,	
	TEST_CHARGE_ING,
}TestCharger_t;

typedef enum 
{
	TEST_WIFI_NONE = 0,
	TEST_WIFI_INIT,
	TEST_WIFI_VERSION,
	TEST_WIFI_FUNCTION,	
}TestWifi_t;

typedef enum 
{
	TEST_WIFI_VERSION_NONE = 0,
	TEST_WIFI_VERSION_PASS,
	TEST_WIFI_VERSION_UART_ERROR,
	TEST_WIFI_VERSION_ERROR,	
}TestWifiVersion_t;

typedef enum 
{
	TEST_WIFI_FUNCTION_NONE = 0,
	TEST_WIFI_FUNCTION_PASS,
	TEST_WIFI_FUNCTION_ERROR,	
}TestWifiFunction_t;


typedef struct
{
    TestChoice_t choice;
    TestSelfCheck_t self_check;
    TestAllFunction_t all_function;
    TestMobility_t mobility;
    uint8_t error_state;
    uint8_t error_data;
}Test_t;

extern Test_t g_test;

void Test_Init(void);
uint8_t Test_IsEnter(void);
void Test_IsExit(void);
void Test_SelectMode(void);

void Test_SelfCheck(void);
void Test_SelfCheck_ResultFail(void);
void Test_SelfCheck_ResultHandler(TestResult_t result);
TestResult_t Test_SelfCheck_BaseVol(void);
TestResult_t Test_SelfCheck_BaseCurrent(void);
TestResult_t Test_SelfCheck_Rtc(void);
TestResult_t Test_SelfCheck_Battery(void);
TestResult_t Test_SelfCheck_Gyro(void);
TestResult_t Test_SelfCheck_Obs(void);
uint8_t Test_SelfCheck_SwitchChoice(void);

void Test_AllFunction(void);
void Test_AllFunction_ResultFail(void);
void Test_AllFunction_ResultHandler(uint8_t result);
uint8_t Test_AllFunction_SwitchChoice(void);
TestResult_t Test_AllFunction_MainBrush(void);
TestResult_t Test_AllFunction_Vacuum(void);
TestResult_t Test_AllFunction_Pumper(void);
TestResult_t Test_AllFunction_OBS(void);
TestResult_t Test_AllFunction_Bumper(void);
TestResult_t Test_AllFunction_Cliff(void);
TestResult_t Test_AllFunction_LeftWheel(void);
TestResult_t Test_AllFunction_RightWheel(void);
TestResult_t Test_AllFunction_Led(void);
TestResult_t Test_AllFunction_Rcon(void);
TestResult_t Test_AllFunction_Mobility(void);
TestResult_t Test_AllFunction_Charging(void);


void Test_Mobility(void);
void Test_MobilityResult(void);
void Test_MobilityResult_Handler(uint8_t result);
TestResult_t Test_Mobility_Wifi(void);
TestResult_t Test_Mobility_Gyro(void);
TestResult_t Test_Mobility_MainBrush(void);
TestResult_t Test_Mobility_Vacuum(void);
TestResult_t Test_Mobility_Rcon(void);
TestResult_t Test_Mobility_Obs(void);
TestResult_t Test_Mobility_Bumper(void);
TestResult_t Test_Mobility_Cliff(void);
TestResult_t Test_Mobility_Pumper(void);
uint8_t Test_Mobility_Stop(void);

TestWifiVersion_t Test_WifiVersion(void);
TestWifiFunction_t Test_WifiFunction(void);
void Test_Wifi(void);
void Test_Aging(void);
void Test_Pump(void);
void Test_Bumper(void);
void Test_Wheel(void);
void Test_Vacuum(void);
void Test_Speaker(void);
void Test_Speacial(void);
void Test_Gyro(void);
uint8_t Test_Stop(void);
void Test_ResultHandler(uint8_t test_choice,TestResult_t result);


TestChange_t Test_ChangeStep(void);



#endif

















