
#include "Include.h"

AliCloudTx_UsartSend on_ali_tx_usart_send;
AliCloud_MapGetCell on_ali_tx_get_cell;
AliStruct_t *p_ali_tx;

void AliCloudTx_Init(void)
{
	on_ali_tx_usart_send = USART6_DMA_String;	
	on_ali_tx_get_cell = Map_GetCell;	
	p_ali_tx = &g_ali;		
}

void AliCloudTx_ListReset(void)
{
	p_ali_tx->tx_queue_msg_cnt = 0;
	p_ali_tx->tx_queue_msg_input = 0;
	p_ali_tx->tx_queue_msg_output = 0;    
}

uint8_t AliCloudTx_IsListError(void)
{
	if ((p_ali_tx->tx_queue_msg_input != p_ali_tx->tx_queue_msg_output) && !p_ali_tx->tx_queue_msg_cnt)
	{
		return 1;
	}	
	return 0;
}

uint8_t AliCloudTx_ListPush(AliTxMsgStruct_t msg)
{
	if(p_ali_tx->tx_stop)return 0;

	if (p_ali_tx->tx_queue_msg_cnt > ALI_TX_QUEUE_MAX)return 0;

	p_ali_tx->tx_queue_list[p_ali_tx->tx_queue_msg_input] = msg;	
	p_ali_tx->tx_queue_msg_input++;
	p_ali_tx->tx_queue_msg_cnt++;

	if (p_ali_tx->tx_queue_msg_input > ALI_TX_QUEUE_MAX)p_ali_tx->tx_queue_msg_input = 0;
	
	return 1;
}

uint8_t AliCloudTx_ListShift(AliTxMsgStruct_t *msg)
{
	if (!p_ali_tx->tx_queue_msg_cnt)return 0;
	
	*msg = p_ali_tx->tx_queue_list[p_ali_tx->tx_queue_msg_output];
	
	p_ali_tx->tx_queue_msg_cnt--;
	p_ali_tx->tx_queue_msg_output++;

	if (p_ali_tx->tx_queue_msg_output > ALI_TX_QUEUE_MAX)p_ali_tx->tx_queue_msg_output = 0;
	return 1;
}

uint8_t AliCloudTx_GetListCnt(void)
{
	return p_ali_tx->tx_queue_msg_cnt;
}

uint16_t AliCloudTx_SendData(void)
{
	if(p_ali_tx->tx_size >= ALI_TX_BUF_MAX)return 0;

    on_ali_tx_usart_send(p_ali_tx->tx_size,p_ali_tx->tx_buf);

	return 1;
}

uint16_t AliCloudTx_GetSize(void)
{
	return p_ali_tx->tx_size;
}

void AliCloudTx_ClearSize(void)
{
	p_ali_tx->tx_size = 0;
}

