#ifndef _ProjectTask_H_
#define _ProjectTask_H_
//#include "Include.h"
#include "cmsis_os.h"

#define USART2_TASK_STK_SIZE           200//200//
#define MAIN_TASK_STK_SIZE             450//600//600//
#define SENSORS_TASK_STK_SIZE          350//150//400//600
#define CONTROL_TASK_STK_SIZE          200//400//600//
#define CALCULATE_PATH_TASK_STK_SIZE   200//600//

extern xSemaphoreHandle g_mutex_semaphore_clac;
extern TaskHandle_t g_test_task_handler;
extern xSemaphoreHandle g_binary_calc_success;
extern xQueueHandle g_queue_usart,g_queue_calc;

typedef struct
{
	uint8_t idx;
	uint8_t length;
}Usart_Queue_TypeDef;

extern xSemaphoreHandle g_binary_wallcalculate_found;
extern xSemaphoreHandle g_binary_wallcalculate_start;
extern xSemaphoreHandle g_binary_semaphore_UsartTXF;

void Task_CreateSensorTask(void);
void Task_CreateDebugTask(void);
void Task_CreateMotorControlTask(void);
void Task_CreateCalculatePathTask(void);
void Task_CreateMainTask(void);
void Task_ReportStackTask(void *argument);
void Task_DebugPrintTask(void *p_agument);
void Task_MotorControlTask(void *p_agument);
void Task_MainTask(void *p_agument);
void Task_SensorTask(void *p_agument);
void Task_CalculatePathTask(void *p_agument);
#endif
