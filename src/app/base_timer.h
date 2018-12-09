/*
 * base_timer.h
 *
 *  Created on: Dec 9, 2018
 *      Author: murphy
 */

#ifndef APP_BASE_TIMER_H_
#define APP_BASE_TIMER_H_

#include "stm32f0xx_conf.h"

void base_timer_Init(void);
uint32_t base_timer_get_cnt(void);

#endif /* APP_BASE_TIMER_H_ */
