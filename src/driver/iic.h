#ifndef __MYIIC_H
#define __MYIIC_H
#include "stm32f0xx.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/9
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////

//IO��������
 
#define SDA_IN()  {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=0<<7*2;}
#define SDA_OUT() {GPIOF->MODER&=~(3<<(7*2));GPIOF->MODER|=1<<7*2;}

//IO��������	 
//#define IIC_SCL    //PFout(6) //SCL
//#define IIC_SDA    //PFout(7) //SDA
//#define READ_SDA  //PFin(7)  //����SDA

#define IIC_SCL_HIGH() GPIOF->BSRR|=GPIO_Pin_6
#define IIC_SCL_LOW() GPIOF->BRR|=GPIO_Pin_6
#define IIC_SDA_HIGH() GPIOF->BSRR|=GPIO_Pin_7
#define IIC_SDA_LOW() GPIOF->BRR|=GPIO_Pin_7
#define READ_SDA() (GPIOF->IDR>>7)&0x01

//IIC���в�������
void IIC_Init(void);                //��ʼ��IIC��IO��				 
void IIC_Start(void);				//����IIC��ʼ�ź�
void IIC_Stop(void);	  			//����IICֹͣ�ź�
void IIC_Send_Byte(uint8_t txd);			//IIC����һ���ֽ�
uint8_t IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
uint8_t IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void IIC_Ack(void);					//IIC����ACK�ź�
void IIC_NAck(void);				//IIC������ACK�ź�

void IIC_Write_One_Byte(uint8_t daddr,uint8_t addr,uint8_t data);
uint8_t IIC_Read_One_Byte(uint8_t daddr,uint8_t addr);	  
#endif
















