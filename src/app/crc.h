/*
 * crc.h
 *
 *  Created on: 2018��11��29��
 *      Author: ecarx
 */

#ifndef APP_CRC_H_
#define APP_CRC_H_

#include <stdint.h>

uint16_t ModBusCRC16(uint8_t cmd[], int len);

#endif /* APP_CRC_H_ */
