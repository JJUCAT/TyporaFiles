#ifndef __HW_WHEEL_H
#define __HW_WHEEL_H

#include "Hw_SysInitialize.h"
#include <stdint.h>


#define LW_SLEEP_OFF				    (GPIOB->BSRRL|=MCU_L_WHEEL_SLP)
#define LW_SLEEP_ON				      	(GPIOB->BSRRH|=MCU_L_WHEEL_SLP)

#define RW_SLEEP_OFF				    (GPIOD->BSRRL|=MCU_R_WHEEL_SLP)
#define RW_SLEEP_ON				      	(GPIOD->BSRRH|=MCU_R_WHEEL_SLP)

#define RW_DIR_FORWARD			    	(GPIOD->BSRRH|=MCU_RW_DIR)
#define RW_DIR_BACKWARD			    	(GPIOD->BSRRL|=MCU_RW_DIR)

#define LW_DIR_FORWARD			    	(GPIOE->BSRRL|=MCU_LW_DIR)
#define LW_DIR_BACKWARD			    	(GPIOE->BSRRH|=MCU_LW_DIR)


void Hw_Wheel_SetLeftEnable(uint8_t state);
void Hw_Wheel_SetRightEnable(uint8_t state);
void Hw_Wheel_SetLeftDir(uint8_t state);
void Hw_Wheel_SetRightDir(uint8_t state);
uint16_t Hw_Wheel_GetLeftAdc(void);
uint16_t Hw_Wheel_GetRightAdc(void);
volatile uint32_t *Hw_Wheel_LeftPwm(void);
volatile uint32_t *Hw_Wheel_RightPwm(void);







#endif



















































