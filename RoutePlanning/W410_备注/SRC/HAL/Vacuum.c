 #include "Include.h"

Vacuum_t g_vacuum;

void Vacuum_Init(void)
{
	memset(&g_vacuum,0,sizeof(g_vacuum));
	g_vacuum.on_get_adc = Hw_Vacuum_GetAdc;
	g_vacuum.on_set_power_state = Hw_Vacuum_SetPowerState;
	g_vacuum.p_pwm = Hw_Vacuum_Pwm();
}

void Vacuum_TuneProcess(void)
{
	int32_t diff_rpm = 0;
	static uint32_t vac_fail_cnt = 0;

	if(!g_vacuum.speed)return;

	if(GPIOA->ODR & MCU_VACUUM_PWM)
	{
		diff_rpm = g_vacuum.speed - g_vacuum.pulse_cnt;
		if(diff_rpm > 0)
		{
			if(diff_rpm > 300)
			{
				g_vacuum.temp_pwm += 20;
			}
			else if(diff_rpm > 80)
			{
				g_vacuum.temp_pwm += 3;
			}
			if(g_vacuum.temp_pwm > 99)g_vacuum.temp_pwm = 99;
		}
		else
		{
			if(diff_rpm <- 100)
			{
				if(g_vacuum.temp_pwm > 5)g_vacuum.temp_pwm -= 3;
			}
			else if(diff_rpm <- 80)
			{
				g_vacuum.temp_pwm --;
			}
		}
		if(g_vacuum.temp_pwm < 10)g_vacuum.temp_pwm = 10;
		*g_vacuum.p_pwm = g_vacuum.temp_pwm;
		if(g_vacuum.pulse_cnt == 0)
		{
			vac_fail_cnt++;
			if(vac_fail_cnt > 10)
			{
				vac_fail_cnt = 0;
				g_vacuum.fail_flag = 1;
			}
		}
		else
		{
			vac_fail_cnt = 0;
		}
	}
	else
	{
		vac_fail_cnt = 0;
	}
	g_vacuum.pulse_cnt = 0;
	
}

void Vacuum_SetSpeed(uint32_t speed)
{
	g_vacuum.speed = speed;
	if(g_vacuum.speed == 0)
	{
		g_vacuum.on_set_power_state(OFF);
		return;
	}
	g_vacuum.on_set_power_state(ON);
}

int32_t Vacuum_GetSpeed(void)
{
	return g_vacuum.speed;
}

void Vacuum_StoreBaseline(void)
{
	uint8_t i = 0;
	int32_t vac_adc_sum = 0;
	for(i = 0; i < 10; i++)
	{
		vac_adc_sum += g_vacuum.on_get_adc();
		delay(20);
	}
	g_vacuum.baseline = vac_adc_sum / 10;
}

void Vacuum_CurrentDetectionCycle(float adc2current)
{
	static uint32_t vac_current_sum = 0;
	static uint8_t cycle_cnt = 8;	
	
	vac_current_sum += g_vacuum.on_get_adc();
	cycle_cnt--;
	if(cycle_cnt == 0)
	{
		cycle_cnt=8;
		vac_current_sum /= 8;
		vac_current_sum = (vac_current_sum - g_vacuum.baseline) * adc2current;
		g_vacuum.current = vac_current_sum;
		vac_current_sum = 0;
	}
}

uint8_t Vacuum_IsFail(void)
{
	return g_vacuum.fail_flag;
}

void Vacuum_ResetFail(void)
{
	g_vacuum.fail_flag = 0;
}

uint8_t Vacuum_CheckCurrent(void) 
{
	static uint16_t vac_temp_stall_cnt=0;
	int16_t temp_current = 0;
	
	temp_current = g_vacuum.current;
	if(*g_vacuum.p_pwm < 10)return 0;
	if((temp_current >  VACUUM_STALL_LIMIT)
		|| (temp_current < VACUUM_STALL_NOLOAD))
	{
		vac_temp_stall_cnt++;
	}
	else
	{
		vac_temp_stall_cnt = 0;
	}
	if(vac_temp_stall_cnt > 200)
	{
		vac_temp_stall_cnt = 0;
		return 1;
	}
	return 0;
}

int16_t Vacuum_GetCurrent(void)
{
	return g_vacuum.current;
}

















