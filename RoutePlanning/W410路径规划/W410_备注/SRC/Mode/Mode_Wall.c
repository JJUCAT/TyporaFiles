 #include "Include.h"

void Mode_Wall(void)
{
    TouringResult_t touring_result = TOURING_RESULT_NONE;    
    Touring_t touring = TOURING_INIT; 
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    Usprintf("------- Mode Wall Start---------\n");
    Touring_Start();
    while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
    {
        Event_Handler();
		switch(touring)
		{
			case	TOURING_INIT:                     	
                    if(Gyro_IsError())
                    {
                        Error_SetCode(ERROR_GYRO);
                        Mode_SetMode(MODE_USERINTERFACE);	
                        break;
                    } 
                    if(Gyro_IsReady()) 
                    {
                        Usprintf("TOURING_INIT");								                                                                              	
                        Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN); 
                        g_robot.update_position = TRUE;		
                        touring = TOURING_WALL;	
                    }           																																	
                    break;

			case	TOURING_WALL:                           
                    Usprintf("TOURING_WALL");    
                    touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_EAST_RIGHT, WALLFOLLOW_MODE,touring); 
                    switch(touring_result)
                    {
                        case TOURING_RESULT_WALL_BACK_START:
                                                            Usprintf("TOURING_RESULT_WALL_BACK_START"); 
                                                            Mode_SetMode(MODE_USERINTERFACE);    
                        break;
                        case TOURING_RESULT_TIME_OUT:       
                                                            Usprintf("TOURING_RESULT_TIME_OUT"); 
                                                            Mode_SetMode(MODE_USERINTERFACE);    
                        break;                                                                
                        default:break;
                    }                                                            
                    break;
			default:break;
		}


        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
    Touring_End();
    if(Error_GetCode() == ERROR_NONE)Speaker_Voice(SPK_CLEAN_FINISH);
    Usprintf("------- Mode Wall End---------\n");
}




















































