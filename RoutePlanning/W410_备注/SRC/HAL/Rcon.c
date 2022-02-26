 #include "Include.h"

Rcon_t g_rcon;

void Rcon_Init(void)
{
	memset(&g_rcon,0,sizeof(g_rcon));
	g_rcon.on_set_temp_remote = Remote_SetTempRemoteWithCode;
	g_rcon.on_get_fl_state = Hw_Rcon_GetFlState;
	g_rcon.on_get_fr_state = Hw_Rcon_GetFrState;
}

uint32_t Rcon_GetStatus(void)
{
  return g_rcon.status;
}

void Rcon_ResetStatus(void)
{
  g_rcon.status=0;
}

void Rcon_ResetTheStatus(uint32_t status)
{
  g_rcon.status &= ~status;
}

void Rcon_IncreaseCounter(void)
{
	g_rcon.fl.time_counter++;
	if(g_rcon.fl.time_counter > 0xfffd)g_rcon.fl.time_counter = 0xfffd;

	g_rcon.fr.time_counter++;
	if(g_rcon.fr.time_counter > 0xfffd)g_rcon.fr.time_counter = 0xfffd;
}

void Rcon_ClearState(volatile Rcon_Element *rcon)
{
	rcon->bit_counter = 0;
	rcon->receive_start = 0;
	rcon->time_counter = 0;
	rcon->temp_counter = 0 ;
	rcon->remote_flag = 0;
}

void Rcon_ResetTempCode(Rcon_Element *rcon)
{
	rcon->temp_code = 0;
}

void Rcon_SetState(uint64_t temp_code,uint8_t remote)
{
	switch(remote)
	{
		case RCONSIDE_FL:
										switch(temp_code)
										{
											case RCON_CHARGE_LEFT:
																						
																						 break;
											case RCON_CHARGE_RIGHT:
																						 
																						 break;
											case RCON_CHARGE_TOP:  
																						 g_rcon.status |= RCON_FL_HOME_T;
																						 break;
											case RCON_VW_NORMAL: 
																						 g_rcon.status |= RCON_FL_VW_N;
																						 break;
											case RCON_VW_TOP:
																						 g_rcon.status |= RCON_FL_VW_T;
																						 break;
											case RCON_W_CHARGE_LEFT:
																						 g_rcon.status |= RCON_FL_HOME_L;																					 
																						 break;
											case RCON_W_CHARGE_RIGHT:
																						 g_rcon.status |= RCON_FL_HOME_R;
																						 break;				
											case RCON_W_CHARGE_TOP:
																						 g_rcon.status |= RCON_FL_HOME_T;
																						 break;																											 
											default:             
																						 break;
										}
										break;
		case RCONSIDE_FR:
										switch(temp_code)
										{
											case RCON_CHARGE_LEFT:
																			
																						 break;
											case RCON_CHARGE_RIGHT:
																				
																						 break;
											case RCON_CHARGE_TOP:  
																						 g_rcon.status |= RCON_FR_HOME_T;
																						 break;
											case RCON_VW_NORMAL: 
																						 g_rcon.status |= RCON_FR_VW_N;
																						 break;
											case RCON_VW_TOP:
																						 g_rcon.status |= RCON_FR_VW_T;
																						 break;
											case RCON_W_CHARGE_LEFT:
																						 g_rcon.status |= RCON_FR_HOME_L;
																						 break;
											case RCON_W_CHARGE_RIGHT:
																						 g_rcon.status |= RCON_FR_HOME_R;																						 
																						 break;	
											case RCON_W_CHARGE_TOP:
																						 g_rcon.status |= RCON_FR_HOME_T;
																						 break;																							 																					 
											default:             
																						 break;
										}
										break;
		default:
										break;
	}

}

void Rcon_Decoder(Rcon_Element *element,uint8_t state,RconSide_t side)
{
	uint8_t delta = 0;
	if(element->time_counter>250)
	{
		Rcon_ClearState(element);
		Rcon_ResetTempCode(element);
	}
		
	if(state)
	{
		if(element->receive_start == 2)
		{
			delta = element->time_counter - element->temp_counter;
			if(delta > RCON_LOW_MIN && delta < RCON_LOW_MAX)  
			{
				element->time_counter = 0;
				element->temp_counter = 0;
				element->bit_counter++;

				if(element->bit_counter == 8)
				{
				  if(element->temp_code != 0x01)
					{					
						if(element->remote_flag)
						{
							g_rcon.on_set_temp_remote(element->temp_code);	
						}						
						else
						{
							Rcon_SetState(element->temp_code,side);	
						}									
						Rcon_ResetTempCode(element);	
						Rcon_ClearState(element);			
					}				
				}
				else if(element->bit_counter>8)
				{
					Rcon_ClearState(element);
					Rcon_ResetTempCode(element);
					element->receive_start = 0;
				}				
			}
			else if(delta < 3)   
			{
				element->temp_code >>= 1;	
			}
			else
			{
				Rcon_ClearState(element);
				Rcon_ResetTempCode(element);
			}
		}
		else if(element->receive_start == 1)
		{
			delta = element->time_counter - element->temp_counter;
			if(delta > RCON_LOW_MIN && delta < RCON_LOW_MAX)  
		  	{
				element->receive_start = 2;
				element->time_counter = 0;
				element->bit_counter = 0;	
				Rcon_ResetTempCode(element);
			}
			else if(delta < 3)     
			{
			}
			else
			{
				Rcon_ClearState(element);
			}
		}
		else if(element->receive_start == 0)
		{
			if((element->time_counter > RCON_HEAD_LOW_MIN) && (element->time_counter < RCON_HEAD_LOW_MAX))
			{
				element->receive_start = 1;
				element->temp_counter = 0;
				element->time_counter = 0;
				element->remote_flag = 0;
			}
			else if((element->time_counter > RCON_STANDRD_HEAD_LOW_MIN) && (element->time_counter < RCON_STANDRD_HEAD_LOW_MAX))
			{				
				element->receive_start = 1;
				element->time_counter = 0;
				element->temp_counter = 0;
				element->remote_flag = 1;
			}
			else 
			{
				element->temp_counter = element->time_counter;
			}
		}
	}
	else	
	{
		if(element->receive_start == 2)
		{
			element->temp_code <<= 1;
			
			if((element->time_counter > RCON_LOGIC_1_MIN)&&(element->time_counter < RCON_LOGIC_1_MAX))
			{
				element->temp_code |=1;
				element->temp_counter = element->time_counter;
			}
			else if((element->time_counter > RCON_LOGIC_0_MIN) && (element->time_counter < RCON_LOGIC_0_MAX))
			{
				element->temp_counter = element->time_counter;
			}
			else
			{
				if(element->time_counter<3)
				{
				  element->temp_counter = element->time_counter;						
				}
				else
				{
					Rcon_ClearState(element);
					Rcon_ResetTempCode(element);
				}
			}
		}		
		else if(element->receive_start == 1)	
		{
			if((element->time_counter > RCON_HEAD_HIGH_MIN) && (element->time_counter < RCON_HEAD_HIGH_MAX))
			{
				element->receive_start = 2;
				element->time_counter = 0;
				element->temp_code = 0;
				element->temp_counter = 0;
				element->bit_counter = 0;
			}
			else
			{
				element->temp_counter = element->time_counter;
			}
		}		
		else if(element->receive_start == 0)
		{
			if((element->time_counter - element->temp_counter)<3)
			{
			}
			else
			{
			  element->time_counter = 0;
			  element->temp_counter = 0;
			}			
		}
		else
		{
    		Rcon_ClearState(element);			
		}
	}
}










