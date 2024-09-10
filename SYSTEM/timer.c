#include "timer.h"
#include "uart.h"
#include <stdio.h>

void tim3_update_init( uint32_t Period, uint16_t Prescaler)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;  
	NVIC_InitTypeDef  NVIC_InitStruct;
	
	/*step1: 使能定时器TIM3的时钟*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	/*step2: 定时器时基单元的配置*/
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增模式
	TIM_TimeBaseInitStruct.TIM_Period = Period;//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler;//设置预分频值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	/*step3: 定时器使能*/
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);
	/*step4: 中断配置*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//由更新事件触发中断
	NVIC_InitStruct.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

void beep_pwm_init( uint32_t Period, uint16_t Prescaler, uint32_t Pulse)//PF8 ==> TIM13_CH1
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;  
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	//(1)GPIO初始化，定时器通道是由GPIO功能复用的
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//功能复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出类型
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOF, GPIO_PinSource8, GPIO_AF_TIM13);
	//(2)使能定时器TIM13的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM13, ENABLE);
	//(3)定时器时基单元的配置(Time Base Unit)
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增模式
	TIM_TimeBaseInitStruct.TIM_Period = Period;//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler;//设置预分频值
	TIM_TimeBaseInit(TIM13, &TIM_TimeBaseInitStruct);
	//(4) 输出通道配置
	TIM_OCInitStruct.TIM_Pulse = Pulse;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC1Init(TIM13, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM13, TIM_OCPreload_Enable);
	//(5)定时器使能
	TIM_ARRPreloadConfig(TIM13, ENABLE);
	TIM_Cmd(TIM13, ENABLE);

}

void led_pwm_init( uint32_t Period, uint16_t Prescaler, uint32_t Pulse)//PC6 ==> TIM3_CH1
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct;  
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_OCInitTypeDef  TIM_OCInitStruct;
	
	//(1)GPIO初始化，定时器通道是由GPIO功能复用的
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;//功能复用模式
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出类型
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_TIM3);
	//(2)使能定时器TIM3的时钟
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	//(3)定时器时基单元的配置(Time Base Unit)
	TIM_TimeBaseInitStruct.TIM_ClockDivision=0;
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增模式
	TIM_TimeBaseInitStruct.TIM_Period = Period;//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = Prescaler;//设置预分频值
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);
	//(4) 输出通道配置
	TIM_OCInitStruct.TIM_Pulse = Pulse;
	TIM_OCInitStruct.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStruct.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStruct.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OC1Init(TIM3, &TIM_OCInitStruct);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
	//(5)定时器使能
	TIM_ARRPreloadConfig(TIM3, ENABLE);
	TIM_Cmd(TIM3, ENABLE);

}

void light_ctrl(int a)
{
	printf("ctrl\n");
	TIM3->CCR1 = a;
}

