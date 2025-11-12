/*
 * menu.c
 *
 *  Created on: Oct 20, 2025
 *      Author: WangQingChuan
 */

#include "menu.h"
#include "oled.h"
#include "key.h"

uint8_t KeyNum; // 用于存储键码值

int menu2_games()
{
	int action = 0;
	uint8_t flag = 1;
	OLED_NewFrame();
	OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 0, "小恐龙游戏", &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 16, "星球大战游戏", &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();

	while (1)
	{
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
			OLED_NewFrame();
			OLED_ShowFrame();
			action = flag;
		}
		if (KeyNum == 4) // 后退
		{
			return 0;
		}

		if (1 == action)
		{
			show_line();
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
			OLED_NewFrame();
			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "跳跃小恐龙", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "星球大战", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "下坠小鸟", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();

			break;
		}
		case 2:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "跳跃小恐龙", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "星球大战", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "下坠小鸟", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 3:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 32, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "跳跃小恐龙", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "星球大战", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "下坠小鸟", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 4:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 48, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "跳跃小恐龙", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "星球大战", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "下坠小鸟", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		default:
			break;
		}
	}
}
int menu1(void)
{
	uint8_t flag = 1;
	/*初始显示*/
	OLED_NewFrame();
	OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();

	while (1)
	{
		KeyNum = Key_GetNum();
		if (KeyNum == 1) // 上一项
		{
			flag--;
			if (flag == 0)
			{
				flag = 6;
			}
		}
		if (KeyNum == 2) // 下一项
		{
			flag++;
			if (flag == 7)
			{
				flag = 1;
			}
		}
		if (KeyNum == 3) // 确认
		{
			OLED_NewFrame();
			OLED_ShowFrame();
			return flag;
		}
		switch (flag)
		{
		case 1:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 2:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 3:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 32, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 4:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 48, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "时间", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "游戏", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 32, "风扇控制", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 48, "温湿度", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 5:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 0, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "设置", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "版本信息", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		case 6:
		{
			OLED_NewFrame();
			OLED_DrawImage(0, 16, &Arrow_Data_Img, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 0, "设置", &font16x16, OLED_COLOR_NORMAL);
			OLED_PrintString(10, 16, "版本信息", &font16x16, OLED_COLOR_NORMAL);
			OLED_ShowFrame();
			break;
		}
		default:
			break;
		}
	}
}