uint8_t AliCloudTx_AttrIdTpTypeId(AliAttrIdType_t attrid)
{
	switch(attrid)
	{
		case ALI_ATTRID_BATTERYSTATE    :return 10;
		case ALI_ATTRID_WORKMODE        :return  2;
		case ALI_ATTRID_PAUSESWITCH     :return  0;
		case ALI_ATTRID_POWERSWITCH     :return  0;
		case ALI_ATTRID_DIRECTION       :return  2;
		case ALI_ATTRID_SCHEDU_1        :return 30;
		case ALI_ATTRID_SCHEDU_2        :return 31;
		case ALI_ATTRID_SCHEDU_3        :return 32;
		case ALI_ATTRID_SCHEDU_4        :return 33;
		case ALI_ATTRID_SCHEDU_5        :return 34;
		case ALI_ATTRID_SCHEDU_6        :return 35;
		case ALI_ATTRID_SCHEDU_7        :return 36;
		case ALI_ATTRID_MAXMODE         :return  2;
		case ALI_ATTRID_WATERTANKLEVEL  :return  2;
		case ALI_ATTRID_LIFE            :return 37;
		case ALI_ATTRID_BEEPVOLUME      :return  2;
		case ALI_ATTRID_BEEPTYPE        :return  2;
		case ALI_ATTRID_N0_DISTURB      :return 42;
		case ALI_ATTRID_FINDROBOT       :return  0;
		case ALI_ATTRID_CARPETCONTROL   :return  2;
		case ALI_ATTRID_CLEANTYPE       :return  2;
		case ALI_ATTRID_REALMAPSTART    :return  5;
		case ALI_ATTRID_REALMAP         :return 38;
		case ALI_ATTRID_HISTORYSTARTTING:return  5;
		case ALI_ATTRID_HISTORY         :return 39;
		case ALI_ATTRID_CHARGER_POSITION:return 43;
		case ALI_ATTRID_VW_ENABLE       :return  0;
		case ALI_ATTRID_VW_DATA         :return 40;
		case ALI_ATTRID_PARTITIONDATA   :return 41;
		case ALI_ATTRID_WIFI_INFO       :return 44;
		case ALI_ATTRID_ROBOT_INFO      :return 45;
		case ALI_ATTRID_OTA_INFO        :return 46;
		case ALI_ATTRID_TIMEZONE        :return 47;
		case ALI_ATTRID_RESFACTORY      :return  0;
		case ALI_ATTRID_WIFI_STATE      :return  2;
		case ALI_ATTRID_WATERCONTROL    :return  2;
		case ALI_ATTRID_WHEELSPEED      :return  2;
		case ALI_ATTRID_MAINBRUSHPOWER  :return  2;
		case ALI_ATTRID_SIDRBRUSHPOWER  :return  2;
		case ALI_ATTRID_FANPOWER        :return  2;
		case ALI_ATTRID_CLEANLOOPCNT    :return  78;
		case ALI_ATTRID_AREA_SET        :return 80;	
		case ALI_ATTRID_WIFI_MAC 		:return 0;
		case ALI_ATTRID_FACTORY_TEST 	:return 0;
		case ALI_ATTRID_TIME_CALIBRATION:return 89;
		default:return  0;
	}
}

