#include "Delay.h"


// 设置 my_us 为 21MHz 时 1 微秒所需要的计数值
u32 my_us = 21;  	// 21MHz 时，1 微秒对应的计数值
u32 my_ms = 21000;  // 21MHz 时，1 毫秒对应的计数值

/**
  * @brief  初始化 SysTick 延迟函数。
  * @param  None
  * @retval None
  */
void Delay_Init(void)
{
	// 配置 SysTick 时钟源为 HCLK/8，HCLK 为 168 MHz，HCLK/8 为 21 MHz
	// 这样 SysTick 计数器每次计数一个时钟周期就表示 1/21MHz 的时间
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);//
}

/**
  * @brief  微秒级延迟。
  * @param  nus 延迟的微秒数
  * @retval None
  */
void Delay_us(u32 nus)
{
	u32 temp = 0x00;
	
	// 计算重装载值，SysTick 计数器会从此值开始递减，直到 0
	// nus*my_us 是为了让 SysTick 计时符合微秒延迟
	SysTick->LOAD = nus * my_us - 1;
	// 清除当前计数值，保证计时从 0 开始
	SysTick->VAL = 0x00;
	
	// 启动 SysTick 定时器
	SysTick->CTRL |= (0x01 << 0);
	
	// 等待 SysTick 计数到 0
	// 当 SysTick 计数到 0 时，SysTick 的 CTRL 寄存器会设置第 16 位为 1
	// 通过检查第 0 位和第 16 位来判断计时是否完成
	do {
		temp = SysTick->CTRL;
		// 等待 SysTick 计时完成（第 16 位为 1）
	} while ((temp & (0x01 << 0)) && (!(temp & (0x01 << 16))));
	
	// 关闭 SysTick 定时器
	SysTick->CTRL &= ~(0x01 << 0);	
}

/**
  * @brief  毫秒级延迟。
  * @param  nms 延迟的毫秒数
  * @retval None
  */
void Delay_ms(u32 nms)
{
	u32 temp = 0x00;
	
	// 计算重装载值，SysTick 计数器会从此值开始递减，直到 0
	// nms*my_ms 是为了让 SysTick 计时符合毫秒延迟
	SysTick->LOAD = nms * my_ms - 1;
	// 清除当前计数值，保证计时从 0 开始
	SysTick->VAL = 0x00;
	
	// 启动 SysTick 定时器
	SysTick->CTRL |= (0x01 << 0);
	
	// 等待 SysTick 计数到 0
	// 当 SysTick 计数到 0 时，SysTick 的 CTRL 寄存器会设置第 16 位为 1
	// 通过检查第 0 位和第 16 位来判断计时是否完成
	do {
		temp = SysTick->CTRL;
		// 等待 SysTick 计时完成（第 16 位为 1）
	} while ((temp & (0x01 << 0)) && (!(temp & (0x01 << 16))));
	
	// 关闭 SysTick 定时器
	SysTick->CTRL &= ~(0x01 << 0);	
}

/**
  * @brief  秒级延迟。
  * @param  ns 延迟的秒数
  * @retval None
  */
void Delay_s(u32 ns)
{
	int i;
	
	// 延时指定的秒数，每秒调用 2 次 500 毫秒延迟
	for(i = 0; i < ns; i++)
	{
		Delay_ms(500);  // 延时 500 毫秒
		Delay_ms(500);  // 再延时 500 毫秒，累计延时 1 秒
	}
}
