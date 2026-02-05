/*
 * interrupt.h
 *
 *  Created on: Feb 5, 2026
 *      Author: WangQingChuan
 */

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include "main.h"

extern uint8_t temperature; // 温度
extern uint8_t humidity;    // 湿度
extern uint32_t count_num;
extern uint8_t temp_humid_updated;

extern uint32_t adc_value;

#endif /* INTERRUPT_H_ */
