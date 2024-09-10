#include "led.h"
#include "bitband.h"

void led_init(void)//PF9,PF10,PE13,PE14
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*step1: 使能GPIO分组时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF|RCC_AHB1Periph_GPIOE, ENABLE);
	/*step2: GPIO初始化*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_OD;//开漏输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14;
	GPIO_Init(GPIOE, &GPIO_InitStruct);
	/*step3: 默认输出高电平*/
	GPIO_SetBits(GPIOF, GPIO_Pin_9|GPIO_Pin_10);
	GPIO_SetBits(GPIOE, GPIO_Pin_13|GPIO_Pin_14);
	
}

//void led_init_new(void)//PA14
//{
//	GPIO_InitTypeDef  GPIO_InitStruct;
//	
//	/*step1: 使能GPIO分组时钟*/
//	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
//	/*step2: GPIO初始化*/
//	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_14;
//	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
//	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//开漏输出
//	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStruct);
//	/*step3: 默认输出高电平*/
//	GPIO_SetBits(GPIOA, GPIO_Pin_14);
//	
//}

void led_init_new(void)//PC7
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*step1: 使能GPIO分组时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	/*step2: GPIO初始化*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//开漏输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	/*step3: 默认输出高电平*/
	GPIO_SetBits(GPIOC, GPIO_Pin_7);
	
}

void led_ctrl(int led_num, int led_state)
{
	switch(led_num)
	{
	case LED0:
		//GPIO_WriteBit(GPIOF, GPIO_Pin_9, (BitAction) led_state);break;
		PFout(9) = led_state;break;
	case LED1:
		//GPIO_WriteBit(GPIOF, GPIO_Pin_10, (BitAction) led_state);break;
		PFout(10) = led_state;break;
	case LED2:
		//GPIO_WriteBit(GPIOE, GPIO_Pin_13, (BitAction) led_state);break;
		PEout(13) = led_state;break;
	case LED3:
		//GPIO_WriteBit(GPIOE, GPIO_Pin_14, (BitAction) led_state);break;
		PEout(14) = led_state;break;
	}
}



