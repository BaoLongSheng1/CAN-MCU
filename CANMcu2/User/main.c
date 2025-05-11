#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "DHT11.h"
#include "Timer.h"
#include "PWM.h"
#include "Moter.h"
#include "Encoder.h"
#include "MyCAN.h"
#include "Key.h"
uint8_t temp;
uint8_t humi;
uint8_t Num;
uint8_t i;
uint8_t pTxMsgArray = 0;
uint8_t KeyNum;
uint16_t SpeedNum;
int16_t Speed;
uint8_t CarSpeed = 0;
CanTxMsg TxMsgArray[] = {
// stdid	     Extid         IDE            RTR       DLC     DATA[8]
{0x300,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
//{0x300,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0xAA, 0xBB,0xCC,0xDD}},
//{0x400,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x22, 0x33,0x44,0x44}},
//{0x500,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x44, 0x55,0x66,0x77}},
//{0x666,0x00000000,CAN_Id_Standard,CAN_RTR_REMOTE, 0 , {0x00, 0x00,0x00,0x00}},
//{0x000,0x0789ABCD,CAN_Id_Extended,CAN_RTR_REMOTE, 0 , {0x00, 0x00,0x00,0x00}}
};
//定义接收结构体
CanRxMsg RxMsg;
uint16_t Can_RxADx[8];
int main(void)
{
	/*模块初始化*/
	OLED_Init();		//OLED初始化
	Timer_Init();
	MyCAN_Init();
	Encoder_Init();
	Moter_Init();
	Key_Init();
	//DHT11_Init();
	PWM_Init();
	while (1)
	{
		OLED_ShowNum(2,1,SpeedNum,8);
		OLED_ShowNum(3,1,RxMsg.Data[0]<<8| RxMsg.Data[1],4);
		OLED_ShowNum(4,1,Can_RxADx[0],8);
		OLED_ShowNum(3,6,CarSpeed,3);

		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
		CarSpeed=CarSpeed+20;
			if(CarSpeed > 100)
			{
				CarSpeed =0;
			}
		}
		
		if(RxMsg.StdId == 0x300)
				{
					//Can_RxADx[0] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
					//Can_RxADx[5] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
					Can_RxADx[0] = RxMsg.Data[1];
				}
		
		if(MyCAN_ReceiveFlag())
		{
			MyCAN_RECEIVE(&RxMsg);
			
			/*
			//判断收到的是标准格式还是拓展格式
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
			
			//判断接收到的是数据还是遥控帧
			if(RxMsg.RTR == CAN_RTR_Data)
			{
				//OLED_ShowString(1,10,"Data  ");
				
				//OLED_ShowHexNum(3,6,RxMsg.DLC,1);
				
				//OLED_ShowHexNum(4,6,RxMsg.Data[0],2);
			//	OLED_ShowHexNum(4,9,RxMsg.Data[1],2);
				//OLED_ShowHexNum(4,12,RxMsg.Data[2],2);
			//	OLED_ShowHexNum(4,15,RxMsg.Data[3],2);
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
		Moter_SetSpeed(CarSpeed);
	}
}


void TIM4_IRQHandler(void)
{
			
		//TxMsgArray[0].Data[0] = SpeedNum>>8;

	
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == SET)
	{
		SpeedNum = Get_Encoder();//隔一秒读取速度，当电机速度太快时应修改时间，负责容易溢出
		TxMsgArray[0].Data[0] =SpeedNum>>8;
		TxMsgArray[0].Data[1] = SpeedNum;
		
		MyCAN_Transmit(&TxMsgArray[pTxMsgArray]);
			pTxMsgArray++;
		if(pTxMsgArray == sizeof(TxMsgArray)/sizeof(TxMsgArray[0])) pTxMsgArray =0;
		
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);

	}	
}
