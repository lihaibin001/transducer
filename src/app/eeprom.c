
#include "eeprom.h"
#include "delay.h"
#include "debug.h"

static void EE_Write_Byte(uint16_t WriteAddr, uint8_t ByteValue)
{

}


ee_ret_t EE_Write_Data(uint16_t WriteAddr, uint8_t *pData, int DataLen)
{

	return ee_operation_ok;
}

static uint8_t  EE_Read_Byte(uint16_t ReadAddr)
{
	uint8_t temp;
	return temp;
}

ee_ret_t EE_Read_Data(uint16_t addr, uint8_t *pBuff, uint32_t DataLen)
{

	return ee_operation_ok;
}

void EE_Test(void)
{

}
