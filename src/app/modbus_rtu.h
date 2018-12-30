/*
 * modbus_rtu.h
 *
 *  Created on: Dec 16, 2018
 *      Author: murphy
 */

#ifndef APP_MODBUS_RTU_H_
#define APP_MODBUS_RTU_H_

#include <stdint.h>

void modbus_rtu_task(void);
uint8_t modbus_get_one_frame(uint8_t *pBuff, uint8_t *buff_len);
uint8_t modbus_encode_and_send(uint8_t fnc_code, uint8_t len, uint8_t *pData);
uint8_t modbus_encode(uint8_t fnc_code, uint8_t len, uint8_t *pData, uint8_t *pBuff);
uint8_t modbus_send(uint8_t *pData, uint8_t len);
#endif /* APP_MODBUS_RTU_H_ */
