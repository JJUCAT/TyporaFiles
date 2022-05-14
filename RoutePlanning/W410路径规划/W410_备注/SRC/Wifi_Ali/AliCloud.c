#include "Include.h"


AliStruct_t g_ali;
AliCloud_MapGetCell on_ali_get_cell;
AliCloud_SetPowerState on_ali_set_power_state;

void AliCloud_Init(void)
{
	memset(&g_ali,0,sizeof(g_ali));	
	AliCloudRx_Init();
	AliCloudTx_Init();
	on_ali_get_cell = Map_GetCell;	
	on_ali_set_power_state = Hw_Power_Set_Wifi_State;
}

void AliCloud_Cycle(void)
{
	AliCloud_CommProcess();
	AliCloud_CheckWorkMode();
	AliCloud_RealMapReprotCycle();
	AliCloud_CheckBattery();
	AliCloud_CheckParameters();
	AliCloud_FindingRobotCycle();
	AliCloud_MapHistoryReprotCycle();
	AliCloud_TimerCycle();
	AliCloud_CheckError();
	AliCloud_CheckStationStartState();
	AliCloud_UpdateStates();
	AliCloud_CheckWifiDisplayState();
}

void AliCloud_CommProcess(void)
{
	uint8_t i,list_cnt = AliCloudTx_GetListCnt();
	AliTxMsgStruct_t msg;    
	if (AliCloudTx_GetListCnt())
	{	
		AliCloudTx_ClearSize();	
		for (i = 0;i < list_cnt;i++)
		{
			if (AliCloudTx_ListShift(&msg))
			{
				AliCloudTx_InSertPayload(msg);
			}
			if (AliCloudTx_GetSize() > (ALI_TX_BUF_MAX - 100))
			{
				break;
			}			
		}
		AliCloudTx_SendData();
	}
	else
	{
		if (AliCloudTx_IsListError()) 
		{
			AliCloudTx_ListReset();
		}
	}	
}

void AliCloud_Reset(void)
{
	memset(&g_ali,0,sizeof(g_ali));
}

void AliCloud_GetMsg(AliAttrIdType_t attr_id)
{
	AliTxMsgStruct_t msg;
	msg.method = ALI_METHOD_SYSTEM_GET;
	msg.attr_id = attr_id;
	msg.seq_num = g_ali.seq_num++;
	//Usprintf_Wifi("ali get method :%d attr id:%d seq_num:%d\n",msg.method,msg.attr_id,msg.seq_num);
	AliCloudTx_ListPush(msg);
}

void AliCloud_SetMsg(AliAttrIdType_t attr_id)
{
	AliTxMsgStruct_t msg;
	msg.method = ALI_METHOD_SYSTEM_SET;
	msg.attr_id = attr_id;
	msg.seq_num = g_ali.seq_num++;
	//Usprintf_Wifi("ali set method :%d attr id:%d seq_num:%d\n",msg.method,msg.attr_id,msg.seq_num);
	AliCloudTx_ListPush(msg);
}

void AliCloud_ReportMsg(AliAttrIdType_t attr_id)
{
	AliTxMsgStruct_t msg;	
	msg.method = ALI_METHOD_REPORT_ATTR;
	msg.attr_id = attr_id;
	msg.seq_num = g_ali.seq_num++;	
	//Usprintf_Wifi("ali report method :%d attr id:%d seq_num:%d\n",msg.method,msg.attr_id,msg.seq_num);
	AliCloudTx_ListPush(msg);
}

void AliCloud_ReportErrorMsg(AliEventIdType_t event_id)
{
	AliTxMsgStruct_t msg;

	msg.method = ALI_METHOD_REPORT_EVENT;
	msg.event_id = event_id;
	msg.attr_id = (AliAttrIdType_t)0;
	msg.seq_num = g_ali.seq_num++;	
	//Usprintf_Wifi("ali error method :%d attr id:%d seq_num:%d\n",msg.method,msg.attr_id,msg.seq_num);
	AliCloudTx_ListPush(msg);
}

