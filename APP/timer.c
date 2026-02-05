/*
 * timer.c
 *
 *  Created on: Jan 26, 2026
 *      Author: WangQingChuan
 */
#include <stdio.h>
#include "main.h"
#include "led.h"
#include "share_func.h"
#include "tim.h"
#include "font.h"
#include "oled.h"
#include "timer.h"
#include "key.h"
#include "clock.h"
#include "ds3231.h"
#include "interrupt.h"

TIME_DATA time_data;

int timer_UI()
{
    uint8_t key_val = 0;
    count_num = 0;
    char tmp[20] = {0};

    int32_t min_val = 0;
    int8_t sec_digit_val = 0;
    int8_t ms_tens_val = 0;
    int8_t ms_digit_val = 0;

    while (1)
    {
#if USE_DS3231
        DS3231_Read_All();
        DS3231_Read_Time();
        time_data.hour = DS3231_Time.hour;
        time_data.min = DS3231_Time.min;
        time_data.sec = DS3231_Time.sec;
        time_data.year = DS3231_Time.year;
        time_data.mon = DS3231_Time.mon;
        time_data.day = DS3231_Time.day;
        time_data.date = DS3231_Time.date;
#else
        HAL_RTC_GetTime(&hrtc, &MyRTC_Time, RTC_FORMAT_BIN);
        HAL_RTC_GetDate(&hrtc, &MyRTC_Date, RTC_FORMAT_BIN);

        time_data.hour = MyRTC_Time.Hours;
        time_data.min = MyRTC_Time.Minutes;
        time_data.sec = MyRTC_Time.Seconds;
        time_data.year = MyRTC_Date.Year;
        time_data.mon = MyRTC_Date.Month;
        time_data.day = MyRTC_Date.Date;
        time_data.date = DS3231_Time.date;
#endif
        OLED_NewFrame();

        key_val = check_key_press();
        if (key_val == 1)
        {
            // 返回主界面
            HAL_TIM_Base_Stop_IT(&htim4);
            return 0;
        }
        else if (key_val == 2)
        {
            // 开始计时
            HAL_TIM_Base_Start_IT(&htim4);
        }
        else if (key_val == 3)
        {
            // 暂停计时
            HAL_TIM_Base_Stop_IT(&htim4); // 停止定时器中断
        }
        else if (key_val == 4)
        {
            // 重置计时
            count_num = 0;
        }

        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%02d:%02d:%02d", time_data.hour, time_data.min, time_data.sec);
        OLED_PrintASCIIString(TIMER_CLOCK_X, TIMER_CLOCK_Y, tmp, &afont8x6, OLED_COLOR_NORMAL);

        // 绘制分数
        min_val = count_num / 6000;
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%ld", min_val / 10);
        OLED_PrintASCIIString(TIMER_MIN_TENS_DIGIT_X, TIMER_MIN_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%ld", min_val % 10);
        OLED_PrintASCIIString(TIMER_MIN_ONES_DIGIT_X, TIMER_MIN_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

        OLED_PrintASCIIString(TIMER_COLON_X, TIMER_COLON_Y, ".", &afont24x19, OLED_COLOR_NORMAL);

        // 绘制秒数
        sec_digit_val = (count_num / 100) % 60;
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d", sec_digit_val / 10);
        OLED_PrintASCIIString(TIMER_SEC_TENS_DIGIT_X, TIMER_SEC_TENS_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d", sec_digit_val % 10);
        OLED_PrintASCIIString(TIMER_SEC_ONES_DIGIT_X, TIMER_SEC_ONES_DIGIT_Y, tmp, &afont24x19, OLED_COLOR_NORMAL);

        // 绘制毫秒十位数
        ms_tens_val = (count_num / 10) % 10;
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d", ms_tens_val);
        OLED_PrintASCIIString(TIMER_MS_TENS_DIGIT_X, TIMER_MS_TENS_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);

        // 绘制毫秒个位数
        ms_digit_val = count_num % 10;
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d", ms_digit_val);
        OLED_PrintASCIIString(TIMER_MS_ONES_DIGIT_X, TIMER_MS_ONES_DIGIT_Y, tmp, &afont16x11, OLED_COLOR_NORMAL);
        OLED_ShowFrame();
    }

    OLED_ShowFrame();

    return 0;
}