void AliCloudTx_InSertPayload(AliTxMsgStruct_t msg)
{
    uint8_t check_sum = 0;
	uint16_t msg_idx = 0;
	char *p_msg = &p_ali_tx->tx_buf[p_ali_tx->tx_size];	

	p_msg[msg_idx++] = ALI_HEADER;
	
	msg_idx += 2;   

	p_msg[msg_idx++] = ALI_VERSION;
	p_msg[msg_idx++] = ALI_CMD_TX;
	p_msg[msg_idx++] = msg.method;

	p_msg[msg_idx++] = (msg.seq_num>>24)&0xff;
	p_msg[msg_idx++] = (msg.seq_num>>16)&0xff;
	p_msg[msg_idx++] = (msg.seq_num>>8)&0xff;
	p_msg[msg_idx++] = (msg.seq_num>>0)&0xff;

	if (msg.method == ALI_METHOD_REPORT_ATTR)
	{
		p_msg[msg_idx++] = AliCloudTx_AttrIdTpTypeId(msg.attr_id);
		p_msg[msg_idx++] = msg.attr_id;
		
		switch (msg.attr_id)
		{
			case ALI_ATTRID_BATTERYSTATE:	
				{				
					double temp_bat = 0;
					if(p_ali_tx->battery_pct > BATTERY_LEVEL_LOW)
					{
						temp_bat = (double)((p_ali_tx->battery_pct - BATTERY_LEVEL_LOW) * 100/(100 - BATTERY_LEVEL_LOW));
					}		
					AliCloudTx_PayloadData_Double_t(&p_msg[msg_idx],temp_bat);
					msg_idx += 8;						
				}
				break;			
			case ALI_ATTRID_WORKMODE :
				p_msg[msg_idx++] = p_ali_tx->cur_workmode;										
				break;				
			case ALI_ATTRID_PAUSESWITCH :
				break;				
			case ALI_ATTRID_POWERSWITCH :
				break;	
			case ALI_ATTRID_DIRECTION :	
				break;	
			case ALI_ATTRID_SCHEDU_1 :
			case ALI_ATTRID_SCHEDU_2 :				
			case ALI_ATTRID_SCHEDU_3 :				
			case ALI_ATTRID_SCHEDU_4 :							
			case ALI_ATTRID_SCHEDU_5 :			
			case ALI_ATTRID_SCHEDU_6 :				
			case ALI_ATTRID_SCHEDU_7 :
				break;	
			case ALI_ATTRID_MAXMODE :				
				break;	
			case ALI_ATTRID_WATERTANKLEVEL:				
				break;	
			case ALI_ATTRID_LIFE :
				
				break;
			case ALI_ATTRID_BEEPVOLUME :
				
				break;
			case ALI_ATTRID_BEEPTYPE :
				
				break;
			case ALI_ATTRID_N0_DISTURB :
				{
					p_msg[msg_idx++] = p_ali_tx->voice_mute_cur_state;

					p_msg[msg_idx++] = p_ali_tx->voice_mute.start_hour;

					p_msg[msg_idx++] = p_ali_tx->voice_mute.start_min;

					p_msg[msg_idx++] = p_ali_tx->voice_mute.end_hour;

					p_msg[msg_idx++] = p_ali_tx->voice_mute.end_min;
				}			
				break;

			case ALI_ATTRID_FINDROBOT :
				
				break;

			case ALI_ATTRID_CARPETCONTROL:
				
				break;

			case ALI_ATTRID_CLEANTYPE :
				
				break;

			case ALI_ATTRID_REALMAPSTART :
				{
					uint32_t curtime = p_ali_tx->time.utc + p_ali_tx->time_zone * 3600;
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx],curtime);
					msg_idx += sizeof(curtime);
				}
				break;

			case ALI_ATTRID_REALMAP :				
				{									
					uint16_t heading = 360 - p_ali_tx->robot_angle/10;
					uint16_t cleaned_area = ((uint16_t)(p_ali_tx->real_map.cleaned_area*ALI_REALMAP_RATE));
					uint16_t cells_size = p_ali_tx->real_map.list_cnt;
					Usprintf_Wifi("ali real map tx size:%d area:%d %d\n",cells_size,p_ali_tx->real_map.cleaned_area,cleaned_area);	
					if(heading == 360)heading = 0;
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],p_ali_tx->robot_cell.x);
					msg_idx += 2;
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],p_ali_tx->robot_cell.y);
					msg_idx += 2;
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],heading);
					msg_idx += 2;					
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],cleaned_area);
					msg_idx += 2;
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx], (uint16_t)p_ali_tx->real_map.cleaned_time);
					msg_idx += 2;	
					
					p_msg[msg_idx++] = 0; 
					p_msg[msg_idx++] = 0; 
					p_msg[msg_idx++] = 0; 
					p_msg[msg_idx++] = 0; 
					p_msg[msg_idx++] = 0;
					
					p_ali_tx->real_map.list_busy = ALI_TRUE;
					if (cells_size)
					{
						for(uint16_t i = 0;i < cells_size;++i)
						{
							Point16_t temp_send_cell;
							CellState_t temp_cell_state;
							AliCloudTx_RealMapListShift(&temp_send_cell);
							
							temp_cell_state = on_ali_tx_get_cell(temp_send_cell.x,temp_send_cell.y);

							AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],temp_send_cell.x);
							msg_idx += 2;
							AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],temp_send_cell.y);
							msg_idx += 2;

							//Usprintf_Wifi("ali send real cell:%d %d %d\n",temp_send_cell.x,temp_send_cell.y,temp_cell_state);		

							if(temp_cell_state == CELL_STATE_CLEANED)
							{
								p_msg[msg_idx++] = 10;
							}
							else if(temp_cell_state == CELL_STATE_CLEANED_TWICE)
							{
								p_msg[msg_idx++] = 11;
							}
							else if (temp_cell_state == CELL_STATE_BLOCKED)
							{
								p_msg[msg_idx++] = 2;
							}														
							else
							{
								p_msg[msg_idx++] = temp_cell_state;
							}
																															
							if(msg_idx >= ALI_REAL_MAP_MAX_CNT)
							{
								Usprintf_Wifi("ALI_REAL_MAP_MAX_CNT ,break\n");
								break;
							}							
						}
					}
					p_ali_tx->real_map.list_busy = ALI_FALSE;						
				}
				break;

			case ALI_ATTRID_HISTORYSTARTTING: 
				{
					uint32_t curtime = p_ali_tx->history_map.start_time;  
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx], curtime);
					msg_idx += sizeof(curtime);
					Usprintf("send history start time to cloud:%d\n", curtime);
				}				
				break;

			case ALI_ATTRID_HISTORY :				
				{
					p_ali_tx->history_map.list_busy  = ALI_TRUE;
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx],p_ali_tx->history_map.start_time);
					msg_idx += 4;
					p_msg[msg_idx++] = p_ali_tx->history_map.start_rsn; 
					p_msg[msg_idx++] = p_ali_tx->history_map.stop_rsn;	
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],p_ali_tx->history_map.total_area);
					msg_idx += 2;
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],p_ali_tx->history_map.total_time);
					msg_idx += 2;
					p_msg[msg_idx++] = p_ali_tx->history_map.packet_id;
					p_msg[msg_idx++] = p_ali_tx->history_map.packet_num;
					p_msg[msg_idx++] = 0;
					p_msg[msg_idx++] = 0;
					p_msg[msg_idx++] = 0;
					p_msg[msg_idx++] = 0;
					p_msg[msg_idx++] = 1;
					
					p_msg[msg_idx++] = (p_ali_tx->history_map.origin_cell.x >> 8) & 0xff; 
					p_msg[msg_idx++] = (p_ali_tx->history_map.origin_cell.x >> 0) & 0xff; 
					p_msg[msg_idx++] = (p_ali_tx->history_map.origin_cell.y >> 8) & 0xff; 
					p_msg[msg_idx++] = (p_ali_tx->history_map.origin_cell.y >> 0) & 0xff;
					
					AliCloudTx_PayloadData_Uint16_t(&p_msg[msg_idx],p_ali_tx->history_map.row_width);
					msg_idx += 2;	
					while(p_ali_tx->history_map.list_cnt)
					{
						AliMapCompress_t temp_map_compress;
						AliCloudTx_HistoryMapListShift(&temp_map_compress);
						p_msg[msg_idx++] = p_ali_tx->history_map.type;	
						if(temp_map_compress.status == CELL_STATE_CLEANED)
						{
							p_msg[msg_idx++] = 10;	
						}
						else if (temp_map_compress.status == CELL_STATE_CLEANED_TWICE)
						{
							p_msg[msg_idx++] = 11;
						}
						else if (temp_map_compress.status == CELL_STATE_BLOCKED)
						{
							p_msg[msg_idx++] = 2;
						}
						else
						{
							p_msg[msg_idx++] = temp_map_compress.status;
						}														
						p_msg[msg_idx++] = temp_map_compress.length;							
					}
					p_ali_tx->history_map.packet_id++;
					p_ali_tx->history_map.list_busy  = ALI_FALSE;
					p_ali_tx->history_map.is_last_pocket_finish = 1;
				}																								
				break;

			case ALI_ATTRID_CHARGER_POSITION :
				
				break;

			case ALI_ATTRID_VW_ENABLE :
				
				break;

			case ALI_ATTRID_VW_DATA :
				
				break;

			case ALI_ATTRID_PARTITIONDATA :
				
				break;

			case ALI_ATTRID_WIFI_INFO :

				break;

			case ALI_ATTRID_ROBOT_INFO :
				{
					p_ali_tx->robot_info.robot_type = 0x45;	
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx],p_ali_tx->robot_info.robot_type);
					msg_idx += 4;	
					p_ali_tx->robot_info.hardware_version = 0x401;//0x401	
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx],p_ali_tx->robot_info.hardware_version);
					msg_idx += 4;
					p_ali_tx->robot_info.software_version = 0x02;	
					AliCloudTx_PayloadData_Uint32_t(&p_msg[msg_idx],p_ali_tx->robot_info.software_version);
					msg_idx += 4;	
				}					
				break;

			case ALI_ATTRID_OTA_INFO :
				
				break;

			case ALI_ATTRID_TIMEZONE :
				
				break;

			case ALI_ATTRID_RESFACTORY :	
				
				break;

			case ALI_ATTRID_UTC :
				
				break;

			case ALI_ATTRID_UNBANDING :
				
				break;

			case ALI_ATTRID_FACTORY_TEST:
				
				break;	
			case ALI_ATTRID_WIFI_MAC:

				break;
			case ALI_ATTRID_WATERCONTROL:
				p_msg[msg_idx++] = p_ali_tx->cur_pump_spd;
				break;
			case ALI_ATTRID_WHEELSPEED:
				p_msg[msg_idx++] = p_ali_tx->cur_mov_spd;
				break;

			case ALI_ATTRID_MAINBRUSHPOWER:
				p_msg[msg_idx++] = p_ali_tx->cur_mainbrush_spd;
				break;		

			case ALI_ATTRID_SIDRBRUSHPOWER:
				break;

			case ALI_ATTRID_FANPOWER:
	
				break;		

			case ALI_ATTRID_CLEANLOOPCNT:
				p_msg[msg_idx++] = 1;
				p_msg[msg_idx++] = p_ali_tx->over_cleanloop_cnt;
				p_msg[msg_idx++] = p_ali_tx->cur_set_cleanloop_cnt;
				p_msg[msg_idx++] = p_ali_tx->over_dryloop_cnt;
				p_msg[msg_idx++] = p_ali_tx->cur_set_dryloop_cnt;				
				break;
			case ALI_ATTRID_AREA_SET:
						
				break;				
			default:
				break;

		}
	}
	else if (msg.method == ALI_METHOD_ACK)
	{
		p_msg[msg_idx++] = 200;
	}
	else if (msg.method == ALI_METHOD_REPORT_EVENT)
	{
		p_msg[msg_idx++] = msg.event_id;
		
		switch(msg.event_id)
		{
			case ALI_EVENT_ERROR:								 
				if(p_ali_tx->error_rsn == ERROR_LOW_BATTERY)p_ali_tx->error_rsn = ERROR_NONE;							 
				p_msg[msg_idx++] = p_ali_tx->error_rsn;								 
				break;
			default:
				break;
		}
	}
	else if (msg.method == ALI_METHOD_SYSTEM_GET)
	{
		p_msg[msg_idx++] = msg.attr_id;
		Usprintf_Wifi("ali get state:%d\n",msg.attr_id);
	}
	else if (msg.method == ALI_METHOD_SYSTEM_SET)
	{
		p_msg[msg_idx++] = AliCloudTx_AttrIdTpTypeId(msg.attr_id);
		p_msg[msg_idx++] = msg.attr_id;
		switch(msg.attr_id)
		{
			case ALI_ATTRID_WIFI_STATE:
				p_msg[msg_idx++] = p_ali_tx->set_wifi_state;
				break;
			case ALI_ATTRID_FACTORY_TEST:
				p_msg[msg_idx++] = p_ali_tx->factory_test_mode;
				p_ali_tx->factorytest_seqnum = msg.seq_num;
				break;				
			default:
				break;
		}
		
	}

	p_msg[ALI_PROTOCOL_LENGHT_H] = ((msg_idx-2)>>8) & 0xff;
	p_msg[ALI_PROTOCOL_LENGHT_L] = ((msg_idx-2)>>0) & 0xff;
	
	check_sum = Math_CalArraySum(p_msg,msg_idx);
	p_msg[msg_idx++] = check_sum;
	
	p_ali_tx->tx_size += msg_idx;
}

