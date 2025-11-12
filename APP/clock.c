/*
 * clock.c
 *
 *  Created on: Nov 12, 2025
 *      Author: WangQingChuan
 */
#include <time.h>

#include "clock.h"
#include "key.h"
#include "rtc.h"
#include "font.h"
#include "oled.h"

RTC_TimeTypeDef MyRTC_Time; // 时间
RTC_DateTypeDef MyRTC_Date; // 日期

int check_key_press(void)
{
	// 可以添加按键消抖逻辑
	if (KEY_Back == GPIO_PIN_RESET)
	{
		HAL_Delay(10); // 简单消抖
		if (KEY_Back == GPIO_PIN_RESET)
		{
			return 1; // 按键确实被按下
		}
	}
	return 0;
}

void my_RTC_settime()
{
	return;
}

int clock_UI()
{
	unsigned char tmp[10] = {0};
	while (1)
	{
		if (check_key_press())
		{
			return; // 按键按下后立即返回
		}

		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		OLED_NewFrame();
		// sprintf(tmp, "%02d-%02d-%02d", MyRTC_Date.Year, MyRTC_Date.Month, MyRTC_Date.Date);
		// OLED_PrintASCIIString(16, 0, tmp, &afont16x8, OLED_COLOR_NORMAL);

		// sprintf(tmp, "%02d:%02d:%02d", MyRTC_Time.Hours, MyRTC_Time.Minutes, MyRTC_Time.Seconds);
		// OLED_PrintASCIIString(16, 20, tmp, &afont24x12, OLED_COLOR_NORMAL);

		sprintf(tmp, "%02d:%02d:%02d", MyRTC_Time.Hours, MyRTC_Time.Minutes, MyRTC_Time.Seconds);
		OLED_PrintString(0, 0, tmp, &font64x16, OLED_COLOR_NORMAL);

		OLED_ShowFrame();

		for (int i = 0; i < 10; i++)
		{
			if (check_key_press())
			{
				return;
			}
			HAL_Delay(100);
		}
	}

	return 0;
}
