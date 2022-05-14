#include "Include.h"

AliStruct_t *p_ali_rx;
AliCloud_InsertTheAck on_ali_rx_insert_ack;

void AliCloudRx_Init(void)
{
	p_ali_rx = &g_ali;	
	on_ali_rx_insert_ack = AliCloud_InsertAck;	
}

void AliCloudRx_ParseMsg(char * data_fifo, uint16_t length)
{
	static uint8_t pre_reserve_flag = 0;
	static uint8_t pre_reserve_cnt = 0;	
 	int16_t index = 0,frame_length = 0,start_index = -1,last_index = -1;   
	//uint8_t protocol_ver = 0;
	uint8_t check_sum = 0;

	if(pre_reserve_flag == 1)
	{
		pre_reserve_flag = 0;
		for(index = (length + pre_reserve_cnt - 1);index >= pre_reserve_cnt; index--)
		{	
			if(index > ALI_RX_BUF_SIZE)break;
			if(index < 0)index = 0;
			data_fifo[index] = data_fifo[index - pre_reserve_cnt];
		}
		
		for(index = 0;index < pre_reserve_cnt; index++)
		{
			if(index > ALI_RX_BUF_SIZE)break;
			if(index < 0)index = 0;
			data_fifo[index] = p_ali_rx->rx_buffer[index];
		}
		length = ((length + pre_reserve_cnt) <= ALI_RX_BUF_SIZE)? (length + pre_reserve_cnt):ALI_RX_BUF_SIZE;
	}	
	
    for(index = 0;index < length; index++)
    {
        if(data_fifo[index] == 0xAA)
        {
            start_index = index;
			if(index >= (ALI_RX_BUF_TEMP_SIZE - 2))
			{
				//Usprintf_Wifi("ali index over buffer size\n");
				continue;
			}	
			/*		
            protocol_ver = data_fifo[start_index + 3];
			
            if(protocol_ver >= ALI_PROTOCOL_VER) //check protocol version
            {
                frame_length = (data_fifo[start_index + 1] << 8) + data_fifo[start_index + 2];
                //Usprintf("protocol ver:%x frame_length:%d \n",protocol_ver,frame_length);  
            } 
            else
            {
                //Usprintf("protocol ver error:%x\n",protocol_ver);
                continue;               
            }
			*/
            frame_length = (data_fifo[index + 1] << 8) + data_fifo[index + 2];

 			if((index + frame_length - 1) > ALI_RX_BUF_SIZE)
			{
				//Usprintf_Wifi("ali frame_length over buffer size\n");
				continue;
			}
			else if(frame_length < 3)
			{
				//Usprintf_Wifi("ali frame_length lower length\n");
				continue;
			}              
            check_sum = Math_CalArraySum(&data_fifo[index],frame_length + 2);                
            if(check_sum == data_fifo[index + frame_length + 2])
            {			
				AliCloudRx_ParsePacket(&data_fifo[index]);
				last_index = index + frame_length + 2;
				index = last_index;
							
					
            }         
        }
    }
	
	if((last_index != (length - 1)) && (start_index != -1))
	{
		pre_reserve_flag = 1;
		pre_reserve_cnt = 0;

		index = (last_index != -1) ? (last_index + 1) : start_index;
		
		for(;index < length; index++)
		{
			if(index < 0)break;
			if(index > ALI_RX_BUF_SIZE)break;
			
			p_ali_rx->rx_buffer[pre_reserve_cnt] = data_fifo[index];
			if(pre_reserve_cnt < ALI_RX_BUF_PRE_SIZE)
			{
				pre_reserve_cnt++;
			}
			else
			{
				break;
			}
		}		
	}	
}

void AliCloudRx_ParsePacket(char *rec_data)
{
	p_ali_rx->is_working = TRUE;
	AliCloudRx_DecodePacket(&p_ali_rx->rx_packet,rec_data);
	if (p_ali_rx->rx_packet.method != ALI_POCKET_METHOD_ACK)
	{	
		on_ali_rx_insert_ack(&p_ali_rx->rx_packet);
	}
	switch(p_ali_rx->rx_packet.method)
	{
		case ALI_POCKET_METHOD_SET:	
			AliCloudRx_ParseStreamSet(&p_ali_rx->rx_packet);	
			break;

		case ALI_POCKET_METHOD_ACK:
			AliCloudRx_ParseStreamAck(&p_ali_rx->rx_packet);
			break;
		default:break;
	}
}

