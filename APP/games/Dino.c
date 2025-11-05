/*
 * Dino.c
 *
 *  Created on: Oct 4, 2025
 *      Author: WangQingChuan
 */

#include "Dino.h"
#include "font.h"
#include "oled.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define DINO_INIT_X 10      // Dino initial spawn location
#define DINO_INIT_Y 62 - 23 // Dino initial spawn location

#define BASE_LINE_X 0
#define BASE_LINE_Y 63
#define BASE_LINE_X1 127

// Number of pixel dino will jump
#define JUMP_PIXEL 32

const Image tree_bigImg = {12, 23, tree_bigData};

/* Jump Time-Distance Table */
const uint8_t time_distance[38] = {0, 4, 7, 11, 14, 17, 20, 23, 25, 28, 30, 31, 33, 34, 35, 36, 37, 37, 38, 38, 37, 37, 36, 35, 34, 33, 31, 30, 28, 25, 23, 20, 17, 14, 11, 7, 4, 0};

/* Higheset Score */
int highest_score = 0;

/**
 * @brief Draw the introduction screen
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void introMessage()
{
  OLED_NewFrame();
  OLED_PrintString((128 - 80) / 2, 8, "小恐龙游戏", &font16x16, OLED_COLOR_NORMAL);
  OLED_DrawImage(DINO_INIT_X, DINO_INIT_Y, &dino_jumpsImg, OLED_COLOR_NORMAL);
  OLED_PrintString((128 - 32) / 2, 43, "开始", &font16x16, OLED_COLOR_NORMAL);
  OLED_ShowFrame();
}

void show_line()
{
  int line_end = 1;
  OLED_NewFrame();
  OLED_DrawImage(DINO_INIT_X, DINO_INIT_Y, &dino_jumpsImg, OLED_COLOR_NORMAL); // 绘制恐龙
  for (;;)
  {
    if (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) == GPIO_PIN_RESET)
    {
      return;
    }

    OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, line_end, BASE_LINE_Y, OLED_COLOR_NORMAL); // 绘制横线
    OLED_ShowFrame();
    line_end += 4;
    if (line_end > BASE_LINE_X1)
      break;
  }
}

/**
 * @brief Move dino
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void move_dino(int16_t y, int type)
{
  if (type == 0)
    OLED_DrawImage(DINO_INIT_X, y, &dino_front_legImg, OLED_COLOR_NORMAL);
  else if (type == 1)
    OLED_DrawImage(DINO_INIT_X, y, &dino_back_legImg, OLED_COLOR_NORMAL);
  else if (type == -1)
    OLED_DrawImage(DINO_INIT_X, y, &dino_jumpsImg, OLED_COLOR_NORMAL);
  else if (type == -2)
    OLED_DrawImage(DINO_INIT_X, y, &dino_crashedImg, OLED_COLOR_NORMAL);
}

/**
 * @brief Move tree
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void move_tree(int16_t x, Image *type)
{
  if (x > 127 - type->w)
    return;
  OLED_DrawImage(x, BASE_LINE_Y - type->h, type, OLED_COLOR_NORMAL);
}

/**
 * @brief Display score while running the game
 * @note Referenced from github@harshmittal2210/dino_game.ino
 */
void display_score(int score, int8_t type)
{
  static char tmp[15];
  if (type == 0)
  {
    sprintf(tmp, "%d", score);
    OLED_PrintString(72, 3, "得分：", &font12x12, OLED_COLOR_NORMAL);
    OLED_PrintString(72 + 30, 3, tmp, &font12x12, OLED_COLOR_NORMAL);
  }

  else
  {
    sprintf(tmp, "%d", highest_score);
    OLED_PrintString(72 - 12, 22, "最高分：", &font12x12, OLED_COLOR_NORMAL);
    OLED_PrintString(72 + 30, 22, tmp, &font12x12, OLED_COLOR_NORMAL);
  }
}

