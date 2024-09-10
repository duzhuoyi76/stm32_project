#include "rtc.h"
#include "stdio.h"

void rtc_calendar_init(RTC_TimeTypeDef        *RTC_TimeStruct, RTC_DateTypeDef * RTC_DateStruct)
{
	RTC_InitTypeDef  RTC_InitStruct;
	//(1)利用后备寄存器实现只对RTC初始化一次
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);//使能电源控制器PWR时钟
	PWR_BackupAccessCmd(ENABLE);//使能后备寄存器访问
	if(RTC_ReadBackupRegister(RTC_BKP_DR1) != 0x1111)
	{
		//(2) 先尝试使用LSE作为RTC时钟源，如果失败则改为 LSI 
		__IO int16_t StartUpCounter=0x5000;
		RCC_LSEConfig(RCC_LSE_ON);//启动LSE
		while((RCC_GetFlagStatus(RCC_FLAG_LSERDY)==RESET) && (StartUpCounter--));
		if(StartUpCounter != -1)
		{
			printf("\n\rLSE success\r\n");
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);//选择LSE作为RTC的时钟源
			/*把后备寄存器中的标志位置位*/
			RTC_WriteBackupRegister(RTC_BKP_DR1, 0x1111);
		}
		else
		{
			printf("\n\rLSE error，start LSI\r\n");
			RCC_LSICmd(ENABLE);//使能LSI
			while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY)==RESET);//等待LSI稳定
			printf("\n\rLSI success!\r\n");
			RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);//选择LSI作为RTC的时钟源
		}
		RCC_RTCCLKCmd(ENABLE);//使能RTC时钟
		RTC_WaitForSynchro();//等待RTC APB寄存器同步

		//(3) 初始化RTC
		RTC_InitStruct.RTC_HourFormat = RTC_HourFormat_24;
		RTC_InitStruct.RTC_AsynchPrediv = 127;
		RTC_InitStruct.RTC_SynchPrediv = 255;
		RTC_Init(&RTC_InitStruct);
		
		//(4) 设置RTC时间和日期
		RTC_SetTime(RTC_Format_BIN, RTC_TimeStruct);
		RTC_SetDate(RTC_Format_BIN, RTC_DateStruct);
	}
	
}

void rtc_alramA_init(void)
{
	RTC_AlarmTypeDef  RTC_AlarmStruct;
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//(1) 配置RTC闹钟前，先关闭闹钟
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	//(2) RTC闹钟的日期时间设置 
	RTC_AlarmStruct.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;//按日期闹
	RTC_AlarmStruct.RTC_AlarmDateWeekDay = 15;
	RTC_AlarmStruct.RTC_AlarmMask = RTC_AlarmMask_Minutes;//屏蔽分
	RTC_AlarmStruct.RTC_AlarmTime.RTC_H12 = RTC_H12_PM;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Hours = 16;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Minutes = 50;
	RTC_AlarmStruct.RTC_AlarmTime.RTC_Seconds = 0;
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStruct);
	//(3) RTC闹钟中断配置
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);//开启闹钟A中断
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);//开启闹钟A 
	EXTI_InitStruct.EXTI_Line = EXTI_Line17;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;//上升沿触发
	EXTI_Init(&EXTI_InitStruct);

	NVIC_InitStruct.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_Init(&NVIC_InitStruct);
}

void RTC_Alarm_IRQHandler(void)
{
	if(RTC_GetFlagStatus(RTC_FLAG_ALRAF) == SET)//确认由闹钟A触发中断
	{
		RTC_ClearFlag(RTC_FLAG_ALRAF);//清除中断标志
		printf("ALARM A\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line17);//清除外部中断线17的中断标志
}

void rtc_wakeup_init(void)
{
	EXTI_InitTypeDef  EXTI_InitStruct;
	NVIC_InitTypeDef  NVIC_InitStruct;
	//(1) 配置RTC唤醒前，先关闭RTC唤醒功能
	RTC_WakeUpCmd(DISABLE);
	//(2)RTC唤醒的时钟设置
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);//保持1Hz时钟频率
	//(3)设置RTC唤醒自动重载寄存器
	RTC_SetWakeUpCounter(20);
	//(4)RTC唤醒中断配置
	RTC_ClearITPendingBit(RTC_IT_WUT);//清除RTC WAKE UP 的标志 
	EXTI_ClearITPendingBit(EXTI_Line22);//清除EXTI_Line22的中断标志位
	RTC_ITConfig(RTC_IT_WUT, ENABLE);//开启WAKE UP 定时器中断
	RTC_WakeUpCmd(ENABLE);//开启WAKE UP 定时器
	EXTI_InitStruct.EXTI_Line = EXTI_Line22;
	EXTI_InitStruct.EXTI_LineCmd = ENABLE;
	EXTI_InitStruct.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStruct.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_Init(&EXTI_InitStruct);
	NVIC_InitStruct.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 1;
	NVIC_Init(&NVIC_InitStruct);
}

void RTC_WKUP_IRQHandler(void)
{
	if(RTC_GetFlagStatus(RTC_FLAG_WUTF) == SET)//由WAKE UP产生中断
	{
		RTC_ClearFlag(RTC_FLAG_WUTF);//清除中断标志
		printf("wake up!\r\n");
	}
	EXTI_ClearITPendingBit(EXTI_Line22);//清除中断线22的中断标志位
}

