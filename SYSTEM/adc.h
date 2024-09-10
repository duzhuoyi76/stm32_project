#ifndef  __ADC_H__
#define  __ADC_H__
#include "stm32f4xx.h"

void lightSensor_init(void);
uint16_t get_lightSensor_value(int n);

#endif 

