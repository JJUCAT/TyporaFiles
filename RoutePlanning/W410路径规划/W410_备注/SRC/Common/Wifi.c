 #include "Include.h"

Wifi_t g_wifi; 

void Wifi_Cycle(void)
{
    #ifdef ALICLOUD

    AliCloud_Cycle();		

    #endif

    #ifdef TUYA

    Tuya_Cycle();		
    
    #endif
}


void Wifi_Init(void)
{
    #ifdef ALICLOUD

    AliCloud_Init();

    #endif

    #ifdef TUYA

    Tuya_Init();

    #endif
}


void Wifi_Reset(void)
{
    #ifdef ALICLOUD

	AliCloud_Reset();

    
    #endif

    #ifdef TUYA

    
    #endif

    Wifi_ResetParameters();
}

void Wifi_PowerConfig(void)
{
    uint8_t data = 0;
    data = Flash_ReadByte(FLASH_START_ADDRESS);
    if(data != WIFI_FLASH_VALUE)
    {
        g_power.on_set_wifi_state(OFF);
				g_wifi.power_state = FALSE;
    }
    else
    {
        g_power.on_set_wifi_state(ON);
				g_wifi.power_state = TRUE;
    } 
    #ifdef DEBUG_WIFI

    g_power.on_set_wifi_state(ON);

    #endif 
}

void Wifi_SaveState(void)
{
    #ifdef ALICLOUD

    AliCloud_SaveState();

    #endif

    #ifdef TUYA

    Tuya_SaveState();
    
    #endif
}


void Wifi_RecieveData(void)
{          
    #ifdef ALICLOUD
    static uint8_t rx_segment = 0;
	uint8_t status = 0;
	uint8_t new_data = 0;
	uint16_t length = 0; 
	if((USART6->SR & USART_SR_IDLE)||(USART6->SR & USART_SR_ORE)) 
	{	
		if(USART6->SR & USART_SR_IDLE)new_data = 1;				
		status = USART6->SR;
		status = USART6->DR; 
		DMA2_Stream1->CR &= ~ DMA_SxCR_EN ;
		DMA2_Stream1->PAR =  (uint32_t)&(USART6->DR);
		length = ALI_RX_BUF_SIZE - DMA2_Stream1->NDTR;	
		rx_segment = 1 - rx_segment;
		DMA2_Stream1->M0AR = (uint32_t)&g_ali.rx_array[rx_segment][0];
		DMA2_Stream1->NDTR = ALI_RX_BUF_SIZE;
		DMA2->LIFCR |= (DMA_LIFCR_CTCIF1|DMA_LIFCR_CHTIF1|DMA_LIFCR_CFEIF1);
		DMA2_Stream1->CR |= DMA_SxCR_EN ; 
		if(new_data)AliCloudRx_ParseMsg(g_ali.rx_array[1 - rx_segment], length);  
		status++;
	}  

    #else
    if((USART6->SR&USART_SR_RXNE)==USART_SR_RXNE)
    {
        USART6->SR&=~USART_SR_RXNE;
        TuyaRx_RecieveData(USART6->DR);
    }
    #endif
}

void Wifi_NavStart(void)
{
    #ifdef ALICLOUD

    AliCloud_NavCleanStart();		

    #endif

    #ifdef TUYA

    Tuya_NavCleanStart();		
    
    #endif
}

void Wifi_NavEnd(void)
{
    #ifdef ALICLOUD

    AliCloud_NavCleanEnd();		

    #endif

    #ifdef TUYA

    Tuya_NavCleanEnd();		
    
    #endif
}

void Wifi_ResetStartNav(void)
{
    #ifdef ALICLOUD
	
    g_ali.is_start_nav = 0;

    #endif

    #ifdef TUYA

	Tuya_NavCleanStart();
    
    #endif
}

void Wifi_AddRealMapToSend(Point16_t point)
{
    #ifdef ALICLOUD

    AliCloud_AddRealMapToSend(point);		

    #endif

    #ifdef TUYA

    Tuya_AddRealMapToSend(point);		
    
    #endif
}

