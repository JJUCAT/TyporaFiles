#ifndef __POWER_H
#define __POWER_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef void(*Power_SetState)(uint8_t state);

typedef struct 
{
    Power_SetState on_set_3v3_5v_state;
    Power_SetState on_set_obs_state;    
    Power_SetState on_set_wifi_state; 
    Power_SetState on_set_battery_state; 
    Power_SetState on_set_kill_vcc_state;
}Power_t;


extern Power_t g_power;

void Power_Init(void);
void Power_StartUp(void);
void Power_EnableAll(void);
void Power_DisableAll(void);
void Power_KillAllVcc(void);



#endif



