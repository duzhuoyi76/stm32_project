#include "beep.h"
#include "bitband.h"
void beep_init(void)//PF8
{
	
	GPIO_InitTypeDef  GPIO_InitStruct;
	
	/*step1: 使能GPIO分组时钟*/
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	/*step2: GPIO初始化*/
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_8;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	/*step3: 默认输出高电平*/
	GPIO_ResetBits(GPIOF, GPIO_Pin_8);

}
void beep_ctrl(int beep_state)
{
	//GPIO_WriteBit(GPIOF, GPIO_Pin_8, (BitAction) beep_state);
	PFout(8) = beep_state;
}

