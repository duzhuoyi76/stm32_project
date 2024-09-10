#ifndef  __TIMER_H__
#define  __TIMER_H__
#include "stm32f4xx.h"
void tim3_update_init( uint32_t Period, uint16_t Prescaler);
void beep_pwm_init( uint32_t Period, uint16_t Prescaler, uint32_t Pulse);
void led_pwm_init( uint32_t Period, uint16_t Prescaler, uint32_t Pulse);
void light_ctrl(int a);
#endif 

