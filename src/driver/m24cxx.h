/*
 * m24cxx.h
 *
 *  Created on: 2018Äê12ÔÂ5ÈÕ
 *      Author: ecarx
 */

#include "stm32f0xx_conf.h"
#include "iic.h"
#ifndef DRIVER_M24CXX_H_
#define DRIVER_M24CXX_H_

#define M24Cxx_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define M24Cxx_LONG_TIMEOUT         ((uint32_t)(10 * M24Cxx_FLAG_TIMEOUT))

#define M24Cxx_MAX_TRIALS_NUMBER     300

#define M24Cxx_OK                    0
#define M24Cxx_FAIL                  1
#define M24Cxx_I2C                   I2C1

#define M24Cxx_PAGESIZE              16

#define M24Cxx_Address            0xA0

#define M24Cxx_SIZE					4000

#endif /* DRIVER_M24CXX_H_ */