void AliCloudRx_DecodePacket(AliPacketStruct_t  *packet,char *rec_data)
{
	packet->cmd = rec_data[4];
	packet->method = rec_data[5];
	packet->seq_num = ((rec_data[6] << 24) | (rec_data[7] << 16)| (rec_data[8] << 8) | rec_data[9]);
	//Usprintf_Wifi("cmd:%d method:%d seq_num:%d\n",packet->cmd,packet->method,packet->seq_num);
	switch(packet->method)
	{
		case ALI_POCKET_METHOD_GET:
			packet->attr_id = rec_data[10];	
			packet->p_data = &rec_data[11];
			break;
		case ALI_POCKET_METHOD_SET:
			packet->type_id = rec_data[10];
			packet->attr_id = rec_data[11];
			packet->p_data = &rec_data[12];
			break;
		case ALI_POCKET_METHOD_SERVICE: 
			break;
		case ALI_POCKET_METHOD_ACK: 
			packet->ack = rec_data[10];
			break;
		default:
			break;
	}	
}

void AliCloudRx_ParseStreamAck(AliPacketStruct_t  *packet)
{
	p_ali_rx->ack_seqnum = packet->seq_num;
	p_ali_rx->ack_ok = (packet->ack == 200)? 1 : 0;
	if(p_ali_rx->factory_test_mode)
	{
		if(p_ali_rx->ack_seqnum == p_ali_rx->factorytest_seqnum)
		{
			p_ali_rx->factorytest_ack_ok = p_ali_rx->ack_ok;
		}
	}	
}

void AliCloudRx_ParseStreamSet(AliPacketStruct_t  *packet)
{
	switch(packet->attr_id)
	{
	case ALI_ATTRID_WORKMODE: 
		AliCloudRx_Parse_RxLink_WorkMode(packet->p_data);
		break;
	case ALI_ATTRID_PAUSESWITCH:

		break;
	case ALI_ATTRID_POWERSWITCH:

		break;
	case ALI_ATTRID_DIRECTION: 
		AliCloudRx_Parse_RxLink_RemoteDir(packet->p_data);
		break;
	case ALI_ATTRID_SCHEDU_1: 
	case ALI_ATTRID_SCHEDU_2: 
	case ALI_ATTRID_SCHEDU_3: 
	case ALI_ATTRID_SCHEDU_4: 	
	case ALI_ATTRID_SCHEDU_5: 		
	case ALI_ATTRID_SCHEDU_6:		
	case ALI_ATTRID_SCHEDU_7: 

		break;		
	case ALI_ATTRID_MAXMODE:

		break;		
	case ALI_ATTRID_WATERTANKLEVEL :

		break;		
	case ALI_ATTRID_LIFE :

		break;		
	case ALI_ATTRID_BEEPVOLUME :

		break;		
	case ALI_ATTRID_BEEPTYPE :

		break;		
	case ALI_ATTRID_N0_DISTURB :
		AliCloudRx_Parse_RxLink_BeepNoDisturb(packet->p_data);
		break;		
	case ALI_ATTRID_FINDROBOT :
		AliCloudRx_Parse_RxLink_FindRobot(packet->p_data);
		break;		
	case ALI_ATTRID_CARPETCONTROL :

		break;
	case ALI_ATTRID_CLEANTYPE :

		break;		
	case ALI_ATTRID_WIFI_INFO:

		break;		
	case ALI_ATTRID_ROBOT_INFO:

		break;		
	case ALI_ATTRID_OTA_INFO :

		break;		
	case ALI_ATTRID_TIMEZONE: 
		AliCloudRx_Parse_RxLink_TimeZone(packet->p_data);
		break;		
	case ALI_ATTRID_RESFACTORY:
		AliCloudRx_Parse_RxLink_ResFactory(packet->p_data);
		break;		
	case ALI_ATTRID_WIFI_STATE :
		AliCloudRx_Parse_RxLink_WifiState(packet->p_data);
		break;		
	case ALI_ATTRID_UTC :
		AliCloudRx_Parse_RxLink_Utc(packet->p_data);
		break;		
	case ALI_ATTRID_UNBANDING :
		AliCloudRx_Parse_RxLink_Unbanding(packet->p_data);
		break;		
	case ALI_ATTRID_FACTORY_TEST: 
		AliCloudRx_Parse_RxLink_FactoryTest(packet->p_data);
		break;		
	case ALI_ATTRID_WIFI_MAC: 
		AliCloudRx_Parse_RxLink_WifiMac(packet->p_data);
		break;		
	case ALI_ATTRID_WATERCONTROL:
		AliCloudRx_Parse_RxLink_WaterControl(packet->p_data);
		break;
	case ALI_ATTRID_WHEELSPEED:
		AliCloudRx_Parse_RxLink_WheelSpeed(packet->p_data);	
		break;
	case ALI_ATTRID_MAINBRUSHPOWER:
		AliCloudRx_Parse_RxLink_MainBrushPower(packet->p_data);	
		break;
	case ALI_ATTRID_FANPOWER :
		break;
	case ALI_ATTRID_CLEANLOOPCNT:
		AliCloudRx_Parse_RxLink_CleanLoopCnt(packet->p_data);		
		break;
	case ALI_ATTRID_AREA_SET:

		break;	
	case ALI_ATTRID_TIME_CALIBRATION:	
		AliCloudRx_Parse_RxLink_TimeCalibration(packet->p_data);
		break;
	default:
		break;
	}
}

