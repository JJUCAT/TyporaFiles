#ifndef __DELAY_H
#define __DELAY_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


extern volatile uint32_t g_delay_counter;


void delay(uint16_t time);
void delay_increase_counter(void);



#endif

