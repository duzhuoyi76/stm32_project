#include "key.h"
#include "bitband.h"

void key_init(void)//PA0,PE2,PE3,PE4
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	/*step1:使能GPIO分组的时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOE, ENABLE);
	/*step2:GPIO初始化*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
}

void key_exti_init(void)
{
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	/*step1: 中断源配置*/
	key_init();
	/*SYSCFG配置*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource0);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource2);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);
	/*EXTI外部中断的配置*/
	EXTI_InitStruct.EXTI_Line = EXTI_Line0|EXTI_Line2|EXTI_Line3|EXTI_Line4;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;//中断模式
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Falling;
	EXTI_Init(&EXTI_InitStruct);
	/*step3: NVIC配置*/
	NVIC_InitStruct.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI2_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI3_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = EXTI4_IRQn;
	NVIC_Init(&NVIC_InitStruct);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

int key_get_status(int key_num)
{
	switch(key_num)
	{
		case KEY0:
			//return GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0);
			return PAin(0);
		case KEY1:
			//return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2);
			return PEin(2);
		case KEY2:
			//return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3);
			return PEin(3);
		case KEY3:
			//return GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4);
			return PEin(4);
		default:
			return KEY_UNKNOW;
	}
}

