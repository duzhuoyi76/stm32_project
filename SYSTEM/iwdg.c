#include "iwdg.h"

void iwdg_init(void)//超时时间为2s
{
	//1、使能IWDG的读写操作
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable);
	//2、设置IWDG分频系数
	IWDG_SetPrescaler(IWDG_Prescaler_32);//32分频
	//3、设置IWDG的装载值
	IWDG_SetReload(2000);
	//4,使能IWDG
	IWDG_Enable();
	//5、重新加载计算值——喂狗
	IWDG_ReloadCounter();
}
