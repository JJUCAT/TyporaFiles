 #include "Include.h"

Remote_t g_remote;

void Remote_Init(void)
{
	memset(&g_remote,0,sizeof(g_remote));
}

void Remote_SetTempRemote(RemoteSignal_t remote)
{
	g_remote.temp = remote;
	g_remote.is_new = 1;
}

void Remote_ResetTempRemote(void)
{
	g_remote.temp = REMOTE_SIGNAL_NONE;
	g_remote.is_new = 0;
}

void Remote_SetTempRemoteWithCode(uint8_t code)
{ 
    RemoteCode_t temp_code = (RemoteCode_t)code;
	switch(temp_code)
	{
		case REMOTE_CODE_NAV:  			Remote_SetTempRemote(REMOTE_SIGNAL_NAV);
														break;
		case REMOTE_CODE_FORWARD:		Remote_SetTempRemote(REMOTE_SIGNAL_FORWARD);
														break;
		case REMOTE_CODE_LEFT:       	Remote_SetTempRemote(REMOTE_SIGNAL_LEFT);
														break;
		case REMOTE_CODE_BACK:  		Remote_SetTempRemote(REMOTE_SIGNAL_BACK);
														break;
		case REMOTE_CODE_RIGHT:			Remote_SetTempRemote(REMOTE_SIGNAL_RIGHT);
														break;																	
		case REMOTE_CODE_WATER_SPOT:			
										if(g_robot.type == ROBOT_W410)
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_WATER);
										}
										else
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_SPOT);
										}										
										break;
		case REMOTE_CODE_SPEAKER_WALL:	
										if(g_robot.type == ROBOT_W410)
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_SPEAKER);
										}
										else
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_WALL);
										}
														break;
		case REMOTE_CODE_SPOT_HOME:		
										if(g_robot.type == ROBOT_W410)
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_SPOT);
										}
										else
										{
											Remote_SetTempRemote(REMOTE_SIGNAL_GO_HOME);
										}
														break;
		case REMOTE_CODE_AREA:			Remote_SetTempRemote(REMOTE_SIGNAL_AREA);									
														break;		
		case REMOTE_CODE_MAINBRUSH:		Remote_SetTempRemote(REMOTE_SIGNAL_MAINBRUSH);									
														break;
		case REMOTE_CODE_CLEAN_CNT:		Remote_SetTempRemote(REMOTE_SIGNAL_CLEAN_CNT);									
														break;																																						
		default:						
														break;
	}
}

void Remote_FilterProcess(void)
{
	uint8_t remote_store = 1;
	uint32_t dif_t = 0;

	if(g_remote.is_new)
	{
		g_remote.is_new = 0;
		dif_t = Timer_Get_DiffNow(g_remote.recieve_time);	

		if(g_remote.temp == g_remote.pre_temp)
		{			
			if(dif_t <= TIME_500MS)
			{	
				remote_store = 0;
			}
			else
			{
				g_remote.recieve_time = Timer_GetTime();	
			}
		}
		else
		{
			g_remote.recieve_time = Timer_GetTime();	
		}
		
		g_remote.pre_temp = g_remote.temp;
		#ifdef REMOTE_ENABLE
		if(remote_store)
		{
			g_remote.real |= g_remote.temp;	 
			Usprintf("real remote:%x\n",g_remote.real);	
		}
		#endif
	}
}


uint8_t Remote_IsRemoteEvent(RemoteSignal_t remote)
{
	if(g_remote.real & remote)
	{
		g_remote.real &= ~remote;
		return 1;
	}
	else
	{
		return 0;	
	}	
}

uint8_t Remote_IsRemoteExist(RemoteSignal_t remote)
{
	if(g_remote.real & remote)
	{
		return 1;
	}
	else
	{
		return 0;	
	}	
}

void Remote_Reset(void)
{
	g_remote.real = REMOTE_SIGNAL_NONE;
}











