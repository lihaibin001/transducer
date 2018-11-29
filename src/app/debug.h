/*
 * debug.h
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: ecarx
 */

#ifndef APP_DEBUG_H_
#define APP_DEBUG_H_

#include "uart.h"
#include <stdio.h>
#define DEBUG_ENABLE
#ifdef DEBUG_ENABLE
#define DEBUG(_f, ...) printf(_f, ## __VA_ARGS__)
#else
#define DEBUG(_f, ...)
#endif

void debug_init(void);

#endif /* APP_DEBUG_H_ */
