 #include "Include.h"

void Mode_Test(void)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
    Usprintf_Test("------- Mode Test Start---------\n");
    Robot_MotorsSetState(DISABLE);
    Robot_SensorsSetState(DISABLE);
    Test_Init();
    Test_SelectMode();
    Test_SelfCheck();
    while(Mode_GetMode() == MODE_TEST)
    {
		switch(g_test.choice)
		{	
			case TEST_CHOICE_ALLFUNCTION:
										Test_AllFunction();
			break;		
			case TEST_CHOICE_MOBILITY:
										Test_Mobility();
			break;
			case TEST_CHOICE_AGING:  		
										Test_Aging();				                    
			break;															
			case TEST_CHOICE_GYRO:												 
										Test_Gyro();				 
			break;	
			case TEST_CHOICE_PUMP:
										Test_Pump();
			break;
			case TEST_CHOICE_WIFI:
										Test_Wifi();
			break;			
			case TEST_CHOICE_BUMPER: 		
										Test_Bumper();													 
			break;								
			case TEST_CHOICE_WHEEL:
										Test_Wheel();
			break;	
      		case TEST_CHOICE_VACUUM:
										Test_Vacuum();
			break;			
			case TEST_CHOICE_SPEAKER: 		
										Test_Speaker();			 
			break;
			case TEST_CHOICE_SPEACAIL:
										Test_Speacial();	
			break;																		 														 
		  default: break;
		}		
        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
    Usprintf_Test("------- Mode Test End---------\n");
}























