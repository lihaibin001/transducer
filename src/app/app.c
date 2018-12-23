/*
 * app.c
 *
 *  Created on: Dec 8, 2018
 *      Author: murphy
 */

#include "rs485.h"
#include <string.h>
#include "crc.h"
#include "debug.h"
#include "base_timer.h"
#include "modbus_rtu.h"
#include <stdbool.h>
#include "tm7711.h"

#define MODBUS_RTU_FRAME_LEN 8
#define CHANLE_NUM 5

typedef struct
{
	uint8_t is_shelling : 1;
	uint8_t reserve : 7;
}App_flag_t;

typedef struct {
	GPIO_TypeDef *pPort;
	uint16_t clk_pin;
	uint16_t data_pin;
} Channel_t;


static uint32_t shelling = {0};
static App_flag_t app_flag;
const Channel_t g_channel[CHANLE_NUM] = {
		{ GPIOA, GPIO_Pin_12, GPIO_Pin_11 },
		{ GPIOB, GPIO_Pin_9, GPIO_Pin_8   },
		{ GPIOB, GPIO_Pin_11, GPIO_Pin_10  },
		{ GPIOB, GPIO_Pin_12, GPIO_Pin_13 },
		{ GPIOB, GPIO_Pin_15, GPIO_Pin_14  },
};

static uint32_t App_read_adc(uint8_t channel) {

	return Read_TM7711(CH1_10HZ, g_channel[channel].pPort, g_channel[channel].clk_pin, g_channel[channel].data_pin);
}

static uint32_t App_read_weight(uint16_t reg_addr) {
	uint32_t weight;
	if(!app_flag.is_shelling)
	{
		shelling = (App_read_adc(reg_addr / 2 - 1) & 0x00FFFE00) / 64;
		app_flag.is_shelling = 1;
//		delay_ms(20);
	}
	weight = (App_read_adc(reg_addr / 2 - 1) & 0x00FFFE00) / 64;
	return  weight - shelling;
}

void App_task(void) {
	uint8_t buff[MODBUS_RTU_FRAME_LEN] = "";
	uint8_t len = MODBUS_RTU_FRAME_LEN;
	if (modbus_get_one_frame(buff, &len) == 0) {
		switch (buff[1]) {
		case 0x03:
		{
			uint32_t weight_int =  App_read_weight(buff[3]);
			uint8_t weight_char[4];
			weight_char[0] = (uint8_t)(weight_int >> 24);
			weight_char[1] = (uint8_t)(weight_int >> 16);
			weight_char[2] = (uint8_t)(weight_int >> 8);
			weight_char[3] = (uint8_t)weight_int;
			modbus_encode_and_send(0x03, 4, weight_char);
			break;
		}
		default:
			break;
		}
	}
}
