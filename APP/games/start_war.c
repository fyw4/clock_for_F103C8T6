/*
 * start_war.c
 *
 *  Created on: Nov 2, 2025
 *      Author: wanqi
 */

#include <stdio.h>
#include "star_war.h"
#include "key.h"

int metx = 0;                     // 子弹初始位置（横坐标）
int mety = 0;                     // 子弹初始位置（纵坐标）
int bullet_exit = 0;              // 是否存在子弹
int flying_saucer_position_y = 8; // 飞碟初始位置（纵坐标）
int direction = 0;                // 飞碟移动方向
int gameover = 0;
int rx = 95;   // 飞碟子弹1初始位置（横坐标）
int ry = 0;    // 飞碟子弹1初始位置（纵坐标）
int rx2 = 95;  // 飞碟子弹2初始位置（横坐标）
int ry2 = 0;   // 飞碟子弹2初始位置（纵坐标）
int rx3 = 95;  // 飞碟子弹3初始位置（横坐标）
int ry3 = 0;   // 飞碟子弹4初始位置（纵坐标）
int score = 0; // 得分

int flying_saucer_bullet_speed = 3; // 飞碟子弹移动速度
int flying_saucer_move_step = 1;    // 飞碟移动步长
int flying_saucer_radius = 10;      // 半径

int rx4 = 95;                                      // 飞碟子弹4初始位置（横坐标）
int ry4 = 0;                                       // 飞碟子弹4初始位置（纵坐标）
int life_count = 5;                                // 生命值
int poc = 0;                                       // 是否开始游戏
int flying_saucer_bullet_size_level = 0;           // 是否存在飞碟子弹
int level = 1;                                     // 游戏等级
int centar = 95;                                   // 飞碟初始位置（横坐标）
unsigned char key_num = 0;                         // 按键状态
unsigned long game_begin_time = 0;                 // 游戏开始时间
unsigned long flying_saucer_bullet_occur_time = 0; // 随机生成下一个飞碟子弹出现时间
unsigned long game_total_time = 0;                 // 游戏时间
unsigned long level_time = 0;                      // 游戏时间
int spaceship_position_y = 30;                     // 飞机初始位置（纵坐标）

void game_init()
{
    metx = 0;
    mety = 0;
    bullet_exit = 0;
    flying_saucer_position_y = 8;
    direction = 0;
    gameover = 0;
    rx = 95;
    ry = 0;
    rx2 = 95;
    ry2 = 0;
    rx3 = 95;
    ry3 = 0;
    score = 0;

    flying_saucer_bullet_speed = 3;
    flying_saucer_move_step = 1;
    flying_saucer_radius = 12;

    rx4 = 95;
    ry4 = 0;
    life_count = 5;
    poc = 0;
    flying_saucer_bullet_size_level = 0;
    level = 1;
    game_begin_time = 0;
    flying_saucer_bullet_occur_time = 0;
    game_total_time = 0;
    level_time = 0;
}

int get_key_status()
{
    if (KEY_Up == 0)
        return SHOOT_KEY;
    if (KEY_Down == 0)
        return UP_KEY;
    if (KEY_Confirm == 0)
        return DOWN_KEY;
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
    int current_spaceship_position_y = 0; // 飞船当前位置（横坐标）
    unsigned long start_tt = 0;           // 游戏开始时间
    unsigned long dt = 0;                 // 时间间隔
    unsigned long start_time;             // 开始时间
    char tmp[15] = {0};
    while (1)
    {
        dt = HAL_GetTick() - start_tt;

        if (gameover == 0)
        {
            OLED_NewFrame();

            star_draw(); // 画背景星星

            if (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) == GPIO_PIN_RESET)
            {
                return;
            }

            // test
            score = 100;
            sprintf(tmp, "%d", score);
            OLED_PrintString(7, 30, "得分:", &font12x12, OLED_COLOR_NORMAL);
            OLED_PrintString(40, 30, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(7, 40, "等级:", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", level);
            OLED_PrintString(40, 40, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(7, 50, "时间(s):", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", score);
            OLED_PrintString(40, 50, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_ShowFrame();
        }

        if (gameover == 1)
        {
            OLED_NewFrame();

            OLED_PrintString(7, 30, "得分:", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", score);
            OLED_PrintString(72, 30, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(7, 40, "等级:", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", score);
            OLED_PrintString(72, 40, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_PrintString(7, 50, "时间(s):", &font12x12, OLED_COLOR_NORMAL);
            sprintf(tmp, "%d", score);
            OLED_PrintString(72, 50, tmp, &font12x12, OLED_COLOR_NORMAL);

            OLED_ShowFrame();

            if (life_count == 0)
            {
                life_count = 5;
                HAL_Delay(1000);
            }

            if (get_key_status() == SHOOT_KEY)
            {
                start_tt = HAL_GetTick();
                game_init(); // 初始化
            }
        }
    }
    start_tt = HAL_GetTick();
}
