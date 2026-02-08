/*
 * interrupt.c
 *
 *  Created on: Feb 5, 2026
 *      Author: WangQingChuan
 */

#include "interrupt.h"
#include "dht11.h"

uint8_t temperature_h = 1; // 温度
uint8_t temperature_l = 1; // 温度
uint8_t humidity_h = 1;    // 湿度
uint8_t humidity_l = 1;    // 湿度

uint8_t temp_humid_updated = 0;

uint32_t adc_value = 0;

uint32_t count_num = 0;
static uint32_t counter = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM3)
    {
        counter++;

        // 每2秒读取一次传感器（避免频繁读取导致卡顿）
        if (counter >= 200) // 200 * 10ms = 2000毫秒
        {
            counter = 0;
            DHT11_HIGH;
            DHT11_Rst();
            DHT11_Check();

            // 读取DHT11传感器
            if (!DHT11_Read_Data(&temperature_h, &temperature_l, &humidity_h, &humidity_l))
            {
                temp_humid_updated = 1;
            }
            else
            {
                temp_humid_updated = 0;
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

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef *hadc)
{
    if (hadc->Instance == ADC1)
    {
        adc_value = HAL_ADC_GetValue(hadc);
    }
}
