#include "iic.h"
#include "delay.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK��ӢSTM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
 

//��ʼ��IIC
void IIC_Init(void)
{					     
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	GPIO_SetBits(GPIOF,GPIO_Pin_6|GPIO_Pin_7);
}
//����IIC��ʼ�ź�
void IIC_Start(void)
{
	SDA_OUT();     //sda�����
	IIC_SDA_HIGH();
	IIC_SCL_HIGH();
	delay_us(2);
 	IIC_SDA_LOW();//START:when CLK is high,DATA change form high to low
	delay_us(2);
	IIC_SCL_LOW();//ǯסI2C���ߣ�׼�����ͻ��������
}	  
//����IICֹͣ�ź�
void IIC_Stop(void)
{
	SDA_OUT();//sda�����
	IIC_SCL_LOW();
	IIC_SDA_LOW();//STOP:when CLK is high DATA change form low to high
 	delay_us(2);
	IIC_SCL_HIGH();
	IIC_SDA_HIGH();//����I2C���߽����ź�
	delay_us(2);
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
uint8_t IIC_Wait_Ack(void)
{
	uint8_t ucErrTime=0;
	SDA_IN();      //SDA����Ϊ����  
	IIC_SDA_HIGH();delay_us(1);
	IIC_SCL_HIGH();delay_us(1);
	while(READ_SDA())
	{
		ucErrTime++;
		if(ucErrTime>250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_LOW();//ʱ�����0
	return 0;  
} 
//����ACKӦ��
void IIC_Ack(void)
{
	IIC_SCL_LOW();
	SDA_OUT();
	IIC_SDA_LOW();
	delay_us(2);
	IIC_SCL_HIGH();
	delay_us(2);
	IIC_SCL_LOW();
}
//������ACKӦ��		    
void IIC_NAck(void)
{
	IIC_SCL_LOW();
	SDA_OUT();
	IIC_SDA_HIGH();
	delay_us(2);
	IIC_SCL_HIGH();
	delay_us(2);
	IIC_SCL_LOW();
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void IIC_Send_Byte(uint8_t txd)
{                        
    uint8_t t;   
	SDA_OUT(); 	    
    IIC_SCL_LOW();//����ʱ�ӿ�ʼ���ݴ���
    for(t=0;t<8;t++)
    {
    	if((txd&0x80)>>7)
    	{
    		IIC_SDA_HIGH();
    	}
    	else
    	{
    		IIC_SDA_LOW();
    	}
        txd<<=1; 	  
		delay_us(2);   //��TEA5767��������ʱ���Ǳ����
		IIC_SCL_HIGH();
		delay_us(2); 
		IIC_SCL_LOW();
		delay_us(2);
    }	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
uint8_t IIC_Read_Byte(unsigned char ack)
{
	unsigned char i,receive=0;
	SDA_IN();//SDA����Ϊ����
    for(i=0;i<8;i++ )
	{
        IIC_SCL_LOW();
        delay_us(2);
		IIC_SCL_HIGH();
        receive<<=1;
        if(READ_SDA())receive++;
		delay_us(1); 
    }					 
    if (!ack)
        IIC_NAck();//����nACK
    else
        IIC_Ack(); //����ACK   
    return receive;
}



























