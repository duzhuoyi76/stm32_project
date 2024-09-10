#ifndef __ASRPRO_H__
#define __ASRPRO_H__
#include "stm32f4xx.h"

void asrpro_init(void);
int asrpro_get_status(void);
int asrpro_get_status_ctrl(void);
int asrpro_get_status_change(void);
#endif
