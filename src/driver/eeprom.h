#ifndef __EE_H__
#define __EE_H__

#include "iic.h"


#define EE_SIZE 4000
#define EE_ADDRES 0xA0
#define EE_WRITE_COM (EE_ADDRES & 0xFE)
#define EE_READ_COM (EE_ADDRES | 0x01)

typedef enum
{
	ee_operation_ok,
	ee_invalid_paramter,
}ee_ret_t;


ee_ret_t EE_Write_Data(uint16_t WriteAddr, const uint8_t *pData, uint16_t DataLen);
ee_ret_t EE_Read_Data(uint16_t addr, uint8_t *pBuff, uint16_t DataLen);
void EE_Test(void);
#endif //__EE_H__
