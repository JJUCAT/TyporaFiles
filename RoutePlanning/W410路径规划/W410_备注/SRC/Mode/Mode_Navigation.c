 #include "Include.h"

void Mode_Navigation(void)
{
    uint8_t pathlist_state;
    PathListCell_t pathlist_point;
    PathPlanningState_t path_status;
    TouringResult_t touring_result = TOURING_RESULT_NONE;
    Touring_t touring = TOURING_INIT;
    WallDir_t wall_dir;
    Point16_t robot_cell; 
    uint8_t wall_next_lane_flag = 0;
    uint8_t try_find_station_cnt = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
    Usprintf("------- Mode Navigation ---------\n");
    Touring_Start();
    while((Mode_GetMode() != MODE_USERINTERFACE) && (Mode_GetMode() != MODE_CHARGE))
    {
        Event_Handler();
        switch(touring)
        {
            case  TOURING_INIT:
                    if(Gyro_IsError())
                    {
                        Error_SetCode(ERROR_GYRO);
                        Mode_SetMode(MODE_USERINTERFACE);	
                        break;
                    } 
                    if(Gyro_IsReady()) 
                    {
                        switch(Mode_GetMode())
                        {
                            case MODE_NAVIGATION:
                                                Usprintf("MODE_NAVIGATION\n");
                                                Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);                                            																																								                                                       																				                                                     
                                                if(!Station_GetStartState())
                                                {
                                                    Usprintf("To Find Wall\n");
                                                    touring = TOURING_FIND_WALL;                                                     
                                                }
                                                else
                                                {
                                                    Usprintf("To Touring\n");
                                                    touring = TOURING_ING;
                                                    Gyro_ResetWithOffset(2700);	
                                                    robot_cell = Map_GetRobotCell();	
                                                    pathlist_point.cell_pos.x = robot_cell.x +  (MAP_LIMIT - 2)/2;
                                                    pathlist_point.cell_pos.y = robot_cell.y;
                                                    pathlist_point.status = PATHLIST_NORMAL_CLEAN;  
                                                    PathList_AddNewPoint(pathlist_point);                                              
                                                }																																																													
                            break;
                            case MODE_AREA:		
                                                Usprintf("MODE_AREA\n");
                                                if(g_robot.type == ROBOT_W400)
                                                {
                                                    Display_SetCleanState(LED_MODE_STATIC,LED_STATE_GREEN);
                                                }	
                                                else
                                                {
                                                    Display_SetAreaState(LED_MODE_STATIC,LED_STATE_GREEN);
                                                    Display_SetCleanState(LED_MODE_STATIC,LED_STATE_OFF);																						
                                                }																																						
                                                Map_AreaInitialize();	
                                                Gyro_ResetWithOffset(900);
                                                robot_cell = Map_GetRobotCell();	
                                                pathlist_point.cell_pos.x = robot_cell.x +  (MAP_LIMIT - 2)/2;
                                                pathlist_point.cell_pos.y = robot_cell.y;
                                                pathlist_point.status = PATHLIST_NORMAL_CLEAN;
                                                PathList_AddNewPoint(pathlist_point);
                                                touring = TOURING_ING;                                                                                              
                            break;
                            case MODE_GO_HOME:	
                                                Usprintf("MODE_GO_HOME\n");
                                                Display_SetCleanState(LED_MODE_BLINK,LED_STATE_GREEN);																				
                                                Touring_GoStation();
                                                Map_SetMapEdg(MAP_LIMIT);
                                                Map_BoundarySet(MAP_LIMIT - 2);                                                        
                                                touring = TOURING_FIND_CHARGER;
                                                Station_SetStartState(TRUE);
                            break;															
                            default:
                            break;
                        }	
                        //Map_ResetMap(0,0);//test --------------
                        robot_cell = Map_GetRobotCell();													
                        Map_SetLevelCell(robot_cell);
                        g_robot.update_position = TRUE;                       
                        Map_SetCellRobotSizeClean(robot_cell.x,robot_cell.y);	
                    }                  
            break;     													                               
			case	TOURING_FIND_WALL:	
                    Usprintf("TOURING_FIND_WALL\n");
                    touring_result = WallFollow(Map_GetRobotCount(), WALLDIR_EAST_RIGHT, WALLFOLLOW_FIND,touring);
                    switch(touring_result)
                    {
                        case TOURING_RESULT_FINDWALL :	
                                            Usprintf("TOURING_RESULT_FINDWALL\n");																				
                                            robot_cell = Map_GetRobotCell();													
                                            Map_SetLevelCell(robot_cell);	
                                            touring = TOURING_ING; 															
                        break;				
                        case TOURING_RESULT_GO_HOME :
                                            Usprintf("TOURING_RESULT_GO_HOME\n");
                                            Touring_GoStartPoint();
                                            touring = TOURING_BACK_HOME; 	
                        break;	
                        case TOURING_RESULT_BOUNDARY:
                                            Usprintf("TOURING_RESULT_BOUNDARY\n");
                                            touring = TOURING_ING;	
                        break;
                        default:
                        break;
                    }			
			break;
			case 	TOURING_WALL_CHARGE:
                    Usprintf("TOURING_WALL_CHARGE\n");    
                    touring_result = WallFollow(Map_GetRobotCount(), WALLDIR_EAST_RIGHT, WALLFOLLOW_CHARGE,touring);		
                    switch(touring_result)
                    {
                        case TOURING_RESULT_ARRIVED:
                                            Usprintf("TOURING_RESULT_ARRIVED\n");
                                            Touring_ExploreStation();
                                            touring = TOURING_FIND_CHARGER; 																																								
                        break;															
                        case TOURING_RESULT_FIND_CHARGE:
                                            Usprintf("TOURING_RESULT_FIND_CHARGE\n");
                                            touring = TOURING_DOCKING; 		
                        break;
                        case TOURING_RESULT_NO_CHARGE:
                                            Usprintf("TOURING_RESULT_NO_CHARGE\n");	
                                            if(try_find_station_cnt <= 5)
                                            {	
                                                try_find_station_cnt++;
                                                Touring_ExploreStation();
                                                touring = TOURING_FIND_CHARGER; 																				
                                            }
                                            else
                                            {
                                                Mode_SetMode(MODE_USERINTERFACE);	
                                            }																	
                        break;					
                        default:
                        break;
                    }
			break;	
			case 	TOURING_DOCKING:
                    Usprintf("TOURING_DOCKING\n");   
                    touring_result = Docking();
                    switch(touring_result)
                    {
                        case TOURING_RESULT_ARRIVE_CHARGE:
                                            Usprintf("TOURING_RESULT_ARRIVE_CHARGE\n");
                                            if(Charge_IsAdapterInput())	
                                            {
                                                Mode_SetMode(MODE_CHARGE);
                                            }
                        break;
                        case TOURING_RESULT_NO_CHARGE:
                                            Usprintf("TOURING_RESULT_NO_CHARGE\n");
                                            Touring_ExploreStation();
                                            touring = TOURING_FIND_CHARGER; 																																																										
                        break;															
                        default:
                        break;
                    }																													
			break; 
			case 	TOURING_BACK_HOME:													
                    Usprintf("TOURING_BACK_HOME\n");
                    pathlist_state  = PathList_Out(&pathlist_point);
                    if(!pathlist_state)
                    {	
                        robot_cell = Map_GetRobotCell();			
                        path_status =  PathPlanning_BackHome(robot_cell);																																																															
                        switch(path_status)
                        {	
                            case PATHPLANNING_STATE_ARRIVE:			
                            case PATHPLANNING_STATE_UNREACHABLE:
                            Usprintf("BACK_HOME\n");
                            if(Map_GetTwoCellsDistance(robot_cell,Map_GetHomeCell()) <= 6)
                            {                                
                                if(Station_GetStartState())
                                {
                                    touring = TOURING_WALL_CHARGE; 	
                                }
                                else
                                {																																												
                                    Mode_SetMode(MODE_USERINTERFACE);																	
                                }														
                            }
                            else
                            {	
                                Usprintf("Start Wall\n");								
                                touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_EAST_RIGHT, WALLFOLLOW_TRAPPED,touring);
                                switch(touring_result)
                                {
                                    case TOURING_RESULT_ARRIVED :	
                                                        Usprintf("TOURING_RESULT_ARRIVED\n");                                                                         
                                    break;				
                                    case TOURING_RESULT_BOUNDARY:
                                                        Usprintf("TOURING_RESULT_BOUNDARY\n"); 
                                                        Touring_GoBoundary();
                                    break;
                                    case TOURING_RESULT_TIME_OUT:
                                                        Usprintf("TOURING_RESULT_TIME_OUT\n"); 
                                                        Touring_ExploreStation();	
                                                        touring = TOURING_FIND_CHARGER; 																			
                                    break;	
                                    case TOURING_RESULT_FIND_CHARGE:
                                                        Usprintf("TOURING_RESULT_FIND_CHARGE\n"); 
                                                        touring = TOURING_WALL_CHARGE; 																															
                                    break;																																																			
                                    default:
                                    break;
                                }
                            }																
                                                                                                                                                                                                                                                                                                                                                                                                                                
                            break;					
                            case PATHPLANNING_STATE_NORMAL_CLEAN:
                            Usprintf("Shortest Path Found, go go go!\n");
                            break;																										
                            default:                            
                            break;
                        }																																							
                    }
                    if(pathlist_state)
                    {
                        touring_result = MoveToPoint(Map_CellToPoint(pathlist_point.cell_pos),touring);
                        switch(touring_result)
                        {
                            case TOURING_RESULT_ARRIVED :
                                                Usprintf("TOURING_RESULT_ARRIVED\n");
                                                if(Station_GetStartState())
                                                {
                                                    touring = TOURING_WALL_CHARGE;
                                                }
                                                else
                                                {
                                                    Mode_SetMode(MODE_USERINTERFACE);																						
                                                }																																																																																				
                            break;				
                            case TOURING_RESULT_OBSTACLE:	
                                                Usprintf("TOURING_RESULT_OBSTACLE\n");
                                                Touring_MoveStop();	
                            break;
                            case TOURING_RESULT_GO_HOME:
                                                Usprintf("TOURING_RESULT_GO_HOME\n");
                                                touring = TOURING_WALL_CHARGE;
                            break;																																	
                            default:
                            break;
                        }																																		
                    }														
			break;  
			case	TOURING_FIND_CHARGER:
                    Usprintf("TOURING_FIND_CHARGER\n");														
                    pathlist_state = PathList_Out(&pathlist_point);									
                    if(!pathlist_state)
                    {		
                        robot_cell = Map_GetRobotCell();				
                        path_status =  PathPlanning_Cleaning(robot_cell);																																																																			
                        switch(path_status)
                        {
                            case PATHPLANNING_STATE_NOTARGET:
                                Usprintf("PATHPLANNING_STATE_NOTARGET\n");
                                Mode_SetMode(MODE_USERINTERFACE);																													                                                                           
                            break;
                            case PATHPLANNING_STATE_UNREACHABLE:
                                Usprintf("PATHPLANNING_STATE_UNREACHABLE\n");																
                                if(g_pathplanning_cleaned_rate > 75)
                                {
                                    Mode_SetMode(MODE_USERINTERFACE);																																																													
                                }
                                else
                                {                                      
                                    touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_EAST_RIGHT, WALLFOLLOW_TRAPPED,touring);
                                    switch(touring_result)
                                    {
                                        case TOURING_RESULT_ARRIVED :	
                                                            Usprintf("TOURING_RESULT_ARRIVED\n");             																					
                                        break;				
                                        case TOURING_RESULT_BOUNDARY:
                                                            Usprintf("TOURING_RESULT_BOUNDARY\n");
                                                            Touring_GoBoundary();
                                        break;
                                        case TOURING_RESULT_TIME_OUT:
                                                            Usprintf("TOURING_RESULT_TIME_OUT\n");
                                                            Mode_SetMode(MODE_USERINTERFACE);							
                                        break;
                                        case TOURING_RESULT_FIND_CHARGE:
                                                            Usprintf("TOURING_RESULT_FIND_CHARGE\n");
                                                            touring = TOURING_WALL_CHARGE;																															
                                        break;																															
                                        default:
                                        break;
                                    }																																																											
                                }																																																																																												
                            break;					
                            case PATHPLANNING_STATE_NORMAL_CLEAN:
                                    Usprintf("Shortest Path Found, go go go!\n");
                            break;																										
                            default:                      
                            break;
                        }																																							
                    }                    
                    if(pathlist_state)
                    {
                        touring_result = MoveToPoint(Map_CellToPoint(pathlist_point.cell_pos),touring);
                        switch(touring_result)
                        {
                            case TOURING_RESULT_ARRIVED :	
                                                Usprintf("TOURING_RESULT_ARRIVED\n");   	
                                                Touring_MoveStop();
                            break;				
                            case TOURING_RESULT_OBSTACLE:	
                                                Usprintf("TOURING_RESULT_OBSTACLE\n");   																																					
                                                Touring_MoveStop();																						
                            break;	
                            case TOURING_RESULT_BOUNDARY:
                                                Usprintf("TOURING_RESULT_BOUNDARY\n"); 
                                                Touring_MoveStop();
                            break;	
                            case TOURING_RESULT_GO_HOME:
                                                Usprintf("TOURING_RESULT_GO_HOME\n"); 
                                                touring = TOURING_WALL_CHARGE;		
                            break;	
                            case TOURING_RESULT_ARRIVE_CHARGE:
                                                Usprintf("TOURING_RESULT_ARRIVE_CHARGE\n"); 
                                                if(Charge_IsAdapterInput())	
                                                {
                                                    Mode_SetMode(MODE_CHARGE);
                                                }
                                                else
                                                {
                                                    touring = TOURING_WALL_CHARGE;	
                                                }
                            break;																																			
                            default:
                            break;
                        }
                    }													
            break;	            
			case	TOURING_ING:
                    Usprintf("TOURING_ING\n");				
                    pathlist_state = PathList_Out(&pathlist_point);									
                    if(!pathlist_state)
                    {				
                        robot_cell = Map_GetRobotCell();				
                        path_status =  PathPlanning_Cleaning(robot_cell);																	                                                                                                                                                         
                        switch(path_status)
                        {
                            case PATHPLANNING_STATE_NOTARGET: 
                                Usprintf("PATHPLANNING_STATE_NOTARGET\n");                                                                              
                                if(CleanCycleCount_IsArriveTargetCnt())
                                {
                                    Touring_GoStartPoint();
                                    touring = TOURING_BACK_HOME; 	
                                }																												                                                                               
                            break;
                            case PATHPLANNING_STATE_UNREACHABLE:
                                Usprintf("PATHPLANNING_STATE_UNREACHABLE\n");
                                if(g_pathplanning_cleaned_rate > 75)				
                                {
                                    Touring_GoStartPoint();
                                    touring = TOURING_BACK_HOME; 																																																														
                                }
                                else
                                {
                                    touring_result = WallFollow(Map_GetRobotCount(),WALLDIR_EAST_RIGHT, WALLFOLLOW_TRAPPED,touring);
                                    switch(touring_result)
                                    {
                                        case TOURING_RESULT_ARRIVED :	
                                            Usprintf("TOURING_RESULT_ARRIVED\n");               
                                        break;				
                                        case TOURING_RESULT_GO_HOME :
                                            Usprintf("TOURING_RESULT_GO_HOME\n"); 
                                            Touring_GoStartPoint();
                                            touring = TOURING_BACK_HOME; 
                                        break;
                                        case TOURING_RESULT_TRAP:
                                            Usprintf("TOURING_RESULT_TRAP\n"); 
                                            if(CleanCycleCount_IsArriveTargetCnt())
                                            {
                                                Touring_GoStartPoint();
                                                touring = TOURING_BACK_HOME; 	
                                            }
                                        break;
                                        case TOURING_RESULT_BOUNDARY:
                                            Usprintf("TOURING_RESULT_BOUNDARY\n"); 
                                            Touring_GoBoundary();
                                        break;
                                        default:
                                        break;
                                    }																																																											
                                }																																																																																												
                            break;					
                            case PATHPLANNING_STATE_NORMAL_CLEAN:
                                    Usprintf("Shortest Path Found, go go go \n");
                            break;																										
                            default:                     
                            break;
                        }																																							
                    }
                    
                    if(pathlist_state)
                    {
                        if(pathlist_point.status == PATHLIST_SHORT)
                        {
                            touring_result = MoveToPoint(Map_CellToPoint(pathlist_point.cell_pos),touring);
                            switch(touring_result)
                            {
                                case TOURING_RESULT_ARRIVED :	
                                    Usprintf("TOURING_RESULT_ARRIVED\n");
                                    wall_next_lane_flag = 0;																					
                                break;				
                                case TOURING_RESULT_OBSTACLE:	
                                    Usprintf("TOURING_RESULT_OBSTACLE\n");																																					
                                    Touring_MoveStop();	
                                    wall_next_lane_flag = 1;
                                break;
                                case TOURING_RESULT_GO_HOME :
                                    Usprintf("TOURING_RESULT_GO_HOME\n");
                                    Touring_GoStartPoint();
                                    touring = TOURING_BACK_HOME;
                                break;
                                case TOURING_RESULT_BOUNDARY:
                                    Usprintf("TOURING_RESULT_BOUNDARY\n");
                                    Touring_GoBoundary();
                                break;
                                case TOURING_RESULT_CALCULATE_GYRO:
                                    Usprintf("TOURING_RESULT_CALCULATE_GYRO\n");
                                    Touring_MoveStop();
                                    wall_next_lane_flag = 0;	
                                break;
                                default:
                                break;
                            }																																		
                        }
                        else
                        {
                            if((pathlist_point.cell_pos.x == Map_GetRobotCellX()) && wall_next_lane_flag)
                            {
                                wall_dir = Touring_GetWallDir(pathlist_point.cell_pos);
                                touring_result = WallFollow(Map_GetRobotCount(), wall_dir, WALLFOLLOW_NEXTLANE,touring);
                                switch(touring_result)
                                {
                                    case TOURING_RESULT_ARRIVED :
                                        Usprintf("TOURING_RESULT_ARRIVED\n");	
                                        wall_next_lane_flag = 0;
                                        Touring_MoveStop();
                                    break;				
                                    case TOURING_RESULT_GO_HOME :
                                        Usprintf("TOURING_RESULT_GO_HOME\n");
                                        Touring_GoStartPoint();
                                        touring = TOURING_BACK_HOME; 
                                    break;		
                                    case TOURING_RESULT_BOUNDARY:
                                        Usprintf("TOURING_RESULT_BOUNDARY\n");
                                        Touring_GoBoundary();
                                    break;																		
                                    default:
                                    break;
                                }	
                                break;
                            }
                            touring_result = MoveToPoint(Map_CellToPoint(pathlist_point.cell_pos),touring);
                            switch(touring_result)
                            {
                                case TOURING_RESULT_ARRIVED :
                                        Usprintf("TOURING_RESULT_ARRIVED\n");	
                                        wall_next_lane_flag = 0;																					
                                break;				
                                case TOURING_RESULT_OBSTACLE:	
                                        Usprintf("TOURING_RESULT_OBSTACLE\n");																																					
                                        Touring_MoveStop();	
                                        wall_next_lane_flag = 1;
                                break;
                                case TOURING_RESULT_GO_HOME :
                                        Usprintf("TOURING_RESULT_GO_HOME\n");
                                        Touring_GoStartPoint();
                                        touring = TOURING_BACK_HOME; 
                                break;	
                                case TOURING_RESULT_BOUNDARY:
                                        Usprintf("TOURING_RESULT_BOUNDARY\n");
                                        Touring_GoBoundary();
                                break;		
                                case TOURING_RESULT_CALCULATE_GYRO:
                                        Usprintf("TOURING_RESULT_CALCULATE_GYRO\n");
                                        Touring_MoveStop();
                                        wall_next_lane_flag = 0;	
                                break;                                																
                                default:
                                break;
                            }																
                            break;
                        }
                    }
            break; 
            default:Mode_SetMode(MODE_USERINTERFACE);break;
        }	        
        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
    if(Mode_GetMode() == MODE_CHARGE)
    {
        Station_SetMoveState(1);
    }    
    Touring_End();
    if(Error_GetCode() == ERROR_NONE)Speaker_Voice(SPK_CLEAN_FINISH);
}





