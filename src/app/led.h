#ifndef __LED_H
#define __LED_H

#include "stm32f0xx.h"

#define LED_RUN_OFF                       GPIOA->BSRR = 0x1<<15;
#define LED_RUN_ON                        GPIOA->BRR = 0x1<<15;
#define LED_RUN_TURN                      GPIOA->ODR ^= (0x1<<15);

#define LED_SP1_OFF                       GPIOF->BSRR = 0x1<<0;
#define LED_SP1_ON                        GPIOF->BRR = 0x1<<0;
#define LED_SP1_TURN                      GPIOF->ODR ^= (0x1<<0);

#define LED_SP2_OFF                       GPIOF->BSRR = 0x1<<1;
#define LED_SP2_ON                        GPIOF->BRR = 0x1<<1;
#define LED_SP2_TURN                      GPIOF->ODR ^= (0x1<<1);



void LED_Init(void);
void RS485EN_PORT_Init(void);

void TIM3_Init(void); 

#endif