void Wifi_Config(void)
{
    #ifdef ALICLOUD

    AliCloud_SetWifiState(ALI_WIFI_STATE_AP_CONFIG);

    #endif

    #ifdef TUYA

    static uint8_t config_state = 0;

    config_state = 1 - config_state;
    if(config_state)
    {
        Tuya_SetWifiMode(SMART_CONFIG);
        Usprintf_Wifi("smart config\n");
    }
	else
    {
        Tuya_SetWifiMode(AP_CONFIG);
        Usprintf_Wifi("ap config\n");	
    }
    
    #endif    
}

uint8_t Wifi_Config_Start(void)
{
    uint8_t reval = 0;

    #ifdef ALICLOUD

    if(!g_ali.is_wifi_connected)
    {
        g_power.on_set_wifi_state(ON);	
        reval = 1;							
        Usprintf("Delay to Start wifi connecting\n");									
    }
    else
    {
				g_wifi.power_state = TRUE;
        AliCloud_SetWifiState(ALI_WIFI_STATE_AP_CONFIG);
        g_ali.is_wifi_connected = 0;
        Usprintf("Start wifi connecting\n");							
    }

    #endif

    #ifdef TUYA		


    if(!g_tuya.is_wifi_connected)
    {
        g_power.on_set_wifi_state(ON);	
        reval = 1;							
        Usprintf("Delay to Start wifi connecting\n");	
    }
    else
    {
    	g_tuya.is_wifi_connected = 0;
        reval = 1;	
    }
    #endif

    return reval;
}

uint8_t Wifi_IsParameterUpdate(void) 
{
    uint8_t reval = 0;

    #ifdef ALICLOUD

  	reval = AliCloud_IsParameterUpdate();

    #endif

    #ifdef TUYA


    
    #endif       


    return reval;    
}

void Wifi_ResetParameters(void)
{
    #ifdef ALICLOUD

  	AliCloud_ResetParameters();

    #endif

    #ifdef TUYA

 
    #endif 
}


void Wifi_GetState(void)
{ 
    #ifdef ALICLOUD

    if(g_ali.cur_wifi_state == ALI_WIFI_STATE_CLOUD_CONNOECTED)
    {
        Usprintf("get wifi state\n");
        AliCloud_GetMsg(ALI_ATTRID_WIFI_STATE);	
    } 

    #endif

    #ifdef TUYA


    
    #endif                  
}

uint8_t Wifi_IsSendingHistory(void)
{
    uint8_t reval = 0;

    #ifdef ALICLOUD

    reval = AliCloud_IsMapHistoryBusy();	

    #endif

    #ifdef TUYA


    #endif  
    return reval;  
}



void Wifi_ResetConnected(void)
{
    #ifdef ALICLOUD

    g_ali.is_wifi_connected = 0;

    #endif

    #ifdef TUYA


    #endif    
}

void Wifi_Stop(void)
{
    #ifdef ALICLOUD
    g_ali.cur_workmode = ALI_WM_SLEEP;
    AliCloud_ReportMsg(ALI_ATTRID_WORKMODE);
    g_ali.tx_stop = 1;
    #endif

    #ifdef TUYA


    #endif 
}

void Wifi_WakeUp(void)
{
    #ifdef ALICLOUD

	g_ali.tx_stop = 0;

    #endif

    #ifdef TUYA


    #endif 
}

uint8_t Wifi_IsConnected(void)
{
    uint8_t reval = 0;    

    #ifdef ALICLOUD

    reval =  g_ali.is_wifi_connected;

    #endif

    #ifdef TUYA



    #endif 

    return reval;   
}

void Wifi_ParameterInit(void)
{
    #ifdef ALICLOUD
    /*
    g_ali.is_wifi_connected = g_sensor_baseline.is_wifi_connect;
    g_ali.cur_pump_spd		= g_sensor_baseline.waterflow;
    g_ali.cur_mainbrush_spd = g_sensor_baseline.brush_power;
    g_ali.voice_mute_cur_state = g_sensor_baseline.speaker_mute;
    g_ali.cur_set_cleanloop_cnt = g_sensor_baseline.clean_setcnt;
    */
    #endif

    #ifdef TUYA


    #endif 

}


