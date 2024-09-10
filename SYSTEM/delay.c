#include "delay.h"

void delay_ms(uint16_t n)
{
	int p=n/500;
	int q=n%500;
	int k=500;

	uint32_t tmp;

	for(int i=0;i<p+1;i++)
	{
		if(i == p)//最后一次循环        
		{
			if(q == 0)
				break;
			k = q;
		}
		SysTick->LOAD = SystemCoreClock/8/1000*k;//设置初值,24bits,且不能设置为0
		SysTick->VAL = 0x00;//把当前数值清空
		SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开启SysTick,开始计数
		do
		{
			tmp = SysTick->CTRL;
		}
		while((tmp&0x01)&&!(tmp&(1<<16)));//等待倒数到0
		SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭SysTick,停止计数
		SysTick->VAL = 0x00;//把当前数值清空
	}
}

void delay_us(uint16_t n)
{
	uint32_t tmp;
	SysTick->LOAD = SystemCoreClock/8/1000000*n;//24bits 16,777,215
	SysTick->VAL = 0x00;//把当前数值清空              10,500,000
	SysTick->CTRL |= SysTick_CTRL_ENABLE_Msk;//开启SysTick,开始计数
	do
	{
		tmp = SysTick->CTRL;
	}
	while((tmp&0x01)&&!(tmp&(1<<16)));//等待倒数到0
	SysTick->CTRL &= ~SysTick_CTRL_ENABLE_Msk;//关闭SysTick,停止计数
	SysTick->VAL = 0x00;//把当前数值清空
}


