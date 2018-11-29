/*
 * debug.c
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: ecarx
 */

#include "debug.h"

int __io_putchar(int ch)
{
	while (!((USART1->ISR) & (1 << 7))) {
	}
	USART1->TDR = ch;
	return ch;
}

int _write(int file, char *ptr, int len)
{
      int DataIdx;
      for (DataIdx = 0; DataIdx < len;DataIdx++)
     {
           __io_putchar(*ptr++);
     }
      return len;
}


void debug_init(void) {
	Uart_Initialize(0);
}