void AliCloud_InsertAck(AliPacketStruct_t *rec_packet)
{
	AliTxMsgStruct_t msg;
	msg.method = ALI_METHOD_ACK;
	msg.attr_id = (AliAttrIdType_t)rec_packet->attr_id;
	msg.seq_num = rec_packet->seq_num;
	//Usprintf_Wifi("ali ack method :%d attr id:%d seq_num:%d\n",msg.method,msg.attr_id,msg.seq_num);
	AliCloudTx_ListPush(msg);
}

void AliCloud_ResetParameters(void)
{
	Usprintf_Wifi("ali reset parameters\n");

	g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_lOW - g_ali.cur_pump_spd * 10;
	
	g_ali.cur_mov_spd = g_movement.base_spd;

	g_mainbrush.adjust_voltage = MAINBRUSH_BASE_VOLTAGE + (MAINBRUSH_SPOT_VOLTAGE - MAINBRUSH_BASE_VOLTAGE)*g_ali.cur_mainbrush_spd/100;

	g_speaker.enable = !g_ali.voice_mute_cur_state;

	g_cleancycle_count.set_cnt = g_ali.cur_set_cleanloop_cnt = 1;
	
	if(g_ali.is_wifi_connected == 1)
	{
		on_ali_set_power_state(ON);
	}
}

void AliCloud_SetWifiState(AliWifiState_t state)
{
	g_ali.set_wifi_state = state;
	Usprintf_Wifi("ali set wifi state :%d\n",state);
	AliCloud_SetMsg(ALI_ATTRID_WIFI_STATE);
	if(state == ALI_WIFI_STATE_AP_CONFIG)
	{
		g_ali.is_wifi_connecting = ALI_TRUE;
	}
}

void AliCloud_CheckStationStartState(void)
{
	if(g_ali.station_start_state != Station_GetStartState())
	{
		g_ali.station_start_state = Station_GetStartState();
		Usprintf_Wifi("ali station_start_state :%d\n",g_ali.station_start_state);
	}
}

void AliCloud_CheckParameters(void)
{
	//voice mute
	if(g_ali.voice_mute_cur_state != (!g_speaker.enable))
	{
		g_ali.voice_mute_cur_state = (!g_speaker.enable);
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_N0_DISTURB);
		Usprintf_Wifi("ali voice mute  change,send to cloud:%d\n",g_ali.voice_mute_cur_state);
	}
	//main brush
	if(g_ali.cur_mainbrush_spd != MainBrush_GetSpeed())
	{
		g_ali.cur_mainbrush_spd = MainBrush_GetSpeed();	
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_MAINBRUSHPOWER);
		Usprintf_Wifi("ali target mainbrush speed change,send to cloud:%d\n",g_ali.cur_mainbrush_spd);
	}
	//pump
	if(g_ali.cur_pump_spd != Pump_GetSpeed())
	{
		g_ali.cur_pump_spd = Pump_GetSpeed();
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_WATERCONTROL);
		Usprintf_Wifi("ali target pump speed change,send to cloud:%d\n",g_ali.cur_pump_spd);
	}	
	//move spd
	if(g_ali.cur_mov_spd != g_movement.base_spd)
	{
		g_ali.cur_mov_spd = g_movement.base_spd;
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_WHEELSPEED);
		Usprintf_Wifi("ali target mov speed change,send to cloud:%d\n",g_ali.cur_mov_spd);
	}	
	//clean loop cnt
	//set
	if((g_ali.cur_set_cleanloop_cnt != g_cleancycle_count.set_cnt))	
	{
		g_ali.cur_set_cleanloop_cnt = g_cleancycle_count.set_cnt;
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_CLEANLOOPCNT);
		Usprintf_Wifi("ali set clean loop cnt change,send to cloud:%d\n",g_cleancycle_count.set_cnt);
	}
	//cur
	if (g_ali.over_cleanloop_cnt != g_cleancycle_count.over_cnt)	
	{
		g_ali.over_cleanloop_cnt = g_cleancycle_count.over_cnt;
		g_ali.is_parameter_update = 1;
		AliCloud_ReportMsg(ALI_ATTRID_CLEANLOOPCNT);
		Usprintf_Wifi("ali over clean loop cnt change,send to cloud,cleanloop_cnt:%d\n",g_ali.over_cleanloop_cnt);
	}		
}

