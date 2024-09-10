#include "gy39.h"
#include "uart.h"
#include "delay.h"

int WD=0,SD=0,QY=0,GZ=0,HB=0;
int flag_recvOver=0;

void gy39_init(void)
{
	usart3_init(9600);
}

void gy39_get_LightIntensity(void)
{
	uint8_t cmd[3]={0xA5,0x51,0xF6};
	
	flag_recvOver=0;
	
	while(flag_recvOver == 0)
	{
		USART_SendDatas(USART3, cmd, 3);
		delay_ms(200);
	}
}

void gy39_get_OtherValue(void)
{
	uint8_t cmd[3]={0xA5,0x52,0xF7};
	
	flag_recvOver=0;
	
	while(flag_recvOver == 0)
	{
		USART_SendDatas(USART3, cmd, 3);
		delay_ms(200);
	}
}


void USART3_IRQHandler(void)
{
	static uint8_t recvBuf[15]={0};//保存完整的数据帧的缓冲区
	static uint8_t recvCount=0;//已接收的字节个数
	if(USART_GetITStatus(USART3, USART_IT_RXNE) == SET)
	{
		recvBuf[recvCount++] = USART_ReceiveData(USART3);
		if(recvCount >= 2)
		{
			if(recvBuf[0]!=0x5A || recvBuf[1]!=0x5A)//判断帧头
			{
				recvCount=0;
			}
		}
		
		if(recvCount>=9 && recvBuf[2]==0x15)//光照强度的数据帧接收完毕
		{
			recvCount=0;
			uint8_t sum=0;
			for(int i=0;i<8;i++)
			{
				sum += recvBuf[i];
			}
			if(sum == recvBuf[8])//检查校验
			{
				GZ = recvBuf[4]<<24 | recvBuf[5]<<16 | recvBuf[6]<<8 | recvBuf[7];
				GZ /= 100;
			}
			flag_recvOver = 1;
		}
		else if(recvCount>=15 && recvBuf[2]==0x45)
		{
			recvCount=0;
			uint8_t sum=0;
			for(int i=0;i<14;i++)
			{
				sum += recvBuf[i];
			}
			if(sum == recvBuf[14])//检查校验
			{
				WD = recvBuf[4]<<8 | recvBuf[5];
				WD /= 100;
				QY = recvBuf[6]<<24 | recvBuf[7]<<16 | recvBuf[8]<<8 | recvBuf[9];
				QY /= 100;
				SD = recvBuf[10]<<8 | recvBuf[11];
				SD /= 100;
				HB = recvBuf[12]<<8 | recvBuf[13];
			}
			flag_recvOver = 1;
		}
		
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	}
}
