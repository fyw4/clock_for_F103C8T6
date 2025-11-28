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

RTC_TimeTypeDef MyRTC_Time; // ʱ��
RTC_DateTypeDef MyRTC_Date; // ����

int check_key_press(void)
{
	// ������Ӱ��������߼�?
	if (KEY_Back == GPIO_PIN_RESET)
	{
		HAL_Delay(10); // ������
		if (KEY_Back == GPIO_PIN_RESET)
		{
			return 1; // ����ȷʵ������
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
	unsigned char tmp[10] = {0};
	uint8_t flag = 0;

	static uint8_t mins = 0;

	static uint8_t hour1 = 0;
	static uint8_t hour1_moving = 0;
	static uint8_t hour1_offset = 0;

	static uint8_t hour2 = 0;
	static uint8_t hour2_moving = 0;
	static uint8_t hour2_offset = 0;

	static uint8_t min1 = 0;
	static uint8_t min1_moving = 0;
	static uint8_t min1_offset = 0;

	static uint8_t min2 = 0;
	static uint8_t min2_moving = 0;
	static uint8_t min2_offset = 0;

	static uint8_t secs1 = 0;
	static uint8_t sec1_moving = 0;
	static uint8_t sec1_offset = 0;

	static uint8_t secs2 = 0;
	static uint8_t sec2_moving = 0;
	static uint8_t sec2_offset = 0;

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
#if slide
		if (hour1 != MyRTC_Time.Hours / 10)
		{
			hour1_moving = 1;
			hour1_offset = 0;
			hour1 = MyRTC_Time.Hours / 10;
		}

		if (hour2 != MyRTC_Time.Hours % 10)
		{
			hour2_moving = 1;
			hour2_offset = 0;
			hour2 = MyRTC_Time.Hours % 10;
		}

		if (min1 != MyRTC_Time.Minutes / 10)
		{
			min1_moving = 1;
			min1_offset = 0;
			min1 = MyRTC_Time.Minutes / 10;
		}

		if (min2 != MyRTC_Time.Minutes % 10)
		{
			min2_moving = 1;
			min2_offset = 0;
			min2 = MyRTC_Time.Minutes % 10;
		}

		if (secs1 != MyRTC_Time.Seconds / 10)
		{
			sec1_moving = 1;
			sec1_offset = 0;
			secs1 = MyRTC_Time.Seconds / 10;
		}

		if (secs2 != MyRTC_Time.Seconds % 10)
		{
			sec2_moving = 1;
			sec2_offset = 0;
			secs2 = MyRTC_Time.Seconds % 10;
		}

		if (MyRTC_Time.Hours % 10 == 9 && MyRTC_Time.Minutes / 10 == 5 && MyRTC_Time.Minutes % 10 == 9 && MyRTC_Time.Seconds / 10 == 5 && MyRTC_Time.Seconds % 10 == 9)
		{
			hour1_offset += 1;
			if (hour1_offset > 15)
			{
				hour1_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
				OLED_PrintASCIIString_offset(3, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours / 10);
			OLED_PrintASCIIString(3, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes / 10 == 5 && MyRTC_Time.Minutes % 10 == 9 && MyRTC_Time.Seconds / 10 == 5 && MyRTC_Time.Seconds % 10 == 9)
		{
			hour2_offset += 1;
			if (hour2_offset > 15)
			{
				hour2_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
				OLED_PrintASCIIString_offset(30, 20, tmp, &afont24x19, hour2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Hours % 10);
			OLED_PrintASCIIString(30, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		if (MyRTC_Time.Seconds % 2 == 0)
		{
			OLED_PrintASCIIString(50, 20, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(50, 20, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Minutes % 10 == 9 && MyRTC_Time.Seconds / 10 == 5 && MyRTC_Time.Seconds % 10 == 9)
		{
			min1_offset += 1;
			if (min1_offset > 15)
			{
				min1_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
				OLED_PrintASCIIString_offset(60, 20, tmp, &afont24x19, min1_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes / 10);
			OLED_PrintASCIIString(60, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds / 10 == 5 && MyRTC_Time.Seconds % 10 == 9)
		{
			min2_offset += 1;
			if (min2_offset > 15)
			{
				min2_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
				OLED_PrintASCIIString_offset(80, 20, tmp, &afont24x19, min2_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", MyRTC_Time.Minutes % 10);
			OLED_PrintASCIIString(80, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (MyRTC_Time.Seconds % 10 == 9)
		{
			sec1_offset += 1;
			if (sec1_offset > 15)
			{
				sec1_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds / 10);
				OLED_PrintASCIIString_offset(103, 28, tmp, &afont16x11, sec1_offset, OLED_COLOR_NORMAL);
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
			sec2_offset += 1;
			if (sec2_offset > 15)
			{
				sec2_moving = 0;
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", MyRTC_Time.Seconds % 10);
				OLED_PrintASCIIString_offset(116, 28, tmp, &afont16x11, sec2_offset, OLED_COLOR_NORMAL);
			}
		}

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
