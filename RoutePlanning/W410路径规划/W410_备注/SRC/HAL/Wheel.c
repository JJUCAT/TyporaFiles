/**
  ******************************************************************************
  * @file    AI Cleaning Robot
  * @author  ILife Team Dxsong
  * @version V1.0
  * @date    17-Nov-2011
  * @brief   wheel functions
  ******************************************************************************
  * <h2><center>&copy; COPYRIGHT 2011 ILife CO.LTD</center></h2>
  ******************************************************************************
  */  

/* Includes ------------------------------------------------------------------*/
#include "Include.h"

Wheel_t g_wheel;

void Wheel_Init(void)
{
	memset(&g_wheel,0,sizeof(g_wheel));
	g_wheel.on_set_left_dir = Hw_Wheel_SetLeftDir;
	g_wheel.on_set_right_dir = Hw_Wheel_SetRightDir;
	g_wheel.on_set_left_enable = Hw_Wheel_SetLeftEnable;
	g_wheel.on_set_right_enable = Hw_Wheel_SetRightEnable;
	g_wheel.on_get_left_adc = Hw_Wheel_GetLeftAdc;
	g_wheel.on_get_right_adc = Hw_Wheel_GetRightAdc;
	g_wheel.p_pwm_left = Hw_Wheel_LeftPwm();
	g_wheel.p_pwm_right = Hw_Wheel_RightPwm();
} 

void Wheel_SetTuneState(FunctionalState state)
{
	g_wheel.tune_state = state;
}

void Wheel_TuneSpeedProcess(int8_t l_wheel_speed, int8_t r_wheel_speed,uint8_t state)
{
	if(g_wheel.tune_state == DISABLE)return;

	static volatile int32_t right_proportion = 0, left_proportion = 0;

	uint16_t proportion_biggest = 10;
	
	if(l_wheel_speed > 15)
	{
		if(g_wheel.left_speed_cnt < 15)
		{
			if(g_wheel.left_speed_cnt < (l_wheel_speed * 3 / 5))g_wheel.left_slow++;
			else g_wheel.left_slow = 0;
			if(g_wheel.left_slow > WHEEL_SLOW_LIMIT)g_wheel.left_slow = WHEEL_SLOW_LIMIT;
		}
		else
		{
			g_wheel.left_slow = 0;
		}
	}
	else
	{
		g_wheel.left_slow = 0;
	}
	if(r_wheel_speed > 15)
	{
		if(g_wheel.right_speed_cnt < 15)
		{
			if(g_wheel.right_speed_cnt < (r_wheel_speed * 3 / 5))g_wheel.right_slow++;
			else g_wheel.right_slow = 0;
			if(g_wheel.right_slow > WHEEL_SLOW_LIMIT)g_wheel.right_slow = WHEEL_SLOW_LIMIT;
		}
		else
		{
			g_wheel.right_slow = 0;
		}
	}
	else
	{
		g_wheel.right_slow = 0;
	}

	if(l_wheel_speed > 1)
	{
		g_wheel.left_real_speed = (uint8_t)g_wheel.left_speed_cnt;		
		left_proportion = l_wheel_speed - g_wheel.left_speed_cnt;
		
		if(left_proportion > proportion_biggest)left_proportion = proportion_biggest;
		g_wheel.temp_left_pwm += left_proportion;

		if(g_wheel.temp_left_pwm < 0)g_wheel.temp_left_pwm = 0;
		else if(g_wheel.temp_left_pwm > 99)g_wheel.temp_left_pwm = 100;

		//Usprintf("speed_cnt:%d speed:%d proportion:%d pwm:%d\n",g_wheel.left_speed_cnt,l_wheel_speed,left_proportion,g_wheel.temp_left_pwm);
	}
	else
	{
		g_wheel.temp_left_pwm = 0;
	}
	if(r_wheel_speed > 1)
	{
		g_wheel.right_real_speed = (uint8_t)g_wheel.right_speed_cnt;
		right_proportion =  r_wheel_speed - g_wheel.right_speed_cnt	;
		if(right_proportion > proportion_biggest)right_proportion = proportion_biggest;
		g_wheel.temp_right_pwm += right_proportion;

		if(g_wheel.temp_right_pwm < 0)g_wheel.temp_right_pwm = 0;
		else if(g_wheel.temp_right_pwm > 100)g_wheel.temp_right_pwm = 100;

		//Usprintf("speed_cnt:%d speed:%d proportion:%d pwm:%d\n",g_wheel.right_speed_cnt,r_wheel_speed,right_proportion,g_wheel.temp_right_pwm);
	}
	else
	{
		g_wheel.temp_right_pwm = 0;
	}
	
	g_wheel.left_speed_cnt = 0;
	g_wheel.right_speed_cnt = 0;

	if(state)
	{
		g_wheel.temp_left_pwm = 0;
		g_wheel.temp_right_pwm = 0;
	}

	*g_wheel.p_pwm_left = (uint16_t)g_wheel.temp_left_pwm;
	*g_wheel.p_pwm_right = (uint16_t)g_wheel.temp_right_pwm;	
}

