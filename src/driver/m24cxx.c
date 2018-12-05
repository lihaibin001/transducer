/*
 * m24cxx.c
 *
 *  Created on: 2018Äê12ÔÂ5ÈÕ
 *      Author: ecarx
 */

uint32_t M24Cxx_TIMEOUT_UserCallback(void)
{
    /* Block communication and all processes */
    while (1)
    {
    }
}

void M24CXX_Open(void)
{
    IIC_Open();
}



uint32_t M24Cxx_WaitEepromStandbyState(void)
{
    __IO uint32_t sEETrials = 0;

/* Configure CR2 register : set Slave Address and end mode */
    I2C_TransferHandling(M24Cxx_I2C,
                         M24Cxx_Address,
                         0,
                         I2C_AutoEnd_Mode,
                         I2C_No_StartStop);

    do
    {
        /* Initialize sEETimeout */
        M24Cxx_Timeout = M24Cxx_FLAG_TIMEOUT;

        /* Clear NACKF */
        I2C_ClearFlag(M24Cxx_I2C,
                      I2C_ICR_NACKCF | I2C_ICR_STOPCF);

        /* Generate start */
        I2C_GenerateSTART(M24Cxx_I2C, ENABLE);

        /* Wait until timeout elapsed */
        while (M24Cxx_Timeout-- != 0);

        /* Check if the maximum allowed numbe of trials has bee reached */
        if (sEETrials++ == M24Cxx_MAX_TRIALS_NUMBER)
        {
            /* If the maximum number of trials has been reached, exit the function */
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_NACKF) != RESET);

    /* Clear STOPF */
    I2C_ClearFlag(M24Cxx_I2C, I2C_ICR_STOPCF);

    /* Return sEE_OK if device is ready */
    return M24Cxx_OK;
}


uint32_t M24Cxx_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite)
{
    uint32_t DataNum = 0;

    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 2, I2C_Reload_Mode, I2C_Generate_Start_Write);

    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TXIS) == RESET)
    {
        if((M24Cxx_Timeout--) == 0)
        {
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

  /* Send MSB of memory address */
    I2C_SendData(M24Cxx_I2C, (uint8_t)((WriteAddr & 0xFF00) >> 8));

  /* Wait until TXIS flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;

    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TXIS) == RESET)
    {
        if((M24Cxx_Timeout--) == 0)
        {
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

    /* Send LSB of memory address  */
    I2C_SendData(M24Cxx_I2C, (uint8_t)(WriteAddr & 0x00FF));

    /* Wait until TCR flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TCR) == RESET)
    {
        if((M24Cxx_Timeout--) == 0)
        {
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

    /* Update CR2 : set Slave Address , set write request, generate Start and set end mode */
    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, (uint8_t)(*NumByteToWrite), I2C_AutoEnd_Mode, I2C_No_StartStop);

    while (DataNum != (*NumByteToWrite))
    {
        /* Wait until TXIS flag is set */
        M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
        while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TXIS) == RESET)
        {
        if((M24Cxx_Timeout--) == 0)
        {
          return M24Cxx_TIMEOUT_UserCallback();
        }
        }

        /* Write data to TXDR */
        I2C_SendData(M24Cxx_I2C, (uint8_t)(pBuffer[DataNum]));

        /* Update number of transmitted data */
        DataNum++;
    }

    /* Wait until STOPF flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_STOPF) == RESET)
    {
        if((M24Cxx_Timeout--) == 0)
        {
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(M24Cxx_I2C, I2C_ICR_STOPCF);
    return M24Cxx_OK;
}


void M24Cxx_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite)
{
    uint16_t NumOfPage = 0, NumOfSingle = 0, count = 0;
    uint16_t Addr = 0;

    Addr = WriteAddr % M24Cxx_PAGESIZE;
    count = M24Cxx_PAGESIZE - Addr;
    NumOfPage =  NumByteToWrite / M24Cxx_PAGESIZE;
    NumOfSingle = NumByteToWrite % M24Cxx_PAGESIZE;

    /*!< If WriteAddr is sEE_PAGESIZE aligned  */
    if(Addr == 0)
    {
        /*!< If NumByteToWrite < sEE_PAGESIZE */
        if(NumOfPage == 0)
        {
            /* Store the number of data to be written */
            M24Cxx_DataNum = NumOfSingle;
            /* Start writing data */
            M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
            M24Cxx_WaitEepromStandbyState();
        }
        /*!< If NumByteToWrite > sEE_PAGESIZE */
        else
        {
            while(NumOfPage--)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = M24Cxx_PAGESIZE;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
                WriteAddr +=  M24Cxx_PAGESIZE;
                pBuffer += M24Cxx_PAGESIZE;
            }

            if(NumOfSingle!=0)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = NumOfSingle;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
            }
        }
    }
    /*!< If WriteAddr is not sEE_PAGESIZE aligned  */
    else
    {
        /*!< If NumByteToWrite < sEE_PAGESIZE */
        if(NumOfPage== 0)
        {
            /*!< If the number of data to be written is more than the remaining space
            in the current page: */
            if (NumByteToWrite > count)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = count;
                /*!< Write the data conained in same page */
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();

                /* Store the number of data to be written */
                M24Cxx_DataNum = (NumByteToWrite - count);
                /*!< Write the remaining data in the following page */
                M24Cxx_WritePage((uint8_t*)(pBuffer + count), (WriteAddr + count), (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
            }
            else
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = NumOfSingle;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
            }
        }
        /*!< If NumByteToWrite > sEE_PAGESIZE */
        else
        {
            NumByteToWrite -= count;
            NumOfPage =  NumByteToWrite / M24Cxx_PAGESIZE;
            NumOfSingle = NumByteToWrite % M24Cxx_PAGESIZE;

            if(count != 0)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = count;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
                WriteAddr += count;
                pBuffer += count;
            }

            while(NumOfPage--)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = M24Cxx_PAGESIZE;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
                M24Cxx_WaitEepromStandbyState();
                WriteAddr +=  M24Cxx_PAGESIZE;
                pBuffer += M24Cxx_PAGESIZE;
            }
            if(NumOfSingle != 0)
            {
                /* Store the number of data to be written */
                M24Cxx_DataNum = NumOfSingle;
                M24Cxx_WritePage(pBuffer, WriteAddr, (uint8_t*)(&M24Cxx_DataNum));
                M24Cxx_WaitEepromStandbyState();
            }
        }
    }
}


