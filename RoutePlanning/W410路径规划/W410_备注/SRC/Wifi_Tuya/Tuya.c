#include "Include.h"

TuyaStruct_t g_tuya;

void Tuya_Init(void)
{
    memset(&g_tuya,0,sizeof(g_tuya));
    TuyaTx_Init();
    TuyaRx_Init();
    g_tuya.wifi_set_work_state = NO_CONFIG; 
}

void Tuya_Cycle(void)
{
    Tuya_ParseData();
    Tuya_MapStreamStateSwitch();
    Tuya_CheckError();
    Tuya_CheckWorkModeOnChange();
    Tuya_CheckBatteryOnChange();
    Tuya_TimerHandle();
    Tuya_FindingRobotHandle();
    Tuya_CheckWifiDisplayState();
    Tuya_CheckSpeakerEnable();
    Tuya_CheckWaterFlow();
}

void Tuya_CheckSpeakerEnable(void)
{
    if(g_speaker.enable != g_tuya.speaker_enable)
    {
        g_tuya.speaker_enable = g_speaker.enable;
        Tuya_DpBoolUpdate(DPID_TONESWITCH,g_tuya.speaker_enable);
    }
}

void Tuya_CheckWaterFlow(void)
{
    if(g_tuya.pump_cycle_cnt != g_pump.cycle_cnt)
    {
        g_tuya.pump_cycle_cnt = g_pump.cycle_cnt;
        if(g_tuya.pump_cycle_cnt == PUMP_WATER_FLOW_LEVEL_lOW)
        {
            g_tuya.water_flow = 0;
        }
        if(g_tuya.pump_cycle_cnt == PUMP_WATER_FLOW_LEVEL_NORMAL)
        {
            g_tuya.water_flow = 1;
        }
        if(g_tuya.pump_cycle_cnt == PUMP_WATER_FLOW_LEVEL_HIGH)
        {
            g_tuya.water_flow = 2;
        }   
        Tuya_DpEnumUpdate(DPID_WATERLEVELCONTROL,g_tuya.water_flow);     
    }
}

void Tuya_CheckWifiDisplayState(void)
{
	if(Mode_GetMode() != MODE_SLEEP)
	{
		if(g_tuya.is_working)
		{
            if(g_tuya.wifi_cur_work_state == WIFI_CONNECTED || g_tuya.wifi_cur_work_state == WIFI_CONN_CLOUD)
            {
                if(g_display.wifi_led_state == LED_STATE_OFF || g_display.wifi_led_mode == LED_MODE_BLINK)
                {
                    Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
                    Usprintf_Wifi("tuya light up wifi led\n");
                }
            }
            else
            {
                if((g_tuya.wifi_set_work_state != SMART_CONFIG) && (g_tuya.wifi_set_work_state != AP_CONFIG))
                {
                    if(g_tuya.wifi_cur_work_state == WIFI_NOT_CONNECTED)
                    {
                        if(g_display.wifi_led_state == LED_STATE_GREEN)
                        {
                            Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
                            Usprintf_Wifi("tuya light off wifi led\n");
                        } 
                    } 
                }              
            }
		}
	}
}

void Tuya_NavCleanStart(void)
{
    Tuya_RealMapListReset();
    Tuya_RealMapBusyListReset();
    Tuya_AskLocalTime();
    g_tuya.realtime_map.report_switch = TRUE;
    g_tuya.realtime_map.cleaned_area = 0;
    g_tuya.realtime_map.cleaned_time = 0;
    g_tuya.realtime_map.robot_cell.x = 0;
    g_tuya.realtime_map.robot_cell.y = 0;
    /*
    g_tuya.history.start_time_valid = 0;
 
    if(g_tuya.time.get_time_state)
    {
        g_tuya.history.year = g_tuya.time.year + 2000;

        g_tuya.history.month = g_tuya.time.month;

        g_tuya.history.day = g_tuya.time.day;

        g_tuya.history.hour = g_tuya.time.hour;

        g_tuya.history.min = g_tuya.time.min;   

        g_tuya.history.start_time_valid = 1;     
    }  
    */
    Tuya_SetMapStreamState(TY_MAP_STREAM_START);
}

void Tuya_NavCleanEnd(void)
{
    if(g_tuya.realtime_map.report_switch)
    {        
        g_tuya.realtime_map.report_switch = FALSE;

        Tuya_SetHistory(); 

        Tuya_SetMapStreamState(TY_MAP_STREAM_END);

        Usprintf_Wifi("-----wifi end----------\n"); 
    }
}

void Tuya_FindingRobotHandle(void)
{
	static uint32_t pre_time;

	if (g_tuya.find_robot_cnt > 0)
	{
		if (Timer_IsTimeReach(&pre_time,TIME_1S))
		{
			Speaker_Voice(SPK_FIND_ROBOT);
			g_tuya.find_robot_cnt--;
		}
	}
}

void Tuya_CheckWorkModeOnChange(void)
{
	if(Mode_GetMode() != g_tuya.clean_mode)
	{
		g_tuya.clean_mode = Mode_GetMode();
        g_tuya.cur_workmode = TY_DL_WM_ERROR;
		switch (g_tuya.clean_mode) 
		{
			case MODE_USERINTERFACE:			
				g_tuya.cur_workstatus = TY_UL_WM_STANBY;	
                g_tuya.cur_workmode = TY_DL_WM_SLEEP;										
				break;
			case MODE_NAVIGATION:				
				g_tuya.cur_workstatus = TY_UL_WM_SMART;	
                g_tuya.cur_workmode = TY_DL_WM_SMART;
				break;
            case MODE_AREA:
				g_tuya.cur_workstatus = TY_UL_WM_SMART;	
                g_tuya.cur_workmode = TY_DL_WM_SMART;            
                break;    
			case MODE_WALL:
				g_tuya.cur_workstatus = TY_UL_WM_WALLFOLLOW;
                g_tuya.cur_workmode = TY_DL_WM_WALLFOLLOW;	
				break;
			case MODE_CHARGE: //waiting modify
				g_tuya.cur_workstatus = TY_UL_WM_BASE;		
                g_tuya.cur_workmode = TY_DL_WM_SLEEP;
				break;
			case MODE_SLEEP:
				g_tuya.cur_workstatus = TY_UL_WM_SLEEP;	
                g_tuya.cur_workmode = TY_DL_WM_SLEEP;
				break;
			case MODE_REMOTE:
				g_tuya.cur_workstatus = TY_UL_WM_REMOTE;		
				break;
			case MODE_SPOT:
				g_tuya.cur_workstatus = TY_UL_WM_SPOT;	
                g_tuya.cur_workmode = TY_DL_WM_SPOT;	
				break;
			case MODE_GO_HOME: 
				g_tuya.cur_workstatus = TY_UL_WM_CHARGE_GO;	
                g_tuya.cur_workmode = TY_DL_WM_CHARGE_GO;	
				break;
			default:
				break;
		}
		if(g_tuya.cur_workmode != TY_DL_WM_ERROR)Tuya_DpEnumUpdate(DPID_MODE, g_tuya.cur_workmode);

        Tuya_DpEnumUpdate(DPID_STATUS, g_tuya.cur_workstatus);
	}
}

