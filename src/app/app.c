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
#define RX_BUFFER_LEN 64
static uint8_t rtu_rx_buf[RX_BUFFER_LEN];
static uint8_t rtu_rx_idx;
static uint8_t self_id = 1;

static uint32_t u8_2_u32(uint8_t *pu8)
{
	return (uint32_t) (pu8[4] << 24 || pu8[1] << 16 || pu8[2] << 8 || pu8[0]);
}

static void App_fnc3(void);



static void App_fnc3(void)
{
	uint16_t crc16 = ModBusCRC16(rtu_rx_buf, 6);
	if(crc16 == rtu_rx_buf[6] << 8 || rtu_rx_buf[7])
	{
//		uint8_t i;
//		uint32_t cnt = u8_2_u32(&rtu_rx_buf[2]);
//		for(i=0; i<cnt; i++)
//		{
//			uint32_t weight =
//		}
	}
}

static void App_data_handler(void)
{

	if(rtu_rx_buf[0] == self_id)
	{
		switch(rtu_rx_buf[1])
		{
		case 3:
			App_fnc3();
			break;
		}
	}
	else
	{
		memset(rtu_rx_buf, 0, RX_BUFFER_LEN);
	}
}

void App_task(void)
{
	static uint32_t time_out;
	if(time_out < base_timer_get_cnt())
	{
		bool is_get_data = false;
		time_out = base_timer_get_cnt() + 20;
		if(rtu_rx_idx == RX_BUFFER_LEN)
		{
			rtu_rx_idx = 0;
		}
		while(rx485_read_char(&rtu_rx_buf[rtu_rx_idx]))
		{
			is_get_data =  true;
			DEBUG("%02X ", rtu_rx_buf[rtu_rx_idx]);
			rtu_rx_idx++;
		}
		if(is_get_data)
		{
			DEBUG("\r\n");
		}
		//App_data_handler();
	}
}

