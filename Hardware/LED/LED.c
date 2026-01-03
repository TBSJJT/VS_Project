#include "LED.h"

void LED_Init(void)
{
	/*开启时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);		//开启GPIOA的时钟
	
	/*GPIO初始化*/
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);						//将PA3和PA5引脚初始化为推挽输出
	
	/*设置GPIO初始化后的默认电平*/
	GPIO_SetBits(GPIOA, GPIO_Pin_3 | GPIO_Pin_5);				//设置PA1和PA2引脚为高电平
}


void LED1_ON(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_3);		//设置PA1引脚为低电平
}


void LED1_OFF(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_3);		//设置PA1引脚为高电平
}


void LED1_Turn(void)
{
	if (GPIO_ReadOutputDataBit(GPIOA, GPIO_Pin_3) == 0)		//获取输出寄存器的状态，如果当前引脚输出低电平
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_3);					//则设置PA1引脚为高电平
	}
	else													//否则，即当前引脚输出高电平
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);					//则设置PA1引脚为低电平
	}
}


void LED2_ON(void)
{
	GPIO_SetBits(GPIOA, GPIO_Pin_5);		//设置PA2引脚为低电平
}


void LED2_OFF(void)
{
	GPIO_ResetBits(GPIOA, GPIO_Pin_5);		//设置PA2引脚为高电平
}


void LED2_Turn(void)
{
	GPIO_ToggleBits(GPIOA,GPIO_Pin_5);
}
