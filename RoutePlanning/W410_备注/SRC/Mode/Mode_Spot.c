 #include "Include.h"

void Mode_Spot(void)
{
    TouringResult_t touring_result = TOURING_RESULT_NONE;    
    Touring_t touring = TOURING_INIT;
    Point16_t temp_robot_cell,temp_home_cell;
	uint8_t pathlist_state = 0;
	PathListCell_t pathlist_point;    
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    Usprintf("------- Mode Spot Start---------\n");
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
                        Map_BoundarySet(SPOT_RANGE);	
                        Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);																																																					
                        Spot_SetDirOutin(SPOT_DIR_OUT);	
                        Spot_SetDirCw(SPOT_DIR_CCW);																																																																			
                        temp_robot_cell.x = 1;
                        temp_robot_cell.y = 1;															
                        PathPlanning_Spot(temp_robot_cell);	
                        g_robot.update_position = TRUE;	
                        touring = TOURING_SPOT;	
                    } 
                    Event_Handler();           																					
                    break;
			case	TOURING_SPOT: 
                    temp_robot_cell = Map_GetRobotCell();
                    temp_home_cell = Map_GetHomeCell();               
                    pathlist_state = PathList_Out(&pathlist_point);
                    if(!pathlist_state)
                    {	
                        if(Spot_GetDirOutin() == SPOT_DIR_IN)
                        {
                            if(Map_GetTwoCellsDistance(temp_robot_cell,temp_home_cell) <= 2)
                            {
                                Usprintf("Back To Home Cell\n");
                                Mode_SetMode(MODE_USERINTERFACE);
                                break;
                            }
                        }																
                        PathPlanning_Spot(temp_robot_cell);
                        pathlist_state = PathList_Out(&pathlist_point);																																																	
                    }
                    if(!pathlist_state)
                    {
                        Usprintf("No Path calculate out,Wall to next lane\n");
                        touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_WEST_RIGHT, WALLFOLLOW_SPOT,touring);
                        switch(touring_result)
                        {
                            case TOURING_RESULT_ARRIVED:
                                Usprintf("TOURING_RESULT_ARRIVED\n");       
                            break;
                            default:break;
                        }                                                                																		
                        break;
                    }															
                    if(pathlist_state)
                    {
                        Usprintf("Path calculate out,Move to cell\n");
                        touring_result = MoveToPoint(Map_CellToPoint(pathlist_point.cell_pos),touring);
                        switch(touring_result)
                        {
                            case TOURING_RESULT_OBSTACLE:
                            Usprintf("TOURING_RESULT_OBSTACLE\n");  
                            touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_WEST_RIGHT, WALLFOLLOW_SPOT,touring);
                            switch(touring_result)
                            {
                                case TOURING_RESULT_ARRIVED:
                                Usprintf("TOURING_RESULT_ARRIVED\n");  
                                PathPoint_ClearAllPoints();
                                PathList_Clear();                                                                                                                                               
                                break;
                                default:break;
                            }
                            break;
                            default:Usprintf("error result\n");
                            break;
                        }																														
                    }
                    break;
			default:break;
		}


        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
    Touring_End();
    if(Error_GetCode() == ERROR_NONE)Speaker_Voice(SPK_CLEAN_FINISH);
    Usprintf("------- Mode Spot End---------\n");
}


























