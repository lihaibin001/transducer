/*
 * crc.c
 *
 *  Created on: 2018��11��29��
 *      Author: ecarx
 */

#include "crc.h"

uint16_t ModBusCRC16(uint8_t cmd[], int len)
{
    uint16_t i, j, tmp, CRC16;

    CRC16 = 0xFFFF;             //CRC�Ĵ�����ʼֵ
    for (i = 0; i < len; i++)
    {
        CRC16 ^= cmd[i];
        for (j = 0; j < 8; j++)
        {
            tmp = (uint16_t)(CRC16 & 0x0001);
            CRC16 >>= 1;
            if (tmp == 1)
            {
                CRC16 ^= 0xA001;    //������ʽ
            }
        }
    }
//    cmd[i++] =  (uint8_t)(CRC16 & 0x00FF);
//    cmd[i++] = (uint8_t)((CRC16 & 0xFF00)>>8);
    return CRC16;
}