void AliCloudTx_PayloadData_Uint32_t(char *p_data,uint32_t data)
{
	p_data[0] = (data >> 24) & 0xff;
	p_data[1] = (data >> 16) & 0xff;
	p_data[2] = (data >> 8) & 0xff;
	p_data[3] = (data >> 0) & 0xff;
}

void AliCloudTx_PayloadData_Uint16_t(char *p_data,uint16_t data)
{
	p_data[0] = (data >> 8) & 0xff;
	p_data[1] = (data >> 0) & 0xff;
}

void AliCloudTx_PayloadData_Double_t(char *p_data,double data)
{
	union Double_t
	{
		double double_data;
		uint64_t byte_data;
	}input_double_data;
	uint8_t size,i,j;
	input_double_data.double_data = data; 
	size = sizeof(double);	
	for (i = 0;i < size ;i++) //0--7
	{
		j = (size - i - 1)*8;
		p_data[i] = (uint8_t)(input_double_data.byte_data >> j);
	}	
}

void AliCloudTx_RealMapListReset(void)
{
	p_ali_tx->real_map.list_cnt = 0;
	p_ali_tx->real_map.list_input = 0;
	p_ali_tx->real_map.list_output = 0;
	p_ali_tx->real_map.list_busy = 0;  
	AliCloudTx_RealMapBusyListReset();
}

