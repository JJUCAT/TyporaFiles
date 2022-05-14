#include "Include.h"


volatile Adc_Raw_t g_adc_value;


void Hw_Adc_Configuration(void)
{
	ADC1->CR1 = 0x00000000;
	ADC1->CR2 = 0x00000000;
	
	DMA2_Stream0->CR &= ~ DMA_SxCR_EN ;
	Waiting_Disable_DMA(DMA2_Stream0);
	DMA2_Stream0->CR = (DMA_SxCR_CHSEL_0|DMA_SxCR_MINC|DMA_SxCR_MSIZE_0|DMA_SxCR_PSIZE_0|DMA_SxCR_PL|DMA_SxCR_CIRC); 
	DMA2_Stream0->PAR =  (uint32_t)&(ADC1->DR);
	DMA2_Stream0->M0AR = (uint32_t)&(g_adc_value);
	DMA2_Stream0->NDTR = 12;

	DMA2->LIFCR |= (DMA_LIFCR_CTCIF0|DMA_LIFCR_CHTIF0|DMA_LIFCR_CFEIF0); 
	DMA2_Stream0->CR |= DMA_SxCR_EN;

	ADC1->CR1 &= ~(ADC_CR1_AWDEN|ADC_CR1_JAWDEN) ;
	ADC->CCR |= ADC_CCR_ADCPRE;

	ADC1->SMPR2 |= (ADC_SMPR2_SMP0_1|ADC_SMPR2_SMP4_1|ADC_SMPR2_SMP5_1|ADC_SMPR2_SMP6_1|ADC_SMPR2_SMP7_1|ADC_SMPR2_SMP7_1|ADC_SMPR2_SMP8_1|ADC_SMPR2_SMP9_1); //28 cycle
	ADC1->SMPR1 |= (ADC_SMPR1_SMP10_1|ADC_SMPR1_SMP11_1|ADC_SMPR1_SMP12_1|ADC_SMPR1_SMP13_1|ADC_SMPR1_SMP14_1); //28 cycle
	ADC1->SQR3 	|= (0<<0)|(4<<5)|(5<<10)|(6<<15)|(7<<20)|(8<<25); //Select channel    0, 4, 5, 6, 7, 8
	ADC1->SQR2 	|= (9<<0)|(10<<5)|(11<<10)|(12<<15)|(13<<20)|(14<<25);//Select channel 8,9,11,12,13,14
	ADC1->SQR1 	|= (ADC_SQR1_L_3|ADC_SQR1_L_1|ADC_SQR1_L_0); //1011: Total  12 channels

	ADC1->CR1 |= ADC_CR1_SCAN;
	ADC1->CR2 |= ADC_CR2_DDS| ADC_CR2_DMA ; 
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

void Hw_Adc_Start(void)
{
	ADC1->CR2 |= ADC_CR2_SWSTART;		
}








