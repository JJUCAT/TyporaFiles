 #include "Include.h"

TouringResult_t Docking(void)
{
	uint32_t temp_rcon;
	uint32_t temp_docking_time,round_stop_time,docking_start_time;
	uint8_t bumper_trigger_status = 0,cliff_trigger_status = 0;
	uint8_t round_stop_flag = 0;
	uint8_t no_rcon_cnt = 0;
	TouringResult_t retval = TOURING_RESULT_NONE,arrived_flag = TOURING_RESULT_NONE;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	Usprintf("---------WallFollow Start-----------\n");
	Moving_SetType(MOVING_DOCKING);
	Rcon_ResetStatus();
	Action_ListClear();
	Action_ListAdd(MOVE_ACT_TURN_ROUND, 7200, STATION_TURN_SPEED, STATION_TURN_SPEED);	
	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);									
	Action_SetMove(MOVE_ACT_HANDLER);	
	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		switch(Action_GetMove())
		{	
			case MOVE_ACT_DOCKING:
			if(Timer_IsTimeReach(&temp_docking_time,TIME_500MS))
			{
				temp_rcon = Rcon_GetStatus();										
				if((temp_rcon&RCON_FL_HOME_L) && (temp_rcon&RCON_FR_HOME_R))
				{
					Wheel_SetTargetSpeed(SLOW_SPEED,SLOW_SPEED);
					no_rcon_cnt = 0;	
				}
				else if((!(temp_rcon&RCON_FL_HOME_L))&& temp_rcon&RCON_FR_HOME_R)
				{
					Wheel_SetTargetSpeed(SLOW_SPEED,DOCKING_SPEED);
					no_rcon_cnt = 0;
				}										
				else if(temp_rcon&RCON_FL_HOME_L && (!(temp_rcon&RCON_FR_HOME_R)))
				{
					Wheel_SetTargetSpeed(DOCKING_SPEED,SLOW_SPEED);
					no_rcon_cnt = 0;
				}										
				else if((temp_rcon&RCON_FR_HOME_L) && (!(temp_rcon&RCON_FR_HOME_R)))
				{
					Action_ListClear();										
					Action_ListAdd(MOVE_ACT_TURN_RIGHT, 1500, TURN_SPEED, TURN_SPEED);
					Action_ListAdd(MOVE_ACT_DOCKING, MAX_DISTANCE, SLOW_SPEED, SLOW_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
					no_rcon_cnt = 0;
				}
				else if((temp_rcon&RCON_FL_HOME_R) && (!(temp_rcon&RCON_FL_HOME_L)))
				{
					Action_ListClear();										
					Action_ListAdd(MOVE_ACT_TURN_LEFT, 1500, TURN_SPEED, TURN_SPEED);
					Action_ListAdd(MOVE_ACT_DOCKING, MAX_DISTANCE, SLOW_SPEED, SLOW_SPEED);
					Action_SetMove(MOVE_ACT_HANDLER);
					no_rcon_cnt = 0;
				}
				else
				{
					no_rcon_cnt++;
					if(no_rcon_cnt >= 4)
					{
						no_rcon_cnt = 0;
						arrived_flag = TOURING_RESULT_NO_CHARGE;
						Rcon_ResetStatus();
						Action_ListClear();
						Action_ListAdd(MOVE_ACT_STATIC,0,0,0);									
						Action_SetMove(MOVE_ACT_HANDLER);																				
					}
					else
					{
						Wheel_SetTargetSpeed(SLOW_SPEED,SLOW_SPEED);
					}																																																				
				}
																																	
				Rcon_ResetStatus();
			}
			if(Timer_Get_DiffNow(docking_start_time) > TIME_2MIN)
			{
				docking_start_time = Timer_GetTime();
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_BACK,STATION_BACK_DISTANCE,BACK_SPEED,BACK_SPEED);	
				Action_ListAdd(MOVE_ACT_STATIC,0,0,0);	
				Action_SetMove(MOVE_ACT_HANDLER);																
				arrived_flag = TOURING_RESULT_NO_CHARGE;
			}																
			break;
			case MOVE_ACT_TURN_ROUND:
			if(Action_WaitForMoveEnded())
			{
				arrived_flag = TOURING_RESULT_NO_CHARGE;																	
			}	
			if(!round_stop_flag)
			{
				temp_rcon = Rcon_GetStatus();	
				if(((temp_rcon&RCON_FL_HOME_L) && (temp_rcon&RCON_FR_HOME_R))||((temp_rcon&RCON_FL_HOME_L) && (temp_rcon&RCON_FR_HOME_L))||((temp_rcon&RCON_FL_HOME_R) && (temp_rcon&RCON_FR_HOME_R)))
				{
					round_stop_flag = 1;
					round_stop_time = Timer_GetTime();	
					Wheel_SetTargetSpeed(0,0);
					Rcon_ResetStatus();	
				}																	
			}
			else
			{
				if(Timer_Get_DiffNow(round_stop_time) >= TIME_2S)
				{
					temp_rcon = Rcon_GetStatus();
					if(((temp_rcon & RCON_FL_HOME_L)&&(temp_rcon & RCON_FR_HOME_L))||((temp_rcon & RCON_FL_HOME_R)&&(temp_rcon & RCON_FR_HOME_R))||((temp_rcon & RCON_FL_HOME_L)&&(temp_rcon & RCON_FR_HOME_R)))
					{
						Action_ListClear();
						Action_ListAdd(MOVE_ACT_DOCKING, MAX_DISTANCE, SLOW_SPEED, SLOW_SPEED);
						Action_SetMove(MOVE_ACT_HANDLER);	
						docking_start_time = Timer_GetTime();
					}
					else
					{
						round_stop_flag = 0;
						Wheel_SetTargetSpeed(SLOW_SPEED,SLOW_SPEED);	
					}
				}																	
			}																
			break;
			case MOVE_ACT_FORWARD:
			Action_WaitForMoveEnded();
			break;								
			case MOVE_ACT_DECELERATE:	
			Action_WaitForDecelerateEnded();													
			break;						
			case MOVE_ACT_TURN_RIGHT:
			Action_WaitForMoveEnded();
			break;
			case MOVE_ACT_TURN_LEFT:  
			Action_WaitForMoveEnded();												
			break;
			case MOVE_ACT_BACK:
			Action_WaitForMoveEnded();   	
			break;
			case MOVE_ACT_HANDLER: 								
			Action_ListSwitch();
			break;
			case MOVE_ACT_STATIC:  		
			Action_ListSwitch();	
			if(arrived_flag)
			{
				retval = arrived_flag;
			}
			break;		
			default:
			Action_SetMove(MOVE_ACT_STATIC);
			break;
		}
		if(Action_GetMove() == MOVE_ACT_DOCKING)
		{
			bumper_trigger_status = Bumper_GetTrigStatus();			
			if(bumper_trigger_status)
			{
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_BACK,300,BACK_SPEED,BACK_SPEED);
				if(bumper_trigger_status&BUMPER_TRIG_LEFT)	
				{
					Action_ListAdd(MOVE_ACT_TURN_RIGHT,200,TURN_SPEED,TURN_SPEED);
				}
				else
				{
					Action_ListAdd(MOVE_ACT_TURN_LEFT,200,TURN_SPEED,TURN_SPEED);
				}
				Action_ListAdd(MOVE_ACT_FORWARD,100,FORWARD_SPEED,FORWARD_SPEED);
				Action_ListAdd(MOVE_ACT_DOCKING,100,DOCKING_SPEED,DOCKING_SPEED);
				Action_SetMove(MOVE_ACT_HANDLER);
			}
			cliff_trigger_status = Cliff_GetDetectiontProcess_Result();
			if(cliff_trigger_status)
			{
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_BACK,300,BACK_SPEED,BACK_SPEED);
				if(cliff_trigger_status&CLIFF_TRIG_LEFT)	
				{
					Action_ListAdd(MOVE_ACT_TURN_RIGHT,200,TURN_SPEED,TURN_SPEED);
				}
				else
				{
					Action_ListAdd(MOVE_ACT_TURN_LEFT,200,TURN_SPEED,TURN_SPEED);
				}
				Action_ListAdd(MOVE_ACT_FORWARD,100,FORWARD_SPEED,FORWARD_SPEED);
				Action_ListAdd(MOVE_ACT_DOCKING,100,DOCKING_SPEED,DOCKING_SPEED);
				Action_SetMove(MOVE_ACT_HANDLER);	
			}					
		}
		if(!arrived_flag)
		{
			if(Charge_IsAdapterInput())
			{
				arrived_flag = TOURING_RESULT_ARRIVE_CHARGE;
				Action_ListClear();
				Action_ListAdd(MOVE_ACT_FORWARD,500,FORWARD_SPEED,FORWARD_SPEED);
				Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
				Action_SetMove(MOVE_ACT_HANDLER);
			}
		}
		Event_Handler();

		if(retval!=TOURING_RESULT_NONE)
		{
			break;
		}		
		vTaskDelayUntil(&xLastWakeTime,10/portTICK_RATE_MS);
	}
	return retval;
}

























