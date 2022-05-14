 #include "Include.h"
Obs_t g_obs;

void OBS_Init(void)
{
	memset(&g_obs,0,sizeof(g_obs));	
	g_obs.left_trig_val = 1000;
	g_obs.front_trig_val = 1000;
	g_obs.right_trig_val = 1000;
	g_obs.on_set_driver_state = Hw_Obs_SetDriverState;	
}

void OBS_DynamicProcess(FunctionalState state)
{
	static int32_t front_obs_buffer = 0, left_obs_buffer = 0, right_obs_buffer = 0;
	static uint32_t front_obs_stable_cnt = 0, left_obs_stable_cnt=0, right_obs_stable_cnt = 0;
	static int32_t front_obs_average_value = 0, left_obs_average_value=0, right_obs_average_value = 0;
	static int32_t front_obs_sum = 0, left_obs_sum=0, right_obs_sum = 0;
	
	if(state == DISABLE)return;
	if(g_obs.adc.left ==0)return;	
	if(g_obs.adc.front ==0)return;
	if(g_obs.adc.right ==0)return;	
	
	front_obs_buffer = g_obs.adc.front;
	front_obs_sum += front_obs_buffer;
	front_obs_stable_cnt++;
	front_obs_average_value = front_obs_sum / front_obs_stable_cnt;
	if(Math_Diff_int(front_obs_average_value, front_obs_buffer) > 50)
	{
		front_obs_stable_cnt = 0;
		front_obs_sum = 0;
	}
	if(front_obs_stable_cnt > 10)
	{
		front_obs_stable_cnt = 0;
		front_obs_buffer = g_obs.front_trig_val  - OBS_FRONT_TRIG_BIAS;
		front_obs_buffer = (front_obs_average_value * 10 + front_obs_buffer * 20) / 30;
		if(front_obs_buffer < 300)front_obs_buffer = 300;
		g_obs.front_trig_val  = front_obs_buffer + OBS_FRONT_TRIG_BIAS;
		//Usprintf("front obs trig val------>%d\n",g_obs.front_trig_val );
	}

	left_obs_buffer = g_obs.adc.left;
	left_obs_sum += left_obs_buffer;
	left_obs_stable_cnt++;
	left_obs_average_value=left_obs_sum / left_obs_stable_cnt;
	if(Math_Diff_int(left_obs_average_value, left_obs_buffer) > 50)
	{
		left_obs_stable_cnt = 0;
		left_obs_sum = 0;
	}
	if(left_obs_stable_cnt > 10)
	{
		left_obs_stable_cnt = 0;
		left_obs_buffer = g_obs.left_trig_val - OBS_LEFT_TRIG_BIAS;
		left_obs_buffer = (left_obs_average_value * 10  +left_obs_buffer * 20) / 30;
		if(left_obs_buffer < 300)left_obs_buffer = 300;
		g_obs.left_trig_val = left_obs_buffer + OBS_LEFT_TRIG_BIAS;
		//Usprintf("left obs trig val------>%d\n",g_obs.left_trig_val);
	}
	right_obs_buffer = g_obs.adc.right;
	right_obs_sum += right_obs_buffer;
	right_obs_stable_cnt++;
	right_obs_average_value = right_obs_sum / right_obs_stable_cnt;
	if(Math_Diff_int(right_obs_average_value, right_obs_buffer) > 50)
	{
		right_obs_stable_cnt = 0;
		right_obs_sum = 0;
	}
	if(right_obs_stable_cnt > 10)
	{
		right_obs_stable_cnt = 0;
		right_obs_buffer = g_obs.right_trig_val - OBS_RIGHT_TRIG_BIAS;
		right_obs_buffer = (right_obs_average_value * 10 + right_obs_buffer * 20) / 30;
		if(right_obs_buffer < 300)right_obs_buffer = 300;
		g_obs.right_trig_val = right_obs_buffer + OBS_RIGHT_TRIG_BIAS;
		//Usprintf("right obs trig val------>%d\n",g_obs.right_trig_val);
	}
}

void OBS_SetDynamicState(FunctionalState state)
{
	g_obs.dynamic_state = state;
}

FunctionalState OBS_GetDynamicState(void)
{
	return g_obs.dynamic_state;
}

ObsTrig_t OBS_GetTrigStatus(void)
{
	ObsTrig_t status = OBS_TRIG_NONE;
	if(g_obs.adc.left  > g_obs.left_trig_val)status  = OBS_TRIG_LEFT;
	if(g_obs.adc.front > g_obs.front_trig_val )status = OBS_TRIG_FRONT;
	if(g_obs.adc.right > g_obs.right_trig_val)status = OBS_TRIG_RIGHT;

	return status;
}

uint8_t OBS_FrontClose(void)
{
  if(g_obs.adc.front > (g_obs.front_trig_val  - OBS_FRONT_TRIG_BIAS + OBS_NEAR_TRIG))return 1;
  return 0;
}

uint8_t OBS_IsNear(void)
{
	if(g_obs.adc.left  > (g_obs.left_trig_val - OBS_LEFT_TRIG_BIAS + OBS_NEAR_TRIG))return 1;
	if(g_obs.adc.front > (g_obs.front_trig_val  - OBS_FRONT_TRIG_BIAS + OBS_NEAR_TRIG))return 1;
	if(g_obs.adc.right > (g_obs.right_trig_val - OBS_RIGHT_TRIG_BIAS + OBS_NEAR_TRIG))return 1;	
	return 0;
}

void OBS_IsTrigLimit(ObsTrig_t *status)
{
	static uint16_t g_obs_trig_cnt = 0;	
	if(*status)
	{
		g_obs_trig_cnt ++;	
	}
	else
	{
		g_obs_trig_cnt = 0;		
	}

	if(g_obs_trig_cnt >= 7)
	{
		*status = OBS_TRIG_NONE;
	}
}