uint32_t AliCloudRx_GetUtcWithDay(AliTimeCalibration_t time_calibration,uint16_t time_zone)
{
	uint16_t temp_timezone = 0;
	uint8_t day_month[12] = {31,28,31,30,31,30,31,31,30,31,30,31};
	uint32_t count_second = 0; 
	uint16_t i;
	uint16_t year = 0;

	for(i = 1970;i < time_calibration.year; i++)
	{
		if(((i % 4 == 0) && (i % 100 != 0)) || (i%400 == 0))year++;
	}
	
	count_second = year*366 + (time_calibration.year - 1970 - year)*365;
	
	for(i  =1;i < time_calibration.month; i++)
	{
		if((i == 2) && ((((time_calibration.year % 4) == 0) && ( time_calibration.year% 100 != 0)) || (time_calibration.year % 400 == 0)))
			count_second +=29; 
		else 
			count_second +=  day_month[i-1];
	}
	
	count_second += time_calibration.day - 1;

    if(0==(time_zone&0x8000))//正数
    {   
		count_second = count_second*(3600*24) + time_calibration.hour*3600 + time_calibration.min*60 + time_calibration.second - time_zone*60;
	}
	else
	{
		temp_timezone = (uint16_t)abs((int16_t)time_zone);
		count_second = count_second*(3600*24) + time_calibration.hour*3600 + time_calibration.min*60 + time_calibration.second + temp_timezone*60;
	}
	return count_second;
}

void AliCloudRx_Parse_RxLink_TimeCalibration(char *data)
{
	p_ali_rx->time_calibration.year = *data;
	p_ali_rx->time_calibration.year += 2000;
	p_ali_rx->time_calibration.month = *(data + 1);	
	p_ali_rx->time_calibration.day = *(data + 2);	
	p_ali_rx->time_calibration.weekday = *(data + 3);	
	if(p_ali_rx->time_calibration.weekday == 0)p_ali_rx->time_calibration.weekday = 7;
	p_ali_rx->time_calibration.none = *(data + 4);
	p_ali_rx->time_calibration.hour = *(data + 5);	
	p_ali_rx->time_calibration.min = *(data + 6);	
	p_ali_rx->time_calibration.second = *(data + 7);	

	p_ali_rx->time_calibration.gap = (*(data + 8) << 24) + (*(data + 9) << 16) + (*(data + 10) << 8) + (*(data + 11) << 0); 

	p_ali_rx->update_time_calibration = ALI_TRUE;

	p_ali_rx->time_calibration.utc = AliCloudRx_GetUtcWithDay(p_ali_rx->time_calibration,0);

	p_ali_rx->time_calibration.utc_zero_zone = p_ali_rx->time_calibration.utc - p_ali_rx->time_calibration.gap;

	Usprintf_Wifi("year:%d month:%d day:%d weekday:%d hour:%d min:%d sec:%d gap:%d utc:%d zero:%d\n",
	p_ali_rx->time_calibration.year,p_ali_rx->time_calibration.month,p_ali_rx->time_calibration.day,p_ali_rx->time_calibration.weekday,
	p_ali_rx->time_calibration.hour,p_ali_rx->time_calibration.min,p_ali_rx->time_calibration.second,p_ali_rx->time_calibration.gap,
	p_ali_rx->time_calibration.utc,p_ali_rx->time_calibration.utc_zero_zone);
}

