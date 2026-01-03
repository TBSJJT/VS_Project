/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/main.h 
  * @author  MCD Application Team
  * @version V1.8.1
  * @date    27-January-2022
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2016 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx.h"
#include "OLED.h" 
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "ENCODER.h"
#include "Motor.h"
#include "PWM.h"
#include "NRF24L01.h"
#include "Timer.h"



#endif /* __MAIN_H */

#ifdef  USE_FULL_ASSERT

/**
  * @brief  报告发生 assert_param 错误的源文件名称和行号
  * @param  file: 源文件名指针
  * @param  line: assert_param 错误发生的源行号
  * @retval 无
  */
	
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* 用户可以添加自己的实现来报告文件名和行号，
     例如：printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  // 如果断言失败，进入死循环
  while (1)
  {
  }
}
#endif

/**
  * @}
  */

