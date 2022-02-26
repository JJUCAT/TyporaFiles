#include "Include.h"

uint32_t g_mode_remote_time;
void Mode_Remote(void)
{
	uint8_t ret_val = 0;	
	uint8_t back_to_userinterface_flag = 0;
	uint16_t move_back_distance = 600;	
	portTickType xLastWakeTime = xTaskGetTickCount();
    
    Usprintf("------- Mode Remote Start ---------\n");

	Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
	Robot_MotorsSetState(ENABLE);
	Pump_SetStartControl(ENABLE);
	CodeShow_SetState(DISABLE);
	g_mode_remote_time = Timer_GetTime();
  	while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
	{
		switch(Action_GetMove())
		{
			case MOVE_ACT_FORWARD:   															
															  	Action_WaitForMoveEnded();																
																break;
			case MOVE_ACT_BACK:                									
																Action_WaitForMoveEnded();																
																break;													
			case MOVE_ACT_TURN_LEFT:  							
																Action_WaitForMoveEnded();
																break;
			case MOVE_ACT_TURN_RIGHT: 																							
																Action_WaitForMoveEnded();
																break;
			case MOVE_ACT_TURN_ROUND:																
																Action_WaitForMoveEnded();
																break;
			case MOVE_ACT_DECELERATE:							Action_WaitForDecelerateEnded();
																break;																
			case MOVE_ACT_HANDLER:								Action_ListSwitch();
																break;
			case MOVE_ACT_STATIC:     
                                                                Pump_SetState(DISABLE);
																if(back_to_userinterface_flag)
																{
																	back_to_userinterface_flag = 0;
																	Mode_SetMode(MODE_USERINTERFACE);															
																}
																break;	  																
			default:break;
		}

		Robot_Remote2Act();

		if(Bumper_GetTrigStatus()) 
		{
			Action_ListClear();
			Action_ListAdd(MOVE_ACT_BACK,move_back_distance,BACK_SPEED,BACK_SPEED);
			Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
			Action_ListAdd(MOVE_ACT_STATIC,0,0,0);																				
			Action_SetMove(MOVE_ACT_HANDLER);	
			back_to_userinterface_flag = 1;
			Usprintf("Bumper Event!\n");
		}

        if(Cliff_GetDetectiontProcess_Result() == CLIFF_TRIG_ALL)
        {
            Error_SetCode(ERROR_PICK_UP);
            Usprintf("Pick up Event!\n");
        }
        
        if(Cliff_GetTrigStatus())
        {
            Action_ListClear();
            Action_ListAdd(MOVE_ACT_BACK,move_back_distance,BACK_SPEED,BACK_SPEED);
            Action_ListAdd(MOVE_ACT_DECELERATE,0,0,0);
            Action_ListAdd(MOVE_ACT_STATIC,0,0,0);																				
            Action_SetMove(MOVE_ACT_HANDLER);	
            back_to_userinterface_flag = 1;			
            Usprintf("Cliff Event\n");						
        }			
       	
		if(Error_GetCode() != ERROR_NONE)
		{
			Mode_SetMode(MODE_USERINTERFACE);
		  	break;		  		
		}		
		if(Timer_Get_DiffNow(g_mode_remote_time) >= TIME_10S)
		{
			Mode_SetMode(MODE_USERINTERFACE);
			break;
		}

		if(Exception_GetState())
		{
		  	Mode_SetMode(MODE_USERINTERFACE);
			break;
		}

		if(WaterTank_IsInstall() == 0)
		{
			Error_SetCode(ERROR_TANK);
		}

		Event_Handler();

		if(ret_val)break;
		vTaskDelayUntil(&xLastWakeTime,50/portTICK_RATE_MS);
	}

    Usprintf("------- Mode Remote End ---------\n");
}

















