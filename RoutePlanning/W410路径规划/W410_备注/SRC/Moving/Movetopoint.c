#include "Include.h"


TouringResult_t MoveToPoint(Point32_t target_cnt,Touring_t touring)
{
	TouringResult_t touring_result = TOURING_RESULT_NONE, arrived_flag = TOURING_RESULT_NONE;
	uint8_t temp_exception_state = 0;
	uint8_t charge_buffer = 0;
	BumperTrig_t bumper_trigger_status = BUMPER_TRIG_NO,bumper_buffer = BUMPER_TRIG_NO;
	ObsTrig_t obs_trigger_status = OBS_TRIG_NONE;
	CliffTrig_t cliff_trigger_status = CLIFF_TRIG_NO,cliff_buffer = CLIFF_TRIG_NO;
	Point16_t cur_cell,pre_cell = Map_GetRobotCell();
	uint8_t new_cell = 0;
	uint16_t new_cell_cnt = 0;
	uint32_t temp_rcon_status = 0;
	uint32_t check_rcon_time = 0;
    int32_t target_angle = Map_GetRobotToPointAngle(target_cnt);
	uint8_t  detect_charger = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();

    Usprintf("---------Move To Point Start-----------\n");
	if(touring == TOURING_ING)Movement_PidSetThinkNextPoint(TRUE);
	else Movement_PidSetThinkNextPoint(FALSE);
	if(touring == TOURING_SPOT)Movement_PidSetRobotSizeClean(FALSE);
	else Movement_PidSetRobotSizeClean(FALSE);//TRUE
	Map_SetCellRobotSizeClean(Map_GetRobotCell().x,Map_GetRobotCell().y);
	Moving_SetType(MOVING_MOVO2POINT);
	Wall_SetDynamicState(ENABLE);
	Rcon_ResetStatus();    
	Map_Filter();	
    Action_SetTargetPoint(target_cnt);	
    Action_ListClear();
    Action_ListAdd(MOVE_ACT_HEAD2COURCE,target_angle,HEAD2COURSE_SPEED,HEAD2COURSE_SPEED);
    Action_ListAdd(MOVE_ACT_MOVE2POINT,MAX_DISTANCE,MOVETOPOINT_SPEED,MOVETOPOINT_SPEED);
    Action_SetMove(MOVE_ACT_HANDLER);
	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		cur_cell = Map_GetRobotCell();
		new_cell = Map_IsReach_NewCell(cur_cell,&pre_cell)? 1: 0;
		if(new_cell)new_cell_cnt++;
		switch(Action_GetMove())
		{
			case MOVE_ACT_DECELERATE:	Action_WaitForDecelerateEnded();
			break;			
			case MOVE_ACT_HEAD2COURCE:  Action_WaitForHeadToCourseEnded();
										if(Cliff_ContinueTrig())
										{
											Usprintf("cliff trig continue\n");
											Error_SetCode(ERROR_CLIFF);
										}
										if(Action_GetHead2CourseStuck())
										{
											Usprintf("move to point ,stuck\n");
											temp_rcon_status = Rcon_GetStatus();
											if((temp_rcon_status&RCON_FL_HOME_L)||(temp_rcon_status&RCON_FR_HOME_R)||(temp_rcon_status&RCON_FR_HOME_L)||(temp_rcon_status&RCON_FL_HOME_R))
											{
												Usprintf("charge stuck event\n");
												arrived_flag = TOURING_RESULT_OBSTACLE;
												Action_ListBackToStop(HEAD2COURSE_STUCK_BACK_DISTANCE);
												charge_buffer = TRUE;
											}
											Action_SetHead2CourseStuck(FALSE);
										}																
			break;
			case MOVE_ACT_MOVE2POINT:   
									if(Action_GetArriveTarget())Action_ListDecelerateToStop();																																																																																																																																																																	
									if(touring == TOURING_SPOT)
									{
										if(Spot_GetDirOutin() == SPOT_DIR_OUT)
										{
											if(Spot_GetPointLaneIdx(Map_GetRobotCell()) >= SPOT_RANGE)
											{
												Spot_SetDirOutin(SPOT_DIR_IN);	
											}
										}																
									}																																
			break;
			case MOVE_ACT_BACK:      
									if(Action_WaitForMoveEnded())
									{
										if(bumper_buffer)
										{	
											Touring_UpdateMapBumper(bumper_buffer);
											bumper_buffer = BUMPER_TRIG_NO;
										}
										if(cliff_buffer)
										{
											Touring_UpdateMapCliff(cliff_buffer);
											cliff_buffer = CLIFF_TRIG_NO;
										}
										if(charge_buffer)
										{
											Touring_UpdateMapCharge();
											charge_buffer = FALSE;
										}	
										cliff_trigger_status = Cliff_GetTrigStatus();
										if(cliff_trigger_status)Error_SetCode(ERROR_CLIFF);		
										if(Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)Error_SetCode(ERROR_PICK_UP);																														
									}																				
			break;
			case MOVE_ACT_STATIC:   Gyro_Cal();	
									Heading_Reset_Integral(HEADING_INTEGRAL_MOVETOPOSITION);								
									if(Action_GetArriveTarget())
									{
										Action_ResetArriveTarget();
										touring_result = TOURING_RESULT_ARRIVED;																	
									}	
									else
									{
										if(arrived_flag)
										{																		
											touring_result = arrived_flag;
										}																	
									}																																	
			break;																
			case MOVE_ACT_HANDLER: 	Action_ListSwitch();
			break;
			default:break;
		}		
		
		if(Action_GetMove() == MOVE_ACT_MOVE2POINT)
		{
			bumper_trigger_status = Bumper_GetTrigStatus();
			if(bumper_trigger_status) 
			{						
				arrived_flag = TOURING_RESULT_OBSTACLE;								
				bumper_buffer = bumper_trigger_status;				
				Action_ListBackToStop(MOVETOPOINT_BUMPER_DISTANCE);
				Usprintf("Bumper Event\n");					
			}
			cliff_trigger_status = Cliff_GetTrigStatus();
			if(cliff_trigger_status)
			{
				if(Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)
				{
					Error_SetCode(ERROR_PICK_UP);
					Usprintf("Pick Up Event\n");
				}
				else
				{
					arrived_flag = TOURING_RESULT_OBSTACLE;					
					ActList_BackSideToStop(cliff_trigger_status);
					Touring_UpdateMapCliff(cliff_trigger_status);
					Usprintf("Cliff Event\n");
				}				
			}						
            obs_trigger_status = (ObsTrig_t)OBS_GetTrigStatus();
			OBS_IsTrigLimit(&obs_trigger_status);
            if(obs_trigger_status)
            {					
                Touring_UpdateMapObs(obs_trigger_status);
                Action_Static();
                arrived_flag = TOURING_RESULT_OBSTACLE;
                Usprintf("Obs Event\n");
            }	
            if(Map_BoundaryReach())
            {					
                Usprintf("Map Boundary Reach\n");								
                Action_Static();
                arrived_flag = TOURING_RESULT_BOUNDARY;
            }
            if(Charge_IsAdapterInput())
            {
                Usprintf("Adapter Is Input\n");
                if(touring == TOURING_FIND_CHARGER)
                {
                    Action_Static();
                    arrived_flag = TOURING_RESULT_ARRIVE_CHARGE;
                }
                else
                {
                    Action_ListBackToStop(STATION_BACK_DISTANCE);						
                }					
            }
			if(touring == TOURING_ING)
			{
				if(Gyro_IsNeedCalculate())
				{
					Usprintf("Gyro need to calculte\n");
					Action_Static();
					arrived_flag = TOURING_RESULT_CALCULATE_GYRO;				
				}
			}
            if((touring == TOURING_BACK_HOME)||(touring == TOURING_FIND_CHARGER))
            {
				if(Touring_IsBackhomeTimeout())
				{
					Mode_SetMode(MODE_USERINTERFACE);
					Usprintf("back home time out\n");
				}
                if(Station_GetStartState())
                {
                    if(Timer_IsTimeReach(&check_rcon_time,TIME_1S))
                    {
                        temp_rcon_status = Rcon_GetStatus();
                        
                        if((temp_rcon_status&RCON_FL_HOME_L)&&(temp_rcon_status&RCON_FL_HOME_R))
                        {
                            detect_charger = 1;
                        }
                        else if((temp_rcon_status&RCON_FR_HOME_L)&&(temp_rcon_status&RCON_FR_HOME_R))
                        {
                            detect_charger = 1;
                        }
                        else if((temp_rcon_status&RCON_FL_HOME_L)&&(temp_rcon_status&RCON_FR_HOME_R))
                        {
                            detect_charger = 1;
                        }												
                        Rcon_ResetStatus();
                        if(detect_charger)
                        {
                            Action_Static();
                            arrived_flag = TOURING_RESULT_GO_HOME;		
                            Usprintf("Find Station\n");								
                        }
                    }						
                }
            }												
		}

		if(touring == TOURING_ING)
		{
			if(Remote_IsRemoteEvent(REMOTE_SIGNAL_GO_HOME))
			{
				Usprintf("Remote Go Home\n");	
				Action_Static();
				arrived_flag = TOURING_RESULT_GO_HOME;
			}
		}
		
		temp_exception_state = Exception_GetState();
		if(temp_exception_state == EXCEPTION_ERROR_BUMPER)
		{
			temp_rcon_status = Rcon_GetStatus();
			if((temp_rcon_status&RCON_FL_HOME_L)||(temp_rcon_status&RCON_FR_HOME_R)||(temp_rcon_status&RCON_FR_HOME_L)||(temp_rcon_status&RCON_FL_HOME_R))
			{
				temp_exception_state = 0; 
				Exception_ResetState();
				Usprintf("Maybe Stuck On Station\n");
			}
		}
		if(temp_exception_state)
		{			
			Exception_ResetState();
			Exception_Handler(temp_exception_state);	
			Action_Static();
			arrived_flag = TOURING_RESULT_OBSTACLE;				
		}	

		if(Battery_IsLow())
		{	
			if(Error_GetCode() != ERROR_LOW_BATTERY)
			{
				Display_SetBatteryState(LED_MODE_STATIC,LED_STATE_RED);	
				Error_SetCode(ERROR_LOW_BATTERY);
			}						
		}
		
		if(Error_GetCode())
		{		
			if((Error_GetCode() == ERROR_CLEAN_TANK) || (Error_GetCode() == ERROR_DIRT_TANK) ||(Error_GetCode() == ERROR_LOW_BATTERY))
			{	
				if(Error_GetCode() == ERROR_CLEAN_TANK ||(Error_GetCode() == ERROR_DIRT_TANK))
				{
					if(g_robot.type == ROBOT_W400)
					{
						Display_SetWaterWarnState(LED_STATE_RED);	
					}
					else
					{
						Display_SetWaterWarnState(LED_STATE_GREEN);
					}					
				}
				if(touring == TOURING_ING)
				{
					Usprintf("Water Or Low Power\n");	
					Action_Static();
					arrived_flag = TOURING_RESULT_GO_HOME;
				}	
                else if(touring == TOURING_SPOT)
                {
                    Mode_SetMode(MODE_USERINTERFACE);						
                }                																		
			}
			else
			{
				Usprintf("Error\n");
				Mode_SetMode(MODE_USERINTERFACE);
			}								
		}	
	
		Event_Handler();

		if(touring_result != TOURING_RESULT_NONE)
		{
			break;
		}
		
		vTaskDelayUntil(&xLastWakeTime,10/portTICK_RATE_MS);
	}
	Usprintf("---------Move To Point End-----------\n");	
	return touring_result;
}




























