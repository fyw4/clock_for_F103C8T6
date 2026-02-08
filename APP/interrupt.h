/*
 * interrupt.h
 *
 *  Created on: Feb 5, 2026
 *      Author: WangQingChuan
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "main.h"

extern uint8_t temperature_h; // 温度
extern uint8_t temperature_l; // 温度
extern uint8_t humidity_h;    // 湿度
extern uint8_t humidity_l;    // 湿度
extern uint32_t count_num;
extern uint8_t temp_humid_updated;

extern uint32_t adc_value;

#endif /* INTERRUPT_H_ */
