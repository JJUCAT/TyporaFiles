#ifndef __BATTERY_H
#define __BATTERY_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define BATTERY_CAPACITY_MAX    (2500*3600) //2600mah
#define BATTERY_LEVEL_GREEN  30
#define BATTERY_LEVEL_ORANGE 15
#define BATTERY_LEVEL_RED	 2
#define BATTERY_LEVEL_DIF	 25
#define BATTERY_LEVEL_LOW	 6

#define BATTERY_ReCharge_VOLTAGE  (uint16_t)1600



#define BATTERY_LOW_VOLTAGE  (uint16_t)1300
#define BATTERY_WEAK_VOLTAGE (uint16_t)1390

typedef uint16_t (*Battery_GetAdc)(void);
typedef uint8_t (*Battery_GetLevel)(uint16_t voltage);
typedef struct
{
	volatile uint32_t temporary;
	volatile int32_t cost; 
	volatile uint8_t cnt;
	volatile uint16_t min;
	volatile uint16_t max;
	volatile uint16_t level;
	volatile uint16_t current;
	volatile uint16_t base;
	volatile int32_t fading_offset;
	uint8_t is_full;
}BatteryCapacity_t;

typedef struct
{
	uint8_t level;
	uint8_t pre_level;
	uint16_t voltage;
	uint16_t recharge_voltage;
	BatteryCapacity_t capacity;
	uint8_t capacity_cnt;
	Battery_GetAdc on_get_adc;
	Battery_GetLevel on_get_level;	
}Battery_t;

extern Battery_t g_battery;

void Battery_Init(void);
void Battery_CapacityInit(uint16_t baseline);
void Battery_ResetPreLevel(void);
uint8_t Battery_IsLow(void);
uint8_t Battery_IsChargeLow(void);
void Battery_SetVoltage(uint16_t voltage);
uint16_t Battery_GetVoltage(void);
uint16_t Battery_GetReChargeVoltage(void);
uint8_t Battery_IsWeak(void);
void Battery_AddCapacityCurrentCycle(uint16_t system_current_adc);
void Battery_ResetCapacity(void);
void Battery_AddCapacityCurrent(uint16_t sys_current_adc);
void Battery_CapacityCalc(uint8_t is_charging,float adc_2_current);
void Battery_VolatgeDetectionCycle(float adc2volatge);
#endif








