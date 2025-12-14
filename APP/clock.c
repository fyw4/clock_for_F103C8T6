/*
 * clock.c
 *
 *  Created on: Nov 12, 2025
 *      Author: WangQingChuan
 */
#include <time.h>
#include <stdio.h>
#include <string.h>

#include "clock.h"
#include "key.h"
#include "rtc.h"
#include "font.h"
#include "oled.h"
#include "share_func.h"

RTC_TimeTypeDef MyRTC_Time; // ʱ��
RTC_DateTypeDef MyRTC_Date; // ����

int check_key_press(void)
{
	// 按键返回
	if (KEY_Back == GPIO_PIN_RESET)
	{
		HAL_Delay(10); // 去抖
		if (KEY_Back == GPIO_PIN_RESET)
		{
			return 1; // 按键返回
		}
	}

	return 0;
}

void my_RTC_settime()
{
	SaveTimeToBackup(); // ���˳�ǰ����ʱ��

	return;
}

int clock_UI()
{
	char tmp[10] = {0};

	static uint8_t hour1 = 0;
	static uint8_t hour1_offset = 0;

	static uint8_t hour2 = 0;
	static uint8_t hour2_offset = 0;

	static uint8_t min1 = 0;
	static uint8_t min1_offset = 0;

	static uint8_t min2 = 0;
	static uint8_t min2_offset = 0;

	static uint8_t secs1 = 0;
	static uint8_t sec1_offset = 0;

	static uint8_t secs2 = 0;
	static uint8_t sec2_moving = 0;
	static uint8_t sec2_offset_up_to_down = 0;
	static uint8_t sec2_offset_up_to_down_1 = 0;
	while (1)
	{
		if (check_key_press())
		{
			my_RTC_settime();
			return 0;
		}

		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		OLED_NewFrame();
#if up_down
		if (hour1 != MyRTC_Time.Hours / 10)
		{
			hour1_offset = 0;
			hour1 = MyRTC_Time.Hours / 10;
		}

		if (hour2 != MyRTC_Time.Hours % 10)
		{
			hour2_offset = 0;
			hour2 = MyRTC_Time.Hours % 10;
		}

		if (min1 != MyRTC_Time.Minutes / 10)
		{
			min1_offset = 0;
			min1 = MyRTC_Time.Minutes / 10;
		}

		if (min2 != MyRTC_Time.Minutes % 10)
		{
			min2_offset = 0;
			min2 = MyRTC_Time.Minutes % 10;
		}

		if (secs1 != MyRTC_Time.Seconds / 10)
		{
			sec1_offset = 0;
			secs1 = MyRTC_Time.Seconds / 10;
		}

		if (secs2 != MyRTC_Time.Seconds % 10)
		{
			sec2_moving = 1;
			sec2_offset_up_to_down = 0;
			sec2_offset_up_to_down_1 = 0;
			secs2 = MyRTC_Time.Seconds % 10;
		}

		if (MyRTC_Time.Hours % 10 == 0 && MyRTC_Time.Minutes / 10 == 0 && MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			hour1_offset += 1;
			if (hour1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
				OLED_PrintASCIIString(5, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(5, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(5, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
			OLED_PrintASCIIString(5, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes / 10 == 0 && MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			hour2_offset += 1;
			if (hour2_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
				OLED_PrintASCIIString(25, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
				// OLED_PrintASCIIString_offset_mid_to_down(25, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(25, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
			OLED_PrintASCIIString(25, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		if (MyRTC_Time.Seconds % 2 == 0)
		{
			OLED_PrintASCIIString(45, 20, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(45, 20, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			min1_offset += 1;
			if (min1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
				OLED_PrintASCIIString(60, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(60, 20, tmp, &afont24x19, min1_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(60, 20, tmp, &afont24x19, min1_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
			OLED_PrintASCIIString(60, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			min2_offset += 1;
			if (min2_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
				OLED_PrintASCIIString(80, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
				// OLED_PrintASCIIString_offset_mid_to_down(80, 20, tmp, &afont24x19, min2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(80, 20, tmp, &afont24x19, min2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
			OLED_PrintASCIIString(80, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds % 10 == 0)
		{
			sec1_offset += 1;
			if (sec1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
				OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(103, 28, tmp, &afont16x11, sec1_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(103, 28, tmp, &afont16x11, sec1_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
			OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}

		if (sec2_moving)
		{
			sec2_offset_up_to_down += 1;
			if (sec2_offset_up_to_down > TIME_OFFSET) // 秒数个位从上到下移动
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds % 10); // 确保秒数在0-9之间
				OLED_PrintASCIIString(116, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else // 秒数固定
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds % 10);
				OLED_PrintASCIIString_offset_up_to_down(116, 28, tmp, &afont16x11, sec2_offset_up_to_down, OLED_COLOR_NORMAL);
			}
		}

		OLED_DrawFilledRectangle(116, 12 - ADJUST_VAL, 16, TIME_OFFSET, OLED_COLOR_REVERSED); // 挡住秒数个位上半部
		//挡住秒数个位下半部
		OLED_DrawFilledRectangle(116, 24 - ADJUST_VAL, 16, TIME_OFFSET, OLED_COLOR_REVERSED);
#elif up_to_mid
		if (hour1 != MyRTC_Time.Hours / 10)
		{
			hour1_offset = 0;
			hour1 = MyRTC_Time.Hours / 10;
		}

		if (hour2 != MyRTC_Time.Hours % 10)
		{
			hour2_offset = 0;
			hour2 = MyRTC_Time.Hours % 10;
		}

		if (min1 != MyRTC_Time.Minutes / 10)
		{
			min1_offset = 0;
			min1 = MyRTC_Time.Minutes / 10;
		}

		if (min2 != MyRTC_Time.Minutes % 10)
		{
			min2_offset = 0;
			min2 = MyRTC_Time.Minutes % 10;
		}

		if (secs1 != MyRTC_Time.Seconds / 10)
		{
			sec1_offset = 0;
			secs1 = MyRTC_Time.Seconds / 10;
		}

		if (secs2 != MyRTC_Time.Seconds % 10)
		{
			sec2_moving = 1;
			sec2_offset_up_to_down = 0;
			sec2_offset_up_to_down_1 = 0;
			secs2 = MyRTC_Time.Seconds % 10;
		}

		if (MyRTC_Time.Hours % 10 == 0 && MyRTC_Time.Minutes / 10 == 0 && MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			hour1_offset += 1;
			if (hour1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
				OLED_PrintASCIIString(5, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(5, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(5, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
			OLED_PrintASCIIString(5, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes / 10 == 0 && MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			hour2_offset += 1;
			if (hour2_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
				OLED_PrintASCIIString(25, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
				// OLED_PrintASCIIString_offset_mid_to_down(25, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(25, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
			OLED_PrintASCIIString(25, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		if (MyRTC_Time.Seconds % 2 == 0)
		{
			OLED_PrintASCIIString(45, 20, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(45, 20, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes % 10 == 0 && MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			min1_offset += 1;
			if (min1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
				OLED_PrintASCIIString(60, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(60, 20, tmp, &afont24x19, min1_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(60, 20, tmp, &afont24x19, min1_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
			OLED_PrintASCIIString(60, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds / 10 == 0 && MyRTC_Time.Seconds % 10 == 0)
		{
			min2_offset += 1;
			if (min2_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
				OLED_PrintASCIIString(80, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
				// OLED_PrintASCIIString_offset_mid_to_down(80, 20, tmp, &afont24x19, min2_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(80, 20, tmp, &afont24x19, min2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
			OLED_PrintASCIIString(80, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds % 10 == 0)
		{
			sec1_offset += 1;
			if (sec1_offset > TIME_OFFSET)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
				OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
				// OLED_PrintASCIIString_offset_mid_to_down(103, 28, tmp, &afont16x11, sec1_offset, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString_offset_up_to_down(103, 28, tmp, &afont16x11, sec1_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
			OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}

		if (sec2_moving)
		{
			sec2_offset_up_to_down += 1;
			if (sec2_offset_up_to_down > TIME_OFFSET) // 秒数个位从上到下移动
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds % 10); // 确保秒数在0-9之间
				OLED_PrintASCIIString(116, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else // 秒数固定
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds % 10);
				OLED_PrintASCIIString_offset_up_to_down(116, 28, tmp, &afont16x11, sec2_offset_up_to_down, OLED_COLOR_NORMAL);
			}
		}

		OLED_DrawFilledRectangle(116, 12 - ADJUST_VAL, 16, TIME_OFFSET, OLED_COLOR_REVERSED); // 挡住秒数个位上半部
#else
		if (MyRTC_Time.Seconds % 2 == 0)
		{
			sprintf(tmp, "%02d:%02d", MyRTC_Time.Hours, MyRTC_Time.Minutes);
			OLED_PrintASCIIString(3, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			sprintf(tmp, "%02d %02d", MyRTC_Time.Hours, MyRTC_Time.Minutes);
			OLED_PrintASCIIString(3, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%02d", MyRTC_Time.Seconds);
		OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);

#endif
		OLED_ShowFrame();

		if (check_key_press())
		{
			my_RTC_settime();
			return 0;
		}
	}

	return 0;
}
