/**
  ******************************************************************************
  * @file    AI Cleaning Robot
  * @author  ILife Team Dxsong
  * @version V1.0
  * @date    17-Nov-2011
  * @brief   Initialize Usart function and prosessing the characters transmitting
  ******************************************************************************
  * Initialize the System Clock.ADC converter.EXTI.Timer and USART3
  * <h2><center>&copy; COPYRIGHT 2011 ILife CO.LTD</center></h2>
  ******************************************************************************
  */  

#include "Include.h"


void USART1_Configuration(void)//gyro recieve
{ 
	USART1->BRR = USART_APB2_BAUDRATE;
	USART1->CR3 |=  (USART_CR3_DMAT| USART_CR3_DMAR);  
	USART1->CR1 |= USART_CR1_UE|USART_CR1_IDLEIE| USART_CR1_TE | USART_CR1_RE;

	DMA2_Stream2->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA2_Stream2);
	DMA2_Stream2->CR = (DMA_SxCR_CHSEL_4|DMA_SxCR_MINC); 
	DMA2_Stream2->PAR =  (uint32_t)&(USART1->DR);
	DMA2_Stream2->M0AR =  (uint32_t)g_gyro.rxbuffer;
	DMA2_Stream2->NDTR = GYRO_BUFFER_SIZE;

	DMA2->LIFCR |= (DMA_LIFCR_CTCIF2|DMA_LIFCR_CHTIF2|DMA_LIFCR_CFEIF2); 
	DMA2_Stream2->CR |= DMA_SxCR_EN;
}

void USART2_Configuration(void)//obs recieve
{
	USART2->BRR = USART_APB1_BAUDRATE;
	USART2->CR3 |=  (USART_CR3_DMAT| USART_CR3_DMAR);  
	USART2->CR1 |= USART_CR1_UE|USART_CR1_IDLEIE| USART_CR1_TE | USART_CR1_RE;

	DMA1_Stream5->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA1_Stream5);
	DMA1_Stream5->CR = (DMA_SxCR_CHSEL_4|DMA_SxCR_MINC); 
	DMA1_Stream5->PAR = (uint32_t)&(USART2->DR);
	DMA1_Stream5->M0AR = (uint32_t)g_obsmsg.rxbuffer;
	DMA1_Stream5->NDTR = OBS_BUFFER_SIZE;

	DMA1->HIFCR |= (DMA_HIFCR_CTCIF5|DMA_HIFCR_CHTIF5|DMA_HIFCR_CFEIF5); 
	DMA1_Stream5->CR |= DMA_SxCR_EN;	
}

void USART6_Configuration(void)//wifi recieve
{
	USART6->BRR = USART_APB2_BAUDRATE;
	USART6->CR1 |= USART_CR1_UE| USART_CR1_TE | USART_CR1_RE; 
	#ifdef ALICLOUD
	USART6->CR1 |= USART_CR1_IDLEIE;
	USART6->CR3 |=  (USART_CR3_DMAT|USART_CR3_DMAR);
	DMA2_Stream1->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA2_Stream1);
	DMA2_Stream1->CR = (DMA_SxCR_CHSEL_5|DMA_SxCR_MINC); 
	DMA2_Stream1->PAR = (uint32_t)&(USART6->DR);
	DMA2_Stream1->M0AR = (uint32_t)g_ali.rx_array;
	DMA2_Stream1->NDTR = ALI_RX_BUF_SIZE;

	DMA2->LIFCR |= (DMA_LIFCR_CTCIF1|DMA_LIFCR_CHTIF1|DMA_LIFCR_CFEIF1);
	DMA2_Stream1->CR |= DMA_SxCR_EN;
	#endif
	#ifdef TUYA
	USART6->CR1 |= USART_CR1_RXNEIE;	
	#endif			
}