void AliCloud_CheckWorkMode(void)
{
	if (Mode_GetMode() != g_ali.clean_mode)
	{
		g_ali.clean_mode = Mode_GetMode();
			Usprintf_Wifi("\r\n \r\n ------------>g_ali.clean_mode:%d \r\n \r\n",g_ali.clean_mode);
		switch (g_ali.clean_mode)
		{
			case MODE_USERINTERFACE:			
				g_ali.cur_workmode = ALI_WM_USERINTERFACE;					
				break;
			case MODE_NAVIGATION:				
				g_ali.cur_workmode = ALI_WM_NAV;	
				if(Station_GetStartState())
				{
					g_ali.cur_workmode = ALI_WM_CHARGE_NAV ;	
				}
				break;
			case MODE_AREA:
				g_ali.cur_workmode = ALI_WM_AREA;
				break;
			case MODE_CHARGE: 
				g_ali.cur_workmode = ALI_WM_CHARGE_WIRE;	
				if(Station_GetMoveState())
				{
					g_ali.cur_workmode = ALI_WM_CHARGE_STATION;	
				}
				break;
			case MODE_SLEEP:
				g_ali.cur_workmode = ALI_WM_SLEEP;
				break;
			case MODE_REMOTE:
				g_ali.cur_workmode = ALI_WM_REMOTE;	
				break;
			case MODE_SPOT:
				g_ali.cur_workmode = ALI_WM_SPOT;	
				break;
			case MODE_GO_HOME:
				g_ali.cur_workmode = ALI_WM_GOHOME;	
				break;
			case MODE_WALL:
				g_ali.cur_workmode = ALI_WM_WALLFOLLOW;	
				break;
			default:
				break;
		}
		Usprintf_Wifi("\r\n ---------->>ali work mode: %d,%d \r\n",Mode_GetMode(),g_ali.cur_workmode);
		AliCloud_ReportMsg(ALI_ATTRID_WORKMODE);	
	}
}

void AliCloud_CheckBattery(void)
{
	if(g_battery.capacity.level != g_ali.battery_pct)
	{
		g_ali.battery_pct = g_battery.capacity.level;
		Usprintf_Wifi("ali battery:%d\n",g_ali.battery_pct);
		AliCloud_ReportMsg(ALI_ATTRID_BATTERYSTATE);
	}
}

void AliCloud_CheckError(void)
{
	if(Error_GetCode() < ERROR_END)
	{
		if(Error_GetCode() == ERROR_LOW_BATTERY)
		{
			g_ali.error_rsn = ERROR_NONE;
			return;	
		}			
		if(Error_GetCode() != g_ali.error_rsn)
		{
			g_ali.error_rsn = Error_GetCode();
			Usprintf_Wifi("ali error rsn %d\n",g_ali.error_rsn);
			AliCloud_ReportErrorMsg(ALI_EVENT_ERROR);	
		}
	}
}

void AliCloud_RefreshCloud(void)
{
	Usprintf_Wifi("ali refresh cloud\n");
	AliCloud_GetMsg(ALI_ATTRID_UTC);		
	AliCloud_ReportMsg(ALI_ATTRID_WORKMODE);
	AliCloud_ReportMsg(ALI_ATTRID_BATTERYSTATE);
	AliCloud_ReportMsg(ALI_ATTRID_N0_DISTURB);
	AliCloud_ReportMsg(ALI_ATTRID_MAINBRUSHPOWER);
	AliCloud_ReportMsg(ALI_ATTRID_WATERCONTROL);
	AliCloud_ReportMsg(ALI_ATTRID_WHEELSPEED);
	AliCloud_ReportMsg(ALI_ATTRID_CLEANLOOPCNT);
	AliCloud_ReportMsg(ALI_ATTRID_ROBOT_INFO);
	AliCloud_ReportErrorMsg(ALI_EVENT_ERROR);			
}

void AliCloud_NavCleanStart(void)
{
	if (!g_ali.is_start_nav)
	{
		g_ali.is_start_nav = 1;
		g_ali.real_map.wifi_connected_start = g_ali.is_wifi_connected;
		g_ali.real_map.resend_flag = 0;
		g_ali.real_map.report_switch = 1;
		g_ali.real_map.cleaned_area = 0;
		g_ali.real_map.cleaned_time = 0;
		g_ali.real_map.start_time = AliCloud_GetCurrentUtc();	
		AliCloudTx_RealMapListReset();		
		AliCloud_ReportMsg(ALI_ATTRID_REALMAPSTART);
		AliCloud_ReportMsg(ALI_ATTRID_REALMAP);
		Usprintf_Wifi("ali nav start\n");
	}		
}