void Tuya_SetHistory(void)
{   
    uint8_t index = 0;

    uint8_t *p_history = g_tuya.history_buffer;

    g_tuya.history.cleaned_time = g_tuya.realtime_map.cleaned_time/60;

    g_tuya.history.cleaned_area = g_tuya.realtime_map.cleaned_area/TUYA_REALMAP_RATE;

    g_tuya.history.id = g_tuya.map_stream.id;

    //if(!g_tuya.history.start_time_valid)
    //{
    //    if(g_tuya.time.get_time_state)
        {						
            g_tuya.history.year = g_tuya.time.year + 2000;

            g_tuya.history.month = g_tuya.time.month;

            g_tuya.history.day = g_tuya.time.day;

            g_tuya.history.hour = g_tuya.time.hour;

            g_tuya.history.min = g_tuya.time.min;
        }
    //}

    *(p_history + (index++)) = g_tuya.history.year/1000 + 0x30;
    *(p_history + (index++)) = g_tuya.history.year%1000/100 + 0x30;
    *(p_history + (index++)) = g_tuya.history.year%100/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.year%10 + 0x30;

    *(p_history + (index++)) = g_tuya.history.month/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.month%10 + 0x30;

    *(p_history + (index++)) = g_tuya.history.day/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.day%10 + 0x30;

    *(p_history + (index++)) = g_tuya.history.hour/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.hour%10 + 0x30;

    *(p_history + (index++)) = g_tuya.history.min/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.min%10 + 0x30;

    *(p_history + (index++)) = g_tuya.history.cleaned_time/100 + 0x30;
    *(p_history + (index++)) = g_tuya.history.cleaned_time%100/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.cleaned_time/10 + 0x30;   

    *(p_history + (index++)) = g_tuya.history.cleaned_area/100 + 0x30;
    *(p_history + (index++)) = g_tuya.history.cleaned_area%100/10 + 0x30; 
    *(p_history + (index++)) = g_tuya.history.cleaned_area%10 + 0x30; 

    *(p_history + (index++)) = g_tuya.history.id/10000 + 0x30;
    *(p_history + (index++)) = g_tuya.history.id%10000/1000 + 0x30;
    *(p_history + (index++)) = g_tuya.history.id%1000/100 + 0x30;
    *(p_history + (index++)) = g_tuya.history.id%100/10 + 0x30;
    *(p_history + (index++)) = g_tuya.history.id%10 + 0x30; 

    Tuya_DpStringUpdate(DPID_CLEAN_RECORD,p_history,23);

}

void Tuya_CheckBatteryOnChange(void)
{
    if(g_tuya.battery_percent != g_battery.capacity.level)
    {
        g_tuya.battery_percent = g_battery.capacity.level;
        Tuya_DpValueUpdate(DPID_ELECTRICITY_LEFT,g_tuya.battery_percent); 
    }
}

void Tuya_TimerHandle(void)
{
	static uint32_t pre_time = 0,send_map_time = 0;

	if (Timer_IsTimeReach(&pre_time,TIME_1S))
	{
		if (g_tuya.realtime_map.report_switch)
		{
			g_tuya.realtime_map.cleaned_time++;
		}
	}
    if (Timer_IsTimeReach(&send_map_time,TIME_1S*2))
    {
        if (g_tuya.realtime_map.report_switch&&(g_tuya.cur_workmode == TY_UL_WM_SMART))
        {
            Tuya_UpdateRobotCell();

            Tuya_DpValueUpdate(DPID_CLEANAREA,g_tuya.realtime_map.cleaned_area/TUYA_REALMAP_RATE); 

            Tuya_DpValueUpdate(DPID_CLEARTIME,g_tuya.realtime_map.cleaned_time/60);      

        } 
        if(!g_tuya.time.get_time_state)
        {
            if(g_tuya.wifi_cur_work_state == WIFI_CONNECTED || g_tuya.wifi_cur_work_state == WIFI_CONN_CLOUD)
            {
                //Tuya_AskGreenTime();
                Tuya_AskLocalTime();
            }           
        }        
    }
}

void Tuya_UpdateAllStates(void)
{
    uint8_t bit = 0;    
    //clean mode
    Tuya_DpEnumUpdate(DPID_MODE, g_tuya.cur_workmode);
    Usprintf_Wifi("cur_workmode:%d\n",g_tuya.cur_workmode);
    
    //clean status
    Tuya_DpEnumUpdate(DPID_STATUS, g_tuya.cur_workstatus);
    Usprintf_Wifi("cur_workmode:%d\n",g_tuya.cur_workstatus);

    //battery
    uint32_t temp_battery_persent = (uint32_t)(g_tuya.battery_percent);

    Usprintf_Wifi("temp_battery_persent:%d\n",temp_battery_persent);

    Tuya_DpValueUpdate(DPID_ELECTRICITY_LEFT,temp_battery_persent); 

    //error code
    uint32_t temp_error_code = (uint32_t)(g_tuya.errorcode);

    Usprintf_Wifi("temp_error_code:%d\n",temp_error_code);
    bit = Tuya_GetErrorCode(g_tuya.errorcode);
    temp_error_code = 1<<(bit - 1); 

    Tuya_DpFaultUpdate(DPID_FAULT,temp_error_code);
    //water flow
    Tuya_DpEnumUpdate(DPID_WATERLEVELCONTROL,g_tuya.water_flow);  
    //speaker
    Tuya_DpBoolUpdate(DPID_TONESWITCH,g_tuya.speaker_enable);
}

void Tuya_CheckError(void)
{
    uint32_t temp_error_code = 0;
    uint8_t bit = 0;
	if(Error_GetCode() < ERROR_END)
	{
		if(Error_GetCode() == ERROR_LOW_BATTERY)
		{
			g_tuya.errorcode = ERROR_NONE;
			return;	
		}			
		if(Error_GetCode() != g_tuya.errorcode)
		{
			g_tuya.errorcode = Error_GetCode();
			Usprintf_Wifi("tuya error rsn %d\n",g_tuya.errorcode);
            bit = Tuya_GetErrorCode(g_tuya.errorcode);
            temp_error_code = 1<<(bit - 1);
			Tuya_DpFaultUpdate(DPID_FAULT,temp_error_code);
		}
	}
}

void Tuya_SaveState(void)
{
	uint8_t flash_data = 0;
	if(g_tuya.is_wifi_connected)
	{
		flash_data = Flash_ReadByte(FLASH_START_ADDRESS);
		if(flash_data != WIFI_FLASH_VALUE)
		{
			Flash_WriteOneByte(FLASH_START_ADDRESS,WIFI_FLASH_VALUE);
		}
	}
}

uint8_t Tuya_GetErrorCode(uint8_t code)
{
    uint8_t temp_error_code = 0;

    switch (code)
    {
    case ERROR_NONE:
        temp_error_code = TY_ERROR_CODE_NONE;
        break;
    case ERROR_BUMPER:
        temp_error_code = TY_ERROR_CODE_BUMPER;
        break;
    case ERROR_OBS:
        temp_error_code = TY_ERROR_CODE_OBS;
        break;
    case ERROR_WALL:
        temp_error_code = TY_ERROR_CODE_WALL;
        break;
    case ERROR_CLIFF:
        temp_error_code = TY_ERROR_CODE_CLIFF;
        break;
    case ERROR_PICK_UP:
        temp_error_code = TY_ERROR_CODE_PICKUP;
        break;
    case ERROR_MOBILITY:
        temp_error_code = TY_ERROR_CODE_MOBILITY;
        break;
    case ERROR_LEFT_WHEEL:
        temp_error_code = TY_ERROR_CODE_L_WHEEL;
        break;
    case ERROR_RIGHT_WHEEL:
        temp_error_code = TY_ERROR_CODE_R_WHEEL;
        break;
    case ERROR_MAINBRUSH:
        temp_error_code = TY_ERROR_CODE_M_BRUSH;
        break;
    case ERROR_VAC:
        temp_error_code = TY_ERROR_CODE_FAN;
        break;
    case ERROR_PUMP:
        temp_error_code = TY_ERROR_CODE_PUMP;
        break;
    case ERROR_GYRO:
        temp_error_code = TY_ERROR_CODE_GYRO;
        break;
    case ERROR_STUCK:
        temp_error_code = TY_ERROR_CODE_STUCK;
        break;
    case ERROR_BATTERY:
        temp_error_code = TY_ERROR_CODE_BAT;
        break;
    case ERROR_LOW_BATTERY:
        temp_error_code = TY_ERROR_LOW_BATTERY;
        break;
    default:
        Usprintf_Wifi("no error code match:%d", code);
        break;
    }
    return temp_error_code;
}

void Tuya_SetWifiMode(unsigned char mode)
{
    unsigned short length = 0;
    
    g_tuya.wifi_set_work_state = mode;

    g_tuya.wifi_set_mode = SET_WIFICONFIG_ERROR;
    
    length = TuyaTx_SetUsartByte(length, mode);
    
    TuyaTx_WriteFrame(WIFI_MODE_CMD, MCU_SEND_VER, length);
}

void Tuya_AskGreenTime(void)
{
    unsigned short length = 0;
    
    length = TuyaTx_SetUsartByte(length, 0);
    
    TuyaTx_WriteFrame(GET_ONLINE_TIME_CMD, MCU_SEND_VER, length);
}

void Tuya_AskLocalTime(void)
{
    unsigned short length = 0;

    length = TuyaTx_SetUsartByte(length, 0);

    TuyaTx_WriteFrame(GET_LOCAL_TIME_CMD, MCU_SEND_VER, length);
}

void Tuya_SetMapStreamState(unsigned char state)
{
    g_tuya.map_stream.state = state;  
}

