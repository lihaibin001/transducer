
#include "eeprom.h"
#include "delay.h"
#include "debug.h"
#include "string.h"
u8 IS_NEED_WRITE_EEPROM = 0;

void EE_init(void)
{
	IIC_Init();
}

static void EE_Write_Byte(uint16_t WriteAddr, u8 ByteValue)
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


ee_ret_t EE_Write_Data(uint16_t WriteAddr, uint8_t *pData, int DataLen)
{
	uint16_t DataIndex = 0;
	if(WriteAddr >= 400)
	{
		return ee_invalid_paramter;
	}
	for(DataIndex = 0; DataIndex < DataLen; DataIndex++)
	{
		EE_Write_Byte(WriteAddr+DataIndex,pData[DataIndex]);
	}
	return ee_operation_ok;
}

static uint8_t  EE_Read_Byte(uint16_t ReadAddr)
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

ee_ret_t EE_Read_Data(uint16_t addr, uint8_t *pBuff, uint32_t DataLen)
{
	uint16_t BuffIndex;
	if(addr > EE_DEVICE_SIZE)
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
	  uint16_t i;
	  uint8_t tmp_data[32] = "";
	  for(i=0; i<125; i++)
	  {
		  memset(tmp_data, i, 32);
		  EE_Write_Data(i*32, tmp_data, 32);
	  }
	  for(i=0; i<125; i++)
	  {
		  uint8_t j;
		  memset(tmp_data, i, 32);
		  EE_Read_Data(i*32, tmp_data, 32);
		  DEBUG("[EE] Read Test:\r\n\t");
		  for(j=0; j<32; j++)
		  {
			  DEBUG("%02X ", tmp_data[j]);
		  }
		  DEBUG("\r\n");
	  }
}
