 #include "Include.h"

Wall_t g_wall;

void Wall_Init(void)
{
	memset(&g_wall,0,sizeof(g_wall));
	g_wall.distance = WALL_DEFAULT_DISTANCE;
}

void Wall_DynamicProcess(FunctionalState state)
{
	static int32_t wall_r_sum_value = 0, wall_r_average_value = 0, wall_r_stable_cnt = 0, wall_r_temp_buffer = 0;
	static int32_t wall_l_sum_value = 0, wall_l_average_value = 0, wall_l_stable_cnt = 0, wall_l_temp_buffer = 0;

	if(state == DISABLE)return;

	wall_r_temp_buffer = Wall_GetRightAdcValue();
	wall_r_sum_value += wall_r_temp_buffer;
	wall_r_stable_cnt++;
	wall_r_average_value = wall_r_sum_value / wall_r_stable_cnt;

	if(Math_Diff_int(wall_r_average_value, wall_r_temp_buffer) > 10)
	{
		wall_r_average_value = 0;
		wall_r_stable_cnt = 0;
		wall_r_sum_value = 0;
	}
	if(wall_r_stable_cnt > 20)
	{
		wall_r_average_value += g_wall.right_baseline;
		g_wall.right_baseline = wall_r_average_value;
		wall_r_average_value = 0;
		wall_r_stable_cnt = 0;
		wall_r_sum_value = 0;
	}

	wall_l_temp_buffer = Wall_GetLeftAdcValue();
	wall_l_sum_value += wall_l_temp_buffer;
	wall_l_stable_cnt++;
	wall_l_average_value = wall_l_sum_value / wall_l_stable_cnt;

	if(Math_Diff_int(wall_l_average_value, wall_l_temp_buffer) > 10)
	{
		wall_l_average_value = 0;
		wall_l_stable_cnt = 0;
		wall_l_sum_value = 0;
	}
	if(wall_l_stable_cnt > 20)
	{
		g_wall.left_baseline = wall_l_average_value;
		wall_l_average_value = 0;
		wall_l_stable_cnt = 0;
		wall_l_sum_value = 0;
	}
}

void Wall_SetDynamicState(FunctionalState state)
{
	g_wall.dynamic_state = state;
}

FunctionalState Wall_GetDynamicState(void)
{
	return g_wall.dynamic_state;
}

int16_t Wall_GetRightAdcValue(void)
{
	return g_wall.adc.right - g_wall.right_baseline;
}

int16_t Wall_GetLeftAdcValue(void)
{
	return g_wall.adc.left - g_wall.left_baseline;
}

int32_t Wall_GetDistance(void)
{
	return g_wall.distance;
}

void Wall_SetDistance(int32_t dis)
{
	g_wall.distance = dis;
}

void Wall_ResetDistance(void)
{
	g_wall.distance = WALL_DEFAULT_DISTANCE;
}

void Wall_DistanceDecrease(void)
{
	g_wall.distance -= 30;
	if(g_wall.distance <= 80)
	{
		g_wall.distance = WALL_DEFAULT_DISTANCE;				
	}		
}

uint8_t Wall_UpdateBuffer(int16_t value)
{
	uint8_t reval = 0;
	if(g_wall.distance >= 100)
	{
		g_wall.buffer[2] = g_wall.buffer[1]; 
		g_wall.buffer[1] = g_wall.buffer[0];
		g_wall.buffer[0] = value;
		if(g_wall.buffer[0] < 100)
		{
			if((g_wall.buffer[1] - g_wall.buffer[0]) > (g_wall.distance/50))
			{
				if((g_wall.buffer[2] - g_wall.buffer[1]) > (g_wall.distance/50))
				{
					reval = 1;
					Usprintf("meet right angle\n");
				}				
			}			
		}
	}	
	return reval;
}










