#ifndef		__KEY_H__
#define		__KEY_H__
#include "stm32f4xx.h"

void key_init(void);
int key_get_status(int key_num);
void key_exti_init(void);
enum KEY_NUM{KEY0, KEY1, KEY2, KEY3};
enum KEY_STATUS{KEY_DOWN, KEY_UP, KEY_UNKNOW};

#endif 

