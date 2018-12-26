/*
 * modbus_rtu.c
 *
 *  Created on: Dec 16, 2018
 *      Author: murphy
 */

#include "modbus_rtu.h"
#include "debug.h"
#include "string.h"
#include "rs485.h"
#include "crc.h"
#define MOD_BUS_BUF_LEN 64
#define MOD_BUS_SEND_BUF 128
typedef struct {
	uint8_t rx_complete :1;
	uint8_t reserver :6;
} flag_t;

static flag_t flag;
static uint8_t modbus_buf[MOD_BUS_BUF_LEN];
static uint8_t modbus_len;
static uint8_t data_idx;
static uint8_t self_id;

void modbus_rtu_task(void) {
	if (flag.rx_complete == 0) {
		for (;;) {
			if (!rs485_read_char(&modbus_buf[data_idx])) {
				break;
			} else {
				switch (modbus_buf[1]) {
				case 0:
					break;
				case 3:
					modbus_len = 8;
					break;
				default:
					break;
				}
				data_idx++;
				if (data_idx == modbus_len && modbus_len != 0) {
					uint16_t crc16;
					crc16 = ModBusCRC16(modbus_buf, 6);
					if (crc16 == ((modbus_buf[7] << 8) | modbus_buf[6])) {
						DEBUG("[MODBUS] Rx: ");
						for (data_idx = 0; data_idx < modbus_len; data_idx++) {
							DEBUG("%02X ", modbus_buf[data_idx]);
						}
						DEBUG("\r\n");

						if (modbus_buf[0] == self_id) {
							flag.rx_complete = 1;
							data_idx = 0;
							return;
						} else {
							data_idx = 0;
							if (modbus_len == MOD_BUS_BUF_LEN) {
								modbus_len--;
							}
							memset(modbus_buf, 0, modbus_len);
							modbus_len = 0;
						}
					} else {
						DEBUG("[MODBUS] CRC error\r\n");
						memcpy(modbus_buf, &modbus_buf[1], modbus_len - 1);
						data_idx -= 2;
					}
				} else if (data_idx == MOD_BUS_BUF_LEN) {
					data_idx = 0;
				}
			}
		}
	}
}

uint8_t modbus_get_one_frame(uint8_t *pBuff, uint8_t *buff_len) {
	if (pBuff == NULL || buff_len == NULL || *buff_len > MOD_BUS_BUF_LEN) {
		return 1;
	}
	if (flag.rx_complete) {
		memcpy(pBuff, modbus_buf, modbus_len);
		*buff_len = modbus_len;
		memset(modbus_buf, 0, modbus_len);
		modbus_len = 0;
		flag.rx_complete = 0;
		return 0;
	} else {
		return 1;
	}
}

uint8_t modbus_encode_and_send(uint8_t fnc_code, uint8_t len, uint8_t *pData)
{
	if(fnc_code == 0 || len > (MOD_BUS_BUF_LEN - 5) || pData == NULL )
	{
		return 1;
	}
	uint8_t send_buf[MOD_BUS_SEND_BUF];
	uint16_t crc16;
	send_buf[0] = self_id;
	send_buf[1] = fnc_code;
	send_buf[2] = len;
	memcpy(&send_buf[3], pData, len);
	crc16 = ModBusCRC16(send_buf, len + 3);
	send_buf[len+4] = (uint8_t)(crc16 >> 8);
	send_buf[len+3] = (uint8_t)crc16;
	if(rs485_send_data(send_buf, len + 5) != len + 5)
	{
		return 1;
	}
	DEBUG("[MODBUS] Tx: ");
	uint8_t data_idx;
	for (data_idx = 0; data_idx < len + 5; data_idx++) {
		DEBUG("%02X ", send_buf[data_idx]);
	}
	DEBUG("\r\n");
	return 0;
}

uint8_t modbus_encode(uint8_t fnc_code, uint8_t len, uint8_t *pData, uint8_t *pBuff)
{
	if(fnc_code == 0 || len == 0 || pData == NULL || pBuff == NULL )
	{
		return 0;
	}
	uint16_t crc16;
	pBuff[0] = self_id;
	pBuff[1] = fnc_code;
	pBuff[2] = len;
	memcpy(&pBuff[3], pData, len);
	crc16 = ModBusCRC16(pBuff, len + 3);
	pBuff[len+4] = (uint8_t)(crc16 >> 8);
	pBuff[len+3] = (uint8_t)crc16;
	return len + 5;
}

uint8_t modbus_send(uint8_t *pData, uint8_t len)
{
	return (uint8_t)rs485_send_data(pData, len);
}
