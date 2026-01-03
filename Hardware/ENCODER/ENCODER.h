#ifndef  _ENCODER_H
#define  _ENCODER_H

#include "stm32f4xx_it.h"


void TIM5_IRQHandler(void);
void TIM8_UP_TIM13_IRQHandler(void);
int32_t Read_Speed(int tim_id);
;
void Encoder_TIM5_Init(void);
void Encoder_TIM8_Init(void);
#endif