void Tuya_EditMapStreamState(unsigned char state)
{
    uint16_t length = 0;

    g_tuya.map_stream.set_state = state;

    g_tuya.map_stream.start_time = Timer_GetTime();

    //Usprintf("%s(%d):map_stream_set_state:%d\n",__FUNCTION__,__LINE__,g_tuya.map_stream.set_state);

    switch(state)
    {
        case TY_MAP_STREAM_START:
                                    TuyaTx_WriteFrame(STREAM_START_CMD, MCU_SEND_VER, length);  
                                    g_tuya.map_stream.start_return_state = MAP_STREAM_STATE_WAIT_RETURN;                                   
        break;
        case TY_MAP_STREAM_CREATE_NEW_ID:                                                                  
                                    Usprintf_Wifi("g_tuya map_stream id:%d\n",g_tuya.map_stream.id);
                                    g_tuya.map_stream.new_start = 1;  
                                    g_tuya.map_stream.send_time = 0;                                    
                                    g_tuya.map_stream.offset = 0;        
                                    g_tuya.map_stream.id++;                                          
                                    length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.id >> 8) & 0xff));  
                                    length = TuyaTx_SetUsartByte(length, (g_tuya.map_stream.id & 0xff)); 
                                    TuyaTx_WriteFrame(STREAM_NEW_ID_CMD, MCU_SEND_VER,length); 
                                    g_tuya.map_stream.new_id_return_state = MAP_STREAM_STATE_WAIT_RETURN;                                     
        break;
        case TY_MAP_STREAM_DATA:
                                if(!g_tuya.realmap_busy)    
                                {                                                                                                         
                                    uint8_t cells_size = g_tuya.realmap_cnt;                                   
                                    //Usprintf("real map cells_size:%d\n",cells_size);
                                    if(cells_size||g_tuya.map_stream.new_start)
                                    {    
                                        g_tuya.realmap_busy = 1;
                                        if(!g_tuya.map_stream.new_start)    
                                        {
                                           g_tuya.map_stream.offset += cells_size*5 ;  
                                        }

                                        g_tuya.map_stream.new_start = 0;  
                                                                                                                                   
                                        length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.id >> 8) & 0xff));    
                                        length = TuyaTx_SetUsartByte(length, (g_tuya.map_stream.id & 0xff));                                      
                                        length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 24) & 0xff));    
                                        length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 16) & 0xff));    
                                        length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 8) & 0xff));    
                                        length = TuyaTx_SetUsartByte(length, (g_tuya.map_stream.offset & 0xff));  

                                        TuyaReportCell_t temp_cell;
										CellState_t temp_cell_state;
																				 
                                        //Usprintf("%s(%d):tuya real map cells_size:%d offset:%d\n",__FUNCTION__,__LINE__,cells_size,g_tuya.map_stream.offset);    

                                        for (uint16_t i = 0; i < cells_size; i++)
                                        {
                                            Point16_t temp_send_cell;

                                            Tuya_RealMapListShift(&temp_send_cell);

                                            temp_cell.cell.x = temp_send_cell.x;
                                            temp_cell.cell.y = temp_send_cell.y;

											temp_cell_state = Map_GetCell(temp_send_cell.x,temp_send_cell.y);

                                            switch(temp_cell_state)
                                            {
                                                case CELL_STATE_UNCLEAN:

                                                break;
                                                case CELL_STATE_CLEANED:
                                                                temp_cell.status = TY_CELL_STATE_CLEAN;
                                                break;
                                                case CELL_STATE_CLEANED_TWICE:
                                                                temp_cell.status = TY_CELL_STATE_CLEAN;
                                                break;
                                                case CELL_STATE_BLOCKED:
                                                                temp_cell.status = TY_CELL_STATE_BLOCK;
                                                break;
                                                default:break;
                                            }

                                            if((g_tuya.realtime_map.robot_cell.x == temp_cell.cell.x)&&(g_tuya.realtime_map.robot_cell.y == temp_cell.cell.y))
                                            {
                                                temp_cell.status = TY_CELL_STATE_CURRENT;    
                                            }
                                               
                                            //Usprintf("actual tuya cell:%d %d status:%d\n",temp_cell.cell.X,temp_cell.cell.Y,temp_cell.status);

                                            temp_cell.cell.x += MAP_SIZE/2;   
                                            temp_cell.cell.y += MAP_SIZE/2;   

                                            //Usprintf("input tuya cell:%d %d status:%d\n",temp_cell.cell.X,temp_cell.cell.Y,temp_cell.status);    
                                            
                                            length = TuyaTx_SetUsartByte(length, ((temp_cell.cell.x >> 8) & 0xff));    
                                            length = TuyaTx_SetUsartByte(length, (temp_cell.cell.x & 0xff));                                      
                                            length = TuyaTx_SetUsartByte(length, ((temp_cell.cell.y >> 8) & 0xff));    
                                            length = TuyaTx_SetUsartByte(length, (temp_cell.cell.y & 0xff));                                                                                                                                            
                                            length = TuyaTx_SetUsartByte(length, (temp_cell.status & 0xff));                                                                                                                                      
                                        }
                                        //Usprintf("-----length------:%d\n",length);

                                        TuyaTx_WriteFrame(STREAM_TRANS_CMD, MCU_SEND_VER,length);  

                                        g_tuya.map_stream.state = TY_MAP_STREAM_WAIT; 
                                        g_tuya.map_stream.trans_return_state = MAP_STREAM_STATE_WAIT_RETURN; 
                                        g_tuya.realmap_busy = 0;
                                    }                                                                          
                                }                                 
        break;
        case TY_MAP_STREAM_END:
                                    length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.id >> 8) & 0xff));    
                                    length = TuyaTx_SetUsartByte(length, (g_tuya.map_stream.id & 0xff));    
                                    length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 24) & 0xff));    
                                    length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 16) & 0xff));    
                                    length = TuyaTx_SetUsartByte(length, ((g_tuya.map_stream.offset >> 8) & 0xff));    
                                    length = TuyaTx_SetUsartByte(length, (g_tuya.map_stream.offset & 0xff));   
                                    TuyaTx_WriteFrame(STREAM_END_CMD, MCU_SEND_VER,length);       
                                    g_tuya.map_stream.end_return_state = MAP_STREAM_STATE_WAIT_RETURN;                           	                                                                                                   
        break;
        default:
                Usprintf_Wifi("Error edit stream state\n");
        break; 
    }
    if(state != TY_MAP_STREAM_DATA)
    {
        g_tuya.map_stream.state = TY_MAP_STREAM_WAIT; 
    }  
}

