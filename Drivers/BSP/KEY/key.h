/*
 * key.h
 *
 *  Created on: Oct 20, 2025
 *      Author: WangQingChuan
 */

#ifndef BSP_KEY_KEY_H_
#define BSP_KEY_KEY_H_

#include "main.h"

#define KEY_Up HAL_GPIO_ReadPin(Up_GPIO_Port, Up_Pin)
#define KEY_Down HAL_GPIO_ReadPin(Down_GPIO_Port, Down_Pin)
#define KEY_Confirm HAL_GPIO_ReadPin(Confirm_GPIO_Port, Confirm_Pin)
#define KEY_Back HAL_GPIO_ReadPin(Back_GPIO_Port, Back_Pin)

#endif /* BSP_KEY_KEY_H_ */