void AliCloud_NavCleanEnd(void)
{
	if (g_ali.is_start_nav)
	{
		g_ali.is_start_nav = 0;
		g_ali.real_map.report_switch = 0;

		if (g_ali.real_map.cleaned_time > 300*60)
		{
			g_ali.real_map.cleaned_time = 300*60;
		}
		Usprintf_Wifi("ali nav end\n");
		AliCloud_SendHistoryMap(g_ali.real_map.start_time,(uint16_t)(g_ali.real_map.cleaned_area*ALI_REALMAP_RATE),g_ali.real_map.cleaned_time);
	}	
}

void AliCloud_ResetStartNav(void)
{
	g_ali.is_start_nav = 0;
}

uint32_t AliCloud_GetCurrentUtc(void)
{
	return g_ali.time.utc;
}

void AliCloud_AddRealMapToSend(Point16_t point)
{
	CellState_t temp_cell_state = CELL_STATE_UNCLEAN;
	if (g_ali.real_map.report_switch)
	{
		temp_cell_state = Map_GetCell(point.x,point.y);
		if (temp_cell_state != CELL_STATE_UNCLEAN) 
		{	
			if((temp_cell_state == CELL_STATE_CLEANED)||(temp_cell_state == CELL_STATE_CLEANED_TWICE))
			{
				g_ali.real_map.cleaned_area++;	
			}
			//Usprintf_Wifi("ali add real map cell:%d %d %d %d\n",point.x,point.y,temp_cell_state,g_ali.real_map.cleaned_area);		
			AliCloudTx_RealMapListPush(point);
		}		
	}
}

void AliCloud_RealMapReprotCycle(void)
{
	static uint32_t pre_time = 0;
	Point16_t point;
	if (g_ali.real_map.report_switch)
	{
		if (Timer_IsTimeReach(&pre_time, TIME_4S))
		{
			//Usprintf_Wifi("ali real map cnt:%d\n",g_ali.real_map.list_cnt);
			if(g_ali.real_map.list_cnt)
			{
				AliCloud_ReportMsg(ALI_ATTRID_REALMAP);
			}
			else
			{
				if(g_ali.cur_workmode == ALI_WM_NAV || g_ali.cur_workmode == ALI_WM_AREA || g_ali.cur_workmode == ALI_WM_SPOT || g_ali.cur_workmode == ALI_WM_CHARGE_NAV)
				{
					AliCloud_ReportMsg(ALI_ATTRID_REALMAP);				
				}
			}			
		}
		if(!g_ali.real_map.list_busy)
		{
			if(g_ali.real_map.busy_list_cnt)
			{
				AliCloudTx_RealMapBusyListShift(&point);
				AliCloudTx_RealMapListPush(point);		
			}
		}
	}
}

void AliCloud_TimerCycle(void)
{
	static uint32_t pre_time = 0;

	if (Timer_IsTimeReach(&pre_time,TIME_1S))
	{
		g_ali.time.utc++;	
		g_ali.time.system++;
		if (g_ali.real_map.report_switch)
		{
			g_ali.real_map.cleaned_time++;
		}
	}
}

void AliCloud_FindingRobotCycle(void)
{
	static uint32_t pre_time;

	if (g_ali.find_robot_cnt > 0)
	{
		if (Timer_IsTimeReach(&pre_time,TIME_1S))
		{
			Speaker_Voice(SPK_FIND_ROBOT);
			g_ali.find_robot_cnt--;
		}
	}
}

