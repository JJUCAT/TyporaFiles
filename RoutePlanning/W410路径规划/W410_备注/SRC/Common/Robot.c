#include "Include.h"
Robot_t g_robot;

void Robot_StructInit(void)
{
	Power_Init();
	Action_Init();
	Align_Init();
	Battery_Init();
	Bumper_Init();
	Button_Init();
	Charge_Init();
	CleanCycleCount_Init();
	Cliff_Init();
	CodeShow_Init();
	Display_Init();
	Hc4051_Init();
	Exception_Init();
	Gyro_Init();
	Heading_Init();
	Line_Init();
	MainBrush_Init();
	Map_Init();
	Mobility_Init();
	Movement_Init();
	OBS_Init();
	ObsMsg_Init();
	PathList_Init();
	PathPoint_Init();
	Pump_Init();
	Rcon_Init();
	Remote_Init();
	RealTimeMonitor_Init();
	SensorDriver_Init();
	Sleep_Init();
	Speaker_Init();
	Spot_Init();
	Station_Init();
	System_Init();
	TargetList_Init();
	Test_Init();
	Vacuum_Init();
	Wall_Init();
	WaterTank_Init();
	Wheel_Init();
	Wifi_Init();
	Mode_Init();	
}

RobotType_t Robot_GetType(void)
{
	if(ObsMsg_SetMode(OBSMSG_VERSION))
	{
		Usprintf("OBS disable ok,robot type is w400\n");
		return ROBOT_W400;
		
	}
	else
	{
		Usprintf("OBS disable fail,robot type is w410\n");
		return ROBOT_W410;				
	}	
}

void Robot_Initialize(void)
{
	Power_StartUp();
	Power_EnableAll();	
	SensorDriver_Start();
	Wifi_PowerConfig();
	Detection_BaseLinesInit();	
}

void Robot_MotorsSetState(FunctionalState state)
{
	if(state == ENABLE)
	{
		MainBrush_SetVoltageControl(ENABLE);
		MainBrush_SetVoltage(g_mainbrush.adjust_voltage);
		Pump_SetState(ENABLE);
		Pump_SetVoltageControl(ENABLE);
		Pump_SetVoltage(PUMP_VOLTAGE);
		Vacuum_SetSpeed(VAC_SPEED_NORMAL);	
		Exception_MotorsSetCheckState(ENABLE);
		Wheel_SetTuneState(ENABLE);
		if(Mode_GetMode() == MODE_SPOT)
		{
			MainBrush_SetVoltage(MAINBRUSH_SPOT_VOLTAGE);
			Vacuum_SetSpeed(VAC_SPEED_MAX);	
		}
	}
	else
	{
		MainBrush_SetVoltageControl(DISABLE);
		MainBrush_SetVoltage(0);
		MainBrush_SetPWM(0);
		Pump_SetState(DISABLE);
		Pump_SetVoltageControl(DISABLE);
		Pump_SetVoltage(0);
		g_pump.set_state(OFF);
		Vacuum_SetSpeed(0);	
		Exception_MotorsSetCheckState(DISABLE);
		Exception_ResetState();
		Pump_SetStartControl(DISABLE);
		Action_Static();
		Wheel_SetTuneState(DISABLE);
	}
}

void Robot_SensorsSetState(FunctionalState state)
{
	if(state == ENABLE)
	{
		if(g_robot.type == ROBOT_W400)
		{
			ObsMsg_SetMode(OBSMSG_ENABLE);
			if(ObsMsg_IsIdle())Error_SetCode(ERROR_OBS);
		}
		else
		{
			g_obs.state = ENABLE;
			g_cliff.state = ENABLE;		
		}	
		g_wall.state = ENABLE;
		g_watertank.state = ENABLE;
		Exception_SensorsSetCheckState(ENABLE);
		WaterTank_SetCheckState(ENABLE);
	}
	else
	{
		if(g_robot.type == ROBOT_W400)
		{
			ObsMsg_SetMode(OBSMSG_DISABLE);
			if(ObsMsg_IsIdle())Error_SetCode(ERROR_OBS);
		}
		else
		{
			g_obs.state = DISABLE;
		}
		g_cliff.state = DISABLE;
		g_wall.state = DISABLE;
		g_watertank.state = DISABLE;		
		Rcon_ResetStatus();
		Remote_Reset();	
		Exception_SensorsSetCheckState(DISABLE);
		WaterTank_SetCheckState(DISABLE);
		OBS_SetDynamicState(DISABLE);
		Wall_SetDynamicState(DISABLE);
		Gyro_Cmd(DISABLE);
		WaterTank_ResetErrorCnt();
	}
}

