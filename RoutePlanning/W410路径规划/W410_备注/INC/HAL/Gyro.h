
#ifndef __GYRO_H
#define __GYRO_H

#include <stdint.h>
#include "Hw_SysInitialize.h"


#define GYRO_XV7011		   (1)
#define GYRO_CALIBRATION   (1)
#define GYRO_BUFFER_SIZE   40
#define GYRO_MSG_LENGTH    15

typedef void (*Gyro_UsartSend)(uint16_t length,char *data);

typedef enum
{
    GYRO_INIT_NONE = 0,
    GYRO_INIT_START,
    GYRO_INIT_WAIT,
    GYRO_INIT_RESTART,
    GYRO_INIT_FINISH,
}GyroInit_t;

typedef enum
{
    GYRO_STARTUP_NONE = 0,
    GYRO_STARTUP_START,
    GYRO_STARTUP_DISABLE,
    GYRO_STARTUP_ENABLE,
    GYRO_STARTUP_WAIT_DATA,
    GYRO_STARTUP_RESTART,
    GYRO_STARTUP_READY,
    GYRO_STARTUP_END,
}GyroStartUp_t;

typedef struct
{
    volatile uint8_t rxbuffer[GYRO_BUFFER_SIZE]; 
    uint16_t offset;
    uint16_t time;
    uint8_t idx;
    uint16_t angle[2]; 
    int16_t rate[2];  
    int16_t xacc;       
    int16_t yacc;       
    int16_t zacc;        
    int16_t xacc_offset;
    int16_t yacc_offset;
    int16_t zacc_offset;
    int32_t odometer;
    uint8_t calibration;
    int16_t raw;
    uint8_t update_flag;
    uint32_t cal_time;
    uint8_t is_ready; 
    uint8_t is_error;
    uint8_t try_cnt; 
    GyroInit_t init_step;
    GyroStartUp_t start_up;
    Gyro_UsartSend on_gyro_usart_send; 
}Gyro_t;

extern Gyro_t g_gyro;


void Gyro_Init(void);
void Gyro_Cal(void);
uint8_t Gyro_IsNeedCalculate(void);
uint16_t Gyro_GetAngle(uint8_t id);
uint8_t Gyro_ParseMsg(volatile uint8_t * msg);
void Gyro_ResetTime(void);
void Gyro_ResetCmd(void);
void Gyro_CalibrationEnableCmd(void);
void Gyro_Cmd(FunctionalState state);
void Gyro_On_Cmd(void);
void Gyro_ResetCalibrationValue(void);
uint8_t Gryo_GetUpdateFlag(void);
uint8_t Gyro_IsUpdated(void);
void Gyro_ResetUpdateFlag(void);
void Gyro_Calibration_Cmd(FunctionalState state);
void Gyro_SetOffset(int16_t offset);
int16_t Gyro_GetOffset(void) ;
void Gyro_SetAngle(int16_t theta) ;
void Gyro_ResetWithOffset(int16_t offset);
uint8_t Gyro_GetCalibration(void) ;
void Gyro_Reset_With_Offset(int16_t offset);
void Gyro_StartUpProcess(void);
void Gyro_Start(void);
void Gyro_End(void);
uint8_t Gyro_IsReady(void);
uint8_t Gyro_IsError(void);
#endif


