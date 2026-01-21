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
TIME_DATA time_data;
int8_t hour_tens_val = 0;
uint8_t flag = 0;

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

void dir_left_func(uint8_t *x_position, uint8_t *y_position)
{
	if (*x_position == HOUR_TENS_DIGIT_X)
	{
		*x_position = MINUTE_ONES_DIGIT_X;
	}
	else if (*x_position == MINUTE_ONES_DIGIT_X)
	{
		*x_position = MINUTE_TENS_DIGIT_X;
	}
	else if (*x_position == MINUTE_TENS_DIGIT_X)
	{
		*x_position = HOUR_ONES_DIGIT_X;
	}
	else if (*x_position == HOUR_ONES_DIGIT_X)
	{
		*x_position = HOUR_TENS_DIGIT_X;
	}
}

void dir_right_func(uint8_t *x_position, uint8_t *y_position)
{
	if (*x_position == HOUR_TENS_DIGIT_X)
	{
		*x_position = HOUR_ONES_DIGIT_X;
	}
	else if (*x_position == HOUR_ONES_DIGIT_X)
	{
		*x_position = MINUTE_TENS_DIGIT_X;
	}
	else if (*x_position == MINUTE_TENS_DIGIT_X)
	{
		*x_position = MINUTE_ONES_DIGIT_X;
	}
	else if (*x_position == MINUTE_ONES_DIGIT_X)
	{
		*x_position = SEC_TENS_DIGIT_X;
	}
	else if (*x_position == SEC_TENS_DIGIT_X)
	{
		*x_position = HOUR_TENS_DIGIT_X;
	}
}

void judge_add_val(int8_t *hour_tens_add_val, int8_t *hour_digit_add_val_max_3, int8_t *hour_digit_add_val_max_9, int8_t *min_tens_add_val, int8_t *min_digit_add_val, int8_t *sec_tens_add_val, int8_t *sec_digit_add_val, int8_t *sec_add_val, uint8_t *x_position, uint8_t *y_position)
{
	if (*x_position == HOUR_TENS_DIGIT_X)
	{
		(*hour_tens_add_val)++;
		if (*hour_tens_add_val > 2)
		{
			*hour_tens_add_val = 0;
		}
	}
	else if (*x_position == HOUR_ONES_DIGIT_X)
	{
		if (hour_tens_val <= 1)
		{
			(*hour_digit_add_val_max_9)++;
			if (*hour_digit_add_val_max_9 > 9)
			{
				*hour_digit_add_val_max_9 = 0;
			}
		}
		else
		{
			flag = 1;
			(*hour_digit_add_val_max_3)++;
			if (*hour_digit_add_val_max_3 > 3)
			{
				*hour_digit_add_val_max_3 = 0;
			}
		}
	}
	else if (*x_position == MINUTE_TENS_DIGIT_X)
	{
		(*min_tens_add_val)++;
		if (*min_tens_add_val > 5)
		{
			*min_tens_add_val = 0;
		}
	}
	else if (*x_position == MINUTE_ONES_DIGIT_X)
	{
		(*min_digit_add_val)++;
		if (*min_digit_add_val > 9)
		{
			*min_digit_add_val = 0;
		}
	}
	else if (*x_position == SEC_TENS_DIGIT_X)
	{
		(*sec_add_val) = 1;
	}

	return;
}

void my_RTC_settime()
{
	SaveTimeToBackup(); // ���˳�ǰ����ʱ��

	return;
}

