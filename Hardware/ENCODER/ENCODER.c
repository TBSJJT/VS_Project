#include "ENCODER.h"

void Encoder_TIM5_Init(void)
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef tim;
    TIM_ICInitTypeDef ic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5,  ENABLE);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_0 | GPIO_Pin_1;     
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;            // 或 GPIO_PuPd_UP 更抗干扰
    GPIO_Init(GPIOA, &gpio);

    GPIO_PinAFConfig(GPIOA, GPIO_PinSource0, GPIO_AF_TIM5);//复用
    GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_TIM5);

    TIM_TimeBaseStructInit(&tim);
    tim.TIM_Prescaler   = 0;												//不分频
    tim.TIM_CounterMode = TIM_CounterMode_Up;				//向上计数
    tim.TIM_Period      = 0xFFFFFFFF;              // TIM5 是 32位，建议用满量程
    TIM_TimeBaseInit(TIM5, &tim);

    TIM_EncoderInterfaceConfig(TIM5, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&ic);
    ic.TIM_ICFilter = 10;                           // 输入滤波(0~15)

    ic.TIM_Channel  = TIM_Channel_1;
    TIM_ICInit(TIM5, &ic);
    ic.TIM_Channel  = TIM_Channel_2;
    TIM_ICInit(TIM5, &ic);

    TIM_SetCounter(TIM5, 0);
    TIM_Cmd(TIM5, ENABLE);
}

void Encoder_TIM8_Init(void)
{
    GPIO_InitTypeDef gpio;
    TIM_TimeBaseInitTypeDef tim;
    TIM_ICInitTypeDef ic;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8,  ENABLE);

    GPIO_StructInit(&gpio);
    gpio.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;     // TIM8_CH1/CH2
    gpio.GPIO_Mode  = GPIO_Mode_AF;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    gpio.GPIO_OType = GPIO_OType_PP;
    gpio.GPIO_PuPd  = GPIO_PuPd_NOPULL;            // 或 GPIO_PuPd_UP
    GPIO_Init(GPIOC, &gpio);

    GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM8);
    GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_TIM8);

    TIM_TimeBaseStructInit(&tim);
    tim.TIM_Prescaler   = 0;
    tim.TIM_CounterMode = TIM_CounterMode_Up;
    tim.TIM_Period      = 0xFFFF;                  // TIM8 是 16位
    TIM_TimeBaseInit(TIM8, &tim);

    TIM_EncoderInterfaceConfig(TIM8, TIM_EncoderMode_TI12,
                               TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);

    TIM_ICStructInit(&ic);
    ic.TIM_ICFilter = 10;

    ic.TIM_Channel  = TIM_Channel_1;
    TIM_ICInit(TIM8, &ic);
    ic.TIM_Channel  = TIM_Channel_2;
    TIM_ICInit(TIM8, &ic);

    TIM_SetCounter(TIM8, 0);
    TIM_Cmd(TIM8, ENABLE);
}
/**********************
编码器
速度读取函数
入口参数：定时器
**********************/
int32_t Read_Speed(int tim_id)
{
    int32_t value = 0;

    switch (tim_id)
    {
        case 5:
            value = (int32_t)TIM_GetCounter(TIM5);
            TIM_SetCounter(TIM5, 0);
            break;

        case 8:
            value = (int32_t)(int16_t)TIM_GetCounter(TIM8); // 先当16位有符号，再扩展到32位
            TIM_SetCounter(TIM8, 0);
            break;

        default:
            value = 0;
            break;
    }
    return value;
}


void TIM5_IRQHandler(void)
{
		if(TIM_GetITStatus(TIM5,TIM_IT_Update)!=0)
		{
			TIM_ClearITPendingBit(TIM5,TIM_IT_Update);
		}
}
	
void TIM8_UP_TIM13_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM8, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM8, TIM_IT_Update);
    }
}

