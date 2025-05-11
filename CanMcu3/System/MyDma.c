#include "stm32f10x.h"                  // Device header
#include "MyDma.h"

void MyDma_Init(uint32_t AddrA,uint32_t AddrB,uint16_t Size)
{
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1,ENABLE);
	DMA_InitTypeDef DMA_InitStructe;
	DMA_InitStructe.DMA_PeripheralBaseAddr=AddrA;
	DMA_InitStructe.DMA_PeripheralDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructe.DMA_PeripheralInc=DMA_PeripheralInc_Enable;
	
	DMA_InitStructe.DMA_MemoryBaseAddr=AddrB;
	DMA_InitStructe.DMA_MemoryDataSize=DMA_PeripheralDataSize_Byte;
	DMA_InitStructe.DMA_MemoryInc=DMA_PeripheralInc_Enable;

	DMA_InitStructe.DMA_BufferSize=Size;
	DMA_InitStructe.DMA_DIR=DMA_DIR_PeripheralSRC;
	DMA_InitStructe.DMA_M2M=DMA_M2M_Enable;

	DMA_InitStructe.DMA_Mode=DMA_M2M_Enable;

	DMA_InitStructe.DMA_Priority=DMA_Priority_Medium;
	
	DMA_Init(DMA1_Channel1,&DMA_InitStructe);
	DMA_Cmd(DMA1_Channel1,ENABLE);
}