void AliCloudRx_Parse_RxLink_WorkMode(char *data)
{
	p_ali_rx->set_workmode = (AliWorkMode_t)*data;	
	p_ali_rx->update_work_mode = ALI_TRUE;
	Usprintf_Wifi("\r\n \r\n ------------>ali recieve work mode:%d \r\n \r\n",p_ali_rx->set_workmode);
}

void AliCloudRx_Parse_RxLink_RemoteDir(char *data)
{
	AliCleanDir_t msg = (AliCleanDir_t)(*data);
	p_ali_rx->clean_dir = msg;
	p_ali_rx->update_clean_dir = ALI_TRUE;	
	Usprintf_Wifi("ali recieve clean dir:%d\n",p_ali_rx->clean_dir);
}

void AliCloudRx_Parse_RxLink_CleanLoopCnt(char *data)
{	
	AliCleanLoopCnt_t msg;
	msg.cur_mode = (uint8_t)*data;
	msg.clean_over_value = (uint8_t)*(data + 1);
	msg.clean_set_value = (uint8_t)*(data + 2);
	msg.dry_over_value = (uint8_t)*(data + 3);
	msg.dry_set_value = (uint8_t)*(data + 4);
	if(msg.clean_set_value >= 3)msg.clean_set_value = 1;
	p_ali_rx->set_cleanloop_cnt = msg.clean_set_value;
	p_ali_rx->update_clean_cycle = ALI_TRUE;
	Usprintf_Wifi("ali recieve clean_set_value:%d\n",p_ali_rx->set_cleanloop_cnt);	
}

void AliCloudRx_Parse_RxLink_ResFactory(char *data)
{
	if(data[0])
	{
		p_ali_rx->update_reset_factory = ALI_TRUE;
		Usprintf_Wifi("ali recieve reset_factory\n");	
	}
}

void AliCloudRx_Parse_RxLink_WifiState(char *data)
{	
 	AliWifiState_t msg;
    msg = (AliWifiState_t)data[0];  
	p_ali_rx->update_wifi_state = ALI_TRUE;
	Usprintf_Wifi("ali recieve wifi state:%d\n",msg);
	switch (msg)
	{
		case ALI_WIFI_STATE_ROUTE_CONNECTED:			
			p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_ROUTE_CONNECTED;
			break;
		case ALI_WIFI_STATE_ROUTE_DISCONNECTED:
			p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_ROUTE_DISCONNECTED;
			break;
		case ALI_WIFI_STATE_PASSWORK_ERROR:
			p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_PASSWORK_ERROR;
			break;
		case ALI_WIFI_STATE_CLOUD_CONNOECTED:
			
      p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_CLOUD_CONNOECTED;		
		
			break;
		case ALI_WIFI_STATE_CLOUD_DISCONECTED:
			p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_CLOUD_DISCONECTED;			
			break;
		case ALI_WIFI_STATE_POWER_UP:
			p_ali_rx->cur_wifi_state = ALI_WIFI_STATE_POWER_UP;
			break;
		default:
			break;
	}   
}

void AliCloudRx_Parse_RxLink_Utc(char *data)
{
	p_ali_rx->time.utc = (*(data + 4) << 24) + (*(data + 5) << 16) + (*(data + 6) << 8) + (*(data + 7) << 0); 
	p_ali_rx->time.year =((*(data + 8) << 8) + *(data + 9));
	p_ali_rx->time.mon = *(data + 10);
	p_ali_rx->time.day = *(data + 11);
	p_ali_rx->time.week = *(data + 12);
	p_ali_rx->time.hour =  *(data + 13);
	p_ali_rx->time.min =  *(data + 14);
	p_ali_rx->time.sec =  *(data + 15);
	p_ali_rx->utc_offset = p_ali_rx->time.utc;
	Usprintf_Wifi("ali recieve utc:%d,year:%d,mon:%d,day:%d,week:%d,hour:%d,min:%d,sec:%d\n",p_ali_rx->time.utc,p_ali_rx->time.year,p_ali_rx->time.mon,p_ali_rx->time.day,p_ali_rx->time.week,p_ali_rx->time.hour,p_ali_rx->time.min,p_ali_rx->time.sec);
}

