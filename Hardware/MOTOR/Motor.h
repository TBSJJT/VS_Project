#ifndef __MOTOR_H
#define __MOTOR_H

#include "stm32f4xx.h"


void Motor_Init(void);
void Load(int moto1, int moto2);
void Speed_PID(int16_t target_SpeedA,int16_t target_SpeedB,int16_t current_SpeedA,int16_t current_SpeedB);

#endif
