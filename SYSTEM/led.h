#ifndef	__LED_H__
#define __LED_H__
#include "stm32f4xx.h"

void led_init(void);
void led_ctrl(int led_num, int led_state);
void led_init_new(void);
enum LED_NUM{LED0, LED1, LED2, LED3,LED4};
enum LED_STATE{LED_ON, LED_OFF};

#endif 
