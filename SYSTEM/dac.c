#include "dac.h"
#include <math.h>

void dac_output_Triangle_init(void)//Camera5 ==> PA4 ==> DAC_Channel1
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct; 
	DAC_InitTypeDef  DAC_InitStruct;
	
	//(1)初始化DAC通道对应的IO口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出类型
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//(2)初始化定时器来触发DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增模式
	TIM_TimeBaseInitStruct.TIM_Period = 0xFF;//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;//设置预分频值
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
	//(3)初始化 DAC 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_Triangle;//生成三角波
	DAC_InitStruct.DAC_LFSRUnmask_TriangleAmplitude = DAC_TriangleAmplitude_1023;
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;//选择触发方式
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//使能输出缓冲区
	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	//(4) 打开DAC
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//(5) 选择通道x的数据位数和对齐方式，以及具体输出值
	DAC_SetChannel1Data(DAC_Align_12b_R, 0xFF);
}

#define pi 3.14
#define DAC_DHR12R1_ADDRESS   0x40007408
uint16_t sin_data[64]={0};
void dac_output_sin_init(void)//Camera5 ==> PA4 ==> DAC_Channel1 ==> DMA1_Stream5
{
	for(int i=0;i<64;i++)
	{
		sin_data[i] = 2047+sin(2*pi*i/64)*2047; 
	}
	GPIO_InitTypeDef  GPIO_InitStruct;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseInitStruct; 
	DAC_InitTypeDef  DAC_InitStruct;
	DMA_InitTypeDef  DMA_InitStruct;
	
	//(1)初始化DAC通道对应的IO口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;//推挽输出类型
	GPIO_InitStruct.GPIO_Speed =  GPIO_Speed_100MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
	//(2)初始化定时器来触发DAC
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM6, ENABLE);
	TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;//递增模式
	TIM_TimeBaseInitStruct.TIM_Period = 0xFF;//设置重装载值
	TIM_TimeBaseInitStruct.TIM_Prescaler = 0;//设置预分频值
	TIM_TimeBaseInit(TIM6, &TIM_TimeBaseInitStruct);
	TIM_SelectOutputTrigger(TIM6, TIM_TRGOSource_Update);
	TIM_Cmd(TIM6, ENABLE);
	//(3)初始化 DAC 
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_DAC, ENABLE);
	DAC_InitStruct.DAC_WaveGeneration = DAC_WaveGeneration_None;//不生成任何波
	DAC_InitStruct.DAC_Trigger = DAC_Trigger_T6_TRGO;//选择触发方式
	DAC_InitStruct.DAC_OutputBuffer = DAC_OutputBuffer_Enable;//使能输出缓冲区
	DAC_Init(DAC_Channel_1, &DAC_InitStruct);
	//(4) DMA配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Stream5);
	DMA_InitStruct.DMA_Channel = DMA_Channel_7;
	DMA_InitStruct.DMA_PeripheralBaseAddr = DAC_DHR12R1_ADDRESS;
	DMA_InitStruct.DMA_Memory0BaseAddr = (unsigned long)sin_data;
	DMA_InitStruct.DMA_DIR = DMA_DIR_MemoryToPeripheral;//内存到外设方向
	DMA_InitStruct.DMA_BufferSize = 64;//传输数据个数
	DMA_InitStruct.DMA_PeripheralInc = DMA_PeripheralInc_Disable;//外设地址不需要递增
	DMA_InitStruct.DMA_MemoryInc = DMA_MemoryInc_Enable;//内存地址需要递增
	DMA_InitStruct.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
	DMA_InitStruct.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStruct.DMA_Mode = DMA_Mode_Circular;//循环模式
	DMA_InitStruct.DMA_Priority = DMA_Priority_High;//高优先级
	DMA_InitStruct.DMA_FIFOMode = DMA_FIFOMode_Enable;//使能FIFO模式
	DMA_InitStruct.DMA_FIFOThreshold = DMA_FIFOStatus_HalfFull;
	DMA_InitStruct.DMA_MemoryBurst = DMA_MemoryBurst_Single;
	DMA_InitStruct.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
	DMA_Init(DMA1_Stream5, &DMA_InitStruct);
	//(5)打开DMA 
	DMA_Cmd(DMA1_Stream5, ENABLE);
	//(6) 打开DAC
	DAC_Cmd(DAC_Channel_1, ENABLE);
	//(7) 选择通道x的数据位数和对齐方式，以及具体输出值
	DAC_SetChannel1Data(DAC_Align_12b_R, 0xFF);
	//(8)启用指定的DAC通道DMA请求
	DAC_DMACmd(DAC_Channel_1, ENABLE);
}
