#include "Include.h"
#include "task.h"

TaskHandle_t g_usart2_print_task_handler;
TaskHandle_t g_display_task_handler;
TaskHandle_t g_motorcontrol_task_handler;
TaskHandle_t g_main_task_handler;
TaskHandle_t g_sensor_task_handler;
TaskHandle_t g_calc_task_handler;

xSemaphoreHandle g_binary_wallcalculate_found;
xSemaphoreHandle g_binary_wallcalculate_start;
xSemaphoreHandle g_mutex_semaphore_clac;
xSemaphoreHandle g_binary_calc_success;
xSemaphoreHandle g_binary_semaphore_UsartTXF;

xQueueHandle g_queue_usart,g_queue_calc;
portTickType Hook_WakeTime = 0;

void Task_CreateSensorTask(void)
{
	portBASE_TYPE xTaskStatus;
	xTaskStatus = xTaskCreate(Task_SensorTask,"Sensor_Task",SENSORS_TASK_STK_SIZE,NULL,osPriorityRealtime,&g_sensor_task_handler);
	if(xTaskStatus!=pdPASS)
	{
		printf("Error,sensor_CreateTask create fail!!\n"); 
	}	
}

void Task_CreateCalculatePathTask(void)
{
	portBASE_TYPE xTaskStatus;
	g_queue_calc = xQueueCreate(1,1);
	
	xTaskStatus = xTaskCreate(Task_CalculatePathTask,"CalculatePath_Task",CALCULATE_PATH_TASK_STK_SIZE,NULL,osPriorityLow,&g_calc_task_handler);
	if(xTaskStatus!=pdPASS)
	{
		printf("Error,CalculatePath_Task create fail!!\n");
	}	
}

void Task_CreateMainTask(void)
{
	portBASE_TYPE xTaskStatus;
	xTaskStatus = xTaskCreate(Task_MainTask,"Main_Task",MAIN_TASK_STK_SIZE,NULL,osPriorityAboveNormal,&g_main_task_handler);
	if(xTaskStatus!=pdPASS)
	{
		printf("Error,Main_CreateTask create fail!!\n");
	}
}

void Task_CreateMotorControlTask(void)
{
	portBASE_TYPE xTaskStatus;
	xTaskStatus = xTaskCreate(Task_MotorControlTask,"MotorControl_Task",CONTROL_TASK_STK_SIZE,NULL,osPriorityHigh,&g_motorcontrol_task_handler);
	if(xTaskStatus != pdPASS)
	{
		printf("Error,MotorControl_Task create fail!!\n");
	}
}

void Task_CreateDebugTask(void)
{
#ifdef DEBUG_ENABLE	
	portBASE_TYPE xTaskStatus;
	vSemaphoreCreateBinary(g_binary_semaphore_UsartTXF);
	g_queue_usart = xQueueCreate(DEBUG_SIZE,sizeof(Usart_Queue_TypeDef));

	xTaskStatus = xTaskCreate(Task_DebugPrintTask,"Debug_PrintTask",USART2_TASK_STK_SIZE,NULL,osPriorityHigh,&g_usart2_print_task_handler);
	if(xTaskStatus!=pdPASS)
	{
		printf("Error,Debug_PrintTask create fail!!\n");
	}
#endif
}

void Task_ReportStackTask(void *argument)
{
	osThreadTerminate(NULL);
}

void Task_MotorControlTask(void *p_agument)
{
	uint32_t time_cycle_vac = 0;	
	uint32_t time_cycle_wheel = 0;
	uint32_t time_cycle_display = 0;	
	uint32_t time_cycle_pump = 0;	
	uint32_t time_cycle_motor = 0;	
	uint32_t time_cycle_pid = 0;	

	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();	
	for(;;)
	{
		if(Timer_IsTimeReach(&time_cycle_vac,TIME_500MS))
		{
			#ifdef	MOTOR_ENABLE
			Vacuum_TuneProcess();
			#endif		
		}
		if(Timer_IsTimeReach(&time_cycle_motor,TIME_25MS))
		{	
			#ifdef	MOTOR_ENABLE
			Pump_VoltageControl();
			MainBrush_VoltageControl();			
			#endif
		}					
		if(Timer_IsTimeReach(&time_cycle_wheel,TIME_25MS))
		{	
			Wheel_SpeedAccelerationProcess(Cliff_IsTrigStop()); 
			Wheel_TuneSpeedProcess(Wheel_GetLeftSpeed(),Wheel_GetRightSpeed(),Cliff_IsTrigStop());
		}
		if(Timer_IsTimeReach(&time_cycle_display,TIME_50MS))
		{
			Display_Process();
			Hw_Display_Process();
		}

		if(Timer_IsTimeReach(&time_cycle_pid,TIME_50MS))
		{
			Movement_Pid();
		}

		if(Timer_IsTimeReach(&time_cycle_pump,TIME_50MS))
		{
			#ifdef	MOTOR_ENABLE
			Event_PumperProcess();	
			#endif
		}		
		vTaskDelayUntil(&xLastWakeTime,5/portTICK_RATE_MS);
	}
}

