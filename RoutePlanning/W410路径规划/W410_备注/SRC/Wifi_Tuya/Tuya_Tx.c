#include "Include.h"

TuyaStruct_t *p_tuya_tx;
TuyaTx_UsartSend on_tuya_tx_usart_send;


void TuyaTx_Init(void)
{
    p_tuya_tx = &g_tuya; 
    on_tuya_tx_usart_send = USART6_Transmit_String;      
}

unsigned short TuyaTx_SetUsartByte(unsigned short dest, unsigned char byte)
{
    unsigned char *obj = (unsigned char *)p_tuya_tx->tx_buf + DATA_START + dest;

    *obj = byte;
    dest += 1;

    return dest;
}


void TuyaTx_WriteFrame(unsigned char fr_type, unsigned char fr_ver, unsigned short len)
{
    unsigned char check_sum = 0;

    p_tuya_tx->tx_buf[HEAD_FIRST] = 0x55;
    p_tuya_tx->tx_buf[HEAD_SECOND] = 0xaa;
    p_tuya_tx->tx_buf[PROTOCOL_VERSION] = fr_ver;
    p_tuya_tx->tx_buf[FRAME_TYPE] = fr_type;
    p_tuya_tx->tx_buf[LENGTH_HIGH] = len >> 8;
    p_tuya_tx->tx_buf[LENGTH_LOW] = len & 0xff;

    len += PROTOCOL_HEAD;
    check_sum = Math_CalArraySum((char *)p_tuya_tx->tx_buf, len - 1);
    p_tuya_tx->tx_buf[len - 1] = check_sum;
    
    TuyaTx_WriteData((unsigned char *)p_tuya_tx->tx_buf, len);
}

static void TuyaTx_WriteData(unsigned char *in, unsigned short len)
{
    if((NULL == in) || (0 == len))
    {
        return;
    }
    on_tuya_tx_usart_send(len,(char *)in);
}

unsigned short TuyaTx_SetUsartBuffer(unsigned short dest, const unsigned char *src, unsigned short len)
{
    unsigned char *obj = (unsigned char *)p_tuya_tx->tx_buf + DATA_START + dest;

    TuyaMath_Memcpy(obj,src,len);

    dest += len;
    return dest;
}

unsigned char TuyaTx_DpEnumUpdate(unsigned char dpid,unsigned char value)
{
    unsigned short length = 0;

    length = TuyaTx_SetUsartByte(length,dpid);
    length = TuyaTx_SetUsartByte(length,DP_TYPE_ENUM);
    length = TuyaTx_SetUsartByte(length,0);
    length = TuyaTx_SetUsartByte(length,1);
    length = TuyaTx_SetUsartByte(length,value);

    TuyaTx_WriteFrame(STATE_UPLOAD_CMD,MCU_SEND_VER,length);

    return SUCCESS;
}



