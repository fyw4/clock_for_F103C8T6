/*
 * ds3231.c
 *
 *  Created on: Dec 15, 2025
 *      Author: WangQingChuan
 */

#include "ds3231.h"

volatile uint8_t DS3231_buffer[19];

DS3231_RegisterType DS3231_Register;
DS3231_TimeType DS3231_Time;

uint8_t bcd2dec(uint8_t bcd)
{
    return ((bcd) >> 4) * 10 + ((bcd) & 0x0f);
}

uint8_t dec2bcd(uint8_t dec)
{
    return (((dec) / 10) << 4) + ((dec) % 10);
}

/*一次性读取所有的寄存器*/
void DS3231_Read_All()
{
    uint8_t temp[1] = {0};
    /*设置寄存器指针位置*/
    //DS3231的7位基础地址是0x68，I2C通信时需将地址左移1位并添加读写位（0表示写入，1表示读取），因此写入地址为0x68 << 1 = 0xD0
    HAL_I2C_Master_Transmit(&hi2c2, 0xD0, temp, 1, 0xffff);

    // DS3231的7位基础地址是0x68，I2C通信时需将地址左移1位并添加读写位（0表示写入，1表示读取），因此读取地址为(0x68 << 1) | 0x01 = 0xD1。
    HAL_I2C_Master_Receive(&hi2c2, 0xD1, (uint8_t *)&DS3231_Register, sizeof(DS3231_Register), 0xffff);
}

float DS3231_Read_Temp()
{
    uint8_t sign = (DS3231_Register.Temp_MSB >> 7);
    float ret;
    ret = (float)DS3231_Register.Temp_MSB + (float)(DS3231_Register.Temp_LSB >> 6) * 0.25f;

    if (sign)
    {
        ret = 0 - ret;
    }
    else
    {
    }

    return ret;
}

void DS3231_Read_Time()
{
    DS3231_Time.sec = bcd2dec(DS3231_Register.Seconds);
    DS3231_Time.min = bcd2dec(DS3231_Register.Minutes);

    if ((DS3231_Register.Hours & 0x40) == 0x40)
    {
        DS3231_Time.hour_form = HOUR_FORM_12;
        DS3231_Time.AM_PM = (DS3231_Register.Hours) & 0x20;
        DS3231_Time.hour = bcd2dec(DS3231_Register.Hours & 0x1F);
    }
    else
    {
        DS3231_Time.hour_form = HOUR_FORM_24;
        DS3231_Time.hour = bcd2dec(DS3231_Register.Hours);
    }

    DS3231_Time.year = bcd2dec(DS3231_Register.Year);
    DS3231_Time.mon = bcd2dec(DS3231_Register.Month_Century);
    DS3231_Time.date = bcd2dec(DS3231_Register.Date);
    DS3231_Time.day = DS3231_Register.Day;
}

void DS3231_Set_Time(DS3231_TimeType *time)
{
    DS3231_Register.Seconds = dec2bcd(time->sec);
    DS3231_Register.Minutes = dec2bcd(time->min);

    if (time->hour_form == HOUR_FORM_12)
    {

        DS3231_Register.Hours = (0x40 | (time->AM_PM << 5)) | (dec2bcd(time->hour));
    }
    else
    {
        DS3231_Register.Hours = dec2bcd(time->hour);
    }

    DS3231_Register.Year = dec2bcd(time->year);
    DS3231_Register.Month_Century = dec2bcd(time->mon);
    DS3231_Register.Date = dec2bcd(time->date);
    DS3231_Register.Day = time->day;
}

void DS3231_Update()
{
    // 0x00是秒寄存器，接下来依次是分(0x01)、时(0x02)、星期(0x03)、日(0x04)、月(0x05)、年(0x06)等寄存器。
    HAL_I2C_Mem_Write(&hi2c2, 0xD0, 0x00, 1, (uint8_t *)&DS3231_Register, sizeof(DS3231_Register), 0xffff);
}
