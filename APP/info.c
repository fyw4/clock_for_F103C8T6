/*
 * info.c
 *
 *  Created on: Oct 21, 2025
 *      Author: WangQingChuan
 */

#include "info.h"
#include "oled.h"
#include "key.h"

void info_message()
{
	OLED_NewFrame();
	OLED_PrintString(0, 8, "作者:王晴川", &font16x16, OLED_COLOR_NORMAL);
	OLED_PrintString(0, 43, "版本:V1.0", &font16x16, OLED_COLOR_NORMAL);
	OLED_ShowFrame();

	while(HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin) != GPIO_PIN_RESET);
	return;
}
