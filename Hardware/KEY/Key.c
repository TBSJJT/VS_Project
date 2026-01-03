#include "Key.h"

#define Key1_CLK    RCC_AHB1Periph_GPIOE
#define Key1_Pin    GPIO_Pin_11
#define Key1_Port   GPIOE

#define Key2_CLK    RCC_AHB1Periph_GPIOE
#define Key2_Pin    GPIO_Pin_9
#define Key2_Port   GPIOE

//共阴极

void Key_Init(void)
{
		RCC_AHB1PeriphClockCmd(
			Key1_CLK | Key2_CLK ,
			ENABLE
		);
		GPIO_InitTypeDef GPIO_InitStructure;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
		GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
		GPIO_InitStructure.GPIO_Pin = Key1_Pin | Key2_Pin;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_Init(Key1_Port ,&GPIO_InitStructure);

}


uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;		//定义变量，默认键码值为0
	
	if (GPIO_ReadInputDataBit(Key1_Port, Key1_Pin) == 0)			
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(Key1_Port, Key1_Pin) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 1;												
	}
	if (GPIO_ReadInputDataBit(Key2_Port, Key2_Pin) == 0)			
	{
		Delay_ms(20);											//延时消抖
		while (GPIO_ReadInputDataBit(Key2_Port, Key2_Pin) == 0);	//等待按键松手
		Delay_ms(20);											//延时消抖
		KeyNum = 2;												
	}	
	
	return KeyNum;			//返回键码值，如果没有按键按下，所有if都不成立，则键码为默认值0
}
