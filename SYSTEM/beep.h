#ifndef 	__BEEP_H__
#define 	__BEEP_H__
#include "stm32f4xx.h"

void beep_init(void);
void beep_ctrl(int beep_state);
enum BEEP_STATE{BEEP_OFF, BEEP_ON};
#endif 

