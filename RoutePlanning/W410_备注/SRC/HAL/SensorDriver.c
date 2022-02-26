 #include "Include.h"

SensorDriver_t g_sensordriver;

void SensorDriver_Init(void)
{
	memset(&g_sensordriver,0,sizeof(g_sensordriver));
	g_sensordriver.on_start_adc = Hw_Adc_Start;
}

void SensorDriver_Process(FunctionalState cliff_state,FunctionalState obs_wall_state,FunctionalState cln_dirty_water_state,uint8_t code_show)
{	
	if(!g_sensordriver.start)return;
	g_sensordriver.cycle_cnt++;
	
	if(g_sensordriver.l_start)
	{		
		g_sensordriver.adc = 1 - g_sensordriver.adc;
		if(g_sensordriver.adc)
		{
			g_sensordriver.on_start_adc();		
		}
		else
		{
			
			if(g_adc_value.clean >= g_watertank.temp_cln_dirt_ad.clean)g_watertank.adc.clean = g_adc_value.clean - g_watertank.temp_cln_dirt_ad.clean;
			if(g_adc_value.dirt >= g_watertank.temp_cln_dirt_ad.dirt)g_watertank.adc.dirt = g_adc_value.dirt - g_watertank.temp_cln_dirt_ad.dirt;		
			switch(Hc4051_GetChannel())
			{
					case 0: 	
									g_obs.adc.temp_left =	g_adc_value.obs_wall;
					break;
					case 1: 		
									g_obs.adc.temp_front = g_adc_value.obs_wall;
					break;
					case 2: 	
									g_obs.adc.temp_right =	g_adc_value.obs_wall;
					break;
					case 3: 
									g_wall.adc.temp_right = g_adc_value.obs_wall;									
					break;
					case 4: 	
									g_wall.adc.temp_left = g_adc_value.obs_wall;							
					break;
					case 5: 	
									g_cliff.adc.temp_left =	g_adc_value.obs_wall ;
					break;
					case 6: 
									g_cliff.adc.temp_front = g_adc_value.obs_wall;								
					break;
					case 7: 	
									g_cliff.adc.temp_right = g_adc_value.obs_wall;							
					break;								
					default:
					break;					
			}		
			if(Hc4051_ChannelCycle())
			{
				g_sensordriver.l_start = 0;
			}	
			Hc4051_SetChannel();					
		}
	}		
	if(g_sensordriver.h_start)
	{		
		g_sensordriver.adc = 1 - g_sensordriver.adc;
		if(g_sensordriver.adc)
		{
			g_sensordriver.on_start_adc();			
		}
		else
		{
			g_watertank.temp_cln_dirt_ad.clean = g_adc_value.clean;
			g_watertank.temp_cln_dirt_ad.dirt = g_adc_value.dirt;	
			if(g_robot.type == ROBOT_W410)
			{
				switch(Hc4051_GetChannel())
				{
						case 0: 	
										if(g_adc_value.obs_wall >= g_obs.adc.temp_left)
										if(!code_show)g_obs.adc.left = g_adc_value.obs_wall - g_obs.adc.temp_left;
										if(!obs_wall_state)	g_obs.adc.left = 0;							
						break;
						case 1: 		
										if(g_adc_value.obs_wall >= g_obs.adc.temp_front)
										if(!code_show)g_obs.adc.front =	g_adc_value.obs_wall - g_obs.adc.temp_front;	
										if(!obs_wall_state)	g_obs.adc.front = 0;								
						break;
						case 2: 	
										if(g_adc_value.obs_wall >= g_obs.adc.temp_right)
										if(!code_show)g_obs.adc.right =	g_adc_value.obs_wall - g_obs.adc.temp_right;
										if(!obs_wall_state)	g_obs.adc.right = 0;	
						break;
						case 3: 
										if(g_adc_value.obs_wall >= g_wall.adc.temp_right)
										if(!code_show)g_wall.adc.right = g_adc_value.obs_wall - g_wall.adc.temp_right;									
						break;
						case 4: 	
										if(g_adc_value.obs_wall >= g_wall.adc.temp_left)
										if(!code_show)g_wall.adc.left = g_adc_value.obs_wall - g_wall.adc.temp_left;							
						break;
						case 5: 	
										if(g_adc_value.obs_wall >= g_cliff.adc.temp_left)
										g_cliff.adc.left =	g_adc_value.obs_wall - g_cliff.adc.temp_left;
						break;
						case 6: 
										if(g_adc_value.obs_wall >= g_cliff.adc.temp_front)
										g_cliff.adc.front =	g_adc_value.obs_wall - g_cliff.adc.temp_front;								
						break;
						case 7: 	
										if(g_adc_value.obs_wall >= g_cliff.adc.temp_right)
										g_cliff.adc.right =	g_adc_value.obs_wall - g_cliff.adc.temp_right;							
						break;								
						default:
						break;					
				}	
			}				
			if(Hc4051_ChannelCycle())
			{
				g_sensordriver.h_start = 0;
			}					
			Hc4051_SetChannel();	
		}				
	}		
	if(g_sensordriver.cycle_cnt == 20)
	{					
		if(cln_dirty_water_state)g_watertank.on_set_dirty_driver_state(ON);					
		if(cln_dirty_water_state)g_watertank.on_set_clean_driver_state(ON);					
		if(cliff_state)g_cliff.on_power_ctrl(ON);					
		if(obs_wall_state)
		{
			if(!code_show)
			{
				g_obs.on_set_driver_state(ON);
			}
		}
		g_sensordriver.h_start = 1;
		g_sensordriver.adc = 0;			
	}
	if(g_sensordriver.cycle_cnt >= 40)
	{			
		g_sensordriver.cycle_cnt = 0;
		g_watertank.on_set_dirty_driver_state(OFF);					
		g_watertank.on_set_clean_driver_state(OFF);					
		g_cliff.on_power_ctrl(OFF);					
		if(!code_show)g_obs.on_set_driver_state(OFF);;
		g_sensordriver.l_start = 1;	
		g_sensordriver.adc = 0;		
	}		
}

void SensorDriver_Start(void)
{
	g_hc4051.on_set_a0_state(OFF);				
	g_hc4051.on_set_a1_state(OFF);					
	g_hc4051.on_set_a2_state(OFF);				
	g_watertank.on_set_dirty_driver_state(OFF);						
	g_watertank.on_set_clean_driver_state(OFF);						
	g_cliff.on_power_ctrl(OFF);					
	g_obs.on_set_driver_state(OFF);;
	g_sensordriver.start = 1;
	Hc4051_ResetChannel();
	g_sensordriver.h_start = 0;
	g_sensordriver.l_start = 1;
	g_sensordriver.on = 0;
	g_sensordriver.adc = 0;
}

