void AliCloud_CompressionOfRun_WithType(AliHistoryCompression_t type)
{
	int16_t i,j;
	uint8_t reload_idx = 0;
	CellState_t pre_cell_state,temp_cell_state = CELL_STATE_UNCLEAN;
	uint8_t cell_state_length = 0;
	AliMapCompress_t temp_map_compress;
	uint16_t list_size = 0;
	int16_t dir_start_idx,dir_end_idx;
	int16_t scan_start_idx,scan_end_idx;

	dir_start_idx = g_ali.real_map.map_y_max,dir_end_idx = g_ali.real_map.map_y_min;
	scan_start_idx = g_ali.real_map.map_x_min,scan_end_idx = g_ali.real_map.map_x_max;

	switch(type)
	{
		case ALI_HC_CALCULATE_POCKET_NUM:
										g_ali.history_map.row_width  = scan_end_idx - scan_start_idx + 1;																		
		break;
		case ALI_HC_START_ADD_HISTORY:									
										g_ali.history_map.is_history_remain = 0;	
										AliCloudTx_HistoryMapListReset();
		break;
		case ALI_HC_CONTINUE_ADD_HISTORY:			
										g_ali.history_map.is_history_remain = 0;
										reload_idx = 1;			
										AliCloudTx_HistoryMapListReset();
		break;
		default:break;
	}

	for(i = dir_start_idx;i >= dir_end_idx;i--)
	{
		for(j = scan_start_idx;j <= scan_end_idx;j++)
		{	
			if(reload_idx)
			{
				reload_idx = 0;
				i = g_ali.history_map.stop_dir_idx;
				j = g_ali.history_map.stop_scan_idx;
				pre_cell_state = on_ali_get_cell(j,i);
				cell_state_length = 1;
				continue;
			}	
			temp_cell_state = on_ali_get_cell(j,i);
			if(j == scan_start_idx)
			{
				cell_state_length = 1;
			}
			else
			{
				if(temp_cell_state == pre_cell_state)
				{
					cell_state_length++;
					if(j == scan_end_idx)
					{
						if((type == ALI_HC_START_ADD_HISTORY) || (type == ALI_HC_CONTINUE_ADD_HISTORY))	
						{
							temp_map_compress.length = cell_state_length;
							temp_map_compress.status = pre_cell_state;
							AliCloudTx_HistoryMapListPush(temp_map_compress);
							if(AliCloudTx_HistoryMapListFull())
							{
								g_ali.history_map.is_history_remain = 1;
								g_ali.history_map.stop_dir_idx = i;
								g_ali.history_map.stop_scan_idx = j;
							}								
						}
						else
						{
							list_size++;
						}						
					}
				}
				else
				{
					if(type == ALI_HC_START_ADD_HISTORY || type == ALI_HC_CONTINUE_ADD_HISTORY)	
					{
						temp_map_compress.length = cell_state_length;
						temp_map_compress.status = pre_cell_state;
						AliCloudTx_HistoryMapListPush(temp_map_compress);	
						if(AliCloudTx_HistoryMapListFull())
						{
							g_ali.history_map.is_history_remain = 1;
							g_ali.history_map.stop_dir_idx = i;
							g_ali.history_map.stop_scan_idx = j;
						}												
					}	
					else
					{
						list_size++;
					}									
							
					cell_state_length = 1;
					if(j == scan_end_idx)
					{
						if((type == ALI_HC_START_ADD_HISTORY) || (type == ALI_HC_CONTINUE_ADD_HISTORY))	
						{
							temp_map_compress.length = 1;
							temp_map_compress.status = temp_cell_state;
							AliCloudTx_HistoryMapListPush(temp_map_compress);
							if(AliCloudTx_HistoryMapListFull())
							{
								g_ali.history_map.is_history_remain = 1;
								g_ali.history_map.stop_dir_idx = i;
								g_ali.history_map.stop_scan_idx = j;	
							}															
						}						
						else
						{
							list_size++;
						}
					}					
				}
			}
			pre_cell_state = temp_cell_state;
			if(g_ali.history_map.is_history_remain)break;
		}
		if(g_ali.history_map.is_history_remain)break;
	}
	if(type == ALI_HC_CALCULATE_POCKET_NUM)
	{
		g_ali.history_map.packet_num = list_size/ALI_HISTORY_MAP_SIZE+((list_size%ALI_HISTORY_MAP_SIZE)?1:0);
	}
}

