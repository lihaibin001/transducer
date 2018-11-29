/*
 * debug.c
 *
 *  Created on: 2018Äê11ÔÂ29ÈÕ
 *      Author: ecarx
 */

#include "debug.h"

#pragma import(__use_no_semihosting)

struct __FILE {
	int handle;
/* Whatever you require here. If the only file you are using is */
/* standard output using printf() for debugging, no file handling */
/* is required. */
};
/* FILE is typedef¡¯ d in stdio.h. */
FILE __stdout;

void _sys_exit(int x) {
	x = x;
}

int fputc(int ch, FILE *f) {
	while (!((USART1->ISR) & (1 << 7))) {
	}
	USART1->TDR = ch;
	return (ch);
}

void debug_init(void) {
	Uart_Initialize(0);
}

