#include "eeprom.h"
#include "delay.h"
#include "debug.h"
#include "iic.h"
#include <string.h>

void EE_Write_Byte(uint16_t WriteAddr, uint8_t ByteValue)
{
    IIC_Start();

	IIC_Send_Byte(EE_WRITE_COM);
	IIC_Wait_Ack();
	IIC_Send_Byte((WriteAddr >> 8) & 0x00FF);

	IIC_Wait_Ack();
    IIC_Send_Byte(WriteAddr & 0x00FF);
	IIC_Wait_Ack();
	IIC_Send_Byte(ByteValue);
	IIC_Wait_Ack();
    IIC_Stop();
	delay_ms(5);
}



ee_ret_t EE_Write_Data(uint16_t WriteAddr, const uint8_t *pData, uint16_t DataLen)
{
	uint16_t DataIndex = 0;
	if(WriteAddr > EE_SIZE || DataLen > EE_SIZE - WriteAddr || pData == NULL)
	{
		return ee_invalid_paramter;
	}
	for(DataIndex = 0; DataIndex < DataLen; DataIndex++)
	{
		EE_Write_Byte(WriteAddr+DataIndex,pData[DataIndex]);
	}
	return ee_operation_ok;
}

uint8_t  EE_Read_Byte(uint16_t ReadAddr)
{
	uint8_t temp=0;
    IIC_Start();

	IIC_Send_Byte(EE_WRITE_COM);
	IIC_Wait_Ack();

	IIC_Send_Byte((ReadAddr >> 8) & 0x00FF);
	IIC_Wait_Ack();
    IIC_Send_Byte(ReadAddr & 0x00FF);
	IIC_Wait_Ack();

	IIC_Start();
	IIC_Send_Byte(EE_READ_COM);
	IIC_Wait_Ack();
    temp = IIC_Read_Byte(0);
    IIC_Stop();
	return temp;
}

ee_ret_t EE_Read_Data(uint16_t addr, uint8_t* pBuff, uint16_t DataLen)
{
	int BuffIndex;
	if(addr > EE_SIZE || DataLen > EE_SIZE - addr || pBuff == NULL)
	{
		return ee_invalid_paramter;
	}
	for(BuffIndex = 0; BuffIndex < DataLen; BuffIndex++,addr++)
	{
		pBuff[BuffIndex] = EE_Read_Byte(addr);
	}
	return ee_operation_ok;
}

void EE_Test(void)
{
	uint8_t test_buffer[32] = "";
	uint16_t i,j;
	for(i=0; i<125; i++)
	{
		memset(test_buffer, i, 32);
		EE_Write_Data(i*32, (const uint8_t *)test_buffer, 32);
		DEBUG("[EE] Write complete\r\n");
	}
	for(i=0; i<125; i++)
	{
		EE_Read_Data(i*32, test_buffer, 32);
		DEBUG("[EE] Read data:\r\n\t");
		for(j=0; j<32; j++)
		{
			DEBUG("%02X ", test_buffer[j]);
		}
		DEBUG("\r\n");
	}
	delay_ms(1000);
}