void Wheel_SpeedAccelerationProcess(uint8_t state)
{
	if(state)return;	
	static uint8_t left_increase_cnt = 0, right_increase_cnt = 0;
	uint8_t increase_cnt_limit = 1,speed_increase_limit = 1;
		
	if(g_wheel.left_speed < g_wheel.left_target_speed)
	{
		left_increase_cnt++;
		if(left_increase_cnt > increase_cnt_limit)
		{	
			Wheel_SetLeftSpeed(g_wheel.left_speed + speed_increase_limit);
			left_increase_cnt = 0;
		}
		if(g_wheel.left_speed < PID_ADJUST_TOP_SPEED)Wheel_SetLeftSpeed(g_wheel.left_speed+ speed_increase_limit);
	}
	else if(g_wheel.left_speed > g_wheel.left_target_speed)
	{
		if(g_wheel.left_speed > 1)Wheel_SetLeftSpeed(g_wheel.left_speed - 1);
	}
	
	if(g_wheel.right_speed < g_wheel.right_target_speed)
	{
		right_increase_cnt++;
		if(right_increase_cnt > increase_cnt_limit)
		{
			right_increase_cnt = 0;
			Wheel_SetRightSpeed(g_wheel.right_speed + speed_increase_limit);
		}
		if(g_wheel.right_speed < PID_ADJUST_TOP_SPEED)Wheel_SetRightSpeed(g_wheel.right_speed + speed_increase_limit);
	}
	else if(g_wheel.right_speed > g_wheel.right_target_speed)
	{
		if(g_wheel.right_speed > 1)Wheel_SetRightSpeed(g_wheel.right_speed - 1);
	}
}

void Wheel_Configure(FunctionalState state)
{
	if(state)
	{
		g_wheel.on_set_left_enable(ENABLE);
		g_wheel.on_set_right_enable(ENABLE);
	}
	else
	{
		g_wheel.on_set_left_enable(DISABLE);
		g_wheel.on_set_right_enable(DISABLE);
	}
}

void Wheel_Stop(void)
{
	Wheel_Configure(ENABLE);
	Wheel_SetDir(WHEEL_DIR_FORWARD);
	Wheel_SetSpeed(0,0);
	Wheel_SetTargetSpeed(0,0);
 	Wheel_ResetTempPWM();
	*g_wheel.p_pwm_left = 0;
	*g_wheel.p_pwm_right = 0;
}

void Wheel_ResetStep(void)
{
  	g_wheel.left_step = 0;
	g_wheel.right_step = 0;
}

uint32_t Wheel_GetLeftStep(void)
{
  return g_wheel.left_step;
}

uint32_t Wheel_GetRightStep(void)
{
  return g_wheel.right_step;
}

void Wheel_SetCount(int32_t left, int32_t right)
{
	g_wheel.left_cnt = left;
	g_wheel.right_cnt = right;
}

void Wheel_ResetCounts(void)
{
	g_wheel.left_cnt = 0;
	g_wheel.right_cnt = 0;
}

