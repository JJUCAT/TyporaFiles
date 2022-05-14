#include "Include.h"

TuyaStruct_t *p_tuya_rx;

void TuyaRx_Init(void)
{
    p_tuya_rx = &g_tuya;
    p_tuya_rx->p_rx_queue_in = ( char *)p_tuya_rx->rx_buf;
    p_tuya_rx->p_rx_queue_out = ( char *)p_tuya_rx->rx_buf;     
}

void TuyaRx_RecieveData(unsigned char value)
{
    if(1 == p_tuya_rx->p_rx_queue_out - p_tuya_rx->p_rx_queue_in)
    {
        //Data queue full
    }
    else if((p_tuya_rx->p_rx_queue_in > p_tuya_rx->p_rx_queue_out) && ((p_tuya_rx->p_rx_queue_in - p_tuya_rx->p_rx_queue_out) >= sizeof(p_tuya_rx->rx_buf)))
    {
        //Data queue full
    }
    else
    {
        //Queue is not full       
        if(p_tuya_rx->p_rx_queue_in >= (char *)(p_tuya_rx->rx_buf + sizeof(p_tuya_rx->rx_buf)))
        {
            p_tuya_rx->p_rx_queue_in = (char *)(p_tuya_rx->rx_buf);
        }
        *p_tuya_rx->p_rx_queue_in++ = value;
    }
}

unsigned char TuyaRx_GetQueueTotalData(void)
{
    if(p_tuya_rx->p_rx_queue_in != p_tuya_rx->p_rx_queue_out)
    {
        return 1;
    }
    else
    {
        return 0;
    }    
}

unsigned char TuyaRx_QueueReadByte(void)
{
  unsigned char value;
  
  if(p_tuya_rx->p_rx_queue_out != p_tuya_rx->p_rx_queue_in)
  {
    if(p_tuya_rx->p_rx_queue_out >= ( char *)(p_tuya_rx->rx_buf + sizeof(p_tuya_rx->rx_buf)))
    {
      p_tuya_rx->p_rx_queue_out = ( char *)(p_tuya_rx->rx_buf);
    }
    
    value = *p_tuya_rx->p_rx_queue_out ++;   
  }
  
  return value;
}



























