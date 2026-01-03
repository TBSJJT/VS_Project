#ifndef __DELAY_H
#define __DELAY_H

#include "stm32f4xx_it.h"

void Delay_Init(void);

void Delay_us(u32 nus);

void Delay_ms(u32 nms);

void Delay_s(u32 ns);


#endif


