/*
 * share_func.h
 *
 *  Created on: 2025年12月12日
 *      Author: WangQingChuan
 */

#ifndef SHARE_FUNC_H_
#define SHARE_FUNC_H_

#include "main.h"

extern uint8_t temperature; // 温度
extern uint8_t humidity;    // 湿度
extern uint32_t count_num;
extern uint8_t temp_humid_updated;

void Delay_us(uint32_t xus);
void Delay_ms(uint32_t xms);
void Delay_s(uint32_t xs);

#endif /* SHARE_FUNC_H_ */
