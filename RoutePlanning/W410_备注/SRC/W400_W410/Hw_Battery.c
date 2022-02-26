#include "Include.h"

uint8_t  g_hw_battery_level[47] = 
                            {   
								0 ,1 ,1 ,1 ,1 ,2 ,2 ,2 ,2 ,2,  //12.0----12.9
								2 ,2 , 3, 4, 4, 5, 6,10,13,15, //13.0----13.9
								18,21,26,33,41,48,52,55,58,61, //14.0----14.9
								64,67,69,72,74,76,79,81,83,86, //15.0----15.9
								88,92,95,98,100,100,100,       //16.0----16.9
							};


uint16_t Hw_Battery_GetAdc(void)
{
    return g_adc_value.battery;
}

uint8_t Hw_Battery_Voltage2Level(uint16_t voltage)
{
	int16_t level;

	if(voltage > 1635)return 100;
	if(voltage < 1200)return 1;
	
	level = (voltage - 1200)/10;
	if(level < 0)level = 0;
	if(level > 46)level = 46;;

	level = g_hw_battery_level[level];

	return level;
}



