void Tuya_MapStreamWaitResult(void)
{
    switch(g_tuya.map_stream.set_state)
    {
        case  TY_MAP_STREAM_START:
                                    switch(g_tuya.map_stream.start_return_state)    
                                    {
                                        case  MAP_STREAM_STATE_START_SUCCESS:
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_START_SUCCESS\n",__FUNCTION__,__LINE__);
                                        g_tuya.map_stream.state = TY_MAP_STREAM_CREATE_NEW_ID;
                                        break;
                                        case  MAP_STREAM_STATE_START_FAIL:
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_START_FAIL\n",__FUNCTION__,__LINE__);
                                        g_tuya.map_stream.state = TY_MAP_STREAM_START;
                                        break;                                          
                                        default:break;  
                                    }                                                                                                                            
        break;     
        case  TY_MAP_STREAM_CREATE_NEW_ID:
                                    switch(g_tuya.map_stream.new_id_return_state)    
                                    {
                                        case  MAP_STREAM_STATE_NEW_ID_SUCCESS:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_DATA;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_NEW_ID_SUCCESS\n",__FUNCTION__,__LINE__);
                                        break;
                                        case  MAP_STREAM_STATE_NEW_ID_FUNCTIN_NOT_START:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_START;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_NEW_ID_FUNCTIN_NOT_START\n",__FUNCTION__,__LINE__);
                                        break;         
                                        case  MAP_STREAM_STATE_NEW_ID_CONNECT_FAIL:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_CREATE_NEW_ID;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_NEW_ID_CONNECT_FAIL\n",__FUNCTION__,__LINE__);
                                        break;
                                        case  MAP_STREAM_STATE_NEW_ID_OVER_TIME:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_CREATE_NEW_ID;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_NEW_ID_OVER_TIME\n",__FUNCTION__,__LINE__);
                                        break;                                                                             
                                        default:break;                                                                                
                                    }           
        break;
        case TY_MAP_STREAM_DATA:
                                    switch(g_tuya.map_stream.trans_return_state)    
                                    {
                                        case  MAP_STREAM_STATE_DATA_SUCCESS:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_DATA;                                        
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_DATA_SUCCESS time:%d\n",__FUNCTION__,__LINE__,Timer_Get_DiffNow(g_tuya.map_stream.start_time));                                        
                                        break;
                                        case  MAP_STREAM_STATE_DATA_FUNCTIN_NOT_START:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_START;                                        
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_DATA_FUNCTIN_NOT_START\n",__FUNCTION__,__LINE__);
                                        break; 
                                        case  MAP_STREAM_STATE_DATA_CONNECT_FAIL:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_DATA;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_DATA_CONNECT_FAIL\n",__FUNCTION__,__LINE__);
                                        break;                                        
                                        case  MAP_STREAM_STATE_DATA_OVER_TIME:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_DATA;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_DATA_OVER_TIME\n",__FUNCTION__,__LINE__);
                                        break; 
                                        case  MAP_STREAM_STATE_DATA_LENGTH_ERROR:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_DATA;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_DATA_LENGTH_ERROR\n",__FUNCTION__,__LINE__);
                                        break;
                                        case  MAP_STREAM_STATE_WAIT_RETURN:
                                        if(Timer_Get_DiffNow(g_tuya.map_stream.start_time) > TIME_2S)
                                        {
                                           //Usprintf("%s(%d):------------MAP_STREAM_STATE_WAIT_RETURN over time----------\n",__FUNCTION__,__LINE__);
                                           g_tuya.map_stream.state = TY_MAP_STREAM_DATA;     
                                        }
                                        break;                                         
                                        default:break;  
                                    }  
        break;                                                                      
        case  TY_MAP_STREAM_END:
                                    switch(g_tuya.map_stream.end_return_state)    
                                    {
                                        case  MAP_STREAM_STATE_END_SUCCESS:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_IDLE;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_END_SUCCESS\n",__FUNCTION__,__LINE__);
                                        break;
                                        case  MAP_STREAM_STATE_END_FUNCTIN_NOT_START:
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_END_FUNCTIN_NOT_START\n",__FUNCTION__,__LINE__);
                                        break; 
                                        case  MAP_STREAM_STATE_END_CONNECT_FAIL:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_END;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_END_CONNECT_FAIL\n",__FUNCTION__,__LINE__);
                                        break;                                        
                                        case  MAP_STREAM_STATE_END_OVER_TIME:
                                        g_tuya.map_stream.state = TY_MAP_STREAM_END;
                                        //Usprintf("%s(%d):MAP_STREAM_STATE_END_OVER_TIME\n",__FUNCTION__,__LINE__);
                                        break;                                         
                                        default:break;  
                                    }
        break;
        default:break;                                      
    }
}

void Tuya_MapStreamStateSwitch(void)
{
    static uint32_t debug_time;
    Point16_t point;
    if(Timer_IsTimeReach(&debug_time,TIME_1S*2))
    {
        //Usprintf("map_stream.state:%d set_state:%d return_state:%d %d %d map_stream.id:%d\n",g_tuya.map_stream.state,g_tuya.map_stream.set_state,g_tuya.map_stream.start_return_state,
		//g_tuya.map_stream.new_id_return_state,g_tuya.map_stream.trans_return_state,g_tuya.map_stream.end_return_state,g_tuya.map_stream.id);
    } 
    switch(g_tuya.map_stream.state)
    {
        case TY_MAP_STREAM_IDLE:
                                    
        break;   
        case TY_MAP_STREAM_WAIT:
                                    Tuya_MapStreamWaitResult();
        break; 
        case TY_MAP_STREAM_START:
                                    Tuya_EditMapStreamState(TY_MAP_STREAM_START);                                   
        break; 
        case TY_MAP_STREAM_CREATE_NEW_ID:
                                    Tuya_EditMapStreamState(TY_MAP_STREAM_CREATE_NEW_ID);    
        break;
        case TY_MAP_STREAM_DATA:
                                    if(Timer_IsTimeReach(&g_tuya.map_stream.send_time,TIME_1S*2))                                   
                                    {                                                                                                                            
                                        Tuya_EditMapStreamState(TY_MAP_STREAM_DATA);                                                                          
                                    }  
                                    if(!g_tuya.realmap_busy)  
                                    {
                                        if(g_tuya.realmap_busy_cnt)
                                        {
                                            Tuya_RealMapBusyListShift(&point);
                                            Tuya_RealMapListPush(point);		
                                        }   
                                    }                                                                        
        break; 
        case TY_MAP_STREAM_END:
                                    Tuya_EditMapStreamState(TY_MAP_STREAM_END);  
        break;    

        default:break;                           
    }
}

void Tuya_UpdateRobotCell(void)
{
    if(g_tuya.cur_workmode == TY_UL_WM_SMART)
    {
        if((g_tuya.realtime_map.robot_cell.x != Map_GetRobotCell().x)||(g_tuya.realtime_map.robot_cell.y != Map_GetRobotCell().y))
        {
            g_tuya.realtime_map.robot_cell.x = Map_GetRobotCell().x;
            g_tuya.realtime_map.robot_cell.y = Map_GetRobotCell().y;
            Tuya_AddRealMapToSend(g_tuya.realtime_map.robot_cell); 
        }  
    }
}

void Tuya_ParseData(void)
{
    static unsigned short rx_in = 0;
    unsigned short offset = 0;
    unsigned short rx_value_len = 0;     

    while((rx_in < sizeof(g_tuya.data_buf)) && TuyaRx_GetQueueTotalData() > 0)
    {
        g_tuya.data_buf[rx_in ++] = TuyaRx_QueueReadByte();
    }

    if(rx_in < PROTOCOL_HEAD)return;
    
    //Usprintf("-------");
    while((rx_in - offset) >= PROTOCOL_HEAD)
    {
        if(g_tuya.data_buf[offset + HEAD_FIRST] != FRAME_FIRST)
        {
            offset ++;            
            continue;
        }

        if(g_tuya.data_buf[offset + HEAD_SECOND] != FRAME_SECOND)
        {
            offset ++;
            continue;
        }  

        if(g_tuya.data_buf[offset + PROTOCOL_VERSION] != VERSION)
        {
            offset += 2;
            continue;
        }      

        rx_value_len = g_tuya.data_buf[offset + LENGTH_HIGH] * 0x100;
        rx_value_len += (g_tuya.data_buf[offset + LENGTH_LOW] + PROTOCOL_HEAD);
        if(rx_value_len > sizeof(g_tuya.data_buf) - PROTOCOL_HEAD)
        {
            offset += 3;
            continue;
        }

        if((rx_in - offset) < rx_value_len)
        {
            break;
        }
        if(TuyaMath_GetCheckSum((unsigned char *)g_tuya.data_buf + offset,rx_value_len - 1) != g_tuya.data_buf[offset + rx_value_len - 1])
        {
            offset += 3;
            continue;
        }

        Tuya_DataHandle(offset);
        offset += rx_value_len;
    }
    if(offset > 0)
    {
        rx_in -= offset;
        if((rx_in > 0)&& (rx_in < sizeof(g_tuya.data_buf)))
        {
            TuyaMath_Memcpy((char *)g_tuya.data_buf,(const char *)g_tuya.data_buf + offset,rx_in);
        }
        else
        {
           rx_in = 0; 
        }        
    }
}

static void Tuya_HeartBeatCheck(void)
{
    unsigned char length = 0;
    static unsigned char mcu_reset_state = FALSE;

    if(FALSE == mcu_reset_state)
    {
        length = TuyaTx_SetUsartByte(length,FALSE);
        mcu_reset_state = TRUE;
    }
    else
    {
        length = TuyaTx_SetUsartByte(length,TRUE);
    }

    TuyaTx_WriteFrame(HEAT_BEAT_CMD, MCU_SEND_VER, length);
}

