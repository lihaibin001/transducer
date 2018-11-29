/*
 * debug.h
 *
 *  Created on: 2018��11��29��
 *      Author: ecarx
 */

#ifndef APP_DEBUG_H_
#define APP_DEBUG_H_

#include "uart.h"

#ifdef DEBUG_ENABLE
#define DEBUG(_f, ...) printf(_f, ## __VA_ARGS__)
#else
#define DEBUG(_f, ...)
#endif
#endif /* APP_DEBUG_H_ */
