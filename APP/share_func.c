/*
 * share_func.c
 *
 *  Created on: 2025年12月12日
 *      Author: WangQingChuan
 */

#include <stdint.h>
#include "share_func.h"
#include "dht11.h"

uint8_t temperature = 1; // 温度
uint8_t humidity = 1;    // 湿度
uint8_t temp_humid_updated = 0;

uint32_t count_num = 0;
static uint32_t counter = 0;

/**
 * @brief  微秒级延时
 * @param  xus 延时时长，范围：0~233015
 * @retval 无
 */
void Delay_us(uint32_t us)
{
    uint32_t delay = (HAL_RCC_GetHCLKFreq() / 4000000 * us);
    while (delay--)
    {
        ;
    }
}

/**
 * @brief  毫秒级延时
 * @param  xms 延时时长，范围：0~4294967295
 * @retval 无
 */
void Delay_ms(uint32_t xms)
{
    while (xms--)
    {
        Delay_us(1000);
    }
}

/**
 * @brief  秒级延时
 * @param  xs 延时时长，范围：0~4294967295
 * @retval 无
 */
void Delay_s(uint32_t xs)
{
    while (xs--)
    {
        Delay_ms(1000);
    }
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        counter++;

        // 每2秒读取一次传感器（避免频繁读取导致卡顿）
        if (counter >= 200) // 200 * 10ms = 2秒
        {
            counter = 0;

            // 读取DHT11传感器
            if (DHT11_Read_Data(&temperature, &humidity))
            {
                temp_humid_updated = 1;
            }
        }
    }
    if (htim->Instance == TIM4)
    {
        count_num++;
        if (count_num >= 599999)
        {
            count_num = 599999;
        }
    }
}
