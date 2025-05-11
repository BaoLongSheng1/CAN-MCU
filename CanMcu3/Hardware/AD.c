#include "stm32f10x.h"                  // Device header
#include "AD.h"
uint16_t AD_Value[4];
void AD_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);
	
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AIN;
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStruct);
	
		ADC_RegularChannelConfig(ADC1,ADC_Channel_0,1,ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_1,2,ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_2,3,ADC_SampleTime_55Cycles5);
		ADC_RegularChannelConfig(ADC1,ADC_Channel_3,4,ADC_SampleTime_55Cycles5);
	
	ADC_InitTypeDef ADC_InitStruct;
	ADC_InitStruct.ADC_ContinuousConvMode=ENABLE;
	ADC_InitStruct.ADC_DataAlign=ADC_DataAlign_Right;
	ADC_InitStruct.ADC_ExternalTrigConv=ADC_ExternalTrigConv_None;
	ADC_InitStruct.ADC_Mode=ADC_Mode_Independent;
	ADC_InitStruct.ADC_NbrOfChannel=4;
	ADC_InitStruct.ADC_ScanConvMode=ENABLE;
	ADC_Init(ADC1,&ADC_InitStruct);
	
	
	
	DMA_InitTypeDef DMA_InitStructe;
	DMA_InitStructe.DMA_PeripheralBaseAddr=(uint32_t)&ADC1->DR;
	DMA_InitStructe.DMA_PeripheralDataSize=DMA_PeripheralDataSize_HalfWord;
	DMA_InitStructe.DMA_PeripheralInc=DMA_PeripheralInc_Disable;
	
	DMA_InitStructe.DMA_MemoryBaseAddr=(uint32_t)AD_Value;
	DMA_InitStructe.DMA_MemoryDataSize=DMA_MemoryDataSize_HalfWord;
	DMA_InitStructe.DMA_MemoryInc=DMA_MemoryInc_Enable;

	DMA_InitStructe.DMA_BufferSize=4;
	DMA_InitStructe.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructe.DMA_M2M=DMA_M2M_Disable;
	DMA_InitStructe.DMA_Mode=DMA_Mode_Circular;
	DMA_InitStructe.DMA_Priority=DMA_Priority_Medium;
	DMA_Init(DMA1_Channel1,&DMA_InitStructe);
	
	DMA_Cmd(DMA1_Channel1,ENABLE);
	ADC_DMACmd(ADC1,ENABLE);
	ADC_Cmd(ADC1,ENABLE);
	
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1) == SET);
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1) == SET);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);

}

