#ifndef __CODESHOW_H
#define __CODESHOW_H

#include <stdint.h>
#include "Hw_SysInitialize.h"
typedef void (*CodeShow_UsartSend)(uint16_t length,char *data);
typedef void (*CodeShow_GpioConfig)(uint8_t state);
typedef void (*CodeShow_TimerConfig)(uint8_t state);
typedef enum
{
    CODESHOW_ILDE = 0,
    CODESHOW_INIT_START,
    CODESHOW_INIT_DELAY,
    CODESHOW_SEND_START_INIT,
    CODESHOW_SEND_START_WAIT,
    CODESHOW_SEND_START_END,
    CODESHOW_SEND_DATA_H_START,  
    CODESHOW_SEND_DATA_H_WAIT,
    CODESHOW_SEND_DATA_H_END,      
    CODESHOW_SEND_DATA_L_START,  
    CODESHOW_SEND_DATA_L_WAIT,
    CODESHOW_SEND_DATA_L_END,   
    CODESHOW_SEND_DATA_FINISH,   
    CODESHOW_END,
}CodeShowState_t;

typedef struct
{
    CodeShowState_t state;
    uint8_t step;
    uint32_t data;
    uint8_t code;    
    uint8_t enable;
    uint8_t is_working;
    uint32_t send_data;
    uint8_t send_data_cnt;
    uint32_t delay_time;
    uint8_t buffer[20];
    CodeShow_GpioConfig on_gpio_config;
    CodeShow_TimerConfig on_timer_config;
    CodeShow_UsartSend on_codeshow_usart_send;
}CodeShow_t;



typedef void (*CodeShow_UsartSend)(uint16_t length,char *data);
extern CodeShow_t g_code_show;



#define CODESHOW_INIT_DELAY_T 200
#define CODESHOW_START_INIT_T 59
#define CODESHOW_START_END_T 39
#define CODESHOW_H_BIT_INIT_T 19
#define CODESHOW_H_BIT_END_T 9
#define CODESHOW_L_BIT_INIT_T 9
#define CODESHOW_L_BIT_END_T 9



void CodeShow_Init(void);
void CodeShow_SetState(FunctionalState state);
uint32_t CodeShow_DataConversion(uint8_t step,uint8_t dir,uint8_t error,uint8_t error_data,uint16_t display_data);
void CodeShow_LoadData(volatile uint8_t *buffer,uint8_t step,uint16_t data,uint8_t code);
void CodeShow_ShowData(uint8_t step,uint16_t code,uint16_t data);
void CodeShow_SetData(uint8_t step,uint16_t code,uint16_t data);
void CodeShow_Cycle(void);








#endif