static void Tuya_ProductInfoUpdate(void)
{
  unsigned char length = 0;
//  unsigned char str[10] = {0};
  
  length = TuyaTx_SetUsartBuffer(length, "{\"p\":\"", TuyaMath_Strlen("{\"p\":\""));
  length = TuyaTx_SetUsartBuffer(length,(unsigned char *)PRODUCT_KEY,TuyaMath_Strlen((unsigned char *)PRODUCT_KEY));
  length = TuyaTx_SetUsartBuffer(length, "\",\"v\":\"", TuyaMath_Strlen("\",\"v\":\""));
  length = TuyaTx_SetUsartBuffer(length,(unsigned char *)MCU_VER,TuyaMath_Strlen((unsigned char *)MCU_VER));
  length = TuyaTx_SetUsartBuffer(length, "\",\"m\":", TuyaMath_Strlen("\",\"m\":"));
  length = TuyaTx_SetUsartBuffer(length, (unsigned char *)CONFIG_MODE, TuyaMath_Strlen((unsigned char *)CONFIG_MODE));
#ifdef CONFIG_MODE_DELAY_TIME
  sprintf((char *)str,",\"mt\":%d",CONFIG_MODE_DELAY_TIME);
  length = TuyaTx_SetUsartBuffer(length, str, TuyaMath_Strlen(str));
#endif
#ifdef CONFIG_MODE_CHOOSE
  sprintf((char *)str,",\"n\":%d",CONFIG_MODE_CHOOSE);
  length = TuyaTx_SetUsartBuffer(length, str, TuyaMath_Strlen(str));
#endif
  
  length = TuyaTx_SetUsartBuffer(length, "}", TuyaMath_Strlen("}"));
  
  TuyaTx_WriteFrame(PRODUCT_INFO_CMD, MCU_SEND_VER, length);
}

static void Tuya_GetMcuMode(void)
{
  unsigned char length = 0;
  
#ifdef WIFI_CONTROL_SELF_MODE                                   //Module self-processing
  length = set_wifi_uart_byte(length, WF_STATE_KEY);
  length = set_wifi_uart_byte(length, WF_RESERT_KEY);
#else                                                           
  //No need to process data
#endif
  
  TuyaTx_WriteFrame(WORK_MODE_CMD, MCU_SEND_VER, length);
}

void Tuya_GetGreentime(unsigned char time[])
{
  /*
  time[0] is the flag of success for obtaining the time,0: failure / 1: success
  time[1] is year, 0x00 :2000 year
  time[2] is month, It starts at 1 and ends at 12
  time[3] is date, It starts at 1 and ends at 31
  time[4] is hour, It starts at 0 and ends at 23
  time[5] is minute, It starts at 0 and ends at 59
  time[6] is second, It starts at 0 and ends at 59
*/
    g_tuya.time.get_time_state = time[0];
    if(time[0] == 1)
    {       
        g_tuya.time.year = time[1];
        g_tuya.time.month = time[2];
        g_tuya.time.day = time[3];
        g_tuya.time.hour = time[4];
        g_tuya.time.min = time[5];
        g_tuya.time.second = time[6];
        Usprintf_Wifi("Receive the green data returned by wifi module correctly,time:%d,%d,%d,%d,%d,%d\n",g_tuya.time.year + 2000,g_tuya.time.month,g_tuya.time.day,g_tuya.time.hour,g_tuya.time.min,g_tuya.time.second);
    }
    else
    {
        //Usprintf("There is an error in obtaining the green time,It may be that the current wifi module is not connected to the Internet\n"); 
    }
}

void Tuya_GetLocaltime(unsigned char time[])
{
    g_tuya.time.get_time_state = time[0];
    if(time[0] == 1)
    {       
        g_tuya.time.year = time[1];
        g_tuya.time.month = time[2];
        g_tuya.time.day = time[3];
        g_tuya.time.hour = time[4];
        g_tuya.time.min = time[5];
        g_tuya.time.second = time[6];
        Usprintf_Wifi("Receive the green data returned by wifi module correctly,time:%d,%d,%d,%d,%d,%d\n",g_tuya.time.year + 2000,g_tuya.time.month,g_tuya.time.day,g_tuya.time.hour,g_tuya.time.min,g_tuya.time.second);
    }
    else
    {
        //Usprintf("There is an error in obtaining the green time,It may be that the current wifi module is not connected to the Internet\n"); 
    }
}

static unsigned char Tuya_DpdownloadModeHandle(const unsigned char value[], unsigned short length)
{
    unsigned char ret;
    unsigned char mode;

    mode = TuyaMath_GetDpDownloadEnum(value,length);

    TuyaDownLoadWorkMode_t set_mode = (TuyaDownLoadWorkMode_t)mode;

    g_tuya.set_workmode = set_mode;

    Usprintf_Wifi("tuya set mode:%d\n",set_mode);

    switch(g_tuya.set_workmode) 
    {
        case TY_DL_WM_SLEEP:
                                Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_STOP);
        break;  
        case TY_DL_WM_SPOT:
                                Remote_SetTempRemote(REMOTE_SIGNAL_SPOT);
        break;       
        case TY_DL_WM_SMART:
                                Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_NAV);
        break;       
        case TY_DL_WM_CHARGE_GO:
                                Remote_SetTempRemote(REMOTE_SIGNAL_GO_HOME);
        break;       
        case TY_DL_WM_WALLFOLLOW:
                                Remote_SetTempRemote(REMOTE_SIGNAL_WALL);
        break;  
        case TY_DL_WM_PAUSE:
                                Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_STOP);   
        break;       
        default:   
        break;
    }
    ret = TuyaTx_DpEnumUpdate(DPID_MODE,mode);
    if(ret == SUCCESS)
    return SUCCESS;
    else
    return ERROR;
}

static unsigned char Tuya_DpDownloadFindRobotHandle(const unsigned char value[], unsigned short length)
{
    unsigned char ret;

    unsigned char find_robot;

    Usprintf_Wifi("-----find robot-----\n");

    find_robot = TuyaMath_GetDpDownloadBool(value,length);
    if(find_robot == 0)
    {
        g_tuya.find_robot_cnt = 0;
    }
    else
    {
        if(!g_tuya.find_robot_cnt)
        {
            g_tuya.find_robot_cnt = 10;
        }
    }

    ret = Tuya_DpBoolUpdate(DPID_SEEK,find_robot);
    if(ret == SUCCESS)
        return SUCCESS;
    else
        return ERROR;
}

static unsigned char Tuya_DpDownloadSpeakerHandle(const unsigned char value[], unsigned short length)
{
    unsigned char ret;

    unsigned char voice;

    Usprintf_Wifi("-----Speaker-----\n");

    voice = TuyaMath_GetDpDownloadBool(value,length);
    if(voice == 0)//off
    {
        Speaker_Voice(SPK_VOICE_DISABLE);
        g_speaker.enable = 0;
        g_tuya.speaker_enable = 0;	    
    }
    else
    {
        g_speaker.enable = 1;
        g_tuya.speaker_enable = 1;
        Speaker_Voice(SPK_VIOCE_ENABLE);
    }

    ret = Tuya_DpBoolUpdate(DPID_TONESWITCH,voice);
    if(ret == SUCCESS)
    return SUCCESS;
    else
    return ERROR;
}

static unsigned char Tuya_DpDownloadFactoryResetHandle(const unsigned char value[], unsigned short length)
{
    unsigned char ret;

    unsigned char factory_reset;

    Usprintf_Wifi("-----factory reset-----\n");

    factory_reset = TuyaMath_GetDpDownloadBool(value,length);
    if(factory_reset == 0)//off
    {
        
    }
    else
    {
        //g_tuya.wifi_cur_work_state = WIFI_SATE_UNKNOW;
        g_tuya.time.get_time_state = 0;
        g_tuya.wifi_set_work_state = NO_CONFIG;    
        g_speaker.enable = 1;	
        //Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
		g_tuya.is_wifi_connected = 0;
		g_movement.base_spd = 100;
		g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;
		g_mainbrush.voltage = g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;
		g_cleancycle_count.set_cnt = 1;
		g_speaker.enable = 1;		
		Flash_WriteOneByte(FLASH_START_ADDRESS,0xFF);        
    }

    ret = Tuya_DpBoolUpdate(DPID_RESUME,factory_reset);
    if(ret == SUCCESS)
    return SUCCESS;
    else
    return ERROR;
}

static unsigned char Tuya_DpDownloadWaterControlHandle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char water_control;
  
  water_control = TuyaMath_GetDpDownloadEnum(value,length);
  Usprintf_Wifi("tuya water_control:%d\n",water_control);
  switch(water_control)
  {
    case 0:
            g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_lOW;
            Speaker_Voice(SPK_SMALL_WATERFLOW);	 
      break;
      
    case 1:
            g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;	
            Speaker_Voice(SPK_MEDIUM_WATERFLOW);
      break;
      
    case 2:
            g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_HIGH;	
            Speaker_Voice(SPK_MAX_WATERFLOW); 
      break;
      
    case 3:
      
      break;
      
    default:
      
      break;
  }
  
  ret = Tuya_DpEnumUpdate(DPID_WATERLEVELCONTROL,water_control);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}

