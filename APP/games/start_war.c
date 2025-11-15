/*
 * start_war.c
 *
 *  Created on: Nov 2, 2025
 *      Author: wanqi
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "star_war.h"
#include "key.h"

int metx = 0;                             // 子弹初始位置（横坐标）
int mety = 0;                             // 子弹初始位置（纵坐标）
int bullet_exit = 0;                      // 是否存在子弹
int flying_saucer_center_position_x = 95; // 飞碟初始位置（横坐标）
int flying_saucer_center_position_y = 8;  // 飞碟初始位置（纵坐标）
int direction = 0;                        // 飞碟移动方向
int gameover = 0;
int rx = 95;   // 飞碟子弹1初始位置（横坐标）
int ry = 0;    // 飞碟子弹1初始位置（纵坐标）
int rx2 = 95;  // 飞碟子弹2初始位置（横坐标）
int ry2 = 0;   // 飞碟子弹2初始位置（纵坐标）
int rx3 = 95;  // 飞碟子弹3初始位置（横坐标）
int ry3 = 0;   // 飞碟子弹3初始位置（纵坐标）
int rx4 = 95;  // 飞碟子弹4初始位置（横坐标）
int ry4 = 0;   // 飞碟子弹4初始位置（纵坐标）
int star_war_score = 0; // 得分

int flying_saucer_bullet_speed = 3; // 飞碟子弹移动速度
int flying_saucer_move_step = 1;    // 飞碟移动步长
int flying_saucer_radius = 10;      // 半径

int life_count = 5;                      // 生命值
int flying_saucer_bullet_size_level = 0; // 是否存在飞碟子弹
int level = 1;                           // 游戏等级

unsigned char key_num = 0;                         // 按键状态
unsigned long game_begin_time = 0;                 // 游戏开始时间
unsigned long game_running_time = 0;               // 游戏时间
unsigned long level_time = 0;                      // 等级时间
int spaceship_position_y = 24;                     // 飞船初始位置（纵坐标）

int invincible = 0;         // 无敌状态标志
unsigned long hit_time = 0; // 被击中的时间

// 子弹活跃状态
int bullet1_active = 0;
int bullet2_active = 0;
int bullet3_active = 0;
int bullet4_active = 0;

void game_init()
{
    metx = 0;
    mety = 0;
    bullet_exit = 0;
    flying_saucer_center_position_y = 8;
    direction = 0;
    gameover = 0;
    rx = 95;
    ry = 0;
    rx2 = 95;
    ry2 = 0;
    rx3 = 95;
    ry3 = 0;
    rx4 = 95;
    ry4 = 0;
    star_war_score = 0;

    flying_saucer_bullet_speed = 3;
    flying_saucer_move_step = 1;
    flying_saucer_radius = 10;

    life_count = 5;
    flying_saucer_bullet_size_level = 0;
    level = 1;
    game_begin_time = 0;
    game_running_time = 0;
    level_time = 0;
    spaceship_position_y = 24;

    bullet1_active = 0;
    bullet2_active = 0;
    bullet3_active = 0;
    bullet4_active = 0;

}

int get_key_status()
{
    if (KEY_Up == GPIO_PIN_RESET)
        return UP_KEY;
    if (KEY_Down == GPIO_PIN_RESET)
        return DOWN_KEY;
    if (KEY_Confirm == GPIO_PIN_RESET)
        return SHOOT_KEY;
    return 0;
}

void star_draw()
{
    OLED_DrawPoint(50, 30);
    OLED_DrawPoint(30, 17);
    OLED_DrawPoint(60, 18);
    OLED_DrawPoint(55, 16);
    OLED_DrawPoint(25, 43);
    OLED_DrawPoint(100, 43);
    OLED_DrawPoint(117, 52);
    OLED_DrawPoint(14, 49);
    OLED_DrawPoint(24, 24);
    OLED_DrawPoint(78, 36);
    OLED_DrawPoint(80, 57);
    OLED_DrawPoint(107, 11);
    OLED_DrawPoint(150, 11);
    OLED_DrawPoint(5, 5);
    OLED_DrawPoint(8, 7);
    OLED_DrawPoint(70, 12);
    OLED_DrawPoint(10, 56);
    OLED_DrawPoint(70, 25);
}

void star_war_play()
{
    int current_spaceship_position_y = 0; // 飞船当前位置（纵坐标）

    char tmp[15] = {0};

    game_init();

    game_begin_time = HAL_GetTick(); // 获取游戏开始时间

    while (1)
    {
        if (KEY_Back == GPIO_PIN_RESET)
        {
            return;
        }

        if (gameover == 0)
        {
            OLED_NewFrame();

            star_draw(); // 画背景星星

            game_running_time = HAL_GetTick() - game_begin_time; // 当前时间

            if ((game_running_time - level_time) > 50000) // 游戏时间超过50秒
            {
                level_time = game_running_time; // 更新下一等级开始时间
                level += 1;                     // 等级+1

                flying_saucer_bullet_speed += 1; // 飞碟子弹移动速度
                flying_saucer_move_step += 1;    // 飞碟每次移动步长+1（速度加快）
                flying_saucer_radius -= 1;       // 飞碟半径-1
            }

            flying_saucer_bullet_size_level += 1; // 飞碟子弹类型

            if ((rand() % 800 + 400) % 2 == 0)
            {
                if (flying_saucer_bullet_size_level == 1 && !bullet1_active)
                {
                    rx = 95;
                    ry = flying_saucer_center_position_y;
                    bullet1_active = 1; // 激活子弹1
                }
            }

            if ((rand() % 800 + 400) % 3 == 1)
            {
                if (flying_saucer_bullet_size_level == 2 && !bullet2_active)
                {
                    rx2 = 95;
                    ry2 = flying_saucer_center_position_y;
                    bullet2_active = 1; // 激活子弹2
                }
            }

            if ((rand() % 800 + 400) % 5 == 1)
            {
                if (flying_saucer_bullet_size_level == 3 && !bullet3_active)
                {
                    rx3 = 95;
                    ry3 = flying_saucer_center_position_y;
                    bullet3_active = 1; // 激活子弹3
                }
            }

            if ((rand() % 800 + 400) % 7 == 1)
            {
                if (flying_saucer_bullet_size_level == 4 && !bullet4_active)
                {
                    rx4 = 95;
                    ry4 = flying_saucer_center_position_y;
                    bullet4_active = 1; // 激活子弹4
                }
            }

            if (bullet1_active)
            {
                OLED_DrawCircle(rx, ry, 2, 0);
                if (rx >= -2)
                    rx -= flying_saucer_bullet_speed;
                else
                    bullet1_active = 0; // 子弹1出屏幕，标记为不活跃
            }

            if (bullet2_active)
            {
                OLED_DrawCircle(rx2, ry2, 1, 0);
                if (rx2 >= -1)
                    rx2 -= flying_saucer_bullet_speed;
                else
                    bullet2_active = 0; // 子弹2出屏幕，标记为不活跃
            }

            if (bullet3_active)
            {
                OLED_DrawCircle(rx3, ry3, 4, 0);
                if (rx3 >= -4)
                    rx3 -= flying_saucer_bullet_speed;
                else
                    bullet3_active = 0; // 子弹3出屏幕，标记为不活跃
            }

            if (bullet4_active)
            {
                OLED_DrawCircle(rx4, ry4, 3, 0);
                if (rx4 >= -3)
                    rx4 -= flying_saucer_bullet_speed;
                else
                    bullet4_active = 0; // 子弹4出屏幕，标记为不活跃
            }

            if (get_key_status() == UP_KEY && spaceship_position_y >= 2) // 向上
            {
                spaceship_position_y -= 2;
            }

            if (get_key_status() == DOWN_KEY && spaceship_position_y <= 46) // 向下
            {
                spaceship_position_y += 2;
            }

            if (get_key_status() == SHOOT_KEY && bullet_exit == 0) // 飞船射击
            {
                bullet_exit = 1;
                metx = 6;                        // 飞船中心的枪口横坐标
                mety = spaceship_position_y + 8; // 飞船中心的枪口纵坐标
            }

            if (bullet_exit == 1)
            {
                metx += 8;

                OLED_DrawLine(metx, mety, metx + 4, mety, OLED_COLOR_NORMAL); // 飞船子弹飞行
            }

            if (invincible)
            {
                if ((HAL_GetTick() / 50) % 2 == 0) // 被击中后每50毫秒闪烁一次飞船显示
                {
                    OLED_DrawImage(4, spaceship_position_y, &spaceship_Img, OLED_COLOR_NORMAL);
                }
                if ((game_running_time - hit_time) > 1000) // 无敌时间1秒
                {
                    invincible = 0;
                }
            }
            else
            {
                OLED_DrawImage(4, spaceship_position_y, &spaceship_Img, OLED_COLOR_NORMAL);
            }
            OLED_DrawCircle(flying_saucer_center_position_x, flying_saucer_center_position_y, flying_saucer_radius, OLED_COLOR_NORMAL);             // 飞碟小圈
            OLED_DrawCircle(flying_saucer_center_position_x + 2, flying_saucer_center_position_y + 3, flying_saucer_radius / 3, OLED_COLOR_NORMAL); // 飞碟大圈

            OLED_PrintASCIIString(28, 0, "star_war_score:", &afont8x6, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", star_war_score);
            OLED_PrintASCIIString(64, 0, tmp, &afont8x6, OLED_COLOR_NORMAL);

            OLED_PrintASCIIString(28, 55, "life:", &afont8x6, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", life_count);
            OLED_PrintASCIIString(64, 55, tmp, &afont8x6, OLED_COLOR_NORMAL);

            OLED_PrintASCIIString(116, 45, "tm", &afont8x6, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", game_running_time / 1000);
            OLED_PrintASCIIString(116, 55, tmp, &afont8x6, OLED_COLOR_NORMAL);

            OLED_ShowFrame();

            if (metx > 128) // 子弹超出屏幕，可以重新发射子弹
            {
                bullet_exit = 0;
            }

            if (direction == 0) // 若飞碟向下移动
            {
                flying_saucer_center_position_y += flying_saucer_move_step; // 每次移动步长
            }
            else
            {
                flying_saucer_center_position_y -= flying_saucer_move_step; // 每次移动步长
            }

            if (flying_saucer_center_position_y >= (64 - flying_saucer_radius)) // 若飞碟移动到底
            {
                direction = 1; // 开始向上移动
            }

            if (flying_saucer_center_position_y <= flying_saucer_radius) // 若飞碟移动到顶
            {
                direction = 0; // 开始向下移动
            }

            if (mety >= flying_saucer_center_position_y - flying_saucer_radius && mety <= flying_saucer_center_position_y + flying_saucer_radius) // 若飞船打到飞碟了
            {
                if (metx > (flying_saucer_center_position_x - flying_saucer_radius) && metx < (flying_saucer_center_position_x + flying_saucer_radius))
                {
                    metx = 6;                        // 飞船中心的枪口横坐标
                    mety = spaceship_position_y + 8; // 飞船中心的枪口纵坐标
                    star_war_score += 1;
                    bullet_exit = 0;
                }
            }

            current_spaceship_position_y = spaceship_position_y + 8; // 当前飞船位置（纵坐标）
            if (ry >= current_spaceship_position_y - 8 && ry <= current_spaceship_position_y + 8)
            {
                if (rx <= 20 && rx > 4 && invincible == 0)
                {
                    bullet1_active = 0; // 子弹1打到飞船，标记为不活跃
                    life_count -= 1;
                    invincible = 1;
                    hit_time = game_running_time;
                }
            }

            if (ry2 >= current_spaceship_position_y - 8 && ry2 <= current_spaceship_position_y + 8)
            {
                if (rx2 <= 20 && rx2 > 4 && invincible == 0)
                {
                    bullet2_active = 0; // 子弹2打到飞船，标记为不活跃
                    life_count -= 1;
                    invincible = 1;
                    hit_time = game_running_time;
                }
            }

            if (ry3 >= current_spaceship_position_y - 8 && ry3 <= current_spaceship_position_y + 8)
            {
                if (rx3 <= 20 && rx3 > 4 && invincible == 0)
                {
                    bullet3_active = 0; // 子弹3打到飞船，标记为不活跃
                    life_count -= 1;
                    invincible = 1;
                    hit_time = game_running_time;
                }
            }

            if (ry4 >= current_spaceship_position_y - 8 && ry4 <= current_spaceship_position_y + 8)
            {
                if (rx4 <= 20 && rx4 > 4 && invincible == 0)
                {
                    bullet4_active = 0; // 子弹4打到飞船，标记为不活跃
                    life_count -= 1;
                    invincible = 1;
                    hit_time = game_running_time;
                }
            }

            if (flying_saucer_bullet_size_level > 4)
            {
                flying_saucer_bullet_size_level = 0;
            }

            if (life_count == 0) // 生命值为0
            {
                gameover = 1;
            }
        }

        if (gameover == 1)
        {
            OLED_NewFrame();

            OLED_PrintString(37, 20, "得分:", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", star_war_score);
            OLED_PrintString(85, 20, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(37, 35, "等级:", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", level);
            OLED_PrintString(85, 35, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(37, 50, "时间(s):", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", game_running_time / 1000);
            OLED_PrintString(85, 50, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_ShowFrame();

            if (get_key_status() == SHOOT_KEY)
            {
                game_init(); // 初始化
                game_begin_time = HAL_GetTick();
            }
        }
        HAL_Delay(1);
    }
}
