#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "MyCAN.h"
#include "AD.h"
#include "MyDma.h"
#include "Timer.h"
uint8_t KeyNum = 0;
uint16_t AD0,AD1,AD2,AD3;
//���巢�ͽṹ��
CanTxMsg TxMsgArray[] = {
//  stdid    Extdid        IDE            RTR     DLC     Data[8]  
{0x100,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x200,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
//{0x300,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
//{0x400,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},

/*
{0x200,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x201,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x2FF,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x2FE,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},

{0x310,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x311,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x31F,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x31E,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},

{0x320,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x321,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x32F,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
{0x32E,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
*/
};

uint16_t Can_RxADx[4]; 
uint8_t pTxMsgArray = 0;
uint16_t t;
float Voltage;
//������սṹ��
CanRxMsg RxMsg;
int main(void)
{

	Timer_Init();
	AD_Init();
	OLED_Init();
	Key_Init();
	MyCan_Init();
	OLED_ShowString(1,1,"0.00v");
	while(1)
	{

		
		AD0 = AD_Value[0];
		Voltage = (float)AD0/4095*3.3;
		//OLED_ShowNum(1,1,AD0,4);
		//OLED_ShowNum(2,1,Can_RxADx[0],4);
		OLED_ShowNum(1,1,Voltage,1);
		OLED_ShowNum(1,3,(uint16_t)(Voltage * 100) %100,2);
		
		OLED_ShowNum(2,7,Can_RxADx[1],4);
		
		OLED_ShowNum(3,1,Can_RxADx[2],4);
		OLED_ShowNum(3,7,Can_RxADx[3],4);
		
		AD1 = AD_Value[1];
		AD2 = AD_Value[2];
		AD3 = AD_Value[3];

		/*

		Voltage = (float)AD0 /4095 * 3.3;

		OLED_ShowNum(1,9,AD0,4);
		OLED_ShowNum(2,9,AD1,4);
		OLED_ShowNum(3,9,AD2,4);
		OLED_ShowNum(4,9,AD3,4);

		OLED_ShowNum(4,1,t,4);
		
		OLED_ShowNum(1,1,Voltage,1);
		OLED_ShowString(1,2,".");
		OLED_ShowNum(1,3,(uint16_t)(Voltage * 100) % 100,2);
		*/
		
		KeyNum = Key_GetNum();
		//���ͺ���
	if(KeyNum == 1)
		{
			
		
			
			if(pTxMsgArray>=sizeof(TxMsgArray) / sizeof(CanTxMsg))
		{ 
		pTxMsgArray =0;
		}
		} 
		//���պ���
		if(MyCAN_ReceiveFlag())
		{
			MyCAN_RECEIVE(&RxMsg);
			//�ж��յ����Ǳ�׼��ʽ������չ��ʽ
			
			/*
			if(RxMsg.IDE == CAN_Id_Standard)
			{
				OLED_ShowString(1,6,"Std");
				OLED_ShowHexNum(2,6,RxMsg.StdId,8);
			}
			else if(RxMsg.IDE == CAN_Id_Extended)
			{
					OLED_ShowHexNum(2,6,RxMsg.ExtId,8);
				OLED_ShowString(1,6,"Ext");
			}
			*/
			
			//�жϽ��յ��������ݻ���ң��֡
			if(RxMsg.RTR == CAN_RTR_Data)
			{
				//OLED_ShowString(1,6,"Data  ");
				//OLED_ShowHexNum(3,6,RxMsg.DLC,1);
				//Can_RxADx[0] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
				//Can_RxADx[1] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
				
				
				if(RxMsg.StdId==0x100)
				{
					Can_RxADx[0] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
					Can_RxADx[1] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
				}
				if(RxMsg.StdId==0x200)
				{
					Can_RxADx[2] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
					Can_RxADx[3] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
				}
				/*
				OLED_ShowHexNum(3,6,RxMsg.Data[0],2);
				OLED_ShowHexNum(3,9,RxMsg.Data[1],2);
				OLED_ShowHexNum(3,12,RxMsg.Data[2],2);
				OLED_ShowHexNum(3,15,RxMsg.Data[3],2);
				
				OLED_ShowHexNum(4,6,RxMsg.Data[0],2);
				OLED_ShowHexNum(4,9,RxMsg.Data[1],2);
				OLED_ShowHexNum(4,12,RxMsg.Data[2],2);
				OLED_ShowHexNum(4,15,RxMsg.Data[3],2);
				*/
			}
			
			/*
			else if(RxMsg.RTR ==CAN_RTR_REMOTE)
			{
				OLED_ShowString(1,10,"Remote");
				OLED_ShowHexNum(3,6,RxMsg.DLC,1);
				
				OLED_ShowHexNum(4,6,0x00,2);
				OLED_ShowHexNum(4,9,0x00,2);
				OLED_ShowHexNum(4,12,0x00,2);
				OLED_ShowHexNum(4,15,0x00,2);
			}
			
			*/
			
		}
		
		
	}
}


/*
��������λ��:����
TxMsgArray[0].Data[0] ��TxMsgArray[0].Data[1]�洢����ad0�ĸ߰�λ�͵ڰ�λ 
TxMsgArray[0].Data[2] ��TxMsgArray[0].Data[3]�洢����ad1�ĸ߰�λ�͵ڰ�λ 
TxMsgArray[1].Data[0] ��TxMsgArray[1].Data[1]�洢����ad2�ĸ߰�λ�͵ڰ�λ 
TxMsgArray[1].Data[2] ��TxMsgArray[1].Data[3]�洢����ad3�ĸ߰�λ�͵ڰ�λ 

����������OLED_ShowNum(1,1,AD0,4);��ʾ����ʮ���ƣ�
	OLED_ShowHexNum(4,6,RxMsg.Data[0],2);
				OLED_ShowHexNum(4,9,RxMsg.Data[1],2);����16���ƣ�����Ҫת������
*/

void TIM4_IRQHandler(void)
{
		TxMsgArray[0].Data[0] = AD0>>8;
		TxMsgArray[0].Data[1] = AD0;
		TxMsgArray[0].Data[2] = AD1>>8;
		TxMsgArray[0].Data[3] = AD1;
	
		TxMsgArray[1].Data[0] = AD2>>8;
		TxMsgArray[1].Data[1] = AD2;
		TxMsgArray[1].Data[2] = AD3>>8;
		TxMsgArray[1].Data[3] = AD3;
	
		MyCAN_Transmit(&TxMsgArray[pTxMsgArray]);
			pTxMsgArray++;
	if(pTxMsgArray == sizeof(TxMsgArray)/sizeof(TxMsgArray[0]))
	{
		pTxMsgArray = 0;
	}
	
	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		t++;
		if(t>3) {t = 0;	};
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
	}

	
}


