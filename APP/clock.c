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
#include "ds3231.h"

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

	static uint8_t hour_dec_old = 0;
	static uint8_t hour_dec_offset = 0;
	static uint8_t hour_dig_old = 0;
	static uint8_t hour_dig_offset = 0;

	static uint8_t min_dec_old = 0;
	static uint8_t min_dec_offset = 0;
	static uint8_t min_dig_old = 0;
	static uint8_t min_dig_offset = 0;

	static uint8_t secs_dec_old = 0;
	static uint8_t sec_dec_offset = 0;
	static uint8_t secs_dig_old = 0;
	static uint8_t sec_dec_moving = 0;
	static uint8_t sec_dec_offset_up_to_down = 0;

	TIME_DATA time_data;

	while (1)
	{
		if (check_key_press())
		{
			my_RTC_settime();
			return 0;
		}

#if USE_DS3231
		DS3231_Read_All();
		DS3231_Read_Time();
		time_data.hour = DS3231_Time.hour;
		time_data.min = DS3231_Time.min;
		time_data.sec = DS3231_Time.sec;
#else
		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		time_data.hour = MyRTC_Time.Hours;
		time_data.min = MyRTC_Time.Minutes;
		time_data.sec = MyRTC_Time.Seconds;
#endif

		OLED_NewFrame();

		if (hour_dec_old != time_data.hour / 10)
		{
			hour_dec_offset = 0;
			hour_dec_old = time_data.hour / 10;
		}

		if (hour_dig_old != time_data.hour % 10)
		{
			hour_dig_offset = 0;
			hour_dig_old = time_data.hour % 10;
		}

		if (min_dec_old != time_data.min / 10)
		{
			min_dec_offset = 0;
			min_dec_old = time_data.min / 10;
		}

		if (min_dig_old != time_data.min % 10)
		{
			min_dig_offset = 0;
			min_dig_old = time_data.min % 10;
		}

		if (secs_dec_old != time_data.sec / 10)
		{
			sec_dec_offset = 0;
			secs_dec_old = time_data.sec / 10;
		}

		if (secs_dig_old != time_data.sec % 10)
		{
			sec_dec_moving = 1;
			sec_dec_offset_up_to_down = 0;
			secs_dig_old = time_data.sec % 10;
		}
#if up_to_down
		if (time_data.hour % 10 == 0 && time_data.min / 10 == 0 && time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			hour_dec_offset += 1;
			if (hour_dec_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour / 10);
				OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour / 10);
				OLED_PrintASCIIString_offset_up_to_down(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, hour_dig_offset, OLED_COLOR_NORMAL);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.hour - 1) / 10);
				OLED_PrintASCIIString_offset_mid_to_down(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, hour_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.hour / 10);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.min / 10 == 0 && time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			hour_dig_offset += 1;
			if (hour_dig_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour % 10);
				OLED_PrintASCIIString(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour % 10);
				OLED_PrintASCIIString_offset_up_to_down(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, hour_dig_offset, OLED_COLOR_NORMAL);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.hour - 1) % 10);
				OLED_PrintASCIIString_offset_mid_to_down(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, hour_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.hour % 10);
			OLED_PrintASCIIString(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		if (time_data.sec % 2 == 0)
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			min_dec_offset += 1;
			if (min_dec_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min / 10);
				OLED_PrintASCIIString(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min / 10);
				OLED_PrintASCIIString_offset_up_to_down(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, min_dec_offset, OLED_COLOR_NORMAL);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.min - 1) / 10);
				OLED_PrintASCIIString_offset_mid_to_down(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, min_dec_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.min / 10);
			OLED_PrintASCIIString(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			min_dig_offset += 1;
			if (min_dig_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min % 10);
				OLED_PrintASCIIString(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min % 10);
				OLED_PrintASCIIString_offset_up_to_down(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, min_dig_offset, OLED_COLOR_NORMAL);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.min - 1) % 10);
				OLED_PrintASCIIString_offset_mid_to_down(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, min_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.min % 10);
			OLED_PrintASCIIString(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.sec % 10 == 0)
		{
			sec_dec_offset += 1;
			if (sec_dec_offset > TIME_OFFSET_LITTER)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec / 10);
				OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec / 10);
				OLED_PrintASCIIString_offset_up_to_down(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, TIME_OFFSET_LITTER, sec_dec_offset, OLED_COLOR_NORMAL);

				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.sec - 1) / 10);
				OLED_PrintASCIIString_offset_mid_to_down(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, sec_dec_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.sec / 10);
			OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}

		if (sec_dec_moving)
		{
			sec_dec_offset_up_to_down += 1;
			if (sec_dec_offset_up_to_down > TIME_OFFSET_LITTER)
			{
				// 保持不动
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec % 10); // 确保秒数在0-9之间
				OLED_PrintASCIIString(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				// 当前秒数个位从上到中移动
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec % 10);
				OLED_PrintASCIIString_offset_up_to_down(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, TIME_OFFSET_LITTER, sec_dec_offset_up_to_down, OLED_COLOR_NORMAL);

				// 上一秒数从中到下
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", (time_data.sec - 1) % 10);
				OLED_PrintASCIIString_offset_mid_to_down(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, sec_dec_offset_up_to_down, OLED_COLOR_NORMAL);
			}
		}

		OLED_DrawFilledRectangle(SEC_ONES_DIGIT_X, 12, 16, TIME_OFFSET_LITTER - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住秒数个位上半部
		OLED_DrawFilledRectangle(SEC_TENS_DIGIT_X, 12, 16, TIME_OFFSET_LITTER - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住秒数十位上半部

		OLED_DrawFilledRectangle(MINUTE_ONES_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住分钟数个位上半部
		OLED_DrawFilledRectangle(MINUTE_TENS_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住分钟数十位上半部

		OLED_DrawFilledRectangle(HOUR_ONES_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住小时数十位上半部
		OLED_DrawFilledRectangle(HOUR_TENS_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住日期数个位上半部

#elif up_to_mid
		if (time_data.hour % 10 == 0 && time_data.min / 10 == 0 && time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			hour_dec_offset += 1;
			if (hour_dec_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour / 10);
				OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour / 10);
				OLED_PrintASCIIString_offset_up_to_down(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, hour_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.hour / 10);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.min / 10 == 0 && time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			hour_dig_offset += 1;
			if (hour_dig_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour % 10);
				OLED_PrintASCIIString(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.hour % 10);
				OLED_PrintASCIIString_offset_up_to_down(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, hour_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.hour % 10);
			OLED_PrintASCIIString(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		if (time_data.sec % 2 == 0)
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.min % 10 == 0 && time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			min_dec_offset += 1;
			if (min_dec_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min / 10);
				OLED_PrintASCIIString(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min / 10);
				OLED_PrintASCIIString_offset_up_to_down(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, min_dec_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.min / 10);
			OLED_PrintASCIIString(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.sec / 10 == 0 && time_data.sec % 10 == 0)
		{
			min_dig_offset += 1;
			if (min_dig_offset > TIME_OFFSET_LARGE)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min % 10);
				OLED_PrintASCIIString(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.min % 10);
				OLED_PrintASCIIString_offset_up_to_down(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, TIME_OFFSET_LARGE, min_dig_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.min % 10);
			OLED_PrintASCIIString(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		if (time_data.sec % 10 == 0)
		{
			sec_dec_offset += 1;
			if (sec_dec_offset > TIME_OFFSET_LITTER)
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec / 10);
				OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec / 10);
				OLED_PrintASCIIString_offset_up_to_down(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, TIME_OFFSET_LITTER, sec_dec_offset, OLED_COLOR_NORMAL);
			}
		}
		else
		{
			memset(tmp, 0, sizeof(tmp));
			sprintf(tmp, "%d", time_data.sec / 10);
			OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
		}

		if (sec_dec_moving)
		{
			sec_dec_offset_up_to_down += 1;
			if (sec_dec_offset_up_to_down > TIME_OFFSET_LITTER) // 秒数个位从上到下移动
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec % 10); // 确保秒数在0-9之间
				OLED_PrintASCIIString(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
			}
			else // 秒数固定
			{
				memset(tmp, 0, sizeof(tmp));
				sprintf(tmp, "%d", time_data.sec % 10);
				OLED_PrintASCIIString_offset_up_to_down(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, TIME_OFFSET_LITTER, sec_dec_offset_up_to_down, OLED_COLOR_NORMAL);
			}
		}

		OLED_DrawFilledRectangle(SEC_ONES_DIGIT_X, 12, 16, TIME_OFFSET_LITTER - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住秒数个位上半部
		OLED_DrawFilledRectangle(SEC_TENS_DIGIT_X, 12, 16, TIME_OFFSET_LITTER - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住秒数十位上半部

		OLED_DrawFilledRectangle(MINUTE_ONES_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住分钟数个位上半部
		OLED_DrawFilledRectangle(MINUTE_TENS_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住分钟数十位上半部

		OLED_DrawFilledRectangle(HOUR_ONES_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住小时数十位上半部
		OLED_DrawFilledRectangle(HOUR_TENS_DIGIT_X, 0, 20, TIME_OFFSET_LARGE - ADJUST_VAL, OLED_COLOR_REVERSED); // 挡住日期数个位上半部

#else
		if (time_data.sec % 2 == 0)
		{
			sprintf(tmp, "%02d:%02d", time_data.hour, time_data.min);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			sprintf(tmp, "%02d %02d", time_data.hour, time_data.min);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%02d", time_data.sec);
		OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

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

int clock_setting()
{
	char tmp[10] = {0};
	TIME_DATA time_data;

	while (1)
	{
#if USE_DS3231
		DS3231_Read_All();
		DS3231_Read_Time();
		time_data.hour = DS3231_Time.hour;
		time_data.min = DS3231_Time.min;
		time_data.sec = DS3231_Time.sec;
#else
		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		time_data.hour = MyRTC_Time.Hours;
		time_data.min = MyRTC_Time.Minutes;
		time_data.sec = MyRTC_Time.Seconds;
#endif
		OLED_NewFrame();

		if (time_data.sec % 2 == 0)
		{
			sprintf(tmp, "%02d:%02d", time_data.hour, time_data.min);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			sprintf(tmp, "%02d %02d", time_data.hour, time_data.min);
			OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%02d", time_data.sec);
		OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

		OLED_ShowFrame();
		if (check_key_press())
		{
			my_RTC_settime();
			return 0;
		}
	}

	return 0;
}