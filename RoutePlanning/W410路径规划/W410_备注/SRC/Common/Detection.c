#include "Include.h"

void Detection_Cycle(void)
{
	Battery_CapacityCalc(Charge_IsCharging(),ADC_TO_VOL_SYSTEM);
	Battery_VolatgeDetectionCycle(ADC_TO_VOL_BATTERY);
	Charge_VolatgeDetectionCycle(ADC_TO_VOL_CHARGE);
	System_CurrentDetectionCycle(ADC_TO_VOL_SYSTEM);
	Wheel_CurrentDetectionCycle(ADC_TO_VOL_WHEEL);
	MainBrush_CurrentDetectionCycle(ADC_TO_VOL_MAINBRUSH);
	Vacuum_CurrentDetectionCycle(ADC_TO_VOL_VACUUM);
}

void Detection_BaseLinesInit(void)
{
	delay(500);
	MainBrush_StoreBaseline();
	Vacuum_StoreBaseline();
	Wheel_StoreBaseline();
	System_StoreBaselineAdc();
}
