int32_t Wheel_GetLeftCount(void)
{
	return g_wheel.left_cnt;
}

int32_t Wheel_GetRightCount(void)
{
	return g_wheel.right_cnt;
}

void Wheel_SetSpeed(uint8_t l_speed, uint8_t r_speed)
{
   	Wheel_SetLeftSpeed(l_speed);
	Wheel_SetRightSpeed(r_speed);
}

void Wheel_ResetSpeed(void)
{
	g_wheel.right_speed = 0;
	g_wheel.left_speed = 0;
}

void Wheel_SetLeftSpeed(uint8_t speed)
{
  	if(speed > 100)speed = 100;
	g_wheel.left_speed = speed;
}

void Wheel_SetRightSpeed(uint8_t speed)
{
  	if(speed > 100)speed = 100;
	g_wheel.right_speed = speed;
}

int8_t Wheel_GetLeftSpeed(void)
{
  return g_wheel.left_speed;
}

int8_t Wheel_GetRightSpeed(void)
{
  return g_wheel.right_speed; 
}

void Wheel_SetTargetSpeed(uint8_t l_speed,uint8_t r_speed)
{
	Wheel_SetLeftTargetSpeed(l_speed);
	Wheel_SetRightTargetSpeed(r_speed);
}

void Wheel_SetLeftTargetSpeed(uint8_t l_speed)
{
	g_wheel.left_target_speed = l_speed;
}

void Wheel_SetRightTargetSpeed(uint8_t r_speed)
{
	g_wheel.right_target_speed = r_speed;
} 

uint8_t Wheel_GetLeftTargetSpeed(void)
{
	return g_wheel.left_target_speed;
}

uint8_t Wheel_GetRightTargetSpeed(void)
{
	return g_wheel.right_target_speed;
}

void Wheel_SetTargetStep(uint32_t l_step,uint32_t r_step)
{
	 g_wheel.left_target_step = l_step;
	 g_wheel.right_target_step = r_step;
}

uint32_t Wheel_GetLeftTargetStep(void)
{
	return g_wheel.left_target_step;
}

uint32_t Wheel_GetRightTargetStep(void)
{
	return g_wheel.right_target_step;
}

void Wheel_SetDir(WheelDir_t dir)
{
	g_wheel.dir	=dir;
	switch(g_wheel.dir)
	{

		case WHEEL_DIR_FORWARD:								if(g_wheel.left_dir != WHEEL_DIR_FORWARD)Wheel_SetLeftSpeed(0);
															if(g_wheel.right_dir!= WHEEL_DIR_FORWARD)Wheel_SetRightSpeed(0);
															Wheel_SetLeftDir(WHEEL_DIR_FORWARD);
															Wheel_SetRightDir(WHEEL_DIR_FORWARD);
															return;
		case WHEEL_DIR_BACKWARD:							if(g_wheel.left_dir != WHEEL_DIR_BACKWARD)Wheel_SetLeftSpeed(0);
															if(g_wheel.right_dir!= WHEEL_DIR_BACKWARD)Wheel_SetRightSpeed(0);
															Wheel_SetLeftDir(WHEEL_DIR_BACKWARD);
															Wheel_SetRightDir(WHEEL_DIR_BACKWARD);
															return;
		case WHEEL_DIR_LEFT:								if(g_wheel.left_dir != WHEEL_DIR_BACKWARD)Wheel_SetLeftSpeed(0);
															if(g_wheel.right_dir != WHEEL_DIR_FORWARD)Wheel_SetRightSpeed(0);
															Wheel_SetLeftDir(WHEEL_DIR_BACKWARD);
															Wheel_SetRightDir(WHEEL_DIR_FORWARD);
															return;
		case WHEEL_DIR_RIGHT:								if(g_wheel.left_dir != WHEEL_DIR_FORWARD)Wheel_SetLeftSpeed(0);
															if(g_wheel.right_dir != WHEEL_DIR_BACKWARD)Wheel_SetRightSpeed(0);
															Wheel_SetLeftDir(WHEEL_DIR_FORWARD);
															Wheel_SetRightDir(WHEEL_DIR_BACKWARD);
															return;
	}
}

