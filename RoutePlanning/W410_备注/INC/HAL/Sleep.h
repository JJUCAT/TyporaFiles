#ifndef __SLEEP_H
#define __SLEEP_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef enum 
{
    SLEEP_INIERRUPT_BUTTON = 0X01,
    SLEEP_INIERRUPT_CHARGE = 0X02,
    SLEEP_INIERRUPT_WIFI   = 0X04,
    SLEEP_INIERRUPT_RTC    = 0X08,
    SLEEP_INIERRUPT_REMOTE = 0X10,
    SLEEP_INIERRUPT_OBS    = 0x20,
		SLEEP_INIERRUPT_TIME   = 0x40,
}SleepInterrupt_t;

typedef enum 
{
    SLEEP_CONFIG_INIT = 0,
    SLEEP_CONFIG_EXIT,
    SLEEP_CONFIG_WAKE_UP,
}SleepConfigState_t;


typedef void (*SleepConfig)(SleepConfigState_t state,uint8_t chargr_state);
typedef void (*SleepEnter)(void);
typedef struct 
{
    uint32_t interrupt;
    FunctionalState state;
    uint8_t is_charging;
    SleepEnter on_enter;
    SleepConfig on_systick_config;
    SleepConfig on_power_config;
    SleepConfig on_exti_config;
    SleepConfig on_gpio_config;
    SleepConfig on_adc_config;
    SleepConfig on_rcc_config;
    SleepConfig on_nvic_config;
    SleepConfig on_exti_pending_config;
		SleepConfig on_time_config;
}Sleep_t;

extern Sleep_t g_sleep;
void Sleep_Init(void);
void Sleep_InitConfiguration(void);
void Sleep_ExitConfiguration(void);
void Sleep_WakeUpConfiguration(void);
void Sleep_EnterStopMode(void);
void Sleep_EditInterrupt(SleepInterrupt_t interrupt);
void Sleep_ResetInterrupt(void);
void Sleep_ResetTheInterrupt(SleepInterrupt_t interrupt);
uint8_t Sleep_IsInterrupt(SleepInterrupt_t interrupt);


#endif







