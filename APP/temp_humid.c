/*
 * temp_humid.c
 *
 *  Created on: 2026年2月11日
 *      Author: WangQingChuan
 */

#include <stdio.h>
#include "temp_humid.h"
#include "dht11.h"
#include "oled.h"
#include "share_func.h"
#include "interrupt.h"
#include "key.h"
#include "tim.h"

uint8_t temperature_h = 1; // 温度
uint8_t temperature_l = 1; // 温度
uint8_t humidity_h = 1;	   // 湿度
uint8_t humidity_l = 1;	   // 湿度

uint8_t temp_humid_ready = 0;
uint8_t temp_humid_updated = 0;

uint8_t key_val = 0;

int temp_humid_UI()
{
	char tmp[20] = {0};
	uint32_t last_read_time = 0;

	while (1)
	{
		OLED_NewFrame();

		if (check_key_press() == 1)
		{
			return 0;
		}

		if (HAL_GetTick() - last_read_time >= 2000)
		{
			DHT11_Read_Data(&humidity_h, &humidity_l, &temperature_h, &temperature_l);
			last_read_time = HAL_GetTick();
		}

		memset(tmp, 0, sizeof(tmp));
		sprintf(tmp, "%02dC %02d%%", temperature_h, humidity_h);
		OLED_PrintASCIIString(80, 50, tmp, &afont12x6, OLED_COLOR_NORMAL);

		OLED_ShowFrame();

		HAL_Delay(200);
	}

	return 0;
}
