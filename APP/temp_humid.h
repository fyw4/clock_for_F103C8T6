/*
 * temp_humid.h
 *
 *  Created on: 2026年2月11日
 *      Author: WangQingChuan
 */

#ifndef TEMP_HUMID_H_
#define TEMP_HUMID_H_

#include "main.h"

extern uint8_t temperature_h; // 温度
extern uint8_t temperature_l; // 温度
extern uint8_t humidity_h;    // 湿度
extern uint8_t humidity_l;    // 湿度

extern uint8_t temp_humid_updated;
extern uint8_t temp_humid_ready;

int temp_humid_UI();

#endif /* TEMP_HUMID_H_ */
