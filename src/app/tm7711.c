/*
 * tm7711.c
 *
 *  Created on: 2018��11��29��
 *      Author: ecarx
 */

#include "tm7711.h"
#include "debug.h"
unsigned long uTm7711_Value_Buf[5] = { 0 };

void TM7711_PORT_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* ʹ��GPIOAʱ�� */
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
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* tm7711 ch2 CH3 CH4 CH5  PB9,11,13,15 clk*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_11 | GPIO_Pin_13
			| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	/* tm7711 ch2 CH3 CH4 CH5  PB8,10,12,14 DAT IN */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_10 | GPIO_Pin_12
			| GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

unsigned long Read_TM7711(unsigned char next_select, GPIO_TypeDef* GPIOx,
		uint16_t GPIO_Pin_CLK, uint16_t GPIO_Pin_Dat) {
	unsigned char i = 0;
	unsigned long data_temp = 0;
	for (i = 0; i < 24; i++) {

		GPIO_SetBits(GPIOx, GPIO_Pin_CLK);
		data_temp <<= 1;
		delay_us(5);
		if (1 == GPIO_ReadInputDataBit(GPIOx, GPIO_Pin_Dat)) {
			data_temp |= 1;
		}
		GPIO_ResetBits(GPIOx, GPIO_Pin_CLK);
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
#if 0
	if ((data_temp & 0x00800000) == 0x00800000) // �ж��Ƿ�Ϊ����
	{
		data_temp = (~(data_temp | 0xff000000)) + 1;
	}
#endif
	return data_temp;
}

void TM7711_task(void) {
	uTm7711_Value_Buf[0] = Read_TM7711(CH1_10HZ, GPIOA, GPIO_Pin_12, GPIO_Pin_11);
	DEBUG("channel 0:%d\r\n",uTm7711_Value_Buf[0]);
	uTm7711_Value_Buf[1] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_9, GPIO_Pin_8);
	DEBUG("channel 1:%d\r\n",uTm7711_Value_Buf[1]);
	uTm7711_Value_Buf[2] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_11, GPIO_Pin_10);
	DEBUG("channel 2:%d\r\n",uTm7711_Value_Buf[2]);
	uTm7711_Value_Buf[3] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_13, GPIO_Pin_12);
	DEBUG("channel 3:%d\r\n",uTm7711_Value_Buf[3]);
	uTm7711_Value_Buf[4] = Read_TM7711(CH1_10HZ, GPIOB, GPIO_Pin_15, GPIO_Pin_14);
	DEBUG("channel 4:%d\r\n",uTm7711_Value_Buf[4]);
}
