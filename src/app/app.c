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

static uint8_t send_buff[];

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

static uint8_t App_read_weight_handle(uint8_t reg, uint8_t *pBuff){
	uint32_t weight_int =  App_read_weight(reg);
//	uint8_t weight_char[4];
//	pBuff[0] = (uint8_t)(weight_int >> 24);
//	pBuff[1] = (uint8_t)(weight_int >> 16);
//	pBuff[2] = (uint8_t)(weight_int >> 8);
//	pBuff[3] = (uint8_t)weight_int;
	return modbus_encode(0x03, 4, (uint8_t *)&weight_int, pBuff);
}

static uint8_t App_read_coil_handle(uint8_t coil_addr, uint8_t coil_cnt, uint8_t *pBuff)
{

}

void App_task(void) {
	uint8_t buff[MODBUS_RTU_FRAME_LEN] = "";
	uint8_t len = MODBUS_RTU_FRAME_LEN;
	uint8_t send_buff[256];
	uint8_t send_len;
	if (modbus_get_one_frame(buff, &len) == 0) {
		switch (buff[1]) {
		case 0x03:
			send_len = App_read_weight_handle(buff[3], send_buff);
			break;
#if 0
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
#endif
		case 0x01:
			send_len = App_read_coil_handle(buff[3], buff[5], send_buff);
			break;
		case 0x05:

			break;
		default:
			return;
		}
		modbus_send(send_buff, send_len);
	}
}
