#include "Menu.h"


uint8_t menu1(void)
{
	uint8_t KeyNum = 0;
	
	uint8_t Index = 0;
	OLED_Clear();
	OLED_ShowString(20,0, "TEXT1",OLED_8X16);
	OLED_ShowString(20,20,"TEXT2",OLED_8X16);
	OLED_ShowString(20,40,"TEXT3",OLED_8X16);
	OLED_Update();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)//选择执行函数
		{
			Index ++;
			if(Index == 3) Index = 0;	
				if(KeyNum == 2)//确认按键
				{
					OLED_Clear();
					OLED_Update();
					return Index;
				}
				switch(Index)
				{
					case 0: 
					{
						OLED_ShowString(0,0,"*",OLED_8X16);
						OLED_ShowString(0,20," ",OLED_8X16);
						OLED_ShowString(0,40," ",OLED_8X16);
						OLED_Update();
					}break;
					case 1: 
					{
						OLED_ShowString(0,0," ",OLED_8X16);
						OLED_ShowString(0,20,"*",OLED_8X16);
						OLED_ShowString(0,40," ",OLED_8X16);
						OLED_Update();
					}break;
					case 2: 
					{
						OLED_ShowString(0,0,"",OLED_8X16);
						OLED_ShowString(0,20," ",OLED_8X16);
						OLED_ShowString(0,40,"*",OLED_8X16);
						OLED_Update();
					}break;
				}
			}
		
		}
}
uint8_t Function1(void)
{

	uint8_t KeyNum = 0;
	
	uint8_t Index = 0;
	OLED_Clear();
	OLED_ShowString(20,0, "Text1Func1",OLED_8X16);
	OLED_ShowString(20,20,"Text1Func2",OLED_8X16);
	OLED_ShowString(20,40,"Text1Func3",OLED_8X16);
	OLED_Update();
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)//选择执行函数
		{
			Index ++;
			if(Index == 3) Index = 0;	
				switch(Index)
				{
					case 0: 
					{
						OLED_ShowString(0,0, "*",OLED_8X16);
						OLED_ShowString(0,20," ",OLED_8X16);
						OLED_ShowString(0,40," ",OLED_8X16);
						OLED_Update();
					}break;
					case 1: 
					{
						OLED_ShowString(0,0, " ",OLED_8X16);
						OLED_ShowString(0,20,"*",OLED_8X16);
						OLED_ShowString(0,40," ",OLED_8X16);
						OLED_Update();
					}break;
					case 2: 
					{
						OLED_ShowString(0,0, " ",OLED_8X16);
						OLED_ShowString(0,20," ",OLED_8X16);
						OLED_ShowString(0,40,"*",OLED_8X16);
						OLED_Update();
					}break;
				}
			}
			if(KeyNum == 2)//确认按键
			{
				if(Index == 0){ }
				if(Index == 1){ }
				if(Index == 2){ }
			}

			if(KeyNum == 3)//返回上一级
			{
				OLED_Clear();
				OLED_Update();
				return 0;
			}
		}
}
