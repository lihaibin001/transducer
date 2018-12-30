/*
 * tm7711.h
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: ecarx
 */

#ifndef __TM7711_H__
#define __TM7711_H__

#include "stm32f0xx_conf.h"
#include "delay.h"
#include <stdbool.h>

#define CH1_10HZ 0x01
#define CH1_40HZ 0x02
#define CH2_TEMP 0x03

#define CH1_10HZ_CLK 25
#define CH2_40HZ_CLK 27
#define CH2_TEMP_CLK 26


uint32_t Read_TM7711(unsigned char next_select, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CLK, uint16_t GPIO_Pin_Dat);
void TM7711_PORT_Init(void);
void TM7711_task(void);
uint32_t TM7711_get_value(uint8_t channel);

#endif
