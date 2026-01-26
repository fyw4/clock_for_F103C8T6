/*
 * share_func.c
 *
 *  Created on: 2025年12月12日
 *      Author: WangQingChuan
 */

#include "share_func.h"
#include "key.h"

int check_key_press(void)
{
	// 按键返回
	if (KEY_Back == GPIO_PIN_RESET)
	{
		HAL_Delay(50); // 去抖
		if (KEY_Back == GPIO_PIN_RESET)
		{
			return 1; // 按键返回
		}
	}
	else if (KEY_Up == GPIO_PIN_RESET)
	{
		HAL_Delay(50); // 去抖
		if (KEY_Up == GPIO_PIN_RESET)
		{
			return 2; // 按键向左
		}
	}
	else if (KEY_Down == GPIO_PIN_RESET)
	{
		HAL_Delay(50); // 去抖
		if (KEY_Down == GPIO_PIN_RESET)
		{
			return 3; // 按键向右
		}
	}
	else if (KEY_Confirm == GPIO_PIN_RESET)
	{
		HAL_Delay(50); // 去抖
		if (KEY_Confirm == GPIO_PIN_RESET)
		{
			return 4; // 按键确认
		}
	}

	return 0;
}