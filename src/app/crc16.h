/*
 * crc16.h
 *
 *  Created on: 2018��11��29��
 *      Author: ecarx
 */

#ifndef APP_CRC16_H_
#define APP_CRC16_H_

#include <stdint.h>

void ModBusCRC16(uint8_t cmd[], int len);

#endif /* APP_CRC16_H_ */
