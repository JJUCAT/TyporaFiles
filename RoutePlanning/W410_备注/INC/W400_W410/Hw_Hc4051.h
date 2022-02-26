#ifndef __HW_HC4051_H
#define __HW_HC4051_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define MCU_4051_A0_OFF					(GPIOD->BSRRH|=MCU_4051_A0)
#define MCU_4051_A0_ON					(GPIOD->BSRRL|=MCU_4051_A0)
#define MCU_4051_A1_OFF					(GPIOD->BSRRH|=MCU_4051_A1)
#define MCU_4051_A1_ON					(GPIOD->BSRRL|=MCU_4051_A1)
#define MCU_4051_A2_OFF					(GPIOD->BSRRH|=MCU_4051_A2)
#define MCU_4051_A2_ON					(GPIOD->BSRRL|=MCU_4051_A2)

void Hw_Hc4051_SetChannelA0State(uint8_t state);
void Hw_Hc4051_SetChannelA1State(uint8_t state);
void Hw_Hc4051_SetChannelA2State(uint8_t state);














#endif



