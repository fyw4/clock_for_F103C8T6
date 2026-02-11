/*
 * dht11.c
 *
 *  Created on: Feb 4, 2026
 *      Author: WangQingChuan
 */

#include <stdint.h>
#include "dht11.h"
#include "share_func.h"
#include "interrupt.h"

void DHT11_OUT(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = DHT11_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

void DHT11_IN(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.Pin = DHT11_Pin;
	GPIO_InitStructure.Mode = GPIO_MODE_INPUT;
	GPIO_InitStructure.Pull = GPIO_PULLUP;
	GPIO_InitStructure.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(DHT11_GPIO_Port, &GPIO_InitStructure);
}

/**
 * @brief  DHT11
 * @param
 * @retval
 */
void DHT11_Rst(void)
{
	DHT11_OUT();
	DHT11_LOW;
	HAL_Delay(20);
	DHT11_HIGH;
	Delay_us(13);
}

/**
 * @brief
 * @param
 * @retval
 */
uint8_t DHT11_Check(void)
{
	uint8_t retry = 0;
	DHT11_IN();
	while (DHT11_IO_IN && retry < 100)
	{
		retry++;
		Delay_us(1); // 1us
	}

	if (retry >= 100)
	{
		return 1;
	}
	else
	{
		retry = 0;
	}

	while (!DHT11_IO_IN && retry < 100)
	{
		retry++;
		Delay_us(1); // 1us
	}

	if (retry >= 100)
	{
		return 1;
	}

	return 0;
}

/**
 * @brief
 * @param
 * @retval
 */
uint8_t DHT11_Read_Bit(void)
{
	uint8_t retry = 0;
	while (DHT11_IO_IN && retry < 100)
	{
		retry++;
		Delay_us(1);
	}

	retry = 0;
	while (!DHT11_IO_IN && retry < 100)
	{
		retry++;
		Delay_us(1);
	}

	Delay_us(40);

	if (DHT11_IO_IN)
		return 1;
	else
		return 0;
}

/**
 * @brief
 * @param
 * @retval
 */
uint8_t DHT11_Read_Byte(void)
{
	uint8_t i, dat;
	dat = 0;

	for (i = 0; i < 8; i++)
	{
		dat <<= 1;
		dat |= DHT11_Read_Bit();
	}

	return dat;
}

/**
 * @brief
 * @param
 * @retval
 */
uint8_t DHT11_Read_Data(uint8_t *humi_h, uint8_t *humi_l, uint8_t *temp_h, uint8_t *temp_l)
{
	uint8_t buf[5] = {0};
	uint8_t i;

	DHT11_Rst();
	if (DHT11_Check() == 0)
	{
		for (i = 0; i < 5; i++)
		{
			buf[i] = DHT11_Read_Byte();
		}
		if ((buf[0] + buf[1] + buf[2] + buf[3])== buf[4])
		{
			*humi_h = buf[0];
			*humi_l = buf[1];
			*temp_h = buf[2];
			*temp_l = buf[3];
			return 0;
		}
	}

	return 1;
}
