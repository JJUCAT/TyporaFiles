 #include "Include.h"

Exception_t g_exception;

void Exception_Init(void)
{
	memset(&g_exception,0,sizeof(g_exception));
}

void Exception_Cycle(void)
{
    Exception_MotorsCheckCycle();
    Exception_SensorsCheckCycle(); 
}

uint16_t Exception_GetState(void)
{
	return g_exception.state;
}

void Exception_ResetState(void)
{
	g_exception.state = 0;
}

void Exception_MotorsSetCheckState(FunctionalState state)
{
	g_exception.motors_check_state = state;	
}

void Exception_MotorsCheckCycle(void)
{
	if(g_exception.motors_check_state == DISABLE)return;

	if(Wheel_CheckLeftCurrent()) g_exception.state  =  EXCEPTION_ERROR_L_WHEEL;

	if(Wheel_CheckRightCurrent())g_exception.state  =  EXCEPTION_ERROR_R_WHEEL;

	if(g_wheel.left_slow > WHEEL_SLOW_LIMIT/2)  g_exception.state  =  EXCEPTION_ERROR_L_WHEEL;

	if(g_wheel.right_slow > WHEEL_SLOW_LIMIT/2) g_exception.state  =  EXCEPTION_ERROR_R_WHEEL;

	if(MainBrush_CheckCurrent()) g_exception.state  =  EXCEPTION_ERROR_MAINBRUSH;

	if(Vacuum_CheckCurrent())    g_exception.state  =  EXCEPTION_ERROR_VACUUM;

	if(Vacuum_IsFail())			 g_exception.state  =  EXCEPTION_ERROR_VACUUM;	
}

void Exception_SensorsSetCheckState(FunctionalState state)
{
	g_exception.sensors_check_state = state;	
}

void Exception_SensorsCheckCycle(void)
{
	if(g_exception.sensors_check_state == DISABLE)return;

	static uint32_t time_cycle_watertank = 0;
	static uint32_t time_cycle_mobility = 0;

	if(Timer_IsTimeReach(&time_cycle_mobility,TIME_200MS))
	{
		Exception_MobilityDetection();

		if(Mobility_IsError())g_exception.state = EXCEPTION_ERROR_MOBILITY;
	}
	
	if(Bumper_IsStuck())g_exception.state = EXCEPTION_ERROR_BUMPER;	
		
	if(Timer_IsTimeReach(&time_cycle_watertank,TIME_1S))
	{
		WaterTank_CheckProcess(g_pump.state);
		if(!WaterTank_IsInstall())
		{
			if(Error_GetCode() == ERROR_NONE)
			{
				Error_SetCode(ERROR_TANK);
				Usprintf("Error Water Tank\n"); 
			}	   
		}	
		if(g_watertank.dirt_error_cnt >= WATERTANK_ERROR_TIME_LIMIT)
		{
			if(Error_GetCode() == ERROR_NONE)
			{
				Error_SetCode(ERROR_DIRT_TANK);
				Display_SetWaterWarnState(LED_STATE_GREEN);	
				Usprintf("Error Dirty Water\n");
			}			
		}
		if(g_watertank.clean_error_cnt >= WATERTANK_ERROR_TIME_LIMIT)
		{
			if(Error_GetCode() == ERROR_NONE)
			{
				Error_SetCode(ERROR_CLEAN_TANK);
				Display_SetWaterWarnState(LED_STATE_GREEN);	
				Usprintf("Error Clean Water\n");
			}			
		}
	}
}

void Exception_MobilityDetection(void)
{
	uint32_t mobility_step = 0;
	static int32_t previous_move_step = 0;
	int32_t move_step_left_buffer = 0;
	uint8_t check_mobility = 0;

	
	if((Mobility_GetCheckState() == DISABLE) || (Wheel_GetDir() != WHEEL_DIR_FORWARD) || (Math_Diff_int(Wheel_GetLeftSpeed(),Wheel_GetRightSpeed()) > 8))
	{
		previous_move_step = Wheel_GetLeftStep();
		Mobility_ResetStep();		
		return;
	}

	move_step_left_buffer = Wheel_GetLeftStep();
	if(move_step_left_buffer > previous_move_step)
	{
		if(move_step_left_buffer - previous_move_step > 10000)
		{
			previous_move_step = move_step_left_buffer;
			mobility_step = Mobility_GetStep();
			Mobility_ResetStep();
			check_mobility = 1;
		}
		if(check_mobility)
		{
			if(mobility_step < 5)
			{
				Mobility_SetError();
				g_mobility.error_cnt++;
				if(g_mobility.error_cnt >= 3)
				{
					g_mobility.error_cnt = 0;	
					Error_SetCode(ERROR_MOBILITY);
				}
			}
			else
			{
				g_mobility.error_cnt = 0;
			}			
		}
	}
}

