#include "Include.h"

uint8_t Hw_Button_CleanTrig(void)
{
	if(!(GPIOC->IDR & MCU_O_CLEAN))
	{
		return 1;
	}
	return 0;    
}

uint8_t Hw_Button_LeftKeyTrig(void)
{
	if(g_hw_display.button_final_value == BUTTON_LEFT)
	{
		return 1;
	}
	return 0;    
}

uint8_t Hw_Button_RightKeyTrig(void)
{
	if(g_hw_display.button_final_value == BUTTON_RIGHT)
	{
		return 1;
	}
	return 0;    
}




















