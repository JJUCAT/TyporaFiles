 #include "Include.h"

uint8_t Hw_Bumper_LeftTrig(void)
{
    if(g_adc_value.l_bumper < BUMPER_TRIG_V_MIN)
    {
        return 1;
    }
    return 0;
}

uint8_t Hw_Bumper_RightTrig(void)
{
    if(g_adc_value.r_bumper < BUMPER_TRIG_V_MIN)
    {
        return 1;
    }    
    return 0;    
}















