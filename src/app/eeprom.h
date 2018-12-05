#ifndef __EE_H__
#define __EE_H__

#include "iic.h"

typedef enum
{
	ee_operation_ok,
	ee_invalid_paramter,
}ee_ret_t;


ee_ret_t EE_Write_Data(uint16_t WriteAddr, uint8_t *pData, int DataLen);
ee_ret_t EE_Read_Data(uint16_t addr, uint8_t *pBuff, uint32_t DataLen);
void EE_Test(void);
#endif //__EE_H__