static unsigned char Tuya_DpDownloadDirectionControlHandle(const unsigned char value[], unsigned short length)
{
  unsigned char ret;
  unsigned char directioncontrol;
  
  directioncontrol = TuyaMath_GetDpDownloadEnum(value,length);
  switch(directioncontrol)
  {
    case 0:
            Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_FORWARD);
      break;
      
    case 1:
            Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_BACK);
      break;
      
    case 2:
            Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_LEFT);
      break;
      
    case 3:
            Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_RIGHT);
      break;
      
    case 4:
            Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_PAUSE);
      break;
      
    default:
      
      break;
  }
  
  ret = Tuya_DpEnumUpdate(DPID_DIRECTIONCONTROL,directioncontrol);
  if(ret == SUCCESS)
    return SUCCESS;
  else
    return ERROR;
}

unsigned char Tuya_DpDownloadHandle(unsigned char dpid,const unsigned char value[], unsigned short length)
{
    unsigned char ret;
    Usprintf_Wifi("--------------dpid:%d-----------\n",dpid);
    switch(dpid)
    {
        case DPID_POWER:
            //ret = dp_download_power_handle(value,length);
            break;
        case DPID_SWITCH_GO:
            //ret = dp_download_switch_go_handle(value,length);
            break;
        case DPID_MODE:
            ret = Tuya_DpdownloadModeHandle(value,length);
            break;
        case DPID_WATER_CONTROL:
            ret = Tuya_DpDownloadWaterControlHandle(value,length);
            break;
        case DPID_DIRECTIONCONTROL:
            ret = Tuya_DpDownloadDirectionControlHandle(value,length);
            break;
        case DPID_RESETFILTER:
            //ret = dp_download_resetfilter_handle(value,length);
            break;
        case DPID_RESETROLLBRUSH:
            //ret = dp_download_resetrollbrush_handle(value,length);
            break;
        case DPID_RESETEDGEBRUSH:
            //ret = dp_download_resetedgebrush_handle(value,length);
            break;
        case DPID_RESUME:
            ret = Tuya_DpDownloadFactoryResetHandle(value,length);
            break;
        case DPID_MAX:
            //ret = dp_download_max_handle(value,length);
            break;
        case DPID_SEEK:
            ret = Tuya_DpDownloadFindRobotHandle(value,length);
            break;
        case DPID_WATERLEVELCONTROL:
            ret = Tuya_DpDownloadWaterControlHandle(value,length);
            break;
        case DPID_TONESWITCH:
            ret = Tuya_DpDownloadSpeakerHandle(value,length);
            break;
        default:
        break;
    }
    return ret;
}

static unsigned char Tuya_DataPointHandle(const unsigned char value[])
{
    unsigned char dp_id,index;
    unsigned char dp_type;
    unsigned char ret;
    unsigned short dp_len;

    dp_id = value[0];
    dp_type = value[1];
    dp_len = value[2] * 0x100;
    dp_len += value[3];

    index = TuyaMath_GetDowmloadDpidIndex(dp_id);

    if(dp_type != TuyaMath_GetDpType(index))
    {
        //Error message
        Usprintf_Wifi("error dp type\n");
        return FALSE;
    }
    else
    {
        ret = Tuya_DpDownloadHandle(dp_id,value + 4,dp_len);
    }

    return ret;
}

void Tuya_DataHandle(unsigned short offset)
{
#ifdef SUPPORT_MCU_FIRM_UPDATE
    unsigned char *firmware_addr = NULL;
    static unsigned short firm_size;                                              //升级包一包的大小
    static unsigned long firm_length;                                             //MCU升级文件长度
    static unsigned char firm_update_flag = 0;                                        //MCU升级标志
    unsigned long dp_len;
    unsigned char firm_flag;                                                      //升级包大小标志
#else
    unsigned short dp_len;
#endif
  
    unsigned char ret;
    unsigned short i,total_len;
    unsigned char cmd_type = g_tuya.data_buf[offset + FRAME_TYPE];
//    unsigned char result;

#ifdef WEATHER_ENABLE
    static unsigned char isWoSend = 0;                    //是否已经打开过天气数据,0:是;1:否
#endif

#ifdef WIFI_TEST_ENABLE
    unsigned char rssi;
#endif

#ifdef FILE_DOWNLOAD_ENABLE
    unsigned char *file_data_addr = NULL;
    static unsigned short file_package_size = 0;                                              //升级包一包的大小
    static unsigned char file_download_flag = 0;         //文件下载标志
    unsigned int file_download_size = 0;
#endif
    //Usprintf("cmd_type:%x \n",cmd_type);
    g_tuya.is_working = TRUE;
    switch(cmd_type)
    {
        case HEAT_BEAT_CMD:                                   //心跳包
            Usprintf("heat beat \n");
            Tuya_HeartBeatCheck();
        break;
    
        case PRODUCT_INFO_CMD:                                //产品信息
            Usprintf("product update\n");
            Tuya_ProductInfoUpdate();
        break;
    
        case WORK_MODE_CMD:                                   //查询MCU设定的模块工作模式
            Usprintf("get muc mode\n");
            Tuya_GetMcuMode();
        break;
    
#ifndef WIFI_CONTROL_SELF_MODE
        case WIFI_STATE_CMD:                                  //wifi工作状态	                           
            g_tuya.wifi_cur_work_state = g_tuya.data_buf[offset + DATA_START];
            Usprintf("wifi_cur_work_state:%d\n",g_tuya.wifi_cur_work_state);
            if(g_tuya.wifi_set_work_state == SMART_CONFIG || g_tuya.wifi_set_work_state == AP_CONFIG)
            {
                if(g_tuya.wifi_cur_work_state == WIFI_CONNECTED)
                {
                    Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
                    Speaker_Voice( SPK_WIFI_CONNECTED); 
                    g_tuya.is_wifi_connected = 1;
                    g_tuya.wifi_set_work_state = NO_CONFIG;                  
                }
            }                       
            TuyaTx_WriteFrame(WIFI_STATE_CMD, MCU_SEND_VER, 0);           
#ifdef WEATHER_ENABLE
            if(wifi_work_state == WIFI_CONNECTED && isWoSend == 0)   //当WIFI连接成功，打开天气数据且仅一次
            {
                mcu_open_weather();
                isWoSend = 1;
            }
#endif
        break;

        case WIFI_RESET_CMD:                                  //重置wifi(wifi返回成功)          
            g_tuya.wifi_reset_state = RESET_WIFI_SUCCESS;
            Usprintf("wifi_reset_state:%d\n",g_tuya.wifi_reset_state);
        break;
    
        case WIFI_MODE_CMD:                                   //选择smartconfig/AP模式(wifi返回成功)	
            g_tuya.wifi_set_mode = SET_WIFICONFIG_SUCCESS;
            Usprintf("wifi_set_mode:%d\n",g_tuya.wifi_set_mode);
        break;
#endif
    
        case DATA_QUERT_CMD:                                  //命令下发
        
            total_len = (g_tuya.data_buf[offset + LENGTH_HIGH] << 8) | g_tuya.data_buf[offset + LENGTH_LOW];
            for(i = 0;i < total_len;)
            {
                dp_len = g_tuya.data_buf[offset + DATA_START + i + 2] * 0x100;
                dp_len += g_tuya.data_buf[offset + DATA_START + i + 3];
                
                ret = Tuya_DataPointHandle((unsigned char *)g_tuya.data_buf + offset + DATA_START + i);
      
                if(SUCCESS == ret)
                {
                    //成功提示
                }
                else
                {
                    //错误提示
                }
      
                i += (dp_len + 4);
            }
        break;
    
        case STATE_QUERY_CMD:                                 //状态查询
            Usprintf("update all states\n");
            Tuya_UpdateAllStates();                               
        break;
    
#ifdef SUPPORT_MCU_FIRM_UPDATE
        case UPDATE_START_CMD:                                //升级开始
            //获取升级包大小全局变量
            firm_flag = PACKAGE_SIZE;
            if(firm_flag == 0) {
                firm_size = 256;
            }else if(firm_flag == 1) {
                firm_size = 512;
            }else if(firm_flag == 2) { 
                firm_size = 1024;
            }

            firm_length = wifi_data_process_buf[offset + DATA_START];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 1];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 2];
            firm_length <<= 8;
            firm_length |= wifi_data_process_buf[offset + DATA_START + 3];
            
            upgrade_package_choose(PACKAGE_SIZE);
            firm_update_flag = UPDATE_START_CMD;
        break;
    
        case UPDATE_TRANS_CMD:                                //升级传输
            if(firm_update_flag == UPDATE_START_CMD)
            {
                //停止一切数据上报
                stop_update_flag = ENABLE;
      
                total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
      
                dp_len = wifi_data_process_buf[offset + DATA_START];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 3];
      
                firmware_addr = (unsigned char *)wifi_data_process_buf;
                firmware_addr += (offset + DATA_START + 4);
      
                if((total_len == 4) && (dp_len == firm_length))
                {
                    //最后一包
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,0);
                    firm_update_flag = 0;
                }
                else if((total_len - 4) <= firm_size)
                {
                    ret = mcu_firm_update_handle(firmware_addr,dp_len,total_len - 4);
                }
                else
                {
                    firm_update_flag = 0;
                    ret = ERROR;
                }
      
                if(ret == SUCCESS)
                {
                    wifi_uart_write_frame(UPDATE_TRANS_CMD, MCU_SEND_VER, 0);
                }
                //恢复一切数据上报
                stop_update_flag = DISABLE;    
            }
        break;
