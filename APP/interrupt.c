/*
 * interrupt.c
 *
 *  Created on: Feb 5, 2026
 *      Author: WangQingChuan
 */

#include "interrupt.h"
#include "dht11.h"

uint32_t adc_value = 0;

uint32_t count_num = 0;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
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