void AliCloud_SendHistoryMap(uint32_t start_time,uint16_t cleaned_area,uint16_t cleaned_time)
{	
	Usprintf_Wifi("ali start send map history\n");

	AliCloud_ReportMsg(ALI_ATTRID_HISTORYSTARTTING);

	AliCloud_CompressionOfRun_WithType(ALI_HC_CALCULATE_POCKET_NUM);

	AliCloud_CompressionOfRun_WithType(ALI_HC_START_ADD_HISTORY);

	g_ali.history_map.start_time = start_time;
	g_ali.history_map.total_area = cleaned_area;
	g_ali.history_map.total_time = cleaned_time;

	g_ali.history_map.type = ALI_CLEAN_TYPE_MAP_DATA;
	g_ali.history_map.packet_id = 1;
	
	g_ali.history_map.start_rsn = (AliHistoryStartReason_t)0;
	g_ali.history_map.stop_rsn =  (AliHistoryEndReason_t)0;	
	g_ali.history_map.report_switch = 1;

	g_ali.history_map.is_last_pocket_finish = 0;
	
}

void AliCloud_MapHistoryReprotCycle(void)
{
	static uint32_t pre_time;

	if (g_ali.history_map.report_switch)
	{	
		if(g_ali.history_map.list_busy )return;

		if(g_ali.history_map.is_last_pocket_finish)
		{
			g_ali.history_map.is_last_pocket_finish = 0;
			if(g_ali.history_map.is_history_remain)
			{
				AliCloud_CompressionOfRun_WithType(ALI_HC_CONTINUE_ADD_HISTORY);
				pre_time = Timer_GetTime();
			}
		}

		if (Timer_IsTimeReach(&pre_time,TIME_100MS))
		{
			if(g_ali.history_map.packet_id <= g_ali.history_map.packet_num)
			{
				AliCloud_ReportMsg(ALI_ATTRID_HISTORY);	
				g_ali.history_map.is_last_pocket_finish = 0;								
			}
			else
			{
				g_ali.history_map.report_switch = 0;
			}
		}
	}
}

uint8_t AliCloud_IsMapHistoryBusy(void)
{
	return g_ali.history_map.report_switch;
}

uint8_t AliCloud_IsParameterUpdate(void)
{
	if(g_ali.is_parameter_update)
	{
		g_ali.is_parameter_update = 0;
		Usprintf_Wifi("parameter_update\n");
		return 1;
	}
	return 0;
}

void AliCloud_UpdateRobot(Point16_t cell,int16_t angle)
{
	g_ali.robot_cell = cell;
	g_ali.robot_angle = angle;
}

void AliCloud_UpdateRealMapEdg(int16_t x_min,int16_t x_max,int16_t y_min,int16_t y_max)
{
	g_ali.real_map.map_x_min = x_min;
	g_ali.real_map.map_x_max = x_max;
	g_ali.real_map.map_y_min = y_min;
	g_ali.real_map.map_y_max = y_max;
}

void AliCloud_UpdateWorkMode(void)
{
	if(g_ali.update_work_mode)
	{
		g_ali.update_work_mode = ALI_FALSE;
		Usprintf_Wifi("ali update work mode\n");
		switch (g_ali.set_workmode)
		{
			case ALI_WM_PWOER_OFF:
				break;
			case ALI_WM_SLEEP:
				break;
			case ALI_WM_USERINTERFACE:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_STOP) ;
				break;
			case ALI_WM_RANDOM:
				break;
			case ALI_WM_WALLFOLLOW:
				Remote_SetTempRemote(REMOTE_SIGNAL_AREA);
				break;
			case ALI_WM_SPOT:
				Remote_SetTempRemote(REMOTE_SIGNAL_SPOT);
				break;
			case ALI_WM_NAV:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_NAV);
				break;
			case ALI_WM_GOHOME:
				Remote_SetTempRemote(REMOTE_SIGNAL_GO_HOME);
				break;
			case ALI_WM_REMOTE:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_DIR);
				break;
			case ALI_WM_PAUSE:
				break;
			case ALI_WM_TEMP_SPOT:
				break;
			case ALI_WM_OTA:
				break;
			case ALI_WM_AREA:
				Remote_SetTempRemote(REMOTE_SIGNAL_AREA);
				break;
			case ALI_WM_CONTINUE:
				break;
			case ALI_WM_BACKHOME:
				Remote_SetTempRemote(REMOTE_SIGNAL_GO_HOME);
				Usprintf_Wifi("ALI_WM_BACKHOME \r\n");
				break;
			
			default:
				break;
		}
		Usprintf("---------->ali mode :%d \r\n",g_ali.set_workmode);
	}
}

