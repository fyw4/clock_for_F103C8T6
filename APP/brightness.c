/*
 * brightness.c
 *
 *  Created on: 2026年1月31日
 *      Author: WangQingChuan
 */
#include <stdio.h>
#include <stdint.h>
#include "brightness.h"
#include "key.h"
#include "oled.h"

int brightness_setting()
{
    uint8_t key_val = 0;
    static uint16_t brightness_val = 0x80; // 初始亮度值
    int16_t brightness_tmp_val = brightness_val;
    int16_t add_val = 64;

    while (1)
    {
        OLED_NewFrame();
        key_val = Key_GetNum();
        if (key_val == 1)
        {
            // 调高
            brightness_tmp_val += add_val;
            if (brightness_tmp_val > 255)
            {
                brightness_tmp_val = 255;
            }
            OLED_SendCmd(0x81);
            OLED_SendCmd(brightness_tmp_val);
        }
        else if (key_val == 2)
        {
            // 调低
            brightness_tmp_val -= add_val;
            if (brightness_tmp_val < 1)
            {
                brightness_tmp_val = 1;
            }
            OLED_SendCmd(0x81);
            OLED_SendCmd(brightness_tmp_val);
        }
        else if (key_val == 3)
        {
            // 确定
            brightness_val = brightness_tmp_val;
            OLED_SendCmd(0x81);
            OLED_SendCmd(brightness_val);
        }
        else if (key_val == 4)
        {
            // 返回主界面
            OLED_SendCmd(0x81);
            OLED_SendCmd(brightness_val);
            return 0;
        }

        char tmp[20] = {0};
        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "%d", 123);
        OLED_PrintASCIIString(0, 33, tmp, &afont24x19, OLED_COLOR_NORMAL);

        memset(tmp, 0, sizeof(tmp));
        sprintf(tmp, "Lv:%d", (uint8_t)(brightness_tmp_val / 64 + 0.5) + 1);
        OLED_PrintASCIIString(70, 35, tmp, &afont24x12, OLED_COLOR_NORMAL);

        // 画个矩形做瞄定
        OLED_DrawFilledRectangle(0, 0, 128, 32, OLED_COLOR_NORMAL);
        OLED_ShowFrame();
    }
    return 0;
}