/*
 * flappy_bird.c
 *
 *  Created on: 2025年11月10日
 *      Author: WangQingChuan
 */

#include "flappy_bird.h"

int x1;     // 管道1横坐标
int y1;     // 管道1纵坐标
int x2;     // 管道2横坐标
int y2;     // 管道2纵坐标
int bs;     // 小鸟跳跃高度
int bird_y; // 小鸟纵坐标
long score; // 得分

int get_button_status()
{
    if (HAL_GPIO_ReadPin(Confirm_GPIO_Port, Confirm_Pin) == 0)
    {
        return 1;
    }
    return 0;
}

void draw()
{
    char tmp[15] = {0};

    OLED_NewFrame();

    sprintf(tmp, "%d", score);
    OLED_PrintASCIIString(80, 0, tmp, &font8x6, OLED_COLOR_NORMAL);
    OLED_DrawRectangle(x1, 0, 10, y1, OLED_COLOR_NORMAL);
    OLED_DrawRectangle(x1 - 2, y1 - 6, 14, 6, OLED_COLOR_NORMAL);

    OLED_DrawRectangle(x1, y1 + 20, 10, 64 - y1, OLED_COLOR_NORMAL);
    OLED_DrawRectangle(x1 - 2, y1 + 20, 14, 6, OLED_COLOR_NORMAL);

    OLED_DrawRectangle(x2, 0, 10, y2, OLED_COLOR_NORMAL);
    OLED_DrawRectangle(x2 - 2, y2 - 6, 14, 6, OLED_COLOR_NORMAL);

    OLED_DrawRectangle(x2, y2 + 20, 10, 64 - y2, OLED_COLOR_NORMAL);
    OLED_DrawRectangle(x2 - 2, y2 + 20, 14, 6, OLED_COLOR_NORMAL);

    OLED_DrawImage(20, bird_y, &bird_Img, OLED_COLOR_NORMAL);

    OLED_ShowFrame();
}

void flappy_bird_play()
{
    char tmp[15] = {0};
    unsigned char is_fail = 0;
    x1 = 128;
    x2 = 192;
    y1 = rand() % 42 + 1;
    y2 = rand() % 42 + 1;

    bs = 32;
    bird_y = 10;
    score = 0;

    while (1)
    {
        if (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) == GPIO_PIN_RESET)
        {
            return;
        }

        if (is_fail == 0)
        {
            if (get_button_status() == 1)
            {
                bs = 32;
            }

            x1 = x1 - 2;               // 管道1横坐标向左移动
            x2 = x2 - 2;               // 管道2横坐标向左移动
            bs = bs - 5;               // 小鸟跳跃高度减少
            bird_y = bird_y - bs / 10; // 小鸟纵坐标减少

            draw();

            if (bird_y < 0) // 小鸟纵坐标不能小于0
            {
                bird_y = 0;
            }
            if (bird_y > 64) // 小鸟纵坐标不能大于64
            {
                is_fail = 1;
            }

            if (x1 == -10) // 管道1横坐标移动到屏幕外
            {
                x1 = 128;
                y1 = rand() % 42 + 1;
            }
            if (x2 == -10) // 管道2横坐标移动到屏幕外
            {
                x2 = 128;
                y2 = rand() % 42 + 1;
            }
            if (x1 >= 20 && x1 <= 20 + 11) // 小鸟与管道1碰撞
            {
                if (bird_y < y1 || bird_y + 8 > y1 + 20) // 小鸟与管道1碰撞
                {
                    is_fail = 1;
                }
            }
            if (x2 >= 20 && x2 <= 20 + 11) // 小鸟与管道2碰撞
            {
                if (bird_y < y2 || bird_y + 8 > y2 + 20) // 小鸟与管道2碰撞
                {
                    is_fail = 1;
                }
            }

            if (x1 == 16 || x2 == 16) // 小鸟通过管道
            {
                score++;
            }

            HAL_Delay(30);
        }

        if (is_fail == 1)
        {
            OLED_NewFrame();
            OLED_DrawImage(0, 0, &game_over_Img, OLED_COLOR_NORMAL);
            OLED_DrawImage(8, 32, &image_Img, OLED_COLOR_NORMAL);
            OLED_PrintString(72, 32, "Score", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", score);
            OLED_PrintASCIIString(72, 48, tmp, &font8x6, OLED_COLOR_NORMAL);

            OLED_ShowFrame();
            if (get_button_status() == 1)
            {
                is_fail = 0;

                x1 = 128;
                x2 = 192;

                y1 = rand() % 42 + 1;
                y2 = rand() % 42 + 1;

                bs = 32;
                bird_y = 50;
                score = 0;
            }

            if (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) == GPIO_PIN_RESET)
            {
                return;
            }
        }
    }
    return;
}
