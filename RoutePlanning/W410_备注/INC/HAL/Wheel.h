/**
  ******************************************************************************
  * @file    AI Cleaning Robot
  * @author  ILife Team Dxsong
  * @version V0.0
  * @date    11-July-2011
  * @brief   Movement
  * @define a lot of IO function for a easier look
  ******************************************************************************
  * Initialize the System Clock.ADC converter.EXTI.Timer and USART3
  * <h2><center>&copy; COPYRIGHT 2011 ILife CO.LTD</center></h2>
  ******************************************************************************
  */  

#ifndef __WHEEL_H
#define __WHEEL_H

#include <stdint.h>

#define WHEEL_STALL_LIMIT         (uint16_t)500  
#define WHEEL_STALL_NOLOAD        (uint16_t)10
#define WHEEL_SLOW_LIMIT          200


typedef void (*Wheel_SetEnable)(uint8_t state);
typedef void (*Wheel_SetMoveDir)(uint8_t state);
typedef uint16_t (*Wheel_GetAdc)(void);
typedef enum
{
	WHEEL_DIR_LEFT			=	0x01,
	WHEEL_DIR_RIGHT			=	0x02,
	WHEEL_DIR_FORWARD		=	0x04,
	WHEEL_DIR_BACKWARD	=	0x08,
}WheelDir_t;


typedef struct 
{
  uint8_t left_real_speed;
  uint8_t right_real_speed;
  uint8_t left_target_speed;
  uint8_t right_target_speed; 
  uint32_t left_step;
  uint32_t right_step;
  uint32_t left_target_step;
  uint32_t right_target_step;
  int16_t temp_right_pwm;
  int16_t temp_left_pwm;  
  uint16_t right_slow;
  uint16_t left_slow;
  uint16_t left_speed;
  uint16_t right_speed;
  int32_t left_move_step;
  int32_t right_move_step;
  int32_t right_cnt;
  int32_t left_cnt;
  uint16_t left_speed_cnt;
  uint16_t right_speed_cnt;
  int16_t left_baseline;
  int16_t right_baseline;
  int16_t left_current;
  int16_t right_current;
  WheelDir_t left_dir;
  WheelDir_t right_dir;
  WheelDir_t dir;
  FunctionalState tune_state;
  Wheel_SetEnable on_set_left_enable;
  Wheel_SetEnable on_set_right_enable;
  Wheel_SetMoveDir on_set_left_dir;
  Wheel_SetMoveDir on_set_right_dir; 
  Wheel_GetAdc on_get_left_adc;
  Wheel_GetAdc on_get_right_adc; 
  volatile uint32_t *p_pwm_left;
  volatile uint32_t *p_pwm_right;
}Wheel_t;

extern Wheel_t g_wheel;






void Wheel_Init(void);
void Wheel_SetTuneState(FunctionalState state);
void Wheel_TuneSpeedProcess(int8_t l_wheel_speed, int8_t r_wheel_speed,uint8_t state);
void Wheel_SpeedAccelerationProcess(uint8_t state);
void Wheel_Configure(FunctionalState state);
void Wheel_Stop(void);
void Wheel_ResetSlowCnt(void);
uint8_t Wheel_IsLeftLoseCnt(void);
uint8_t Wheel_IsRightLoseCnt(void);
void Wheel_ResetStep(void);
uint32_t Wheel_GetLeftStep(void);
uint32_t Wheel_GetRightStep(void);
void Wheel_SetCount(int32_t left, int32_t right);
void Wheel_ResetCounts(void);
int32_t Wheel_GetLeftCount(void);
int32_t Wheel_GetRightCount(void);
void Wheel_SetSpeed(uint8_t left, uint8_t right);
void Wheel_ResetSpeed(void);
void Wheel_SetLeftSpeed(uint8_t speed);
void Wheel_SetRightSpeed(uint8_t speed);
int8_t Wheel_GetLeftSpeed(void);
int8_t Wheel_GetRightSpeed(void);
void Wheel_SetTargetSpeed(uint8_t l_speed, uint8_t r_speed);
void Wheel_SetLeftTargetSpeed(uint8_t speed);
void Wheel_SetRightTargetSpeed(uint8_t speed);
uint8_t Wheel_GetLeftTargetSpeed(void);
uint8_t Wheel_GetRightTargetSpeed(void);
void Wheel_SetTargetStep(uint32_t l_step, uint32_t r_step);
uint32_t Wheel_GetLeftTargetStep(void);
uint32_t Wheel_GetRightTargetStep(void);
void Wheel_SetDir(WheelDir_t dir);
WheelDir_t Wheel_GetDir(void);
void Wheel_SetLeftDir(WheelDir_t dir);
void Wheel_SetRightDir(WheelDir_t dir);
WheelDir_t Wheel_GetLeftDir(void);
WheelDir_t Wheel_GetRightDir(void);
uint8_t Wheel_IsForward(void);
void Wheel_ResetPWM(void);
void Wheel_ResetTempPWM(void);
void Wheel_TuneLeftSpeedDir(int32_t *speed,int32_t speed_limit);
void Wheel_TuneRightSpeedDir(int32_t *speed,int32_t speed_limit);
void Wheel_LeftInterrupt(void);
void Wheel_RightInterrupt(void);
void Wheel_StoreBaseline(void);
void Wheel_CurrentDetectionCycle(float adc2current);
uint8_t Wheel_CheckLeftCurrent(void);
uint8_t Wheel_CheckRightCurrent(void);
int16_t Wheel_GetLeftCurrent(void);
int16_t Wheel_GetRightCurrent(void);
#endif/* __WHEELS_H */

