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

		if (MyRTC_Time.Seconds % 2 == 0)
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%02d:%02d", MyRTC_Time.Hours, MyRTC_Time.Minutes);
			OLED_PrintASCIIString(3, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);

			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%02d", MyRTC_Time.Seconds);
			OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}
		else
		{
			sprintf(tmp, "%02d %02d", MyRTC_Time.Hours, MyRTC_Time.Minutes);
			OLED_PrintASCIIString(3, 20, tmp, &afont24x19, OLED_COLOR_NORMAL);

			memset(tmp, 0, sizeof(tmp));

			sprintf(tmp, "%02d", MyRTC_Time.Seconds);
			OLED_PrintASCIIString(103, 28, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}

		OLED_ShowFrame();

		for (int i = 0; i < 10; i++)
		{
			if (check_key_press())
			{
				my_RTC_settime();
				return 0;
			}
			HAL_Delay(100);
		}
	}

	return 0;
}
