#ifndef __HW_POWER_H
#define __HW_POWER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

#define CTRL_SW_5V_3V3_OFF      		(GPIOE->BSRRH|=MCU_CTRL_SW_3V3_5V)
#define CTRL_SW_5V_3V3_ON       		(GPIOE->BSRRL|=MCU_CTRL_SW_3V3_5V)

#define CTRL_OBS_ON		          	    (GPIOE->BSRRL|=MCU_CTRL_OBS_5V)
#define CTRL_OBS_OFF	         		(GPIOE->BSRRH|=MCU_CTRL_OBS_5V)

#define BATTERY_CTRL_ON			    	(GPIOB->BSRRL|=MCU_CTRL_BAT_VOLT)
#define BATTERY_CTRL_OFF		    	(GPIOB->BSRRH|=MCU_CTRL_BAT_VOLT)

#define CTRL_WIFI_ON		        	(GPIOC->BSRRL|=MCU_CTRL_WIFI_3V3)
#define CTRL_WIFI_OFF		        	(GPIOC->BSRRH|=MCU_CTRL_WIFI_3V3)

#define VCC_ALL_OFF_ON     	      	    (GPIOD->BSRRL|=MCU_VCC_OFF)
#define VCC_ALL_OFF_OFF     	      	(GPIOD->BSRRH|=MCU_VCC_OFF)

void Hw_Power_Set_3V3_5V_State(uint8_t state);
void Hw_Power_Set_Obs_State(uint8_t state);
void Hw_Power_Set_Battery_State(uint8_t state);
void Hw_Power_Set_Wifi_State(uint8_t state);
void Hw_Power_Set_Kill_Vcc_State(uint8_t state);




#endif