void Robot_ReportError(Error_t error_val)
{
	switch(error_val)
	{
		case ERROR_LEFT_WHEEL: 			Speaker_Voice(SPK_CHECK_LEFT_WHEEL);
																		break;
		case ERROR_RIGHT_WHEEL: 		Speaker_Voice(SPK_CHECK_RIGHT_WHEEL);
																		break;
		case ERROR_PICK_UP: 			Speaker_Voice(SPK_PICK_UP);
																		break;
		case ERROR_BUMPER: 				Speaker_Voice(SPK_CHECK_BUMPER);																
																		break;
		case ERROR_MAINBRUSH: 			Speaker_Voice(SPK_CHECK_BRUSH);
																		break;
		case ERROR_STUCK: 				Speaker_Voice(SPK_STUCK);			
																		break;
		case ERROR_LOW_BATTERY: 		Speaker_Voice(SPK_LOW_POWER);
																		break;
		case ERROR_DUSTBIN: 			Speaker_Voice(SPK_DON);
																		break;
		case ERROR_VAC: 				Speaker_Voice(SPK_DON);
																		break;
		case ERROR_DIRT_TANK: 			Speaker_Voice(SPK_CHECK_DIRTY_TANK);
																		break;
		case ERROR_CLEAN_TANK: 			Speaker_Voice(SPK_CHECK_CLEAN_TANK);
																		break;
		case ERROR_TANK: 				Speaker_Voice(SPK_NO_WATER_TANK);
																		break;
		case ERROR_CLIFF:           	Speaker_Voice(SPK_CHECK_CLIFF);
																		break;																	
		case ERROR_PUMP:            	Speaker_Voice(SPK_DON);																	
																		break;
		case ERROR_MOBILITY:      		Speaker_Voice(SPK_CHECK_MOBILITY);  
																    	break;			
		case ERROR_GYRO:      			Speaker_Voice(SPK_DON);
																    	break;	
		case ERROR_OBS:					Speaker_Voice(SPK_DON);
																		break;																																				
		default:Speaker_Voice(SPK_DY);break;
	}
}

void Robot_ModeDisplay(void)
{
	if(g_robot.type != ROBOT_W400)return;
	if((Mode_GetMode() == MODE_NAVIGATION)||(Mode_GetMode() == MODE_WALL)||(Mode_GetMode() == MODE_SPOT)||(Mode_GetMode() == MODE_AREA))
	{
		g_clean_mode.selection = Mode_GetMode();
	}
	Display_SetModeLed(g_clean_mode.selection);		
}

void Robot_Remote2Act(void)
{
	uint8_t is_wifi_remote = 0;
	MoveAct_t act = MOVE_ACT_NONE;

	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_RIGHT))
	{	
		act = MOVE_ACT_TURN_RIGHT;																
	}
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_RIGHT))
	{	
		act = MOVE_ACT_TURN_RIGHT;	
		is_wifi_remote = 1;															
	}	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_LEFT))
	{	
		act = MOVE_ACT_TURN_LEFT;															
	}
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_LEFT))
	{	
		act = MOVE_ACT_TURN_LEFT;
		is_wifi_remote = 1;																
	}	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_FORWARD))
	{	
		act = MOVE_ACT_FORWARD;															
	}
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_FORWARD))
	{	
		act = MOVE_ACT_FORWARD;	
		is_wifi_remote = 1;																
	}	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_BACK))
	{	
		act = MOVE_ACT_BACK;																	
	}
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_BACK))
	{	
		act = MOVE_ACT_BACK;
		is_wifi_remote = 1;																	
	}	
	if(Remote_IsRemoteEvent(REMOTE_SIGNAL_WIFI_PAUSE))
	{	
		act = MOVE_ACT_STATIC;																
	}

	if(act != MOVE_ACT_NONE)
	{
		g_mode_remote_time = Timer_GetTime();
		Robot_AddAct(act,is_wifi_remote);
	}	
}

void Robot_AddAct(MoveAct_t act,uint8_t is_wifi_remote)
{
	uint8_t is_continue = 0;
	switch(Action_GetMove())
	{
		case MOVE_ACT_FORWARD:
								if(act == MOVE_ACT_FORWARD)	
								{
									is_continue = 1;	
								}
		break;
		case MOVE_ACT_TURN_RIGHT:
								if(act == MOVE_ACT_TURN_RIGHT)	
								{
									is_continue = 1;	
								}		
		break;
		case MOVE_ACT_TURN_LEFT:
								if(act == MOVE_ACT_TURN_LEFT)	
								{
									is_continue = 1;	
								}		
		break;
		case MOVE_ACT_BACK:
								if(act == MOVE_ACT_BACK)	
								{
									is_continue = 1;	
								}		
		break;
		default:break;				
	}
	if(is_continue)
	{
		if(is_wifi_remote)
		{
			Wheel_SetTargetStep(Wheel_GetLeftStep() + 3000,Wheel_GetRightStep() + 3000);
		}
		else
		{
			Wheel_SetTargetStep(Wheel_GetLeftStep() + 200,Wheel_GetRightStep() + 200);		
		}
	}
	else
	{
		Action_ListClear();
		if(is_wifi_remote)
		{
			Action_ListAdd(act,3000,REMOTE_SPEED,REMOTE_SPEED);	
		}
		else
		{
			Action_ListAdd(act,400,REMOTE_SPEED,REMOTE_SPEED);	
		}
				
		if(act != MOVE_ACT_STATIC)
		{
			Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
			Action_ListAdd(MOVE_ACT_STATIC,0,0,0);	
		}																			
		Action_SetMove(MOVE_ACT_HANDLER);	
	}	
}









