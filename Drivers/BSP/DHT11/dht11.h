/*
 * dht11.h
 *
 *  Created on: Feb 4, 2026
 *      Author: WangQingChuan
 */

#ifndef BSP_DHT11_DHT11_H_
#define BSP_DHT11_DHT11_H_

#include "main.h"

#define DHT11_HIGH HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_SET)  // 输出高电平
#define DHT11_LOW HAL_GPIO_WritePin(DHT11_GPIO_Port, DHT11_Pin, GPIO_PIN_RESET) // 输出低电平

#define DHT11_IO_IN HAL_GPIO_ReadPin(DHT11_GPIO_Port, DHT11_Pin) // 读取IO口电平

void Delay_us(uint32_t us);

void DHT11_OUT(void);
void DHT11_Rst(void);                                // 主机发出起始信号
uint8_t DHT11_Check(void);                             // DHT11发送响应信号
uint8_t DHT11_Read_Bit(void);                          // 读取DHT11一个BIT的数据
uint8_t DHT11_Read_Byte(void);                         // 读取DHT11一个Byte的数据
uint8_t DHT11_Read_Data(uint8_t *humi_h, uint8_t *humi_l, uint8_t *temp_h, uint8_t *temp_l); // 读取DHT11湿度和温度的数据

#endif /* BSP_DHT11_DHT11_H_ */
