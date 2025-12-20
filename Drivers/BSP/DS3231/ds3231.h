/*
 * ds3231.h
 *
 *  Created on: Dec 15, 2025
 *      Author: WangQingChuan
 */

#ifndef BSP_DS3231_DS3231_H_
#define BSP_DS3231_DS3231_H_

#include "main.h"
#include "i2c.h"

typedef enum
{
    SECONDS = 0,
    MINUTES,
    HOURS,
    DAY,
    DATE,
    MONTH_CENTURY,
    YEAR,
    ALARM_1_SECONDS,
    ALARM_1_MINUTES,
    ALARM_1_HOURS,
    ALARM_1_DAY_DATE,
    ALARM_2_MINUTES,
    ALARM_2_HOURS,
    ALARM_2_DAY_DATE,
    CONTROL,
    CONTROL_STATUS,
    AGING_OFFSET,
    TEMP_MSB,
    TEMP_LSB
} DS3231_REG;

/*小时制*/
typedef enum
{
    HOUR_FORM_24,
    HOUR_FORM_12
} DS3231_HOUR_FORM;

/*上午下午*/
typedef enum
{
    AM,
    PM
} AM_PM;

/*DS3231寄存器结构体*/
typedef struct
{
    uint8_t Seconds;
    uint8_t Minutes;
    uint8_t Hours;
    uint8_t Day;
    uint8_t Date;
    uint8_t Month_Century;
    uint8_t Year;

    uint8_t Alarm_1_Seconds;
    uint8_t Alarm_1_Minutes;
    uint8_t Alarm_1_Hours;
    uint8_t Alarm_1_Day_Date;

    uint8_t Alarm_2_Minutes;
    uint8_t Alarm_2_Hours;
    uint8_t Alarm_2_Day_Date;

    uint8_t Control;
    uint8_t Control_Status;
    uint8_t Aging_Offset;
    uint8_t Temp_MSB;
    uint8_t Temp_LSB;
} DS3231_RegisterType;

/*时间结构体*/
typedef struct
{
    uint8_t hour;
    uint8_t AM_PM;
    DS3231_HOUR_FORM hour_form;
    uint8_t min;
    uint8_t sec;
    uint8_t year;
    uint8_t mon;
    uint8_t date;
    uint8_t day;
} DS3231_TimeType;

/*DS3231结构体缓存*/
extern volatile uint8_t DS3231_buffer[19];

/*DS3231全局变量*/
extern DS3231_RegisterType DS3231_Register;

/*时间全局变量*/
extern DS3231_TimeType DS3231_Time;

void DS3231_Read_All();
void DS3231_Read_Time();
float DS3231_Read_Temp();

void DS3231_Set_Time(DS3231_TimeType *time);
void DS3231_Update();

#endif /* BSP_DS3231_DS3231_H_ */
