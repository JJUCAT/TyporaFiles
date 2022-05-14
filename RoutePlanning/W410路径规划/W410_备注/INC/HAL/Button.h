#ifndef __BUTTON_H
#define __BUTTON_H

#include "Hw_SysInitialize.h"
#include <stdint.h>

typedef uint8_t (*Button_IsTrig)(void);

typedef enum 
{
    BUTTON_VALUE_NONE = 0,
    BUTTON_VALUE_ONE,
    BUTTON_VALUE_TWO,
    BUTTON_VALUE_WIFI = 0XFF,
}ButtonValue_t;

typedef struct 
{
    Button_IsTrig on_clean_trig;
    Button_IsTrig on_key_1_trig;
    Button_IsTrig on_key_2_trig;    
}Button_t;

extern Button_t g_button;


#define BUTTON_WIFI_CHOICE_VALUE BUTTON_VALUE_TWO


void Button_Init(void);
uint8_t Button_GetCleanStatus(void);
uint8_t Button_CleanKeyEvent(void);
uint8_t Button_CleanKeyEventTime(void);
uint8_t Button_KeysEvent(void);
uint32_t Button_CleanEventTime(void);
void Button_WaitCleanUp(void);
void Button_WaitAllKeyUp(void);
uint8_t Button_StartUp(void);



#endif 






