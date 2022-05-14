#ifndef __CHARGE_H
#define __CHARGE_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define CHARGE_INPUT_VOLTAGE      	 (uint16_t)1700
#define CHARGE_OVERLOAD_VOLTAGE      (uint16_t)1900
#define CHARGE_FINISH_CURRENT 			50
#define CHARGE_TARGET_CURRENT_LOWEST    50
#define CHARGE_TARGET_CURRENT 			600
#define CHARGE_RESET_CAPACITY_CURRENT 	300
#define CHARGE_BATTERY_FULL_VOLTAGE     1640   
#define CHARGE_BATTERY_ERROR_VOLTAGE	1720
#define CHARGE_BATTERY_NORMAL_VOLTAGE   1100
#define CHARGE_PWM_RANGE                1000
#define CHARGE_PWM_START                10

typedef uint16_t (*Charge_GetAdc)(void);
typedef void (*Charge_GpioConfig)(uint8_t state);
typedef void (*Charge_TimerConfig)(uint8_t state);
typedef void (*Charge_PowerConfig)(uint8_t state);
typedef enum
{
    CHARGE_MODE_NONE = 0,
    CHARGE_MODE_CC,
    CHARGE_MODE_CV,
    CHARGE_MODE_FINISH,
}ChargerMode_t;

typedef struct 
{
    ChargerMode_t mode;
    uint8_t charging;
    uint16_t voltage;
    uint8_t from_sleep;
    uint8_t is_mov_station;
    volatile uint32_t *p_pwm;
    Charge_GetAdc on_get_adc;  
    Charge_GpioConfig on_gpio_config;
    Charge_TimerConfig on_timer_config;
    Charge_PowerConfig on_power_config;
}Charge_t;


extern Charge_t g_charge;

void Charge_Init(void);
void Charge_SetFromSleep(uint8_t sleep);
uint8_t Charge_IsFromSleep(void);
void Charge_SetMode(ChargerMode_t mode);
ChargerMode_t Charge_GetMode(void);
void Charge_SetCharging(uint8_t charging);
uint8_t Charge_IsCharging(void);
void Charge_SetMovStation(uint8_t state);
uint8_t Charge_IsMovStation(void);
void Charge_SetVoltage(uint16_t voltage);
uint16_t Charge_GetVoltage(void);
void Charge_VolatgeDetectionCycle(float adc2volatge);
uint8_t Charge_IsAdapterInput(void);
void Charge_ConfigurationIn(void);
void Charge_ConfigurationOut(void);
#endif








