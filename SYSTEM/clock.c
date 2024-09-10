#include <stdio.h>
#include "clock.h"
#include "uart.h"
#include "OLED.h"

int h,m,s;
void update_Clock()
{
    h=17;
    m=0;
    s=0;
}

void Clock()
{
	update_Clock();
	for(; h<24; h++)
	{
			for(; m<60; m++)
			{
					for(; s<60; s++)
					{
						printf("%d:%d:%d\n",h,m,s);
						OLED_ShowNum(2, 1,h, 2);
						OLED_ShowString(2, 3, ":");
						OLED_ShowNum(2, 4,m, 2);
						OLED_ShowString(2, 6, ":");
						OLED_ShowNum(2, 7,s, 2);
						OLED_ShowString(2, 9, ":");
					}
					s=0;
			}
			m=0;
	}
	h=0;
}