WheelDir_t Wheel_GetDir(void)
{
	return g_wheel.dir;
}

void Wheel_SetLeftDir(WheelDir_t dir)
{
	if(dir == WHEEL_DIR_FORWARD)
	{
		g_wheel.on_set_left_dir(DIR_FORWARD);
	}
	else
	{
		g_wheel.on_set_left_dir(DIR_BACKWARD);
	}
	g_wheel.left_dir = dir;
}

void Wheel_SetRightDir(WheelDir_t dir)
{
	if(dir == WHEEL_DIR_FORWARD)
	{
		g_wheel.on_set_right_dir(DIR_FORWARD);
	}
	else
	{
		g_wheel.on_set_right_dir(DIR_BACKWARD);
	}
	g_wheel.right_dir = dir;
}

WheelDir_t Wheel_GetLeftDir(void)
{
	return g_wheel.left_dir;
}

WheelDir_t Wheel_GetRightDir(void)
{
	return g_wheel.right_dir;
}

uint8_t Wheel_IsForward(void)
{
	if(g_wheel.dir == WHEEL_DIR_FORWARD)
	{
		return 1;
	}
	return 0;
}

void Wheel_ResetTempPWM(void)
{
  g_wheel.temp_right_pwm = 0;
  g_wheel.temp_left_pwm = 0;
}

void Wheel_ResetPWM(void)
{
	*g_wheel.p_pwm_left = 0;
	*g_wheel.p_pwm_right = 0;
}

void Wheel_ResetSlowCnt(void)
{
	g_wheel.right_slow = 0;
	g_wheel.left_slow = 0;
}

uint8_t Wheel_IsLeftLoseCnt(void)
{
	if(g_wheel.left_slow > 100)
	{
		return 1;
	}	
	return 0;
}

uint8_t Wheel_IsRightLoseCnt(void)
{
	if(g_wheel.right_slow > 100)
	{
		return 1;
	}	
	return 0;
}

void Wheel_TuneLeftSpeedDir(int32_t *speed,int32_t speed_limit)
{
	if(*speed < 0)
	{
		*speed = -*speed;
		if(*speed > speed_limit)*speed = speed_limit;
		
		if(g_wheel.left_speed <= 1)
		{
			Wheel_SetLeftDir(WHEEL_DIR_BACKWARD);
		}
		else if(g_wheel.left_dir == WHEEL_DIR_FORWARD)
		{
			*speed = 0;
		}
	}
	else
	{
		if(g_wheel.left_dir != WHEEL_DIR_FORWARD)Wheel_SetLeftSpeed(0);
		Wheel_SetLeftDir(WHEEL_DIR_FORWARD);
	}
}

void Wheel_TuneRightSpeedDir(int32_t *speed,int32_t speed_limit)
{
	if(*speed < 0)
	{
		*speed = -*speed;
		if(*speed > speed_limit)*speed = speed_limit;
		if(g_wheel.right_speed <= 1)
		{
			Wheel_SetRightDir(WHEEL_DIR_BACKWARD);
		}
		else if(g_wheel.right_dir == WHEEL_DIR_FORWARD)
		{
			*speed = 0;
		}
	}
	else
	{
		if(g_wheel.right_dir != WHEEL_DIR_FORWARD)Wheel_SetRightSpeed(0);
		Wheel_SetRightDir(WHEEL_DIR_FORWARD);
	}
}

void Wheel_LeftInterrupt(void)
{
	g_wheel.left_speed_cnt++;
	g_wheel.left_step++;
	
	if(g_wheel.left_dir == WHEEL_DIR_FORWARD)
	{
		g_wheel.left_move_step++;
		g_wheel.left_cnt++;		 
	}
	else
	{
		g_wheel.left_move_step--;
		g_wheel.left_cnt--;			 
	}	
}

