/*
 * rs485.c
 *
 *  Created on: Nov 28, 2018
 *      Author: murphy
 */

#include "rs485.h"

static void rs485_enable(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_SetBits(GPIOA, GPIO_Pin_4);
}

void rs485_init(void) {
	Uart_Initialize(1);
	rs485_enable();
}

void rs485_task(void) {

}
bool rs485_send_data(uint8_t *pData, uint32_t len) {
	if(len != UART_Transmit(1, pData, len)){
		return false;
	}
	return true;
}