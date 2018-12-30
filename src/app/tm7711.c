/*
 * tm7711.c
 *
 *  Created on: 2018年11月29日
 *      Author: ecarx
 */

#include "tm7711.h"
#include "debug.h"
#include "sort_algorithm.h"

#define CHANNEL_CNT 5

static uint32_t uTm7711_Value_Buf[CHANNEL_CNT][5] = { 0 };
static uint8_t row;
static bool is_stabillize[5] = {false};
static uint32_t uTm7711_Value[CHANNEL_CNT];

void TM7711_PORT_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能GPIOA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB, ENABLE);

	/* tm7711 ch1 pa12 clk*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* tm7711 ch1 pa11 dat_in*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* tm7711 ch2 CH3 CH4 CH5  PB9,11,13,15 clk*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* tm7711 ch2 CH3 CH4 CH5  PB8,10,12,14 DAT IN */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12 | GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

uint32_t Read_TM7711(unsigned char next_select, GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin_CLK, uint16_t GPIO_Pin_Dat) {
	uint8_t i = 0;
	uint32_t data_temp = 0;

	if (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_Dat)) {
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		while (GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_Dat))
			;
	}
	for (i = 0; i < 24; i++) {

		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		data_temp <<= 1;
		delay_us(5);
		if (1 == GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_Dat)) {
			data_temp |= 1;
		}
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
	}
	switch (next_select) {
	case CH1_10HZ:
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		break;
	case CH1_40HZ:
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		break;
	case CH2_TEMP:
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		delay_us(1);
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
		break;
	default:
		break;
	}

	data_temp ^= 0x800000;
	return data_temp;
}
void TM7711_task(void) {
	uTm7711_Value_Buf[4][row] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_15, GPIO_Pin_14);
	uTm7711_Value_Buf[3][row] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_12, GPIO_Pin_13);
	uTm7711_Value_Buf[2][row] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_11, GPIO_Pin_10);
	uTm7711_Value_Buf[1][row] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_9, GPIO_Pin_8);
	uTm7711_Value_Buf[0][row] = Read_TM7711(CH1_10HZ, GPIOA, GPIO_Pin_12, GPIO_Pin_11);
	if (row++ == 5) {
		uint8_t channel;
		for (channel = 0; channel < 5; channel++) {
			quick_sort((int *)uTm7711_Value_Buf[channel], 0, 4);
			if (uTm7711_Value_Buf[channel][4] - uTm7711_Value_Buf[channel][0] > 0x1FF) {
				//not stabilize
				is_stabillize[channel] = false;
			} else {
				//stabilize
				uint32_t tmp = uTm7711_Value_Buf[channel][4] \
						+ uTm7711_Value_Buf[channel][3] \
						+ uTm7711_Value_Buf[channel][2] \
						+ uTm7711_Value_Buf[channel][1] \
						+ uTm7711_Value_Buf[channel][0];
				is_stabillize[channel] = true;
				uTm7711_Value[channel] = tmp / 5;
			}
		}
		row = 0;
	}
}

uint32_t TM7711_get_value(uint8_t channel)
{
	if(channel >= CHANNEL_CNT)
	{
		return 0;
	}
	return uTm7711_Value[channel];
}
