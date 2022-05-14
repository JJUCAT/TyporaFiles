#ifndef __HC4051_H
#define __HC4051_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void (*Hc4051_SetChannelState)(uint8_t state);

typedef struct 
{
    uint8_t channel;
    Hc4051_SetChannelState on_set_a0_state;
    Hc4051_SetChannelState on_set_a1_state;
    Hc4051_SetChannelState on_set_a2_state;    
}Hc4051_t;


extern Hc4051_t g_hc4051;


void Hc4051_Init(void);
void Hc4051_SetChannel(void);
uint8_t Hc4051_GetChannel(void); 
uint8_t Hc4051_ChannelCycle(void);
void Hc4051_ResetChannel(void);



#endif














