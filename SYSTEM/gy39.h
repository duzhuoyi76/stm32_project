#ifndef		__GY39_H__
#define   __GY39_H__
#include "stm32f4xx.h"
void gy39_init(void);
void gy39_get_LightIntensity(void);
void gy39_get_OtherValue(void);

extern int WD,SD,QY,GZ,HB;
#endif
