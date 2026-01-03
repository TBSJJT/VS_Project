#include "Timer.h"


void TIM6_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE); // 使能时钟     84mhz               83 10000
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up; // 计数模式
	TIM_TimeBaseInitStruct.TIM_Period = 84 - 1 ; // 自动重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 10000 -1 ; // 分频系数
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct); // 初始化
	TIM_Cmd(TIM6, ENABLE); // 使能定时器6

	TIM_ITConfig(TIM6, TIM_IT_Update, ENABLE); // 允许定时器6更新中断
	NVIC_InitStructure.NVIC_IRQChannel = TIM6_DAC_IRQn;
	NVIC_EnableIRQ(TIM6_DAC_IRQn); // 使能TIM6中断
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

