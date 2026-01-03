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


int16_t TargetSpeedA;
int16_t TargetSpeedB;

volatile extern uint8_t SpeedPIDFlag;
volatile extern float outA;
volatile extern float outB;

volatile int32_t Left_Speed;
volatile int32_t Right_Speed;

static uint8_t disp_div = 0;

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

		OLED_ShowString(0,0,"TEST",OLED_8X16);
		OLED_Update();
		
		//NRF24L01_Init();
		TIM6_Init();

	
		while (1)
		{

				if (SpeedPIDFlag == 1)
				{
						SpeedPIDFlag = 0;
						Left_Speed  = Read_Speed(8);
						Right_Speed = Read_Speed(5);

						Speed_PID(30, 30, (int16_t)Left_Speed, (int16_t)Right_Speed);
				if (++disp_div >= 10)   // 10 * 10ms = 100ms
						{
								disp_div = 0;
								OLED_ShowSignedNum(50,15,Left_Speed,  3,OLED_8X16);
								OLED_ShowSignedNum(50,30,Right_Speed, 3,OLED_8X16);
								OLED_ShowFloatNum(0,15,outA,3,1,OLED_8X16);
								OLED_ShowFloatNum(0,30,outB,3,1,OLED_8X16);
							
								OLED_Update();
						}
//						Load(99,99);
				}


		}
}


