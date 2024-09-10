#include "adc.h"

void lightSensor_init(void)//PF7 ==> ADC3_IN5
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	ADC_CommonInitTypeDef  ADC_CommonInitStruct;
	ADC_InitTypeDef  ADC_InitStruct;
	//(1)初始化ADC通道对应的IO口
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;//禁用上下拉
	GPIO_Init(GPIOF, &GPIO_InitStruct);
	//(2)ADC 通用功能初始化配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE);
	ADC_CommonInitStruct.ADC_Mode = ADC_Mode_Independent;//独立模式
	ADC_CommonInitStruct.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;//禁用DMA
	ADC_CommonInitStruct.ADC_Prescaler = ADC_Prescaler_Div4;
	ADC_CommonInitStruct.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_10Cycles;
	ADC_CommonInit(&ADC_CommonInitStruct);
	//(3) ADC的初始化
	ADC_InitStruct.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStruct.ADC_ScanConvMode = DISABLE;//禁用扫描模式
	ADC_InitStruct.ADC_ContinuousConvMode = DISABLE;//禁用连续采样
	ADC_InitStruct.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//使用软件触发
	ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;//数据右对齐
	ADC_InitStruct.ADC_NbrOfConversion = 1;//转换通道数量
	ADC_Init(ADC3,&ADC_InitStruct);
	//(4) ADC通道设置、使能ADC
	ADC_RegularChannelConfig(ADC3, ADC_Channel_5, 1, ADC_SampleTime_15Cycles);
	ADC_Cmd(ADC3, ENABLE);
}


uint16_t get_lightSensor_value(int n)
{
	uint16_t tmp=0;
	for(int i=0;i<n;i++)//求平均值，进行线性滤波
	{
		ADC_SoftwareStartConv(ADC3);//软件启动转换
		while(ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC) == RESET);
		tmp += ADC_GetConversionValue(ADC3);
	}
	return tmp/n;
}
