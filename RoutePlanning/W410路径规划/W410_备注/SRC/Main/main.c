#include "stm32f4xx.h"
#include "Include.h"
//v1.0  usart ok   base timer ok   create task ok
//v1.1  pwm ok ,pin ctrl on, adc ok

//timer 10 ---- bldc
//timer 9 ----pumper + brush
//timer 1 ----right wheel + left wheel
//timer 3 ----charger
//timer 4 ----error code

int main(void)
{
	Robot_StructInit();
	SystemInitialize();
	Robot_Initialize();
	Task_CreateDebugTask();
	Task_CreateCalculatePathTask();
	Task_CreateSensorTask();
	Task_CreateMotorControlTask();
	Task_CreateMainTask();
	vTaskStartScheduler();
	while(1);
}

