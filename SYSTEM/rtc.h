#ifndef		__RTC_H__
#define		__RTC_H__
#include "stm32f4xx.h"

void rtc_calendar_init(RTC_TimeTypeDef  *RTC_TimeStruct, RTC_DateTypeDef * RTC_DateStruct);
void rtc_alramA_init(void);
void rtc_wakeup_init(void);

#endif