void AliCloud_UpdateCleanDir(void)
{
	if(g_ali.update_clean_dir)
	{
		g_ali.update_clean_dir = ALI_FALSE;
		Usprintf_Wifi("ali update clean dir\n");
		switch(g_ali.clean_dir)
		{
			case ALI_CLEAN_DIR_FORWARD:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_FORWARD);
			break;
			case ALI_CLEAN_DIR_BACK:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_BACK);
			break;
			case ALI_CLEAN_DIR_LEFT:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_LEFT);
			break;
			case ALI_CLEAN_DIR_RIGHT:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_RIGHT);
			break;
			case ALI_CLEAN_DIR_PAUSE:
				Remote_SetTempRemote(REMOTE_SIGNAL_WIFI_PAUSE);
			break;
			default :break;
		}
	}
}

void AliCloud_UpdateCleanCycle(void)
{
	if(g_ali.update_clean_cycle)
	{
		g_ali.update_clean_cycle = ALI_FALSE;
		Usprintf_Wifi("ali update clean_cycle\n");

		g_cleancycle_count.set_cnt = g_ali.set_cleanloop_cnt;

		switch(g_cleancycle_count.set_cnt)
		{
			case 1:
			Speaker_Voice(SPK_ONE_TIME_CLEANING);
			break;
			case 2:
			Speaker_Voice(SPK_TWO_TIME_CLEANING);
			break;		
			default:
			break;
		}
	}
}

void AliCloud_UpdateResetFactory(void)
{
	if(g_ali.update_reset_factory)
	{
		g_ali.update_reset_factory = ALI_FALSE;
		Usprintf_Wifi("ali update reset_factory\n");
		Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
		g_ali.cur_wifi_state = ALI_WIFI_STATE_CLOUD_DISCONECTED;
		on_ali_set_power_state(OFF);
		g_ali.is_wifi_connected = 0;
		g_movement.base_spd = 100;
		g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;
		g_mainbrush.voltage = g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;
		g_cleancycle_count.set_cnt = 1;
		g_speaker.enable = 1;		
		Flash_WriteOneByte(FLASH_START_ADDRESS,0xFF);
	}	
}

void AliCloud_UpdateUnbanding(void)
{
	if(g_ali.update_unbanding)
	{
		g_ali.update_unbanding = ALI_FALSE;
		Usprintf_Wifi("ali update unbanding\n");
		Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
		g_ali.cur_wifi_state = ALI_WIFI_STATE_CLOUD_DISCONECTED;
		on_ali_set_power_state(OFF);
		g_ali.is_wifi_connected = 0;
		g_movement.base_spd = 100;
		g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_NORMAL;
		g_mainbrush.voltage = g_mainbrush.adjust_voltage = MAINBRUSH_VOLTAGE;
		g_cleancycle_count.set_cnt = 1;
		g_speaker.enable = 1;		
		Flash_WriteOneByte(FLASH_START_ADDRESS,0xFF);
	}	
}

void AliCloud_UpdateWifiState(void)
{
	if(g_ali.update_wifi_state)
	{
		g_ali.update_wifi_state = ALI_FALSE;
		Usprintf_Wifi("ali update_wifi_state\n");
		if(g_ali.cur_wifi_state == ALI_WIFI_STATE_CLOUD_DISCONECTED)
		{
			Display_SetWifiState(LED_MODE_STATIC,LED_STATE_OFF);
			g_ali.is_wifi_connected = ALI_FALSE;
		}
		if(g_ali.cur_wifi_state == ALI_WIFI_STATE_CLOUD_CONNOECTED)
		{
			if(g_ali.is_wifi_connecting)
			{
				g_ali.is_wifi_connecting  = ALI_FALSE;
				Speaker_Voice(SPK_WIFI_CONNECTED);
				Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
			}
			AliCloud_RefreshCloud();
			g_ali.is_wifi_connected = ALI_TRUE;
		}	           	
	}	
}