void Wheel_RightInterrupt(void)
{
	g_wheel.right_speed_cnt++;
	g_wheel.right_step++;
	
	if(g_wheel.right_dir == WHEEL_DIR_FORWARD)
	{
		g_wheel.right_move_step++;
		g_wheel.right_cnt++;		 
	}
	else
	{
		g_wheel.right_move_step--;
		g_wheel.right_cnt--;			 
	}	
}

void Wheel_StoreBaseline(void)
{
	uint8_t i = 0;
	int32_t left_wheel_sum = 0,right_wheel_sum = 0;
	for(i = 0; i < 10; i++)
	{
		left_wheel_sum += g_wheel.on_get_left_adc();
		right_wheel_sum += g_wheel.on_get_right_adc();
		delay(20);
	}
	g_wheel.left_baseline = left_wheel_sum / 10;
	g_wheel.right_baseline = right_wheel_sum / 10;
}

void Wheel_CurrentDetectionCycle(float adc2current)
{
	static uint32_t l_wheel_current_sum = 0,r_wheel_current_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	l_wheel_current_sum += g_wheel.on_get_left_adc();
	r_wheel_current_sum += g_wheel.on_get_right_adc();
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		l_wheel_current_sum /= 8;
		if(l_wheel_current_sum >= g_wheel.left_baseline)
		{
			l_wheel_current_sum = (uint32_t)((l_wheel_current_sum - g_wheel.left_baseline) * adc2current); 
		}
		else
		{
			l_wheel_current_sum = 0;
		}
		g_wheel.left_current = l_wheel_current_sum;
		l_wheel_current_sum = 0;

		r_wheel_current_sum /= 8;
		if(r_wheel_current_sum >= g_wheel.right_baseline)
		{
			r_wheel_current_sum = (uint32_t)((r_wheel_current_sum - g_wheel.right_baseline) * adc2current); 
		}
		else
		{
			r_wheel_current_sum = 0;
		}		
		g_wheel.right_current = r_wheel_current_sum;
		r_wheel_current_sum = 0;
	}
}

uint8_t Wheel_CheckLeftCurrent(void)
{
	static uint8_t left_wheel_overcurrent_cnt = 0;
	int16_t temp_current = 0;	
	if(g_wheel.left_speed > 15)
	{
		temp_current = g_wheel.left_current;
		if(temp_current < WHEEL_STALL_NOLOAD)
		{
			if(*g_wheel.p_pwm_left > 10)left_wheel_overcurrent_cnt++;
		}
		else if(temp_current > WHEEL_STALL_LIMIT)
		{
			left_wheel_overcurrent_cnt += 5;
		}
		else
		{
			left_wheel_overcurrent_cnt = 0;
		}
		if(left_wheel_overcurrent_cnt > 250)
		{
			//Usprintf("Left Wheel Current Over Limit:%d\n",g_wheel.left_current);
			left_wheel_overcurrent_cnt = 0;
			return 1;
		}
	}
	else
	{
		left_wheel_overcurrent_cnt = 0;
	}
	
	return 0;
}

uint8_t Wheel_CheckRightCurrent(void)
{
	static uint16_t right_wheel_overcurrent_cnt = 0;
	int16_t temp_current = 0;	
	if(g_wheel.right_speed > 15)
	{
		temp_current = g_wheel.right_current;
		if(temp_current < WHEEL_STALL_NOLOAD)
		{
			if(*g_wheel.p_pwm_right >10 )right_wheel_overcurrent_cnt++;
		}
		else if(temp_current > WHEEL_STALL_LIMIT)
		{
			right_wheel_overcurrent_cnt += 5;
		}
		else
		{
			right_wheel_overcurrent_cnt = 0;
		}
		if(right_wheel_overcurrent_cnt > 250)
		{
			right_wheel_overcurrent_cnt = 0;
			//Usprintf("Right Wheel Current Over Limit:%d\n",g_wheel.right_current);
			return 1;
		}
	}
	else
	{
		right_wheel_overcurrent_cnt = 0;	
	}
	
	return 0;
}

int16_t Wheel_GetLeftCurrent(void)
{
	return g_wheel.left_current;
}

int16_t Wheel_GetRightCurrent(void)
{
	return g_wheel.right_current;
}