#endif      

#ifdef SUPPORT_GREEN_TIME
        case GET_ONLINE_TIME_CMD:                              //获取格林时间
            Tuya_GetGreentime((unsigned char *)(g_tuya.data_buf + offset + DATA_START));
        break;
#endif

#ifdef SUPPORT_MCU_RTC_CHECK
        case GET_LOCAL_TIME_CMD:                             //获取本地时间
            Tuya_GetLocaltime((unsigned char *)(g_tuya.data_buf + offset + DATA_START));
        break;
#endif
 
#ifdef WIFI_TEST_ENABLE
        case WIFI_TEST_CMD:                                   //wifi功能测试（扫描指定路由）
            result = wifi_data_process_buf[offset + DATA_START];
            rssi = wifi_data_process_buf[offset + DATA_START + 1];
            wifi_test_result(result, rssi);
        break;
#endif

#ifdef WEATHER_ENABLE
        case WEATHER_OPEN_CMD:                                //打开天气服务返回
            weather_open_return_handle(wifi_data_process_buf[offset + DATA_START], wifi_data_process_buf[offset + DATA_START + 1]);
        break;
    
        case WEATHER_DATA_CMD:                                //天气数据下发
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
            weather_data_raw_handle((unsigned char *)wifi_data_process_buf + offset + DATA_START, total_len);
        break;
#endif

#ifdef WIFI_STREAM_ENABLE
        case STREAM_START_CMD:
             g_tuya.map_stream.start_return_state = g_tuya.data_buf[offset + DATA_START];                     
             //Usprintf("STREAM_START_CMD map_stream_return_state:%d\n",g_tuya.map_stream.start_return_state);   
        break;
        case STREAM_NEW_ID_CMD:
             g_tuya.map_stream.new_id_return_state = g_tuya.data_buf[offset + DATA_START];                     
             //Usprintf("STREAM_NEW_ID_CMD map_stream_return_state:%d\n",g_tuya.map_stream.new_id_return_state);  
        break;
        
        case STREAM_TRANS_CMD:                                  
            g_tuya.map_stream.trans_return_state = g_tuya.data_buf[offset + DATA_START];
            //Usprintf("STREAM_TRANS_CMD map_stream_return_state:%d\n",g_tuya.map_stream.trans_return_state); 
        break;       
        case STREAM_END_CMD:
            g_tuya.map_stream.end_return_state = g_tuya.data_buf[offset + DATA_START];
            //Usprintf("STREAM_END_CMD map_stream_return_state:%d\n",g_tuya.map_stream.end_return_state); 
        break;
        case MAPS_STREAM_TRANS_CMD:                             

        break;        
#endif

#ifdef WIFI_CONNECT_TEST_ENABLE
        case WIFI_CONNECT_TEST_CMD:                           //wifi功能测试（连接指定路由）
            result = wifi_data_process_buf[offset + DATA_START];
            wifi_connect_test_result(result);
        break;
#endif

#ifdef GET_MODULE_MAC_ENABLE
        case GET_MAC_CMD:                                     //获取模块mac
            mcu_get_mac((unsigned char *)(wifi_data_process_buf + offset + DATA_START));
        break;
#endif

#ifdef GET_WIFI_STATUS_ENABLE
        case GET_WIFI_STATUS_CMD:                             //获取当前wifi联网状态
            result = wifi_data_process_buf[offset + DATA_START];
            get_wifi_status(result);
        break;
#endif

#ifdef MCU_DP_UPLOAD_SYN
        case STATE_UPLOAD_SYN_RECV_CMD:                             //状态上报（同步）
            result = wifi_data_process_buf[offset + DATA_START];
            get_upload_syn_result(result);
        break;
#endif

#ifdef GET_IR_STATUS_ENABLE
        case GET_IR_STATUS_CMD:                                     //红外状态通知
            result = wifi_data_process_buf[offset + DATA_START];
            get_ir_status(result);
        break;
#endif
      
#ifdef IR_TX_RX_TEST_ENABLE
        case IR_TX_RX_TEST_CMD:                                     //红外进入收发产测
            result = wifi_data_process_buf[offset + DATA_START];
            ir_tx_rx_test_result(result);
        break;
#endif
        
#ifdef FILE_DOWNLOAD_ENABLE
        case FILE_DOWNLOAD_START_CMD:                               //文件下载启动
            //获取文件包大小选择
            if(FILE_DOWNLOAD_PACKAGE_SIZE == 0) {
                file_package_size = 256;
            }else if(FILE_DOWNLOAD_PACKAGE_SIZE == 1) {
                file_package_size = 512;
            }else if(FILE_DOWNLOAD_PACKAGE_SIZE == 2) { 
                file_package_size = 1024;
            }
            
            file_download_size = wifi_data_process_buf[offset + DATA_START];
            file_download_size = (file_download_size << 8) |  wifi_data_process_buf[offset + DATA_START + 1];
            file_download_size = (file_download_size << 8) |  wifi_data_process_buf[offset + DATA_START + 2];
            file_download_size = (file_download_size << 8) |  wifi_data_process_buf[offset + DATA_START + 3];
        
            file_download_package_choose(FILE_DOWNLOAD_PACKAGE_SIZE);
            file_download_flag = FILE_DOWNLOAD_START_CMD;
        break;
        
        case FILE_DOWNLOAD_TRANS_CMD:                               //文件下载数据传输
            if(file_download_flag == FILE_DOWNLOAD_START_CMD)
            {
                total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
      
                dp_len = wifi_data_process_buf[offset + DATA_START];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 1];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 2];
                dp_len <<= 8;
                dp_len |= wifi_data_process_buf[offset + DATA_START + 3];
      
                file_data_addr = (unsigned char *)wifi_data_process_buf;
                file_data_addr += (offset + DATA_START + 4);
      
                if((total_len == 4) && (dp_len == file_download_size))
                {
                    //最后一包
                    ret = file_download_handle(file_data_addr,dp_len,0);
                    file_download_flag = 0;
                }
                else if((total_len - 4) <= file_package_size)
                {
                    ret = file_download_handle(file_data_addr,dp_len,total_len - 4);
                }
                else
                {
                    file_download_flag = 0;
                    ret = ERROR;
                }
      
                if(ret == SUCCESS)
                {
                    wifi_uart_write_frame(FILE_DOWNLOAD_TRANS_CMD, MCU_SEND_VER, 0);
                }
            }
        break;
#endif
        
#ifdef MODULE_EXPANDING_SERVICE_ENABLE
        case OPEN_MODULE_TIME_SERVE_CMD:                              //打开模块时间服务通知
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
            open_module_time_serve_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

#ifdef BLE_RELATED_FUNCTION_ENABLE
        case BLE_TEST_CMD:                                            //蓝牙功能性测试（扫描指定蓝牙信标）
            total_len = (wifi_data_process_buf[offset + LENGTH_HIGH] << 8) | wifi_data_process_buf[offset + LENGTH_LOW];
            BLE_test_result((unsigned char *)(wifi_data_process_buf + offset + DATA_START), total_len);
        break;
