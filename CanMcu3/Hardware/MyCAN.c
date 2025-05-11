#include "MyCAN.H"

void MyCan_Init(void)
{
	//开启GPIOA的时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//开启CAN时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	//初始化GOPIO
	GPIO_InitTypeDef GpioInitStruct;
	GpioInitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//使用复用推挽输出模式，引脚控制权交CAN外设
	GpioInitStruct.GPIO_Pin=GPIO_Pin_12;
	GpioInitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GpioInitStruct);
	
	GpioInitStruct.GPIO_Mode=GPIO_Mode_IPU;//使用上拉输入模式，让引脚默认状态是高电平
	GpioInitStruct.GPIO_Pin=GPIO_Pin_11;
	GpioInitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GpioInitStruct);
	
	//can外设初始化
	CAN_InitTypeDef CANINITSTRUCT;
	CANINITSTRUCT.CAN_ABOM=DISABLE;//离线自动恢复disable：手动恢复
	CANINITSTRUCT.CAN_AWUM =DISABLE;//自动唤醒 ENABLE：自动唤醒
	CANINITSTRUCT.CAN_BS1  =CAN_BS1_2tq;//位时序时间段的TQ数
	CANINITSTRUCT.CAN_BS2  =CAN_BS2_3tq;//位时序时间段的TQ数
	CANINITSTRUCT.CAN_Mode =CAN_Mode_LoopBack;//CAN外设工作模式
	CANINITSTRUCT.CAN_NART =DISABLE;//使能或失能，不自动重传模式，这里不需要不自动重传=自动重传，故输入DISABLE
	CANINITSTRUCT.CAN_Prescaler=48;//分频系数   
	//波特率 = APB1时钟频率/分频系数/一位的TQ数量 
	//=36MHz/48/(1+2+3) = 125K  刚好在高速CAN(125K-1M)的范围内
	CANINITSTRUCT.CAN_RFLM   =DISABLE;//fifo锁定 DISABLE:FIFO溢出时最后的报文被新报文覆盖
	CANINITSTRUCT.CAN_SJW    =CAN_SJW_2tq;//在同步的补偿宽度
	CANINITSTRUCT.CAN_TTCM   =DISABLE;//时间触发通信模式ENABLE:开启
	CANINITSTRUCT.CAN_TXFP   =DISABLE;//发送邮箱优先级DISABLE:ID小的先发送
	CAN_Init(CAN1,&CANINITSTRUCT);
	
	//配置过滤器
	CAN_FilterInitTypeDef CANFIDEF;
	CANFIDEF.CAN_FilterActivation=ENABLE;//配置过滤器关联，也就是通过这个过滤器的报文进FIFO0还是FIFO1排队
	CANFIDEF.CAN_FilterFIFOAssignment =CAN_FilterFIFO0;//过滤器开关
	
	CANFIDEF.CAN_FilterIdHigh=0x100<<5; //过滤器的两个32位核心寄存器R1和R2
	CANFIDEF.CAN_FilterIdLow=0x200<<5;
	CANFIDEF.CAN_FilterMaskIdHigh=0x300<<5;
	CANFIDEF.CAN_FilterMaskIdLow=0x400<<5;
	
	CANFIDEF.CAN_FilterMode=CAN_FilterMode_IdList;//指定过滤器模式 IDMask:屏蔽模式
	CANFIDEF.CAN_FilterNumber=0;//第几个过滤器被初始化
	CANFIDEF.CAN_FilterScale=CAN_FilterScale_16bit;//指定过滤器位宽
	CAN_FilterInit(&CANFIDEF);
}
//发送函数
void MyCAN_Transmit(CanTxMsg *TxMessage)
{
	uint32_t Timer = 0;
	uint8_t TransmitMB = CAN_Transmit(CAN1,TxMessage);
	while(CAN_TransmitStatus(CAN1,TransmitMB) != CAN_TxStatus_Ok)
	{
		Timer++;
		if(Timer > 100000)
		{
			break;
		}
	}
}

uint8_t MyCAN_ReceiveFlag(void)
{
	//返回挂起报文的数目
	if(CAN_MessagePending(CAN1,CAN_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

//接收函数
void MyCAN_RECEIVE(CanRxMsg *CanRxessage)
{
	CAN_Receive(CAN1,CAN_FIFO0,CanRxessage);
}