void Task_CalculatePathTask(void *p_agument)
{
	vSemaphoreCreateBinary(g_binary_wallcalculate_start);
	vSemaphoreCreateBinary(g_binary_wallcalculate_found);
	xSemaphoreTake(g_binary_wallcalculate_start,0);	
	for(;;)
	{
		xSemaphoreTake(g_binary_wallcalculate_start,portMAX_DELAY);
		if(PathPlanning_WallFind(Map_GetRobotCell(),Mode_GetMode()) ==  PATHPLANNING_STATE_NORMAL_CLEAN) 
		{		
			xSemaphoreGive(g_binary_wallcalculate_found);
		}	
	}
}

void Task_DebugPrintTask(void *p_agument)
{	
	#ifdef DEBUG_ENABLE
	portBASE_TYPE xStatus;
	Usart_Queue_TypeDef Usart_Queue;

	xSemaphoreGive(g_binary_semaphore_UsartTXF);	
	for(;;)
	{
		xStatus = xQueueReceive(g_queue_usart,&Usart_Queue,portMAX_DELAY);

		if(xStatus == pdPASS)
		{
			xSemaphoreTake(g_binary_semaphore_UsartTXF,10/portTICK_RATE_MS);
			USART2_DMA_String(Usart_Queue.length,g_debug_fifo[Usart_Queue.idx]);
		}
	}
	#endif
}

void Task_SensorTask(void *p_agument)
{
	uint32_t time_cycle_dynamic = 0;	
	uint32_t time_cycle_update_position = 0;
	uint32_t time_cycle_heading_integral = 0;
	uint32_t time_cycle_exception = 0;
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();
	for(;;)
	{	
		Test_IsExit();
		Debug_Cycle();
		Wifi_Cycle();		
		Detection_Cycle();	
		Event_DisplayProcess();
		Event_AdjustProcess();
		Remote_FilterProcess();
		Cliff_DetectionProcess();
		Gyro_StartUpProcess();		
		if(Timer_IsTimeReach(&time_cycle_exception,TIME_40MS))
		{
			Exception_Cycle();
			
		}
		if(Timer_IsTimeReach(&time_cycle_heading_integral,TIME_40MS))
		{
			Heading_Update_Integral(Gyro_GetAngle(0));
		}
		if(Timer_IsTimeReach(&time_cycle_update_position,TIME_40MS))
		{
			Touring_UpdatePosition(Gyro_GetAngle(0), Gyro_GetAngle(1), Wheel_GetLeftCount(), Wheel_GetRightCount());
		}	
		if(Timer_IsTimeReach(&time_cycle_dynamic,TIME_200MS))
		{				
			Wall_DynamicProcess(Wall_GetDynamicState());
			OBS_DynamicProcess(OBS_GetDynamicState());				
		}		
		vTaskDelayUntil(&xLastWakeTime,5/portTICK_RATE_MS);
	}
}

void Task_MainTask(void *p_agument)
{
	portTickType xLastWakeTime;
	xLastWakeTime = xTaskGetTickCount();	
	for(;;)
	{
		switch(Mode_GetMode()) 
		{
			case MODE_NONE:				Mode_None();
			break;
			case MODE_USERINTERFACE	:	Mode_UserInterface();
			break;
			case MODE_CHARGE:			Mode_Charge();		
			break;
			case MODE_NAVIGATION:		Mode_Navigation();							
			break;
			case MODE_SPOT:				Mode_Spot();
			break;
			case MODE_AREA:				Mode_Navigation();								
			break;
			case MODE_REMOTE:			Mode_Remote();
			break;
			case MODE_SLEEP:			Mode_Sleep();
			break;
			case MODE_GO_HOME:			Mode_Navigation();						
			break;
			case MODE_WALL:				Mode_Wall();
			break;			
			case MODE_TEST:				Mode_Test();
			break;			
			default:					Mode_SetMode(MODE_USERINTERFACE);
			Usprintf("Clean Mode Out of range ,return to Userinterface\n");
			break;
		}
		vTaskDelayUntil(&xLastWakeTime,5/portTICK_RATE_MS);
	}
}

void vApplicationStackOverflowHook( TaskHandle_t xTask, char *pcTaskName )
{
	printf("  Stack Overflow!!!!");
	while(1)
	{
		printf("  Stack Overflow!!!!\n");
	}
}

void vApplicationIdleHook(void)
{

}


