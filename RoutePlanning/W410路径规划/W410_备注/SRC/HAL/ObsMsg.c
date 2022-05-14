#include "Include.h"

ObsMsg_t  g_obsmsg;


void ObsMsg_Init(void)
{
    memset(&g_obsmsg,0,sizeof(g_obsmsg));	
	g_obsmsg.on_set_temp_remote = Remote_SetTempRemoteWithCode;
	g_obsmsg.on_set_temp_rcon = Rcon_SetState;    
    g_obsmsg.on_usart_send = USART2_DMA_String;
    g_obsmsg.on_set_power_state = Hw_Power_Set_Obs_State;
	g_obsmsg.p_obs_adc_left = &g_obs.adc.left;
	g_obsmsg.p_obs_adc_front = &g_obs.adc.front;
	g_obsmsg.p_obs_adc_right = &g_obs.adc.right;
	g_obsmsg.p_wall_adc_left = &g_wall.adc.left;
	g_obsmsg.p_wall_adc_right = &g_wall.adc.right;
	g_obsmsg.p_cliff_adc_left = &g_cliff.adc.left;
	g_obsmsg.p_cliff_adc_front = &g_cliff.adc.front;
	g_obsmsg.p_cliff_adc_right = &g_cliff.adc.right;       
}

uint8_t ObsMsg_Parse(volatile uint8_t * p_msg)
{
    uint8_t c, checksum = 0;
    uint8_t state = 0;

    for (c = 0; c < (p_msg[0]-1); ++c)
    {
        checksum += *(p_msg + c);
    }
    if(checksum != p_msg[p_msg[0] - 1])
    {
        return 0;
    }

    if(g_obsmsg.cur_mode == OBSMSG_ENABLE)
    {
        g_obsmsg.rx_fps++;

        if(Timer_IsTimeReach(&g_obsmsg.check_time,TIME_1S))
        {
            g_obsmsg.real_fps = g_obsmsg.rx_fps;
            g_obsmsg.rx_fps = 0;
        }
    }
    else
    {
        g_obsmsg.real_fps = 0;
        g_obsmsg.rx_fps = 0;        
    }

    if(p_msg[1] == OBSMSG_DATA_WALL)        
    {
        *g_obsmsg.p_obs_adc_left = (((p_msg[2] << 8) + p_msg[3])); 
        *g_obsmsg.p_obs_adc_front = (((p_msg[4] << 8) + p_msg[5]));	
        *g_obsmsg.p_obs_adc_right  = (((p_msg[6] << 8) + p_msg[7]));
        *g_obsmsg.p_wall_adc_left = (((p_msg[8] << 8) + p_msg[9]));
        *g_obsmsg.p_wall_adc_right = (((p_msg[10] << 8) + p_msg[11]));
        //
        //
        *g_obsmsg.p_cliff_adc_left = (((p_msg[16] << 8) + p_msg[17]));
        *g_obsmsg.p_cliff_adc_front = (((p_msg[18] << 8) + p_msg[19]));
        *g_obsmsg.p_cliff_adc_right = (((p_msg[20] << 8) + p_msg[21]));
        state = 1;
    }
    else if(p_msg[1] == OBSMSG_DATA_RCON)
    {  
        switch(p_msg[2])
        {
            case OBSMSG_RECIEVE_REMOTE:    
                                    if(p_msg[OBS_RCON_FL_OFFSET] != 0)
                                    {
                                       g_obsmsg.on_set_temp_remote(p_msg[OBS_RCON_FL_OFFSET]);     
                                    }                                   
                                    if(p_msg[OBS_RCON_FR_OFFSET] != 0)
                                    {
                                        g_obsmsg.on_set_temp_remote(p_msg[OBS_RCON_FR_OFFSET]);	
                                    }                                  
                                    break;
            case OBSMSG_RECIEVE_VIRTUAL:
                                    if(p_msg[OBS_RCON_FL_OFFSET] != 0)
                                    {    
                                        uint64_t temp_code = p_msg[OBS_RCON_FL_OFFSET];
                                        g_obsmsg.on_set_temp_rcon(temp_code,OBSMSG_RCONSIDE_LEFT);	                                        
                                    }
                                    if(p_msg[OBS_RCON_FR_OFFSET] != 0)
                                    {  
                                        uint64_t temp_code = p_msg[OBS_RCON_FR_OFFSET];
                                        g_obsmsg.on_set_temp_rcon(temp_code,OBSMSG_RCONSIDE_RIGHT);
                                    }            
                                   break;
            case OBSMSG_RECIEVE_CHARGE:
                                    if(p_msg[OBS_RCON_FL_OFFSET] != 0)
                                    {
                                        uint64_t temp_code = p_msg[OBS_RCON_FL_OFFSET];
                                        g_obsmsg.on_set_temp_rcon(temp_code,OBSMSG_RCONSIDE_LEFT);
                                    }
                                    if(p_msg[OBS_RCON_FR_OFFSET] != 0)
                                    {  
                                        uint64_t temp_code = p_msg[OBS_RCON_FR_OFFSET];
                                        g_obsmsg.on_set_temp_rcon(temp_code,OBSMSG_RCONSIDE_RIGHT);                                        
                                    }									
            default :													
            break;
        
        }  				
        state = 1;
    }		
    else if(p_msg[1] == OBSMSG_DATA_CMD)     
    {
        if((p_msg[3] == 0xff) && (p_msg[4] == 0xff) && (p_msg[5] == 0xff))
        {
            g_obsmsg.ack = OBSMSG_DISABLE;
        }       
        else if((p_msg[3] == 0xEE) && (p_msg[4] == 0xEE) && (p_msg[5] == 0xEE)) 
        {
            g_obsmsg.ack = OBSMSG_ENABLE;	
        }       
        else if((p_msg[3] == 0xDD) && (p_msg[4] == 0xDD) && (p_msg[5] == 0xDD))
        {
            g_obsmsg.ack = OBSMSG_SLEEP;
        }        
        else if((p_msg[3] == 0xCC) && (p_msg[4] == 0xCC) && (p_msg[5] == 0xCC)) 
        {
            g_obsmsg.ack = OBSMSG_RESTART;
        }      
        else if(p_msg[3] == 0xBB) 
        {
            g_obsmsg.ack = OBSMSG_VERSION;
            g_obsmsg.version	= (p_msg[4] << 8)+ p_msg[5];
        }			
        else
        {
            g_obsmsg.ack = 0;	
        }
         
        
        state = 1;
    }
    return state;
}

