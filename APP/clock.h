/*
 * clock.h
 *
 *  Created on: Nov 12, 2025
 *      Author: WangQingChuan
 */

#ifndef CLOCK_H_
#define CLOCK_H_

#include "main.h"

#define USE_DS3231 1
#define USE_RTC 0

#define up_to_down 1
#define up_to_mid 0
#define test 0

#define TIME_OFFSET_LARGE 24
#define TIME_OFFSET_LITTER 16
#define ADJUST_VAL 4

#define YEAR_DIGIT_X 5
#define YEAR_DIGIT_Y 8
#define YEAR_DIGIT_SETTING_Y 20
#define MON_DIGIT_X 60
#define MON_DIGIT_Y 8
#define MON_DIGIT_SETTING_Y 20
#define DATE_DIGIT_X 90
#define DATE_DIGIT_Y 8
#define DATE_DIGIT_SETTING_Y 20
#define DAY_DIGIT_X 100
#define DAY_DIGIT_Y 8
#define DAY_DIGIT_SETTING_Y 20

#define HOUR_TENS_DIGIT_X 5
#define HOUR_TENS_DIGIT_Y 20
#define HOUR_ONES_DIGIT_X 25
#define HOUR_ONES_DIGIT_Y 20

#define MINUTE_TENS_DIGIT_X 60
#define MINUTE_TENS_DIGIT_Y 20
#define MINUTE_ONES_DIGIT_X 80
#define MINUTE_ONES_DIGIT_Y 20

#define SEC_TENS_DIGIT_X 103
#define SEC_TENS_DIGIT_Y 28
#define SEC_ONES_DIGIT_X 116
#define SEC_ONES_DIGIT_Y 28

#define SEC_DIGIT_X 103
#define SEC_DIGIT_Y 28

#define COLON_X 45
#define COLON_Y 20

typedef struct
{
    uint8_t year;
    uint8_t mon;
    uint8_t day;  // ÐÇÆÚ£¨1~7£©
    uint8_t date; // ÈÕÆÚ£¨1~31£©
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
} TIME_DATA;

int clock_UI(void);

int clock_setting(void);

int calendar_setting(void);

#endif /* CLOCK_H_ */