void AliCloud_SaveState(void)
{
	uint8_t flash_data = 0;
	if(g_ali.cur_wifi_state == ALI_WIFI_STATE_CLOUD_CONNOECTED)
	{
		flash_data = Flash_ReadByte(FLASH_START_ADDRESS);
		if(flash_data != WIFI_FLASH_VALUE)
		{
			Flash_WriteOneByte(FLASH_START_ADDRESS,WIFI_FLASH_VALUE);
		}
	}
}


void AliCloud_UpdateTimeZone(void)
{
	if(g_ali.update_time_zone)
	{
		g_ali.update_time_zone = ALI_FALSE;
		Usprintf_Wifi("ali update_time_zone\n");
		AliCloud_RefreshCloud();
	}
}

void AliCloud_UpdateVoiceMute(void)
{
	if(g_ali.update_voice_mute)
	{
		g_ali.update_voice_mute = ALI_FALSE;
		Usprintf_Wifi("ali update_voice_mute\n");
		if(g_ali.voice_mute_set_state)
		{
			Speaker_Voice(SPK_VOICE_DISABLE);
			g_speaker.enable = 0;		
		}
		else
		{		
			g_speaker.enable = 1;
			Speaker_Voice(SPK_VIOCE_ENABLE);
		}	
	}	
}

void AliCloud_UpdateMainBrush(void)
{
	if(g_ali.update_mainbrush)
	{
		g_ali.update_mainbrush = ALI_FALSE;

		Usprintf_Wifi("ali update_mainbrush\n");

		g_mainbrush.voltage = MAINBRUSH_BASE_VOLTAGE + (MAINBRUSH_SPOT_VOLTAGE - MAINBRUSH_BASE_VOLTAGE)*g_ali.set_mainbrush_spd/100;
		
		g_mainbrush.adjust_voltage = g_mainbrush.voltage;		
	}
}

void AliCloud_UpdateWaterFlow(void)
{
	if(g_ali.update_waterflow)
	{
		g_ali.update_waterflow = ALI_FALSE;

		Usprintf_Wifi("ali update_waterflow\n");

		g_pump.cycle_cnt = PUMP_WATER_FLOW_LEVEL_lOW - g_ali.set_pump_spd * 10;
	}
}

void AliCloud_UpdateMoveSpeed(void)
{
	if(g_ali.update_mov_speed)
	{
		g_ali.update_mov_speed = ALI_FALSE;

		Usprintf_Wifi("ali update_mov_speed\n");

		g_movement.base_spd = g_ali.set_mov_spd;		
	}
}

void AliCloud_UpdateTimeCalculation(void)
{
	if(g_ali.update_time_calibration)
	{
		g_ali.update_time_calibration = ALI_FALSE;

		AliCloud_RefreshCloud();
		Usprintf_Wifi("ali update_time_calibration:%d %d\n",g_ali.time_calibration.utc_zero_zone,g_ali.time.utc);	
	}
}

void AliCloud_UpdateStates(void)
{
	AliCloud_UpdateWifiState();
	AliCloud_UpdateWorkMode();
	AliCloud_UpdateCleanDir();
	AliCloud_UpdateCleanCycle();
	AliCloud_UpdateResetFactory();	
	AliCloud_UpdateTimeZone();
	AliCloud_UpdateVoiceMute();
	AliCloud_UpdateMainBrush();
	AliCloud_UpdateWaterFlow();
	AliCloud_UpdateMoveSpeed();
	AliCloud_UpdateUnbanding();
	AliCloud_UpdateTimeCalculation();
}

void AliCloud_CheckWifiDisplayState(void)
{
	if(g_ali.cur_workmode != ALI_WM_SLEEP)
	{
		if(g_ali.is_working)
		{
			if(g_display.wifi_led_state == LED_STATE_OFF)
			{
				if(Mode_GetMode() != MODE_TEST)
				{
					if(g_ali.cur_wifi_state != ALI_WIFI_STATE_CLOUD_DISCONECTED)
					{
						Display_SetWifiState(LED_MODE_STATIC,LED_STATE_GREEN);
						Usprintf_Wifi("ali light up wifi led\n");
					}
				}
			}
		}
	}
}





