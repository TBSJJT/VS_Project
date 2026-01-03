#include "PWM.h"


void PWM_Init_TIM9(uint16_t Psc, uint16_t Per)
{
    GPIO_InitTypeDef        GPIO_InitStruct;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    TIM_OCInitTypeDef       TIM_OCInitStruct;

    /* 1) 开时钟：GPIOE(AHB1) + TIM9(APB2) */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM9,  ENABLE);

    /* 2) 配 GPIO：PE5/PE6 复用输出 */
    GPIO_StructInit(&GPIO_InitStruct);
    GPIO_InitStruct.GPIO_Pin   = GPIO_Pin_5 | GPIO_Pin_6;
    GPIO_InitStruct.GPIO_Mode  = GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStruct.GPIO_PuPd  = GPIO_PuPd_NOPULL;   // 需要更稳可用 GPIO_PuPd_UP
    GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* 3) 选择复用功能：TIM9 -> AF3 */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource5, GPIO_AF_TIM9); // PE5 = TIM9_CH1
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource6, GPIO_AF_TIM9); // PE6 = TIM9_CH2

    /* 4) 配定时器时基 */
    TIM_TimeBaseStructInit(&TIM_TimeBaseInitStruct);
    TIM_TimeBaseInitStruct.TIM_Prescaler     = Psc;
    TIM_TimeBaseInitStruct.TIM_CounterMode   = TIM_CounterMode_Up;
    TIM_TimeBaseInitStruct.TIM_Period        = Per;
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInit(TIM9, &TIM_TimeBaseInitStruct);

    /* 5) 配 PWM 输出比较（CH1 + CH2） */
    TIM_OCStructInit(&TIM_OCInitStruct);
    TIM_OCInitStruct.TIM_OCMode      = TIM_OCMode_PWM1;
    TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStruct.TIM_OCPolarity  = TIM_OCPolarity_High;
    TIM_OCInitStruct.TIM_Pulse       = 0;               // 初始占空比 0

    TIM_OC1Init(TIM9, &TIM_OCInitStruct);               // PE5
    TIM_OC2Init(TIM9, &TIM_OCInitStruct);               // PE6

    /* 6) 预装载使能 */
    TIM_OC1PreloadConfig(TIM9, TIM_OCPreload_Enable);
    TIM_OC2PreloadConfig(TIM9, TIM_OCPreload_Enable);
    TIM_ARRPreloadConfig(TIM9, ENABLE);

    /* 7) 启动定时器 */
    TIM_Cmd(TIM9, ENABLE);
}


