/*
 * tm7711.h
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: ecarx
 */

#include "stm32f0xx_conf.h"
#include "delay.h"
#define CH1_10HZ 0x01
#define CH1_40HZ 0x02
#define CH2_TEMP 0x03

#define CH1_10HZ_CLK 25
#define CH2_40HZ_CLK 27
#define CH2_TEMP_CLK 26

void TM7711_PORT_Init(void);
void TM7711_task(void);