uint8_t AliCloudTx_RealMapListPush(Point16_t point)
{
	if(p_ali_tx->real_map.list_busy)
	{
		AliCloudTx_RealMapBusyListPush(point);
		return 0;		
	}

	if (p_ali_tx->real_map.list_cnt > ALI_REALMAP_SIZE)
	{
		return 0;
	}	

	p_ali_tx->real_map.list[p_ali_tx->real_map.list_input] = point;
		
	p_ali_tx->real_map.list_input++;
	p_ali_tx->real_map.list_cnt++;

	if (p_ali_tx->real_map.list_input > ALI_REALMAP_SIZE)p_ali_tx->real_map.list_input = 0;
	
	return 1;
}

uint8_t AliCloudTx_RealMapListShift(Point16_t *point)
{
	if (!p_ali_tx->real_map.list_cnt)return 0;
	
	*point = p_ali_tx->real_map.list[p_ali_tx->real_map.list_output];
	
	p_ali_tx->real_map.list_cnt--;
	p_ali_tx->real_map.list_output++;
	
	if (p_ali_tx->real_map.list_output > ALI_REALMAP_SIZE)p_ali_tx->real_map.list_output = 0;
	return 1;
}

void AliCloudTx_RealMapBusyListReset(void)
{
	p_ali_tx->real_map.busy_list_cnt = 0;
	p_ali_tx->real_map.busy_list_input = 0;
	p_ali_tx->real_map.busy_list_output = 0;
}