void AliCloudRx_Parse_RxLink_TimeZone(char *data)
{
	AliTimeZone_t msg;

	msg.timezone = (int8_t)*data;
	msg.summertime = (int8_t)*(data+1);
	p_ali_rx->time_zone = msg.timezone;
	p_ali_rx->update_time_zone = ALI_TRUE;	
	Usprintf_Wifi("ali recieve time zone:%d\n",p_ali_rx->time_zone);
}

void AliCloudRx_Parse_RxLink_WifiMac(char *data)
{	
	uint8_t i ;

	for(i = 0;i<6;i++)
	{
		p_ali_rx->wifi_sysinfo.mac[i] = *(data + i);	
	}

	for (i = 0;i< 4;i++)
	{	
		p_ali_rx->wifi_sysinfo.ip[i] = data[i + 6];
	}
	
	p_ali_rx->wifi_sysinfo.signal_power = data[10];
	
	for (i = 0;i < 4;i++)
	{	
		p_ali_rx->wifi_sysinfo.version[i] = data[11 + i];
	}

	p_ali_rx->wifi_sysinfo.is_receive = 1;

	Usprintf("ali recieve version:%d %d %d %d\n",p_ali_rx->wifi_sysinfo.version[0],p_ali_rx->wifi_sysinfo.version[1],p_ali_rx->wifi_sysinfo.version[2],p_ali_rx->wifi_sysinfo.version[3]);	
}

void AliCloudRx_Parse_RxLink_FactoryTest(char *data)
{
	p_ali_rx->factory_test_mode = (uint8_t)*data;
	Usprintf_Wifi("ali recieve factory_test_mode:%d\n",p_ali_rx->factory_test_mode);
}

void AliCloudRx_Parse_RxLink_BeepNoDisturb(char *data)
{	
	p_ali_rx->voice_mute.switch_state = (uint8_t)*data;

	p_ali_rx->voice_mute.start_hour = (uint8_t)*(data + 1);

	p_ali_rx->voice_mute.start_min = (uint8_t)*(data + 2);

	p_ali_rx->voice_mute.end_hour = (uint8_t)*(data + 3);

	p_ali_rx->voice_mute.end_min = (uint8_t)*(data + 4);

	p_ali_rx->voice_mute_set_state = p_ali_rx->voice_mute.switch_state;

	p_ali_rx->update_voice_mute = ALI_TRUE;

	Usprintf_Wifi("ali recieve voice_mute_set_state:%d\n",p_ali_rx->voice_mute_set_state);

}

void AliCloudRx_Parse_RxLink_MainBrushPower(char *data)
{
	p_ali_rx->set_mainbrush_spd = (uint8_t)*data;
	
	p_ali_rx->update_mainbrush = ALI_TRUE;

	Usprintf_Wifi("ali recieve set_mainbrush_spd:%d\n",p_ali_rx->set_mainbrush_spd);
}

void AliCloudRx_Parse_RxLink_WaterControl(char *data)
{
	p_ali_rx->set_pump_spd = (uint8_t)*data;

	p_ali_rx->update_waterflow = ALI_TRUE;

	Usprintf_Wifi("ali recieve set_pump_spd:%d\n",p_ali_rx->set_pump_spd);

}

void AliCloudRx_Parse_RxLink_WheelSpeed(char *data)
{
	p_ali_rx->set_mov_spd = (uint8_t)*data;
	p_ali_rx->update_mov_speed = ALI_TRUE;
	Usprintf_Wifi("ali recieve set_mov_spd:%d\n",p_ali_rx->set_mov_spd);
}

void AliCloudRx_Parse_RxLink_FindRobot(char *data)
{
	p_ali_rx->find_robot = *data;
	Usprintf_Wifi("ali recieve find_robot:%d\n",p_ali_rx->find_robot);
	if(p_ali_rx->find_robot)
	{
		p_ali_rx->find_robot_cnt = 10;
	}
	else
	{
		p_ali_rx->find_robot_cnt = 0;
	}		
}

void AliCloudRx_Parse_RxLink_Unbanding(char *data)
{
	p_ali_rx->update_unbanding = *data;	
	Usprintf_Wifi("ali recieve update_unbanding:%d\n",p_ali_rx->update_unbanding);	
}







