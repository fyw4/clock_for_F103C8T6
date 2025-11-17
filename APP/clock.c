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

RTC_TimeTypeDef MyRTC_Time; // Ê±ï¿½ï¿½
RTC_DateTypeDef MyRTC_Date; // ï¿½ï¿½ï¿½ï¿½

int check_key_press(void)
{
	// ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½Ó°ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½ß¼ï¿?
	if (KEY_Back == GPIO_PIN_RESET)
	{
		HAL_Delay(10); // ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		if (KEY_Back == GPIO_PIN_RESET)
		{
			return 1; // ï¿½ï¿½ï¿½ï¿½È·Êµï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
		}
	}
	return 0;
}

void my_RTC_settime()
{
	SaveTimeToBackup(); // ÔÚÍË³öÇ°±£´æÊ±¼ä

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
			return;
		}

		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		OLED_NewFrame();

		if (seconds % 2 == 0)
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
				return;
			}
			HAL_Delay(100);
		}
	}

	return 0;
}