uint8_t AliCloudTx_RealMapBusyListPush(Point16_t point)
{
	if (p_ali_tx->real_map.busy_list_cnt > ALI_REALMAP_BUSY_SIZE)
	{
		return 0;
	}	
	p_ali_tx->real_map.busy_list[p_ali_tx->real_map.busy_list_input] = point;
		
	p_ali_tx->real_map.busy_list_input++;
	p_ali_tx->real_map.busy_list_cnt++;

	if (p_ali_tx->real_map.busy_list_input > ALI_REALMAP_BUSY_SIZE)p_ali_tx->real_map.busy_list_input = 0;
	
	return 1;
}

uint8_t AliCloudTx_RealMapBusyListShift(Point16_t *point)
{
	if (!p_ali_tx->real_map.busy_list_cnt)return 0;
	
	*point = p_ali_tx->real_map.busy_list[p_ali_tx->real_map.busy_list_output];
	
	p_ali_tx->real_map.busy_list_cnt--;
	p_ali_tx->real_map.busy_list_output++;
	
	if (p_ali_tx->real_map.busy_list_output > ALI_REALMAP_BUSY_SIZE)p_ali_tx->real_map.busy_list_output = 0;
	return 1;
}

void AliCloudTx_HistoryMapListReset(void)
{
	p_ali_tx->history_map.list_cnt = 0;
	p_ali_tx->history_map.list_input = 0;
	p_ali_tx->history_map.list_output = 0;
	p_ali_tx->history_map.list_busy = 0;  
}

uint8_t AliCloudTx_HistoryMapListPush(AliMapCompress_t input_data)
{
	if(p_ali_tx->history_map.list_busy )
	{
		return 0;		
	}

	if (p_ali_tx->history_map.list_cnt > ALI_HISTORY_MAP_SIZE)
	{
		return 0;
	}	
	p_ali_tx->history_map.list[p_ali_tx->history_map.list_cnt] = input_data;
		
	p_ali_tx->history_map.list_input++;
	p_ali_tx->history_map.list_cnt++;

	if (p_ali_tx->history_map.list_input > ALI_HISTORY_MAP_SIZE)p_ali_tx->history_map.list_input = 0;
	
	return 1;
}

uint8_t AliCloudTx_HistoryMapListShift(AliMapCompress_t *output_data)
{
	if (!p_ali_tx->history_map.list_cnt)return 0;
	
	*output_data = p_ali_tx->history_map.list[p_ali_tx->history_map.list_output];
	
	p_ali_tx->history_map.list_cnt--;
	p_ali_tx->history_map.list_output++;

	if (p_ali_tx->history_map.list_output > ALI_HISTORY_MAP_SIZE)p_ali_tx->history_map.list_output = 0;
	return 1;
}

uint8_t AliCloudTx_HistoryMapListFull(void)
{
	if (p_ali_tx->history_map.list_cnt == ALI_HISTORY_MAP_SIZE)
	{
		return 1;
	}
	return 0;
}































