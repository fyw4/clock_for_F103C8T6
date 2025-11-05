/*
 * led.c
 *
 *  Created on: Sep 28, 2025
 *      Author: WangQingChuan
 */

#include"led.h"

void LED_test()
{
	LED0(0);
	HAL_Delay(500);
	LED0(1);
	HAL_Delay(500);
}
