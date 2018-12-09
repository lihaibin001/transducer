/*
 * rs485.h
 *
 *  Created on: Nov 28, 2018
 *      Author: murphy
 */

#ifndef APP_RS485_H_
#define APP_RS485_H_

#include "uart.h"

#define rx485_read_char(x) Uart_Get_Char(1, x)

void rs485_init(void);
bool rs485_send_data(uint8_t *pData, uint32_t len);

#endif /* APP_RS485_H_ */
