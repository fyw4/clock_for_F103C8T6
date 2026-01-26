/*
 * led.h
 *
 *  Created on: Sep 28, 2025
 *      Author: WangQingChuan
 */

#ifndef BSP_LED_LED_H_
#define BSP_LED_LED_H_

#include "main.h"
#define LED0(x) x ? HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET):\
				   HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET)

#define LED0_TOGGLE() HAL_GPIO_TogglePin(LED0_GPIO_Port,LED0_Pin) /* 翻转 LED0 */

#endif /* BSP_LED_LED_H_ */