uint32_t M24Cxx_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead)
{
    uint32_t NumbOfSingle = 0, Count = 0, DataNum = 0, StartCom = 0;

    /* Get number of reload cycles */
    Count = (*NumByteToRead) / 255;
    NumbOfSingle = (*NumByteToRead) % 255;

    /* Configure slave address, nbytes, reload and generate start */
    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 2, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);

    /* Wait until TXIS flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TXIS) == RESET)
    {
        if((M24Cxx_Timeout--) == 0)
        {
            return M24Cxx_TIMEOUT_UserCallback();
        }
    }

    /* Send MSB of memory address */
    I2C_SendData(M24Cxx_I2C, (uint8_t)((ReadAddr & 0xFF00) >> 8));

    /* Wait until TXIS flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TXIS) == RESET)
    {
        if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
    }

    /* Send LSB of memory address  */
    I2C_SendData(M24Cxx_I2C, (uint8_t)(ReadAddr & 0x00FF));

    /* Wait until TC flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TC) == RESET)
    {
        if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
    }

    /* If number of Reload cycles is not equal to 0 */
    if (Count != 0)
    {
        /* Starting communication */
        StartCom = 1;

        /* Wait until all reload cycles are performed */
        while( Count != 0)
        {
            /* If a read transfer is performed */
            if (StartCom == 0)
            {
                /* Wait until TCR flag is set */
                M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
                while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TCR) == RESET)
                {
                    if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
                }
            }

            /* if remains one read cycle */
            if ((Count == 1) && (NumbOfSingle == 0))
            {
                /* if starting communication */
                if (StartCom != 0)
                {
                    /* Configure slave address, end mode and start condition */
                    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 255, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
                }
                else
                {
                    /* Configure slave address, end mode */
                    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 255, I2C_AutoEnd_Mode, I2C_No_StartStop);
                }
            }
            else
            {
                /* if starting communication */
                if (StartCom != 0)
                {
                    /* Configure slave address, end mode and start condition */
                    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 255, I2C_Reload_Mode, I2C_Generate_Start_Read);
                }
                else
                {
                    /* Configure slave address, end mode */
                    I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, 255, I2C_Reload_Mode, I2C_No_StartStop);
                }
            }

            /* Update local variable */
            StartCom = 0;
            DataNum = 0;

            /* Wait until all data are received */
            while (DataNum != 255)
            {
                /* Wait until RXNE flag is set */
                M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
                while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_RXNE) == RESET)
                {
                    if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
                }

                /* Read data from RXDR */
                pBuffer[DataNum]= I2C_ReceiveData(M24Cxx_I2C);

                /* Update number of received data */
                DataNum++;
                (*NumByteToRead)--;
            }
            /* Update Pointer of received buffer */
            pBuffer += DataNum;

            /* update number of reload cycle */
            Count--;
        }

        /* If number of single data is not equal to 0 */
        if (NumbOfSingle != 0)
        {
            /* Wait until TCR flag is set */
            M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
            while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_TCR) == RESET)
            {
                if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
            }

            /* Update CR2 : set Nbytes and end mode */
            I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, (uint8_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_No_StartStop);

            /* Reset local variable */
            DataNum = 0;

            /* Wait until all data are received */
            while (DataNum != NumbOfSingle)
            {
                /* Wait until RXNE flag is set */
                M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
                while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_RXNE) == RESET)
                {
                    if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
                }

                /* Read data from RXDR */
                pBuffer[DataNum]= I2C_ReceiveData(M24Cxx_I2C);

                /* Update number of received data */
                DataNum++;
                (*NumByteToRead)--;
            }
        }
    }
    else
    {
        /* Update CR2 : set Slave Address , set read request, generate Start and set end mode */
        I2C_TransferHandling(M24Cxx_I2C, M24Cxx_Address, (uint32_t)(NumbOfSingle), I2C_AutoEnd_Mode, I2C_Generate_Start_Read);

        /* Reset local variable */
        DataNum = 0;

        /* Wait until all data are received */
        while (DataNum != NumbOfSingle)
        {
            /* Wait until RXNE flag is set */
            M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
            while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_RXNE) == RESET)
            {
                if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
            }

            /* Read data from RXDR */
            pBuffer[DataNum]= I2C_ReceiveData(M24Cxx_I2C);

            /* Update number of received data */
            DataNum++;
            (*NumByteToRead)--;
        }
    }

    /* Wait until STOPF flag is set */
    M24Cxx_Timeout = M24Cxx_LONG_TIMEOUT;
    while(I2C_GetFlagStatus(M24Cxx_I2C, I2C_ISR_STOPF) == RESET)
    {
        if((M24Cxx_Timeout--) == 0) return M24Cxx_TIMEOUT_UserCallback();
    }

    /* Clear STOPF flag */
    I2C_ClearFlag(M24Cxx_I2C, I2C_ICR_STOPCF);

    /* If all operations OK, return sEE_OK (0) */
    return M24Cxx_OK;
}

