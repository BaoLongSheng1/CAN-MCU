#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "MyCAN.h"
uint8_t KeyNum = 0;
float Voltage;
int16_t SpeedNum;
//定义发送结构体
CanTxMsg TxMsgArray[] = {
// stdid	     Extid         IDE            RTR       DLC     DATA[8]
//{0x100,0x00000000,CAN_Id_Standard,CAN_RTR_Data, 4 , {0x11, 0x22,0x33,0x44}},
//{0x000,0x12345678,CAN_Id_Extended,CAN_RTR_Data, 4 , {0xAA, 0xBB,0xCC,0xDD}},
//{0x666,0x00000000,CAN_Id_Standard,CAN_RTR_REMOTE, 0 , {0x00, 0x00,0x00,0x00}},
{0x000,0x0789ABCD,CAN_Id_Extended,CAN_RTR_REMOTE, 0 , {0x00, 0x00,0x00,0x00}}
};
uint8_t pTxMsgArray = 0;

uint16_t Can_RxADx[8];
//定义接收结构体
CanRxMsg RxMsg;
int main(void)
{
	OLED_Init();
	Key_Init();
	MyCan_Init();
	OLED_ShowString(1,1,"0.00v");
	while(1)
	{
		
		//OLED_ShowNum(1,1,Can_RxADx[0],4);
		OLED_ShowNum(1,7,Can_RxADx[1],4);
		OLED_ShowNum(2,1,Can_RxADx[2],4);
		OLED_ShowNum(2,7,Can_RxADx[3],4);
		OLED_ShowNum(3,1,Can_RxADx[4],4);
		OLED_ShowNum(1,1,Voltage,1);
		OLED_ShowNum(1,3,(uint16_t)(Voltage * 100) %100,2);
		
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			MyCAN_Transmit(&TxMsgArray[pTxMsgArray]);
			pTxMsgArray++;
		}
		
		if(MyCAN_ReceiveFlag())
		{
			MyCAN_RECEIVE(&RxMsg);
			
			if(RxMsg.StdId==0x100)
				{
					Can_RxADx[0] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
					Can_RxADx[1] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
					Voltage = (float)Can_RxADx[0] / 4095 * 3.3 ;
				}
				if(RxMsg.StdId==0x200)
				{
					Can_RxADx[2] = RxMsg.Data[0]<<8 | RxMsg.Data[1];
					Can_RxADx[3] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
				}
				if(RxMsg.StdId == 0x300)
				{
					Can_RxADx[4] = RxMsg.Data[0]<<8 | RxMsg.Data[1];

					//Can_RxADx[5] = RxMsg.Data[2]<<8 | RxMsg.Data[3];
				}
			//判断收到的是标准格式还是拓展格式
			
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
			
			
			//判断接收到的是数据还是遥控帧
			if(RxMsg.RTR == CAN_RTR_Data)
			{
				OLED_ShowString(1,10,"Data  ");
				OLED_ShowHexNum(3,6,RxMsg.DLC,1);
				
				OLED_ShowHexNum(4,6,RxMsg.Data[0],2);
				OLED_ShowHexNum(4,9,RxMsg.Data[1],2);
				OLED_ShowHexNum(4,12,RxMsg.Data[2],2);
				OLED_ShowHexNum(4,15,RxMsg.Data[3],2);
			}
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
