#include "Include.h"

void Mode_None(void)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();    
    uint8_t is_mode_exit = 0;
    Mode_t exit_mode = MODE_NONE;

    while(Mode_GetMode() == MODE_NONE)
    {
        g_robot.type = Robot_GetType();	

        Battery_CapacityInit(g_system.baseline);

        if(Test_IsEnter())
        {	
            exit_mode = MODE_TEST; 
            is_mode_exit = 1;
        }
        else
        {
            if(Charge_IsAdapterInput())
            {
                exit_mode = MODE_CHARGE; 
                is_mode_exit = 1;						
            }
            else
            {	
                if(!Button_StartUp())
                {
                    Power_KillAllVcc();
                }
                osDelay(50);							
                if(Battery_IsLow())
                {
                    Error_SetCode(ERROR_LOW_BATTERY);
                }	
                else
                {
                    Speaker_Voice(SPK_PRESS_BOTTON);	
                }			
                exit_mode = MODE_USERINTERFACE; 
                is_mode_exit = 1;                
            }					
        }   

        if(is_mode_exit)
        {
            is_mode_exit = 0;
            Mode_SetMode(exit_mode);
        }

        vTaskDelayUntil(&xLastWakeTime,20/portTICK_RATE_MS);
    }
}




















