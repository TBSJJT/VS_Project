#include "main.h"

/* 
													[2-63]  [192-432]  {2 4 6 8}
Input Frequency -> HSE -> /PLL_M -> *PLL_N -> /PLL_P    ---> PLLCLK
																								|
																							/PLL_Q    --->48MHz
																							[2-15]
																							
																							
				8M						/			8		*		336			/		2						168MHz
				
																						/		7 = 48MHz
*/

uint8_t NRF_Val[32];
uint8_t KeyNum;


int16_t TargetSpeedA;//目标速度
int16_t TargetSpeedB;

float kp = 1.0f;
float ki = 0.1f;
float kd = 0.0f;


volatile  int32_t Left_Speed;//编码器速度
volatile  int32_t Right_Speed;

volatile extern float outA;//PID输出
volatile extern float outB;

int main(void)
{
		Delay_Init();		
		OLED_Init();//PB7 PB8
		LED_Init();//PA3 PA5
		PWM_Init_TIM9(83, 99);//PE5 PE6 
		Encoder_TIM5_Init();//PA0 PA1
		Encoder_TIM8_Init();//PC6 PC7
		Motor_Init();//PE8 PE10 PB1 PC5
	
		Key_Init();//PE11 PE9

		OLED_ShowString(0,0,"pidoutA",OLED_8X16);
		OLED_ShowString(0,15,"pidoutB",OLED_8X16);
		OLED_Update();
		
		//NRF24L01_Init();
		TIM6_Init();

	
		while (1)
		{

				OLED_ShowFloatNum(0,0,outA,3,1,OLED_8X16);
				OLED_ShowFloatNum(0,15,outB,3,1,OLED_8X16);
			
				OLED_Update();

		}
}

//10ms运算一次速度PID
void TIM6_DAC_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM6, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM6, TIM_IT_Update);
        				
				Left_Speed  = Read_Speed(8);
				Right_Speed = Read_Speed(5);
			
				Speed_PID(TargetSpeedA, TargetSpeedB, (int16_t)Left_Speed, (int16_t)Right_Speed);
    }
}

