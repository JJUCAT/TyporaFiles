#include "Include.h"

RealTimeMonitor_UsartSend on_realtimemonitor_usart_send;

void RealTimeMonitor_Init(void)
{
	on_realtimemonitor_usart_send = Debug_PrintStringByLength;	
}

void RealTimeMonitor_LoadData(int32_t x, int32_t y, int32_t angle, uint8_t type,char *data)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	
	//帧头
	data[0] = 0xfa;
	data[1] = 0xaf;
	//功能字
	data[2] = 0x03;
	//数据长度
	data[3] = 14;
	//X
	data[4] = ((x & 0xff000000) >> 24);
	data[5] = ((x & 0xff0000) >> 16);
	data[6] = ((x & 0xff00) >> 8);
	data[7] = x & 0xff;
	//Y
	data[8] = ((y & 0xff000000) >> 24);
	data[9] = ((y & 0xff0000) >> 16);
	data[10] = ((y & 0xff00) >> 8);
	data[11] = y & 0xff;
	//角度
	data[12] = ((angle & 0xff000000) >> 24);
	data[13] = ((angle & 0xff0000) >> 16);
	data[14] = ((angle & 0xff00) >> 8);
	data[15] = angle & 0xff;
	//类型
	data[16] = type;
	
	for(i = 4;i < 17;i++)
	{
		sum += data[i];
	}
	
	data[17] = sum & 0xff;
}

void RealTimeMonitor_SendData(int32_t x, int32_t y, int32_t angle, uint8_t type)
{
	char data_temp[18]={0};
	RealTimeMonitor_LoadData(x, y, angle, type,data_temp);
	on_realtimemonitor_usart_send(data_temp,18);
}























