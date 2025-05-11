#include "MyCAN.H"

void MyCan_Init(void)
{
	//����GPIOA��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
	
	//����CANʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1,ENABLE);
	
	//��ʼ��GOPIO
	GPIO_InitTypeDef GpioInitStruct;
	GpioInitStruct.GPIO_Mode=GPIO_Mode_AF_PP;//ʹ�ø����������ģʽ�����ſ���Ȩ��CAN����
	GpioInitStruct.GPIO_Pin=GPIO_Pin_12;
	GpioInitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GpioInitStruct);
	
	GpioInitStruct.GPIO_Mode=GPIO_Mode_IPU;//ʹ����������ģʽ��������Ĭ��״̬�Ǹߵ�ƽ
	GpioInitStruct.GPIO_Pin=GPIO_Pin_11;
	GpioInitStruct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&GpioInitStruct);
	
	//can�����ʼ��
	CAN_InitTypeDef CANINITSTRUCT;
	CANINITSTRUCT.CAN_ABOM=DISABLE;//�����Զ��ָ�disable���ֶ��ָ�
	CANINITSTRUCT.CAN_AWUM =DISABLE;//�Զ����� ENABLE���Զ�����
	CANINITSTRUCT.CAN_BS1  =CAN_BS1_2tq;//λʱ��ʱ��ε�TQ��
	CANINITSTRUCT.CAN_BS2  =CAN_BS2_3tq;//λʱ��ʱ��ε�TQ��
	CANINITSTRUCT.CAN_Mode =CAN_Mode_LoopBack;//CAN���蹤��ģʽ
	CANINITSTRUCT.CAN_NART =DISABLE;//ʹ�ܻ�ʧ�ܣ����Զ��ش�ģʽ�����ﲻ��Ҫ���Զ��ش�=�Զ��ش���������DISABLE
	CANINITSTRUCT.CAN_Prescaler=48;//��Ƶϵ��   
	//������ = APB1ʱ��Ƶ��/��Ƶϵ��/һλ��TQ���� 
	//=36MHz/48/(1+2+3) = 125K  �պ��ڸ���CAN(125K-1M)�ķ�Χ��
	CANINITSTRUCT.CAN_RFLM   =DISABLE;//fifo���� DISABLE:FIFO���ʱ���ı��ı��±��ĸ���
	CANINITSTRUCT.CAN_SJW    =CAN_SJW_2tq;//��ͬ���Ĳ������
	CANINITSTRUCT.CAN_TTCM   =DISABLE;//ʱ�䴥��ͨ��ģʽENABLE:����
	CANINITSTRUCT.CAN_TXFP   =DISABLE;//�����������ȼ�DISABLE:IDС���ȷ���
	CAN_Init(CAN1,&CANINITSTRUCT);
	
	//���ù�����
	CAN_FilterInitTypeDef CANFIDEF;
	CANFIDEF.CAN_FilterActivation=ENABLE;//���ù�����������Ҳ����ͨ������������ı��Ľ�FIFO0����FIFO1�Ŷ�
	CANFIDEF.CAN_FilterFIFOAssignment =CAN_FilterFIFO0;//����������
	
	CANFIDEF.CAN_FilterIdHigh=0x100<<5; //������������32λ���ļĴ���R1��R2
	CANFIDEF.CAN_FilterIdLow=0x200<<5;
	CANFIDEF.CAN_FilterMaskIdHigh=0x300<<5;
	CANFIDEF.CAN_FilterMaskIdLow=0x400<<5;
	
	CANFIDEF.CAN_FilterMode=CAN_FilterMode_IdList;//ָ��������ģʽ IDMask:����ģʽ
	CANFIDEF.CAN_FilterNumber=0;//�ڼ�������������ʼ��
	CANFIDEF.CAN_FilterScale=CAN_FilterScale_16bit;//ָ��������λ��
	CAN_FilterInit(&CANFIDEF);
}
//���ͺ���
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
	//���ع����ĵ���Ŀ
	if(CAN_MessagePending(CAN1,CAN_FIFO0) > 0)
	{
		return 1;
	}
	return 0;
}

//���պ���
void MyCAN_RECEIVE(CanRxMsg *CanRxessage)
{
	CAN_Receive(CAN1,CAN_FIFO0,CanRxessage);
}
