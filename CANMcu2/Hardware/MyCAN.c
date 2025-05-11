#include "stm32f10x.h"                  // Device header
void MyCAN_Init()
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	GPIO_InitTypeDef GPIO_InitStructer;
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_AF_PP;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_12;
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);
	
	GPIO_InitStructer.GPIO_Mode=GPIO_Mode_IPU;
	GPIO_InitStructer.GPIO_Pin=GPIO_Pin_11;
	GPIO_InitStructer.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GPIO_InitStructer);

	CAN_InitTypeDef CAN_InitStructer;
	CAN_InitStructer.CAN_ABOM=DISABLE;
	CAN_InitStructer.CAN_AWUM=DISABLE;
	CAN_InitStructer.CAN_BS1=CAN_BS1_2tq;
	CAN_InitStructer.CAN_BS2=CAN_BS2_3tq;
	CAN_InitStructer.CAN_Mode=CAN_Mode_LoopBack;
	CAN_InitStructer.CAN_NART=DISABLE;
	CAN_InitStructer.CAN_Prescaler=48;
	CAN_InitStructer.CAN_RFLM=DISABLE;
	CAN_InitStructer.CAN_SJW=CAN_SJW_2tq;
	CAN_InitStructer.CAN_TTCM=DISABLE;
	CAN_InitStructer.CAN_TXFP=DISABLE;
	CAN_Init(CAN1,&CAN_InitStructer);
	
	CAN_FilterInitTypeDef CAN_FILTERINITSTRUCTER;
	CAN_FILTERINITSTRUCTER.CAN_FilterActivation=ENABLE;
	CAN_FILTERINITSTRUCTER.CAN_FilterFIFOAssignment=CAN_FilterFIFO0;
	
	CAN_FILTERINITSTRUCTER.CAN_FilterIdHigh=0x100<<5;
	CAN_FILTERINITSTRUCTER.CAN_FilterIdLow=0x200<<5;
	
	CAN_FILTERINITSTRUCTER.CAN_FilterMaskIdHigh=0x300<<5;
	CAN_FILTERINITSTRUCTER.CAN_FilterMaskIdLow=0x400<<5;
	
	CAN_FILTERINITSTRUCTER.CAN_FilterMode=CAN_FilterMode_IdList;
	CAN_FILTERINITSTRUCTER.CAN_FilterNumber=0;
	CAN_FILTERINITSTRUCTER.CAN_FilterScale=CAN_FilterScale_16bit;
	CAN_FilterInit(&CAN_FILTERINITSTRUCTER);

}

void MyCAN_Transmit(CanTxMsg *TxMessage)
{
	uint32_t Timer =0;
	uint32_t TransmitMB = CAN_Transmit(CAN1,TxMessage);
	while(CAN_TransmitStatus(CAN1,TransmitMB) != CAN_TxStatus_Ok)
	{
		Timer++;
		if(Timer >= 100000)
		{
			break;
		}
	}
}

uint8_t MyCAN_ReceiveFlag()
{
	if(CAN_MessagePending(CAN1,CAN_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

void MyCAN_RECEIVE(CanRxMsg *CanRxessage)
{
	CAN_Receive(CAN1,CAN_FIFO0,CanRxessage);
}
