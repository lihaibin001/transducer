#include "led.h"

void LED_Init(void) {
	GPIO_InitTypeDef GPIO_InitStructure;

	/* 使能GPIOA时钟 */
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOF, ENABLE);

	/* 配置LED相应引脚PA15 run LED */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* 配置LED相应引脚 sp1 sp2 LED */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
	TIM3_Init();
}

void TIM3_Init(void) {

	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//RCC_APB1PeriphResetCmd(RCC_APB1Periph_TIM3, ENABLE);  //tim3????,APB??48M

	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1; //?????1
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; //????
	TIM_TimeBaseInitStructure.TIM_Period = 499;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 47999;  //??500ms
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE); //?????3????
	TIM_Cmd(TIM3, ENABLE); //?????3

	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //???3??
	NVIC_InitStructure.NVIC_IRQChannelPriority = 0; //???0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

}

void TIM3_IRQHandler(void) {

	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET) //????
	{
		LED_RUN_TURN;   //LED翻转
	}
	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);  //???????
}

