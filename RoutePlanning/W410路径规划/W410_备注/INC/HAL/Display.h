#ifndef __DISPLAY_H
#define __DISPLAY_H

#include "Hw_SysInitialize.h"
#include <stdint.h>

typedef void (*Display_SetState)(void);
typedef void (*Display_SetBrightness)(uint8_t value);
typedef void (*Display_SetBrightnessHigh)(void);
typedef enum 
{
	LED_MODE_STATIC	= 0,
	LED_MODE_BLINK,
	LED_MODE_BREATH,
}DisplayMode_t;

typedef enum
{
  LED_STATE_OFF	= 0,
  LED_STATE_RED,
  LED_STATE_GREEN,
  LED_STATE_ORANGE,
}LedState_t;


typedef struct 
{
  FunctionalState state;
  DisplayMode_t clean_led_mode;
  DisplayMode_t wifi_led_mode;
  DisplayMode_t battery_led_mode;
  DisplayMode_t area_led_mode;
  LedState_t wifi_led_state;
  Display_SetBrightness on_set_brightness;
  Display_SetBrightnessHigh on_brightness_high;
  Display_SetState on_clean_led_off;
  Display_SetState on_clean_led_red;
  Display_SetState on_clean_led_green;
  Display_SetState on_clean_led_orange;
  Display_SetState on_area_led_off;
  Display_SetState on_area_led_green;
  Display_SetState on_wifi_led_off;
  Display_SetState on_wifi_led_green;
  Display_SetState on_battery_led_off;
  Display_SetState on_battery_led_red;
  Display_SetState on_battery_led_green;
  Display_SetState on_battery_led_orange;  
  Display_SetState on_waterflow_led_off;
  Display_SetState on_waterflow_led_red;
  Display_SetState on_waterflow_led_green;
  Display_SetState on_waterflow_led_orange;   
  Display_SetState on_warnkey_led_off;
  Display_SetState on_warnkey_led_red; 
  Display_SetState on_modekey_led_off;
  Display_SetState on_modekey_led_green;    
  Display_SetState on_waterwarn_led_off;
  Display_SetState on_waterwarn_led_red;
  Display_SetState on_waterwarn_led_green;  
  Display_SetState on_cleancnt_led_off;
  Display_SetState on_cleancnt_led_green;  
  Display_SetState on_modenav_led_off;
  Display_SetState on_modenav_led_green;   
  Display_SetState on_modewall_led_off;
  Display_SetState on_modewall_led_green;   
  Display_SetState on_modearea_led_off;
  Display_SetState on_modearea_led_green;   
  Display_SetState on_modespot_led_off;
  Display_SetState on_modespot_led_green;         
}Display_t;

extern Display_t g_display;

void Display_Process(void);
void Display_Init(void);
void Display_Blink(void);
void Display_Static(void);
void Display_Breath(void);
void Display_All_Off(void);
void Display_SetLedBrightness(uint8_t value);
void Display_SetCleanState(DisplayMode_t mode,LedState_t state);
void Display_SetAreaState(DisplayMode_t mode,LedState_t state);
void Display_SetWifiState(DisplayMode_t mode,LedState_t state);
void Display_SetBatteryState(DisplayMode_t mode,LedState_t state);
void Display_SetWaterFlowState(LedState_t state);
void Display_SetWarnKeyState(LedState_t state);
void Display_SetWaterWarnState(LedState_t state);
void Display_SetCleanCntState(LedState_t state);
void Display_SetModeNavState(LedState_t state);
void Display_SetModeAreaState(LedState_t state);
void Display_SetModeSpotState(LedState_t state);
void Display_SetModeWallState(LedState_t state);
void Display_SetModeKeyState(LedState_t state);
void Display_SetModeLed(uint16_t selection);
void Display_WifiBlinkToStatic(void);
#endif /* __DISPLAY_H */

