#include "can.h"
#include "stdio.h"
//CAN1_RX ==> PD0
//CAN1_TX ==> PD1
void can1_init(uint32_t id, uint8_t IDE, uint8_t RTR)
{
	GPIO_InitTypeDef  GPIO_InitStruct;
	CAN_InitTypeDef  CAN_InitStruct;
	CAN_FilterInitTypeDef  CAN_FilterInitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;

	uint32_t Filter_id = id<<3 ;
	Filter_id |= IDE<<2 | RTR<<1; //列表模式的ID由id和IDE,RTR,0组成
	
	//(1)配置 CAN 总线的引脚
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD,  &GPIO_InitStruct);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource0, GPIO_AF_CAN1);
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource1, GPIO_AF_CAN1);

	//(2)初始化CAN控制器
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
	CAN_InitStruct.CAN_Prescaler = 6;
	CAN_InitStruct.CAN_Mode = CAN_Mode_Normal;//正常模式
	CAN_InitStruct.CAN_SJW = CAN_SJW_1tq;//同步时长
	CAN_InitStruct.CAN_BS1 = CAN_BS1_8tq;
	CAN_InitStruct.CAN_BS2 = CAN_BS2_6tq;
	CAN_InitStruct.CAN_TTCM = DISABLE;//禁用时间戳
	CAN_InitStruct.CAN_ABOM = DISABLE;//禁用自动离线模式
	CAN_InitStruct.CAN_AWUM = DISABLE;//禁用自动唤醒管理
	CAN_InitStruct.CAN_NART = ENABLE;//不自动重发
	CAN_InitStruct.CAN_RFLM = DISABLE;//接收缓冲区不上锁
	CAN_InitStruct.CAN_TXFP = ENABLE;//发送换冲区优先级
	CAN_Init(CAN1, &CAN_InitStruct);

	//(3)配置筛选器(过滤器)
	CAN_FilterInitStruct.CAN_FilterNumber = 0;//使用过滤器0
	CAN_FilterInitStruct.CAN_FilterActivation = ENABLE;
	CAN_FilterInitStruct.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStruct.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;
	CAN_FilterInitStruct.CAN_FilterMode = CAN_FilterMode_IdList;//列表模式
	CAN_FilterInitStruct.CAN_FilterIdHigh = (Filter_id&0xFFFF0000)>>16;
	CAN_FilterInitStruct.CAN_FilterIdLow = Filter_id&0xFFFF;
	
	CAN_FilterInit(&CAN_FilterInitStruct);

	//(4)中断设置
	CAN_ITConfig  ( CAN1, CAN_IT_FMP0,  ENABLE);//使能FIFO0来消息触发中断
	NVIC_InitStruct.NVIC_IRQChannel = CAN1_RX0_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}



void CAN1_RX0_IRQHandler(void)
{
	CanRxMsg   RxMessage;
	CAN_Receive  ( CAN1, 0,  &RxMessage ); 
	printf("recv:%s\r\n", RxMessage.Data);
}