void Wifi_SetFactoryTestMode(uint8_t state)
{
    #ifdef ALICLOUD

    g_ali.factory_test_mode = state;

    #endif

    #ifdef TUYA


    #endif    
}

void Wifi_SendFactoryCmd(void)
{
    #ifdef ALICLOUD

    AliCloud_SetMsg(ALI_ATTRID_FACTORY_TEST);		
    AliCloud_GetMsg(ALI_ATTRID_WIFI_MAC);

    #endif

    #ifdef TUYA


    #endif  
}

void Wifi_TestStart(void)
{
    #ifdef ALICLOUD

	g_ali.factory_test_mode = 1;
	g_ali.wifi_sysinfo.is_receive = 0;
	g_ali.factorytest_ack_ok = 0;
    g_ali.test_time = Timer_GetTime();


    #endif

    #ifdef TUYA


    #endif
}

void Wifi_TestInit(void)
{
    #ifdef ALICLOUD

    AliCloud_GetMsg(ALI_ATTRID_WIFI_MAC);
    g_ali.wifi_sysinfo.is_receive = 0;
    g_ali.factory_test_mode = 0;


    #endif

    #ifdef TUYA


    #endif
}

void Wifi_TestCycle(void)
{
    #ifdef ALICLOUD
	static uint8_t is_right = 0;
	static uint8_t g_wifi_aliyun_version[4] = {1,1,0,7};
	
	uint8_t i;

	if((!g_ali.factorytest_ack_ok)&&(!g_ali.wifi_sysinfo.is_receive))
	{
		is_right = 0;
	}

	if(g_ali.factorytest_ack_ok && g_ali.wifi_sysinfo.is_receive && (!is_right))
	{
		for (i = 0;i < 4;i++)
		{
			if (g_ali.wifi_sysinfo.version[i] > g_wifi_aliyun_version[i])
			{
				Usprintf("version is valid\n");
				is_right = 1;
				break; 
			}
			else if (g_ali.wifi_sysinfo.version[i] < g_wifi_aliyun_version[i])
			{
				Usprintf("version is invalid\n");
				is_right = 2;												
				break; 
			}	
		}	
		if(is_right == 1)
		{
			Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
		}		
	}

	if(Timer_IsTimeReach(&g_ali.test_time,TIME_10S))
	{		
		if(!g_ali.factorytest_ack_ok)
		{
			AliCloud_SetMsg(ALI_ATTRID_FACTORY_TEST);
		}
		
		if(!g_ali.wifi_sysinfo.is_receive)
		{
			AliCloud_GetMsg(ALI_ATTRID_WIFI_MAC);
		}

		Usprintf("wifi test every 10s\n");				
	}

    #endif

    #ifdef TUYA


    #endif  
}


void Wifi_UpdateRobot(Point16_t cell,int16_t angle)
{
    #ifdef ALICLOUD

    AliCloud_UpdateRobot(cell,angle);

    #endif

    #ifdef TUYA

    #endif
}


void Wifi_UpdateRealMapEdg(int16_t x_min,int16_t x_max,int16_t y_min,int16_t y_max)
{
    #ifdef ALICLOUD

    AliCloud_UpdateRealMapEdg(x_min,x_max,y_min,y_max);

    #endif

    #ifdef TUYA

    #endif
}

void Wifi_ResetWorking(void)
{
    #ifdef ALICLOUD

    g_ali.is_working = ALI_FALSE;

    #endif

    #ifdef TUYA

    g_tuya.is_working = FALSE;

    #endif   
}


void Wifi_ResetWorkMode(void)
{
    #ifdef ALICLOUD

    g_ali.clean_mode = MODE_NONE;

    #endif

    #ifdef TUYA

    g_tuya.clean_mode = MODE_NONE;

    #endif 
}