void ObsMsg_Cmd(uint8_t cmd)
{
  	ObsMsg_LoadCmdData(g_obsmsg.tx_buffer,cmd);
	g_obsmsg.on_usart_send(15,(char *)g_obsmsg.tx_buffer);
}

void ObsMsg_LoadCmdData(volatile uint8_t *p_buffer,uint8_t cmd)
{
	uint8_t i = 0,sum = 0 ;
	uint8_t tx_idx = 0;
	p_buffer[tx_idx++] = 0XAA;
	p_buffer[tx_idx++] = 0x55;
	p_buffer[tx_idx++] = 0x0D;
    switch(cmd)
    {
        case OBSMSG_ENABLE:
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0xEE; 
                                        }
                    break ;
        case OBSMSG_DISABLE:
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0xFF; 
                                        }
                    break ;
        case OBSMSG_SLEEP:
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0xDD;
                                        }											
                    break ;		
        case OBSMSG_RESTART:
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0xCC;
                                        }											
                    break ;	
        case OBSMSG_VERSION:
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0xBB;
                                        }											
                    break ;										
        default :       
                                        for(i = 0; i < 3; i++)
                                        {
                                            p_buffer[tx_idx++] = 0x00; 
                                        }
                            break ;
    }

	for(i = 0; i < 8; i++)
	{
		p_buffer[tx_idx++] = 0x00; 

	}
	for(i=2; i < tx_idx; i++)
	{
		sum += p_buffer[i];
	}
	p_buffer[tx_idx] = sum;
}

uint8_t ObsMsg_IsAck(uint8_t ack)
{
    if(g_obsmsg.ack == ack)
    {
        g_obsmsg.ack = 0;
        return 1;
    }
    else 
    {
        return 0;
    }  
}

uint8_t ObsMsg_CheckAckFail(uint8_t cmd)
{
    uint8_t fail_count = 0,reval = 0;
    vTaskDelay(100/portTICK_RATE_MS); 

    if(ObsMsg_IsAck(cmd))
    {
        return 1;		
    }

    while(fail_count < 5)  
    {		
        fail_count++;
            
        if(ObsMsg_IsAck(cmd))
        {
            reval = 1;
            break;
        }	 
        ObsMsg_Cmd(cmd);	
        vTaskDelay(50/portTICK_RATE_MS);
    }

    if(!reval)
    {
        Usprintf("obsmsg may bad,restart\n");
        fail_count = 0;
        g_obsmsg.on_set_power_state(OFF);
        vTaskDelay(100/portTICK_RATE_MS);
        g_obsmsg.on_set_power_state(ON);
        while(fail_count < 5)  
        {		
            fail_count++;
                
            if(ObsMsg_IsAck(cmd))
            {
                reval = 1;
                break;
            }	 
            ObsMsg_Cmd(cmd);	
            vTaskDelay(50/portTICK_RATE_MS);
        }       
    }

    return reval;	
}

uint8_t ObsMsg_SetMode(uint8_t cmd)
{
    uint8_t reval = 0;    
    ObsMsg_Cmd(cmd);

    reval = ObsMsg_CheckAckFail(cmd);
    
    if(reval)
    {
        g_obsmsg.check_time = Timer_GetTime();
        g_obsmsg.cur_mode = cmd;
    }
    else
    {
        g_obsmsg.cur_mode = OBSMSG_IDLE;
    }
    Usprintf("obs cur mode:%d\n",g_obsmsg.cur_mode);
    return reval;	
}

uint8_t ObsMsg_IsIdle(void)
{
    if(g_obsmsg.cur_mode == OBSMSG_IDLE)
    {
        return 1;
    }
    return 0;
}






