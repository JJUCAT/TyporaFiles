#include "Include.h"
Sleep_t g_sleep;

void Sleep_Init(void)
{
	memset(&g_sleep,0,sizeof(g_sleep));
	g_sleep.on_enter = Hw_Sleep_Enter;
	g_sleep.on_adc_config = Hw_Sleep_AdcConfiguration;
	g_sleep.on_exti_config = Hw_Sleep_ExtiIConfiguration;
	g_sleep.on_exti_pending_config = Hw_Sleep_ExtiPendingConfiguration;
	g_sleep.on_gpio_config = Hw_Sleep_GpioConfiguration;
	g_sleep.on_power_config = Hw_Sleep_PowerConfiguration;
	g_sleep.on_rcc_config = Hw_Sleep_RccConfiguration;
	g_sleep.on_systick_config = Hw_Sleep_SystickConfiguration;
	g_sleep.on_nvic_config = Hw_Sleep_NvicConfiguration;
	g_sleep.on_time_config = Hw_Sleep_Time_Configuration;
}

void Sleep_EditInterrupt(SleepInterrupt_t interrupt)
{
	g_sleep.interrupt |= interrupt;
}

void Sleep_ResetTheInterrupt(SleepInterrupt_t interrupt)
{
	g_sleep.interrupt &= ~interrupt;
}

void Sleep_ResetInterrupt(void)
{
	g_sleep.interrupt = 0;
}

uint8_t Sleep_IsInterrupt(SleepInterrupt_t interrupt)
{
    uint8_t reval = 0;
    if(g_sleep.interrupt & interrupt)
    {
      reval = 1;  
    }
    return reval;
}

void Sleep_InitConfiguration(void)
{
	__disable_irq();
	g_sleep.on_systick_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_power_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_exti_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_gpio_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_nvic_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_adc_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_rcc_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_exti_pending_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	g_sleep.on_time_config(SLEEP_CONFIG_INIT,Charge_IsAdapterInput());
	__enable_irq();
}

void Sleep_ExitConfiguration(void)
{
	g_sleep.on_systick_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_rcc_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_gpio_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_power_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_exti_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_adc_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_nvic_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
	g_sleep.on_time_config(SLEEP_CONFIG_EXIT,Charge_IsAdapterInput());
}

void Sleep_WakeUpConfiguration(void)
{
	g_sleep.on_gpio_config(SLEEP_CONFIG_WAKE_UP,Charge_IsAdapterInput());
	g_sleep.on_exti_config(SLEEP_CONFIG_WAKE_UP,Charge_IsAdapterInput());
	g_sleep.on_rcc_config(SLEEP_CONFIG_WAKE_UP,Charge_IsAdapterInput());
	g_sleep.on_power_config(SLEEP_CONFIG_WAKE_UP,Charge_IsAdapterInput());
	g_sleep.on_time_config(SLEEP_CONFIG_WAKE_UP,Charge_IsAdapterInput());
}

void Sleep_EnterStopMode(void)
{
    g_sleep.on_enter();
}


























