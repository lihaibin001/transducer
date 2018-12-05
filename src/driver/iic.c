/*
 * iic.c
 *
 *  Created on: 2018Äê12ÔÂ5ÈÕ
 *      Author: ecarx
 */



static void IIC_Config(void)
{
    I2C_InitTypeDef I2C_InitStruct;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, ENABLE);

    I2C_InitStruct.I2C_Ack=I2C_Ack_Enable;
    I2C_InitStruct.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
    I2C_InitStruct.I2C_AnalogFilter=I2C_AnalogFilter_Enable;
    I2C_InitStruct.I2C_DigitalFilter=0x00;
    I2C_InitStruct.I2C_Mode=I2C_Mode_I2C;
    I2C_InitStruct.I2C_OwnAddress1=0x00;
    I2C_InitStruct.I2C_Timing=0x20D22E37;
    I2C_Init(I2C1,&I2C_InitStruct);
}

static void IIC_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct;

    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

    /*PF7-I2C_SDA   PF6-I2C_SCK */
    GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF;
    GPIO_InitStruct.GPIO_OType=GPIO_OType_OD;
    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_8;
    GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.GPIO_Pin=GPIO_Pin_9;
    GPIO_Init(GPIOB, &GPIO_InitStruct);

     /* Connect PXx to I2C_SCL*/
    GPIO_PinAFConfig( GPIOF , GPIO_PinSource8, GPIO_AF_1);
    /* Connect PXx to I2C_SDA*/
    GPIO_PinAFConfig( GPIOF ,GPIO_PinSource9, GPIO_AF_1);
}

void IIC_Open(void)
{
    IIC_GPIO_Config();
    IIC_Config();
    I2C_Cmd(I2C1,ENABLE);
}

void IIC_Close(void)
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1, DISABLE);
    I2C_Cmd(I2C1,DISABLE);
}

