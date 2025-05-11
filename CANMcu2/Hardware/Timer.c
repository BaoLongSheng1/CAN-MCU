#include "stm32f10x.h"                  // Device header
#include "Timer.h"
void Timer_Init()
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef TimBaseInitStruct;
	TimBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;
	TimBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up;
	TimBaseInitStruct.TIM_Period=10000-1;
	TimBaseInitStruct.TIM_Prescaler=7200-1;
	TimBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TimBaseInitStruct);
	
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	
	NVIC_InitTypeDef NVIC_InitStruct;
	NVIC_InitStruct.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority=2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority=1;
	NVIC_Init(&NVIC_InitStruct);
	
	TIM_Cmd(TIM4,ENABLE);
}