void Exception_Handler(uint16_t state)
{	
	uint16_t error_cnt = 0;
	int16_t  temp_current = 0;
	uint8_t  check_brush_flag = 0;
	uint16_t check_brush_time_cnt = 0;
	uint16_t check_vac_time_cnt = 0;
	uint8_t  finish_flag = 0;
	uint8_t  bumper_first_back = 0;
	uint8_t  bumper_cliff_flag = 0;
	uint8_t  bumper_trigger_status = 0;
	uint8_t  cliff_trigger_status = 0; 
    int32_t  mainbrush_voltage = 0;
    int32_t  vac_speed = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();	
	Moving_SetType(MOVING_EXCEPTION);
	Exception_MotorsSetCheckState(DISABLE);	
    Exception_SensorsSetCheckState(DISABLE);	
	Action_ListClear();
	Usprintf("--------Exception_Handler:%d---------\n",state);
    switch(state)
    {
        case	EXCEPTION_ERROR_L_WHEEL:  
                                                Usprintf("EXCEPTION_ERROR_L_WHEEL\n");
                                                {	                                               
                                                    Wheel_ResetSlowCnt();                                           		
                                                    if(Wheel_GetLeftDir() == WHEEL_DIR_FORWARD)
                                                    {			
                                                        Action_ListAdd(MOVE_ACT_TURN_LEFT,1600,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }
                                                    else
                                                    {
                                                        Action_ListAdd(MOVE_ACT_TURN_RIGHT,1600,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }	
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
                                                    Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
                                                    Action_SetMove(MOVE_ACT_HANDLER);			
                                                }				
        break;
        case	EXCEPTION_ERROR_R_WHEEL: 
                                                Usprintf("EXCEPTION_ERROR_R_WHEEL\n");
                                                {
                                                    Wheel_ResetSlowCnt();			
                                                    if(Wheel_GetRightDir() == WHEEL_DIR_FORWARD)
                                                    {
                                                        Action_ListAdd(MOVE_ACT_TURN_RIGHT,1600,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }
                                                    else
                                                    {
                                                        Action_ListAdd(MOVE_ACT_TURN_LEFT,1600,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }	
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
                                                    Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
                                                    Action_SetMove(MOVE_ACT_HANDLER);			
                                                }
        break;
        case	EXCEPTION_ERROR_BUMPER:
                                                Usprintf("EXCEPTION_ERROR_BUMPER\n");			
                                                {
                                                    bumper_first_back = 1;
                                                    bumper_trigger_status = Bumper_GetTrigStatus();
                                                    Action_ListAdd(MOVE_ACT_BACK,400,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);                                                   
                                                    if(bumper_trigger_status & BUMPER_TRIG_LEFT)
                                                    {
                                                        Action_ListAdd(MOVE_ACT_TURN_RIGHT,1000,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }
                                                    else
                                                    {
                                                        Action_ListAdd(MOVE_ACT_TURN_LEFT,1000,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    }
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);					
                                                    Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
                                                    Action_SetMove(MOVE_ACT_HANDLER);			
                                                }
        break;	
        case	EXCEPTION_ERROR_MAINBRUSH:
                                                Usprintf("EXCEPTION_ERROR_MAINBRUSH\n");
                                                {
                                                    mainbrush_voltage = MainBrush_GetVoltage();
                                                    MainBrush_SetVoltage(0);
                                                    Action_ListAdd(MOVE_ACT_BACK,1000,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
                                                    Action_ListAdd(MOVE_ACT_TURN_LEFT,1600,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
                                                    Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
                                                    Action_SetMove(MOVE_ACT_HANDLER);		
                                                }
        break	;
        case	EXCEPTION_ERROR_VACUUM:
                                                Usprintf("EXCEPTION_ERROR_VACUUM\n");			
                                                {
                                                    vac_speed = Vacuum_GetSpeed();
                                                    Vacuum_SetSpeed(0);
                                                    Vacuum_ResetFail();	
                                                    Action_Static();			
                                                }
        break;	
        case	EXCEPTION_ERROR_MOBILITY:
                                                Usprintf("EXCEPTION_ERROR_MOBILITY\n");			
                                                {
                                                    Mobility_ResetError();
                                                    Mobility_TrigCntIncrease();
                                                    Action_ListAdd(MOVE_ACT_BACK,200,EXCEPTION_SPEED,EXCEPTION_SPEED);
                                                    Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
                                                    Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
                                                    Action_SetMove(MOVE_ACT_HANDLER);                                                    
                                                }
        break;	
        default:break;		
    }	
	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_BACK:                         Action_WaitForMoveEnded();
			                                            break;		
			case MOVE_ACT_TURN_LEFT:					Action_WaitForMoveEnded();
														break;		
			case MOVE_ACT_TURN_RIGHT:                   Action_WaitForMoveEnded();
														break;				
			case MOVE_ACT_DECELERATE:                   Action_WaitForDecelerateEnded();
													    break;	
			case MOVE_ACT_FORWARD:														
														if((Wheel_GetLeftStep() >= Wheel_GetLeftTargetStep()) || (Wheel_GetRightStep() >= Wheel_GetRightTargetStep()))
                          	                            {
															finish_flag = 1;
														}
														bumper_trigger_status = Bumper_GetTrigStatus();
														if(bumper_trigger_status)
														{
															bumper_cliff_flag = 1;
															if(bumper_trigger_status & BUMPER_TRIG_LEFT)
															{
																Action_ListClear();
																Action_ListAdd(MOVE_ACT_BACK,500,BACK_SPEED,BACK_SPEED);
																Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);	
																Action_ListAdd(MOVE_ACT_STATIC,0,0,0);																	
																Action_SetMove(MOVE_ACT_HANDLER);																			
															}
															else if(bumper_trigger_status & BUMPER_TRIG_RIGHT)
															{
																Action_ListClear();
																Action_ListAdd(MOVE_ACT_BACK,500,BACK_SPEED,BACK_SPEED);
																Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);																		
																Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
																Action_SetMove(MOVE_ACT_HANDLER);																							
															}																																																																																																												
														}
														cliff_trigger_status = Cliff_GetDetectiontProcess_Result();
														if(cliff_trigger_status)
														{
															bumper_cliff_flag = 1;
															Action_ListClear();
															Action_ListAdd(MOVE_ACT_BACK,500,BACK_SPEED,BACK_SPEED);
															Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);																		
															Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
															Action_SetMove(MOVE_ACT_HANDLER);	
														}
														break;                                                        		
			case MOVE_ACT_STATIC: 
														if(bumper_cliff_flag == 1)
														{
															bumper_cliff_flag = 0;
															finish_flag = 1;	
														}			
														if(state & EXCEPTION_ERROR_L_WHEEL || state & EXCEPTION_ERROR_R_WHEEL)
														{
															finish_flag = 1;													
														}
														else if(state & EXCEPTION_ERROR_BUMPER)
														{		
															if(bumper_first_back)
															{	
																bumper_first_back = 0;
																bumper_trigger_status = Bumper_GetTrigStatus();
																if(bumper_trigger_status)
																{
																	Action_ListAdd(MOVE_ACT_BACK,500,BACK_SPEED,BACK_SPEED);
																	Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);																	
																	if(bumper_trigger_status & BUMPER_TRIG_LEFT)
																	{
																		Action_ListAdd(MOVE_ACT_TURN_RIGHT,1000,EXCEPTION_SPEED,EXCEPTION_SPEED);
																	}
																	else
																	{
																		Action_ListAdd(MOVE_ACT_TURN_LEFT,1000,EXCEPTION_SPEED,EXCEPTION_SPEED);
																	}																
																	Action_ListAdd(MOVE_ACT_STATIC,0,0,0);
																	Action_SetMove(MOVE_ACT_HANDLER);	
																}
																else
																{
																	finish_flag = 1;
																}
															}														
															else
															{
																bumper_trigger_status = Bumper_GetTrigStatus();
																if(bumper_trigger_status)
																{
																	Error_SetCode(ERROR_BUMPER);																	
																}
																finish_flag = 1;
															}																													
														}
														else if (state & EXCEPTION_ERROR_MAINBRUSH)
														{
														    MainBrush_SetVoltage(MAINBRUSH_VOLTAGE);	
															check_brush_flag = 1;														
														}															
														else if(state == EXCEPTION_ERROR_MOBILITY)
														{
															finish_flag = 1;
														}
													  break;

			case MOVE_ACT_HANDLER:                    Action_ListSwitch();
													  break;
			
			default:break;
		
		}
	 	if(state & EXCEPTION_ERROR_L_WHEEL || state & EXCEPTION_ERROR_R_WHEEL)
		{
			if(Action_GetMove() == MOVE_ACT_TURN_LEFT || Action_GetMove() == MOVE_ACT_TURN_RIGHT)
			{
				if(state & EXCEPTION_ERROR_L_WHEEL)
				{
					temp_current = g_wheel.left_current;
				}
				else if (state & EXCEPTION_ERROR_R_WHEEL)
				{
					temp_current = g_wheel.right_current;
				}
				if(temp_current > WHEEL_STALL_LIMIT  || temp_current < WHEEL_STALL_NOLOAD) 
				{
					error_cnt++;	
					if(error_cnt > 75)
					{
						error_cnt = 0;
						finish_flag = 1;
						if(state & EXCEPTION_ERROR_L_WHEEL)
						{
							Error_SetCode(ERROR_LEFT_WHEEL);
						}
						else
						{
							Error_SetCode(ERROR_RIGHT_WHEEL);
						}
					}
				}	
				else
				{
					error_cnt = 0;				
				}
			}		
		}
		if(state & EXCEPTION_ERROR_MAINBRUSH)
        {
            if(check_brush_flag)
            {			
                check_brush_time_cnt++;
                if(check_brush_time_cnt > 150)
                {
                    check_brush_flag = 0;
                    check_brush_time_cnt = 0;                   
                    finish_flag = 1;
                }
                temp_current = MainBrush_GetCurrent();
                if(temp_current > MAINBRUSH_STALL_LIMIT  || temp_current < MAINBRUSH_STALL_NOLOAD)
                {
                    error_cnt++;	
                    if(error_cnt > 50)
                    {
                        error_cnt = 0;
                        Error_SetCode(ERROR_MAINBRUSH);
                        finish_flag = 1;
                    }
                }	
                else
                {
                    error_cnt = 0;				
                }						
            }
        }
		if(state & EXCEPTION_ERROR_VACUUM)
		{
			check_vac_time_cnt++;
			if(check_vac_time_cnt > 150)
			{
                check_vac_time_cnt = 0;
                finish_flag = 1;
			}
			temp_current = g_vacuum.current;
			if(temp_current > VACUUM_STALL_LIMIT  || temp_current < VACUUM_STALL_NOLOAD)
			{
				error_cnt++;	
				if(error_cnt > 75)
				{
					finish_flag = 1;
					error_cnt = 0;
					if(temp_current > VACUUM_STALL_LIMIT)
					{  
						Error_SetCode(ERROR_VAC);
					}
					if(temp_current < VACUUM_STALL_NOLOAD)
					{
						Error_SetCode(ERROR_VAC);
					}
				}
			}	
			else
			{
				error_cnt = 0;				
			}		   		
		}
			
		Event_Handler();

		if(finish_flag)
		{
			finish_flag = 0;
			break;
		}
				
		vTaskDelayUntil(&xLastWakeTime,20 / portTICK_RATE_MS);
	}	
	if(Error_GetCode() == ERROR_NONE)
	{
        Exception_MotorsSetCheckState(ENABLE);	
        Exception_SensorsSetCheckState(ENABLE);        
        MainBrush_SetVoltage(mainbrush_voltage);
        if(!Vacuum_GetSpeed())Vacuum_SetSpeed(vac_speed);
	}
}











