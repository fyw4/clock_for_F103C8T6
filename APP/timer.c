/*
 * timer.c
 *
 *  Created on: Jan 26, 2026
 *      Author: WangQingChuan
 */

#include "main.h"
#include "led.h"
#include "share_func.h"
#include "tim.h"
#include "font.h"
#include "oled.h"

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM4)
    {
        LED0_TOGGLE();
    }
}

int timer_UI()
{
    uint8_t key_val = 0;

    while (1)
    {
        OLED_NewFrame();

        OLED_PrintString(0, 8, "counting test", &font16x16, OLED_COLOR_NORMAL);

        key_val = check_key_press();
        if (key_val == 1)
        {
            // 返回主界面
            return 0;
        }
        else if (key_val == 2)
        {
            // 开始计时
            HAL_TIM_Base_Start_IT(&htim4);

            HAL_Delay(180);
        }
        else if (key_val == 3)
        {
            // 暂停计时
            HAL_TIM_Base_Stop_IT(&htim4);  // 停止定时器中断
            HAL_Delay(180);
        }
        else if (key_val == 4)
        {
            // 重置计时
            HAL_Delay(180);
        }

        OLED_ShowFrame();
    }
    return 0;
}
