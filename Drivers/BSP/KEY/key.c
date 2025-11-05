/*
 * key.c
 *
 *  Created on: Oct 20, 2025
 *      Author: WangQingChuan
 */


#include "key.h"

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;							//默认为0,没有按键按下返回0
	if(KEY_Up == 0)//向上
	{
		HAL_Delay(20);							//按键消抖
		while(KEY_Up == 0);//直到松手往下走
		HAL_Delay(20);							//按键消抖
		KeyNum = 1;								//返回键码1
	}

	if(KEY_Down == 0)//向下
	{
		HAL_Delay(20);							//按键消抖
		while(KEY_Down == 0);//直到松手往下走
		HAL_Delay(20);							//按键消抖
		KeyNum = 2;								//返回键码2
	}

	if(KEY_Confirm == 0)//确认
	{
		HAL_Delay(20);							//按键消抖
		while(KEY_Confirm == 0);//直到松手往下走
		HAL_Delay(20);							//按键消抖
		KeyNum = 3;								//返回键码3
	}

	if(KEY_Back == 0)
	{
		HAL_Delay(20);							//按键消抖
		while(KEY_Back == 0);//直到松手往下走
		HAL_Delay(20);							//按键消抖
		KeyNum = 4;								//返回键码4
	}

	return KeyNum;
}