#endif

            
#ifdef VOICE_MODULE_PROTOCOL_ENABLE
        case GET_VOICE_STATE_CMD:                                     //获取语音状态码
            result = wifi_data_process_buf[offset + DATA_START];
            get_voice_state_result(result);
        break;
        case MIC_SILENCE_CMD:                                         //MIC静音设置
            result = wifi_data_process_buf[offset + DATA_START];
            set_voice_MIC_silence_result(result);
        break;
        case SET_SPEAKER_VOLUME_CMD:                                  //speaker音量设置
            result = wifi_data_process_buf[offset + DATA_START];
            set_speaker_voice_result(result);
        break;
        case VOICE_TEST_CMD:                                          //音频产测
            result = wifi_data_process_buf[offset + DATA_START];
            voice_test_result(result);
        break;
        case VOICE_AWAKEN_TEST_CMD:                                   //唤醒产测
            result = wifi_data_process_buf[offset + DATA_START];
            voice_awaken_test_result(result);
        break;
        
#endif
        

        default:break;
    }
}

void Tuya_RealMapListReset(void)
{
	g_tuya.realmap_cnt = 0;
	g_tuya.realmap_input = 0;
	g_tuya.realmap_output = 0;
	g_tuya.realmap_busy = 0;  	
}

uint8_t Tuya_RealMapListPush(Point16_t point)
{
	if(g_tuya.realmap_busy)
	{
		Tuya_RealMapBusyListPush(point);
		return 0;		
	}

	if (g_tuya.realmap_cnt > TY_REALMAP_SIZE_LIMIT)
	{
		return 0;
	}	

	g_tuya.realmap_list[g_tuya.realmap_input] = point;
		
	g_tuya.realmap_input++;
	g_tuya.realmap_cnt++;

	if (g_tuya.realmap_input > TY_REALMAP_SIZE_LIMIT)g_tuya.realmap_input = 0;
	
	return 1;
}

uint8_t Tuya_RealMapListShift(Point16_t *point)
{
	if (!g_tuya.realmap_cnt)return 0;
	
	*point = g_tuya.realmap_list[g_tuya.realmap_output];
	
	g_tuya.realmap_cnt--;
	g_tuya.realmap_output++;

	if (g_tuya.realmap_output > TY_REALMAP_SIZE_LIMIT)g_tuya.realmap_output = 0;
	return 1;
}

void Tuya_RealMapBusyListReset(void)
{
	g_tuya.realmap_busy_cnt = 0;
	g_tuya.realmap_busy_input = 0;
	g_tuya.realmap_busy_output = 0;
}

uint8_t Tuya_RealMapBusyListPush(Point16_t point)
{
	if (g_tuya.realmap_busy_cnt > TY_REALMAP_BUSY_SIZE_LIMIT)
	{
		return 0;
	}	
	g_tuya.realmap_busy_list[g_tuya.realmap_busy_input] = point;
		
	g_tuya.realmap_busy_input++;
	g_tuya.realmap_busy_cnt++;

	if (g_tuya.realmap_busy_input > TY_REALMAP_BUSY_SIZE_LIMIT)g_tuya.realmap_busy_input = 0;
	
	return 1;
}

uint8_t Tuya_RealMapBusyListShift(Point16_t *point)
{
	if (!g_tuya.realmap_busy_cnt)return 0;
	
	*point = g_tuya.realmap_busy_list[g_tuya.realmap_busy_output];
	
	g_tuya.realmap_busy_cnt--;
	g_tuya.realmap_busy_output++;

	if (g_tuya.realmap_busy_output > TY_REALMAP_BUSY_SIZE_LIMIT)g_tuya.realmap_busy_output = 0;
	return 1;
}

void Tuya_AddRealMapToSend(Point16_t point)
{
	CellState_t temp_status = CELL_STATE_UNCLEAN;
	if (g_tuya.realtime_map.report_switch)
	{
		temp_status = Map_GetCell(point.x,point.y);
		if (temp_status != CELL_STATE_UNCLEAN) 
		{	
			g_tuya.realtime_map.cleaned_area++;			
			Tuya_RealMapListPush(point);
		}		
	}
}

uint8_t Tuya_DpEnumUpdate(uint8_t dpid,uint8_t value)
{
    uint16_t send_len = 0;
  
    send_len = TuyaTx_SetUsartByte(send_len,dpid);
    send_len = TuyaTx_SetUsartByte(send_len,DP_TYPE_ENUM);
    send_len = TuyaTx_SetUsartByte(send_len,0);
    send_len = TuyaTx_SetUsartByte(send_len,1);
    send_len = TuyaTx_SetUsartByte(send_len,value);
    
    TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,send_len);
    
    return SUCCESS;
}

uint8_t Tuya_DpValueUpdate(uint8_t dpid,uint32_t value)
{
    uint16_t send_len = 0;   
    send_len = TuyaTx_SetUsartByte(send_len,dpid);
    send_len = TuyaTx_SetUsartByte(send_len,DP_TYPE_VALUE);
    send_len = TuyaTx_SetUsartByte(send_len,0);
    send_len = TuyaTx_SetUsartByte(send_len,4);
    send_len = TuyaTx_SetUsartByte(send_len,value >> 24);
    send_len = TuyaTx_SetUsartByte(send_len,value >> 16);
    send_len = TuyaTx_SetUsartByte(send_len,value >> 8);
    send_len = TuyaTx_SetUsartByte(send_len,value & 0xff);
    
    TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,send_len);
    
    return SUCCESS;
}

uint8_t Tuya_DpFaultUpdate(uint8_t dpid,uint32_t value)
{
    uint16_t send_len = 0;
     
    send_len = TuyaTx_SetUsartByte(send_len,dpid);
    send_len = TuyaTx_SetUsartByte(send_len,DP_TYPE_FAULT);
    send_len = TuyaTx_SetUsartByte(send_len,0);
    
    if((value | 0xff) == 0xff) 
    {
        send_len = TuyaTx_SetUsartByte(send_len,1);
        send_len = TuyaTx_SetUsartByte(send_len,value);
        Usprintf_Wifi("0xff dp fault value:%d\n",value);
    }
    else if((value | 0xffff) == 0xffff)
    {
        send_len = TuyaTx_SetUsartByte(send_len,2);
        send_len = TuyaTx_SetUsartByte(send_len,value >> 8);
        send_len = TuyaTx_SetUsartByte(send_len,value & 0xff);
        Usprintf_Wifi("0xffff dp fault value:%d\n",value);
    }
    else 
    {
        send_len = TuyaTx_SetUsartByte(send_len,4);
        send_len = TuyaTx_SetUsartByte(send_len,value >> 24);
        send_len = TuyaTx_SetUsartByte(send_len,value >> 16);
        send_len = TuyaTx_SetUsartByte(send_len,value >> 8);
        send_len = TuyaTx_SetUsartByte(send_len,value & 0xff);
        Usprintf_Wifi("dp fault value:%d\n",value);
    }    
    
    TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,send_len);
    
    return SUCCESS;
}

uint8_t Tuya_DpStringUpdate(uint8_t dpid,uint8_t *p_value,uint16_t len)
{
    uint16_t send_len = 0;
    
    send_len = TuyaTx_SetUsartByte(send_len,dpid);
    send_len = TuyaTx_SetUsartByte(send_len,DP_TYPE_STRING);
    
    send_len = TuyaTx_SetUsartByte(send_len,len / 0x100);
    send_len = TuyaTx_SetUsartByte(send_len,len % 0x100);
    
    send_len = TuyaTx_SetUsartBuffer(send_len,(uint8_t *)p_value,len);
    
    TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,send_len);
    
    return SUCCESS;
}

uint8_t Tuya_DpBoolUpdate(uint8_t dpid,uint8_t value)
{
  uint16_t length = 0;
  
  length = TuyaTx_SetUsartByte(length,dpid);
  length = TuyaTx_SetUsartByte(length,DP_TYPE_BOOL);
  length = TuyaTx_SetUsartByte(length,0);
  length = TuyaTx_SetUsartByte(length,1);

  if(value == FALSE)
  {
    length = TuyaTx_SetUsartByte(length,FALSE);
  }
  else
  {
    length = TuyaTx_SetUsartByte(length,1);
  }
  
  TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,length);
  
  return SUCCESS;
}