void tree_collision_judge(int16_t tree_size_main, int16_t tree_size_sub, int current_score, int jump, int16_t dino_y, int16_t dino_right, Image *tree, Image *tree1)
{
  // Collision Happened
  move_dino(dino_y, -2); // 撞树图片显示
  move_tree(tree_size_main, tree);
  move_tree(tree_size_sub, tree1);
  OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y, OLED_COLOR_NORMAL);
  display_score(current_score, 0);
  if (current_score > highest_score)
  {
    highest_score = current_score;
    OLED_PrintString(75, 22, "新纪录！", &font12x12, OLED_COLOR_NORMAL);
  }
  else
  {
    display_score(current_score, 1);
  }
  OLED_ShowFrame();
}

// Main dino_play function
void dino_play()
{
  int16_t tree_interval = 252;
  int16_t tree1_interval = 381;
  int16_t tree_x = tree_interval + 200;
  int16_t tree1_x = tree1_interval + 200;
  Image *tree = (HAL_GetTick() % 8) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
  Image *tree1 = (HAL_GetTick() % 5) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
  int16_t dino_right = DINO_INIT_X + dino_front_legImg.w; // 恐龙右侧坐标

  int16_t dino_y = DINO_INIT_Y;
  int jump = 0;

  int score_raw = 0;
  int score = 0;

  int8_t leg = 0;

  for (;;)
  {
    OLED_NewFrame();

    if (HAL_GPIO_ReadPin(Up_GPIO_Port, Up_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Down_GPIO_Port, Down_Pin) == GPIO_PIN_RESET || HAL_GPIO_ReadPin(Confirm_GPIO_Port, Confirm_Pin) == GPIO_PIN_RESET)
    {
      if (jump == 0)
        jump = 1;
    }
    else if (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) == GPIO_PIN_RESET)
    {
      return;
    }

    if (tree_x <= (dino_right - ((jump < 5) ? jump : 5)) && tree_x > (DINO_INIT_X + 1) && (dino_y + dino_back_legImg.h) >= (BASE_LINE_Y - tree->h))
    { // 撞树了
      tree_collision_judge(tree_x, tree1_x, score, jump, dino_y, dino_right, tree, tree1);
      while (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) != GPIO_PIN_RESET)
        ;
      break;
    }

    if (tree1_x <= (dino_right - ((jump < 5) ? jump : 5)) && tree1_x > (DINO_INIT_X + 1) && (dino_y + dino_back_legImg.h) >= (BASE_LINE_Y - tree1->h))
    {

      tree_collision_judge(tree1_x, tree_x, score, jump, dino_y, dino_right, tree, tree1);
      while (HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) != GPIO_PIN_RESET)
        ;
      break;
    }

    // 模拟恐龙跳跃抛物线动作
    if (jump > 0)
    {
      dino_y = DINO_INIT_Y - time_distance[jump - 1];
      if (++jump > 38)
        jump = 0;
    }

    // 统计分数
    score_raw++;
    score = score_raw / 5;
    display_score(score, 0);

    if (jump != 0)
      move_dino(dino_y, -1); // 跳起来
    else
    {
      move_dino(dino_y, leg > 4 ? 1 : 0); // 每隔4个数换腿跑
      if (++leg > 9)
        leg = 0;
    }

    // 绘制树木
    move_tree(tree_x, tree);
    move_tree(tree1_x, tree1);

    OLED_DrawLine(BASE_LINE_X, BASE_LINE_Y, BASE_LINE_X1, BASE_LINE_Y, OLED_COLOR_NORMAL);

    // 树木移动
    tree_x -= 2;
    tree1_x -= 2;
    if (tree_x <= 0)
    {
      tree_x = tree_interval + HAL_GetTick() / 10 % 20;
      tree = (HAL_GetTick() % 4) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
    }
    if (tree1_x <= 0)
    {
      tree1_x = tree1_interval + HAL_GetTick() / 10 % 20;
      tree1 = (HAL_GetTick() % 2) ? (Image *)&tree_smallImg : (Image *)&tree_bigImg;
    }

    // 间距调整
    static int16_t interval;
    interval = tree_x - tree1_x;
    if (interval > 0 && interval <= 46)
      tree_x += interval;
    else if (interval < 0 && interval >= -46)
      tree1_x += interval;
    OLED_ShowFrame();
  }
}
