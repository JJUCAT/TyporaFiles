
#ifndef __HW_ADC_H
#define __HW_ADC_H

#include <stdint.h>
#include "Hw_SysInitialize.h"

typedef struct
{
	volatile uint16_t left_wheel;	//0  	
	volatile uint16_t clean;		//4
	volatile uint16_t l_bumper;		//5
	volatile uint16_t r_bumper;		//6	
	volatile uint16_t dirt;			//7
	volatile uint16_t right_wheel;  //8
	volatile uint16_t charger;    	//9	
	volatile uint16_t battery;     	//10
	volatile uint16_t vac;  		//11
	volatile uint16_t system;     	//12
	volatile uint16_t mainbrush;  	//13 
	volatile uint16_t obs_wall;		//14 	
}Adc_Raw_t;

extern volatile Adc_Raw_t g_adc_value;

#define ADC_REFERENCE_VOLTAGE      (float)330

#define ADC_TO_VOL_BATTERY		(float)(ADC_REFERENCE_VOLTAGE*66/4096/10)
#define ADC_TO_VOL_CHARGE		(float)(ADC_REFERENCE_VOLTAGE*835/4096/120)
#define ADC_TO_VOL_SYSTEM		(float)(ADC_REFERENCE_VOLTAGE*40/4096)
#define ADC_TO_VOL_MAINBRUSH 	(float)(ADC_REFERENCE_VOLTAGE*20/4096)
#define ADC_TO_VOL_VACUUM 		(float)(ADC_REFERENCE_VOLTAGE*20/4096)
#define ADC_TO_VOL_WHEEL		(float)(ADC_REFERENCE_VOLTAGE*5/1024)

void Hw_Adc_Configuration(void);
void Hw_Adc_Start(void);

#endif











