/*
 * menu.c
 *
 *  Created on: Oct 20, 2025
 *      Author: WangQingChuan
 */

#include "menu.h"
#include "oled.h"
#include "key.h"
#include "flappy_bird.h"
#include "star_war.h"
#include "Dino.h"

uint8_t KeyNum; // 用于存储键码值

int menu2_games()
{
	int action = 0;
	static uint8_t flag = 1;

	while (1)
	{
		OLED_NewFrame();
		KeyNum = Key_GetNum();
		if (KeyNum == 1) // 上一项
		{
			flag--;
			if (flag == 0)
			{
				flag = 3;
			}
		}
		if (KeyNum == 2) // 下一项
		{
			flag++;
			if (flag == 4)
			{
				flag = 1;
			}
		}
		if (KeyNum == 3) // 确认
		{
			action = flag;
		}
		if (KeyNum == 4) // 后退
		{
			return 0;
		}

		if (1 == action)
		{
			dino_play();

			action = 0;
			HAL_Delay(300);
		}
		else if (2 == action)
		{
			star_war_play();

			action = 0;
			HAL_Delay(300);
		}
		else if (3 == action)
		{
			flappy_bird_play();

			action = 0;
			HAL_Delay(300);
		}

		switch (flag)
		{
		case 1:
		{

			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 2:
		{
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 3:
		{
			OLED_DrawImage(0, 32, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 4:
		{
			OLED_DrawImage(0, 48, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		default:
			break;
		}

		OLED_PrintString(10, 0, "跳跃小恐龙", &font16x16, OLED_COLOR_NORMAL);
		OLED_PrintString(10, 16, "星球大战", &font16x16, OLED_COLOR_NORMAL);
		OLED_PrintString(10, 32, "下坠小鸟", &font16x16, OLED_COLOR_NORMAL);

		OLED_ShowFrame();
	}
}
int menu1(void)
{
	static uint8_t flag = 1;

	while (1)
	{
		OLED_NewFrame();
		KeyNum = Key_GetNum();
		if (KeyNum == 1) // 上一项
		{
			flag--;
			if (flag == 0)
			{
				flag = 6;
			}
		}
		else if (KeyNum == 2) // 下一项
		{
			flag++;
			if (flag == 7)
			{
				flag = 1;
			}
		}
		else if (KeyNum == 3) // 确认
		{
			return flag;
		}

		switch (flag)
		{
		case 1:
		{
			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 2:
		{
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 3:
		{
			OLED_DrawImage(0, 32, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 4:
		{
			OLED_DrawImage(0, 48, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 5:
		{
			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		case 6:
		{
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			break;
		}
		default:
			break;
		}

		if (flag <= 4)
		{
			OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintString(10, 0, "设置", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "版本信息", &font16x16, OLED_COLOR_NORMAL);
		}

		OLED_ShowFrame();
	}
}