void USART1_DMA_String(uint16_t Length,char *data)       
{
	Waiting_Sending_DMA(DMA2_Stream7);
	DMA2_Stream7->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA2_Stream7);
	DMA2_Stream7->CR = (DMA_SxCR_CHSEL_4|DMA_SxCR_MINC| DMA_SxCR_DIR_0|DMA_SxCR_TCIE);
	DMA2_Stream7->PAR =  (uint32_t)&(USART1->DR);
	DMA2_Stream7->M0AR =  (uint32_t)data;
	DMA2_Stream7->NDTR = Length;
	DMA2->HIFCR |=  DMA_HIFCR_CTCIF7|DMA_HIFCR_CHTIF7|DMA_HIFCR_CFEIF7;
	DMA2_Stream7->CR |= DMA_SxCR_EN; 
}

void USART2_DMA_String(uint16_t Length,char *data)  
{
	Waiting_Sending_DMA(DMA1_Stream6);
	DMA1_Stream6->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA1_Stream6);
	DMA1_Stream6->CR = (DMA_SxCR_CHSEL_4|DMA_SxCR_MINC|DMA_SxCR_DIR_0|DMA_SxCR_TCIE);
	DMA1_Stream6->PAR =  (uint32_t)&(USART2->DR);
	DMA1_Stream6->M0AR =  (uint32_t)data;
	DMA1_Stream6->NDTR = Length;
	DMA1->HIFCR |=  DMA_HIFCR_CTCIF6|DMA_HIFCR_CHTIF6|DMA_HIFCR_CFEIF6;
	DMA1_Stream6->CR |= DMA_SxCR_EN;  
}

void USART6_DMA_String(uint16_t Length,char *data)
{
	Waiting_Sending_DMA(DMA2_Stream6);
	DMA2_Stream6->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA2_Stream6);
	DMA2_Stream6->CR = (DMA_SxCR_CHSEL_5|DMA_SxCR_MINC| DMA_SxCR_DIR_0); 
	DMA2_Stream6->PAR =  (uint32_t)&(USART6->DR);
	DMA2_Stream6->M0AR =  (uint32_t)data;
	DMA2_Stream6->NDTR = Length;
	DMA2->HIFCR |=  DMA_HIFCR_CTCIF6|DMA_HIFCR_CHTIF6|DMA_HIFCR_CFEIF6;
	DMA2_Stream6->CR |= DMA_SxCR_EN;
}

void USART1_Transmit_String(uint16_t length,char *data)
{
  while(length)
  {
    length--;
    USART1_Transmit_Byte(*data);
    data++;
  }
}

void USART2_Transmit_String(uint16_t length,char *data)
{
  while(length)
  {
    length--;
    USART2_Transmit_Byte(*data);
    data++;
  }
}

void USART6_Transmit_String(uint16_t length,char *data)
{
  while(length)
  {
    length--;
    USART6_Transmit_Byte(*data);
    data++;
  }
}

void USART1_Transmit_Byte(char data)
{
   USART1->DR = data;
   while(!(USART1->SR&USART_SR_TXE));
}

void USART2_Transmit_Byte(char data)
{
   USART2->DR = data;
   while(!(USART2->SR&USART_SR_TXE));
}

void USART6_Transmit_Byte(char data)
{
   USART6->DR = data;
   while(!(USART6->SR&USART_SR_TXE));
}

void ups3(char *data)
{
	char	*data_ptr = data;

	while (*data_ptr)
	{
		USART2_Transmit_Byte(*data_ptr);
		data_ptr++;
	}
}

void ups(char *data)
{
	char	*data_ptr = data;
	uint16_t	length = 0;

	while (*data_ptr)
	{
		length++;
		data_ptr++;
	}

	USART1_DMA_String(length, data);
}

void upn(int32_t data)
{
	uint8_t c = 1, d, e;
	int32_t l;
	char	f;

	if(data < 0)
	{
		f = '-';
		USART1_Transmit_String(1, &f);
		data *= -1;
	}

	l = data;
	while(l >= 10)
	{
		l /= 10;
		++c;
	}

	for(d = c; d > 0; --d)
	{
		l = data;
		for(e = d - 1; e > 0; --e)
		{
			l /= 10;
		}
		f = (l % 10) + 48;
		USART1_Transmit_String(1, &f);
	}
}


