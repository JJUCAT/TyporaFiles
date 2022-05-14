#ifndef __HW_DISPLAY_H
#define __HW_DISPLAY_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define TM1618_DIO_HIGH   (GPIOE->BSRRL |= MCU_LEDKEY_DIO)
#define TM1618_DIO_LOW    (GPIOE->BSRRH |= MCU_LEDKEY_DIO)
#define TM1618_CLK_HIGH   (GPIOE->BSRRL |= MCU_LEDKEY_CLK)
#define TM1618_CLK_LOW    (GPIOE->BSRRH |= MCU_LEDKEY_CLK)
#define TM1618_STB_HIGH   (GPIOE->BSRRL |= MCU_LEDKEY_STB)
#define TM1618_STB_LOW    (GPIOE->BSRRH |= MCU_LEDKEY_STB)
#define TM1618_DIO_IS_HIGH (GPIOE->IDR & MCU_LEDKEY_DIO)
#define TM1618_DIO_OUTPUT (GPIOE->MODER |= GPIO_MODER_MODER8_0)
#define TM1618_DIO_INPUT  (GPIOE->MODER &=~GPIO_MODER_MODER8)
#define TIMER_INT_ENABLE  (TIM11->DIER |= TIM_DIER_UIE)
#define TIMER_INT_DISABLE (TIM11->DIER &=~TIM_DIER_UIE)

#define LED_MODE_NAV   0X0008	
#define LED_MODE_AREA  0X0004	
#define LED_MODE_SPOT  0X0010	
#define LED_MODE_WALL  0X0800	
#define LED_MODE_KEY   0X3000	

#define LED_CLEAN_TIME 0X2000
#define LED_BATTERY_RED_W410 0X0004	
#define LED_BATTERY_GREEN_W410 0X0010 
#define LED_BATTERY_RED_W400 0x1010	
#define LED_BATTERY_GREEN_W400 0x2800 

#define LED_CLEAN_RED 0X0014
#define LED_CLEAN_GREEN 0X0808

#define LED_WARN_KEY_W410	0X0010
#define LED_WARN_KEY_W400	0X000C

#define LED_WARN_WATER	0X1000
#define LED_WATER_FLOW_RED  0X1000
#define LED_WATER_FLOW_GREEN  0X0008
#define LED_WIFI_W400 0X2000 
#define LED_WIFI_W410 0X0800 //w410
#define LED_CLEAN 0X0004
#define	LED_AREA 0X0010

#define LED_BRIGHTNESS_HIGH_VALUE 7

typedef struct 
{
  uint8_t brightness;
  uint16_t data1;//w410:clean times + battery  w400:battery + warning key
  uint16_t data2;//w410:warn key + warn water  w400:clean key + water warn + wifi
  uint16_t data3;//w410:water flow 			w400:mode nav spot wall area + mode key
  uint16_t data4;//w410:wifi clean area w400:none
  uint8_t array_idx;
  uint8_t data_bit_idx;
  uint8_t data_bit_cycle;
  uint8_t data_buffer;
  uint8_t button_buffer;
  uint8_t button_final_value;
}Hw_Display_t;

extern Hw_Display_t g_hw_display;
extern void (*on_hw_display_int)(void);

void Hw_Display_Init(void);
void Hw_Display_Process(void);
void Hw_Display_IntProcess(void);
void Hw_Display_KeyTm1618IntProcess(void);
void Hw_Display_KeyIntProcessStart(void);
void Hw_Display_IntConfig(uint8_t state);
void Hw_Display_DataInit(uint16_t data1, uint16_t data2, uint16_t data3, uint16_t data4,uint8_t Light);
void Hw_Display_IntHandler(void);
void Hw_Display_ALL_On(void);
void Hw_Display_CleanLedOff(void);
void Hw_Display_CleanLedRed(void);
void Hw_Display_CleanLedGreen(void);
void Hw_Display_CleanLedOrange(void);
void Hw_Display_AreaLedOff(void);
void Hw_Display_AreaLedGreen(void);
void Hw_Display_WifiLedOff(void);
void Hw_Display_WifiLedGreen(void);
void Hw_Display_BatteryLedOff(void);
void Hw_Display_BatteryLedRed(void);
void Hw_Display_BatteryLedGreen(void);
void Hw_Display_BatteryLedOrange(void);
void Hw_Display_WaterFlowLedOff(void);
void Hw_Display_WaterFlowLedRed(void);
void Hw_Display_WaterFlowLedGreen(void);
void Hw_Display_WaterFlowLedOrange(void);
void Hw_Display_WarnKeyStateLedOff(void);
void Hw_Display_WarnKeyStateLedRed(void);
void Hw_Display_WaterWarnStateLedOff(void);
void Hw_Display_WaterWarnStateLedRed(void);
void Hw_Display_WaterWarnStateLedGreen(void);
void Hw_Display_ModeNavStateLedOff(void);
void Hw_Display_ModeNavStateLedGreen(void);
void Hw_Display_CleanCntStateLedOff(void);
void Hw_Display_CleanCntStateLedGreen(void);
void Hw_Display_ModeAreaStateLedOff(void);
void Hw_Display_ModeAreaStateLedGreen(void);
void Hw_Display_ModeSpotStateLedOff(void);
void Hw_Display_ModeSpotStateLedGreen(void);
void Hw_Display_ModeWallStateLedOff(void);
void Hw_Display_ModeWallStateLedGreen(void);
void Hw_Display_ModeKeyStateLedOff(void);
void Hw_Display_ModeKeyStateLedGreen(void);
void Hw_Display_SetBrightness(uint8_t value);
void Hw_Display_BrightnessHigh(void);






#endif

