int clock_UI()
{
	char tmp[20] = {0};

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
		time_data.year = DS3231_Time.year;
		time_data.mon = DS3231_Time.mon;
		time_data.day = DS3231_Time.day;
#else
		HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

		time_data.hour = MyRTC_Time.Hours;
		time_data.min = MyRTC_Time.Minutes;
		time_data.sec = MyRTC_Time.Seconds;
		time_data.year = MyRTC_Date.Year;
		time_data.mon = MyRTC_Date.Month;
		time_data.day = MyRTC_Date.Date;
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

		// 年月日显示
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%04d-%02d-%02d", (time_data.year + 2000), time_data.mon, time_data.day);
		OLED_PrintASCIIString(YEAR_DIGIT_X, YEAR_DIGIT_Y, tmp, &afont8x6, OLED_COLOR_NORMAL);

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

	static uint32_t last_blink_time = 0; // 上次闪烁时间
	static uint8_t blink_state = 0;		 // 闪烁状态（0或1）
	const uint16_t blink_interval = 500; // 闪烁间隔（毫秒）
	uint8_t x_position = HOUR_TENS_DIGIT_X;
	uint8_t y_position = HOUR_TENS_DIGIT_Y;
	uint8_t key_val = 0;

	int8_t hour_digit_val = 0;
	int8_t min_tens_val = 0;
	int8_t min_digit_val = 0;
	int8_t sec_tens_val = 0;
	int8_t sec_digit_val = 0;
	int8_t sec_val = 0;

	int8_t hour_tens_add_val = 0;
	int8_t hour_digit_add_val_max_3 = 0;
	int8_t hour_digit_add_val_max_9 = 0;
	int8_t min_tens_add_val = 0;
	int8_t min_digit_add_val = 0;
	int8_t sec_tens_add_val = 0;
	int8_t sec_digit_add_val = 0;
	int8_t sec_add_val = 0;
	int8_t sec_tmp_val = 0;

	int8_t time_val = 0;

	hour_tens_val = 0;

	while (1)
	{
		key_val = 0;

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

		// 绘制小时数十位数
		hour_tens_val = ((int8_t)time_data.hour / 10 + hour_tens_add_val) > 2 ? ((int8_t)time_data.hour / 10 + hour_tens_add_val - 3) : ((int8_t)time_data.hour / 10 + hour_tens_add_val);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", hour_tens_val);
		OLED_PrintASCIIString(HOUR_TENS_DIGIT_X, HOUR_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

		// 绘制小时数个位数
		if (hour_tens_val <= 1)
		{
			if (1 == flag) // 若x坐标在小时个位数上时调整过
			{
				hour_digit_add_val_max_9 = hour_digit_val + 10 - (int8_t)time_data.hour % 10; // 当十位数为0或者1时，保持个位数大小 和 十位数为2时的个位数大小 一样
				hour_digit_add_val_max_3 = 0;
				flag = 0;
			}

			hour_digit_val = ((int8_t)time_data.hour % 10 + hour_digit_add_val_max_9) > 9 ? ((int8_t)time_data.hour % 10 + hour_digit_add_val_max_9 - 10) : ((int8_t)time_data.hour % 10 + hour_digit_add_val_max_9);
		}
		else
		{
			if (((int8_t)time_data.hour % 10 + hour_digit_add_val_max_9) - 3 > 0) // 若个位数大小超过3
			{
				if ((int8_t)time_data.hour % 10 + hour_digit_add_val_max_9 - 10 <= 3 && (int8_t)time_data.hour % 10 + hour_digit_add_val_max_9 - 10 >= 0)
				{
					time_val = (int8_t)time_data.hour % 10 + hour_digit_add_val_max_9 - 10; // 保证十位数为0或1且个位数设置大小小于4后，当十位数变为2时，个位数保持不变
				}
				else
				{
					time_val = 3; // 保证十位数为0或1且个位数设置大小大于等于4后，当十位数变为2时，个位数为3
				}
			}
			else
			{
				time_val = (int8_t)time_data.hour % 10 + hour_digit_add_val_max_9;
			}
			hour_digit_val = (time_val + hour_digit_add_val_max_3) > 3 ? (time_val + hour_digit_add_val_max_3 - 4) : (time_val + hour_digit_add_val_max_3);
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", hour_digit_val);
		OLED_PrintASCIIString(HOUR_ONES_DIGIT_X, HOUR_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

		///////////////////////////////////
		// memset(tmp, 0, sizeof(tmp));
		// sprintf(tmp, "%d", (int8_t)time_data.hour % 10);
		// OLED_PrintASCIIString(0, 50, tmp, &afont8x6, OLED_COLOR_NORMAL);

		// memset(tmp, 0, sizeof(tmp));
		// sprintf(tmp, "%d", hour_digit_add_val_max_3);
		// OLED_PrintASCIIString(10, 50, tmp, &afont8x6, OLED_COLOR_NORMAL);

		// memset(tmp, 0, sizeof(tmp));
		// sprintf(tmp, "%d", hour_digit_add_val_max_9);
		// OLED_PrintASCIIString(20, 50, tmp, &afont8x6, OLED_COLOR_NORMAL);
		///////////////////////////////////

		memset(tmp, 0, sizeof(tmp));
		if (time_data.sec % 2 == 0)
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, ":", &afont24x19, OLED_COLOR_NORMAL);
		}
		else
		{
			OLED_PrintASCIIString(COLON_X, COLON_Y, " ", &afont24x19, OLED_COLOR_NORMAL);
		}

		// 绘制分钟数十位数
		min_tens_val = ((int8_t)time_data.min / 10 + min_tens_add_val) > 5 ? ((int8_t)time_data.min / 10 + min_tens_add_val - 6) : ((int8_t)time_data.min / 10 + min_tens_add_val);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", min_tens_val);
		OLED_PrintASCIIString(MINUTE_TENS_DIGIT_X, MINUTE_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

		// 绘制分钟数个位数
		min_digit_val = ((int8_t)time_data.min % 10 + min_digit_add_val) > 9 ? ((int8_t)time_data.min % 10 + min_digit_add_val - 10) : ((int8_t)time_data.min % 10 + min_digit_add_val);
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", min_digit_val);
		OLED_PrintASCIIString(MINUTE_ONES_DIGIT_X, MINUTE_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

		// 绘制秒数十位数
		// sec_tens_val = ((int8_t)time_data.sec / 10 + sec_tens_add_val) > 5 ? ((int8_t)time_data.sec / 10 + sec_tens_add_val - 6) : ((int8_t)time_data.sec / 10 + sec_tens_add_val);
		// memset(tmp, 0, sizeof(tmp));
		// sprintf(tmp, "%d", sec_tens_val);
		// OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

		// // 绘制秒数个位数
		// sec_digit_val = ((int8_t)time_data.sec % 10 + sec_digit_add_val) > 9 ? ((int8_t)time_data.sec % 10 + sec_digit_add_val - 10) : ((int8_t)time_data.sec % 10 + sec_digit_add_val);
		// memset(tmp, 0, sizeof(tmp));
		// sprintf(tmp, "%d", sec_digit_val);
		// OLED_PrintASCIIString(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

		// 绘制秒数
		if (1 == sec_add_val) // 若秒数增加标志位为1
		{
			sec_tmp_val = (int8_t)time_data.sec;
			sec_add_val = 0;
		}

		if (0 != sec_tmp_val)
		{
			sec_val = (int8_t)time_data.sec - sec_tmp_val;
			if (sec_val < 0)
			{
				sec_val = 0;
				sec_tmp_val = 0;
			}
		}
		else
		{
			sec_val = (int8_t)time_data.sec;
		}

		// 绘制秒数十位数
		sec_tens_val = sec_val / 10;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", sec_tens_val);
		OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

		// 绘制秒数个位数
		sec_digit_val = sec_val % 10;
		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%d", sec_digit_val);
		OLED_PrintASCIIString(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

		// 闪烁效果实现
		if (HAL_GetTick() - last_blink_time >= blink_interval)
		{
			blink_state = !blink_state;		 // 切换闪烁状态
			last_blink_time = HAL_GetTick(); // 更新上次闪烁时间
		}

		if (blink_state)
		{
			if (SEC_TENS_DIGIT_X == x_position) // 秒比别的数字小
			{
				OLED_PrintASCIIString(SEC_TENS_DIGIT_X, SEC_TENS_DIGIT_Y, " ", &afont16x11, OLED_COLOR_NORMAL);
				OLED_PrintASCIIString(SEC_ONES_DIGIT_X, SEC_ONES_DIGIT_Y, " ", &afont16x11, OLED_COLOR_NORMAL);
			}
			else
			{
				OLED_PrintASCIIString(x_position, y_position, " ", &afont24x19, OLED_COLOR_NORMAL);
			}
		}

		OLED_ShowFrame();

		key_val = check_key_press();
		if (key_val == 1) // 返回主界面
		{
			my_RTC_settime();
			HAL_Delay(180);
			return 0;
		}
		else if (key_val == 2) // 向上调整时间
		{
			judge_add_val(&hour_tens_add_val, &hour_digit_add_val_max_3, &hour_digit_add_val_max_9, &min_tens_add_val, &min_digit_add_val, &sec_tens_add_val, &sec_digit_add_val, &sec_add_val, &x_position, &y_position);
			HAL_Delay(180);
		}
		else if (key_val == 3) // 向右移动
		{
			dir_right_func(&x_position, &y_position);
			HAL_Delay(180);
		}
		else if (key_val == 4)
		{
			// 设置时间
			DS3231_TimeType time = {
				.hour = (uint8_t)hour_tens_val * 10 + (uint8_t)hour_digit_val,
				.min = (uint8_t)min_tens_val * 10 + (uint8_t)min_digit_val,
				//.sec = (uint8_t)sec_tens_val * 10 + (uint8_t)sec_digit_val};
				.sec = (uint8_t)sec_val};
			DS3231_Set_Time(&time);
			DS3231_Update();

			// 清除调整值
			hour_tens_add_val = 0;
			hour_digit_add_val_max_3 = 0;
			hour_digit_add_val_max_9 = 0;
			min_tens_add_val = 0;
			min_digit_add_val = 0;
			sec_tens_add_val = 0;
			sec_digit_add_val = 0;
			sec_add_val = 0;
			sec_tmp_val = 0;

			HAL_Delay(180);
		}
	}

	return 0;
}
