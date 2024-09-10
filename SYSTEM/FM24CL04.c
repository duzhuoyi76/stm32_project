#include "FM24CL04.h"
#include "delay.h"

void IIC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;  
	I2C_InitTypeDef I2C_InitStructure; 		

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_I2C1,ENABLE);

	GPIO_PinAFConfig(GPIOB,GPIO_PinSource8,GPIO_AF_I2C1);  
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource9,GPIO_AF_I2C1);

	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_8 | GPIO_Pin_9;   
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;           
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;   
	GPIO_InitStructure.GPIO_PuPd =GPIO_PuPd_NOPULL; 
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	I2C_DeInit(I2C1); 	//还原默认设置						
	I2C_InitStructure.I2C_ClockSpeed=100000;  		//标准模式 时钟频率为100KHZ
	I2C_InitStructure.I2C_Mode=I2C_Mode_I2C;   		//选中I2C模式
	I2C_InitStructure.I2C_DutyCycle=I2C_DutyCycle_2;	
	I2C_InitStructure.I2C_OwnAddress1=0X00;      //当I2C出于从模式时,自身的地址
	I2C_InitStructure.I2C_Ack=I2C_Ack_Enable;
	I2C_InitStructure.I2C_AcknowledgedAddress=I2C_AcknowledgedAddress_7bit;
	I2C_Init(I2C1, &I2C_InitStructure);	//

	I2C_AcknowledgeConfig(I2C1,ENABLE);   //在接收到一个字节后返回一个应答ACK
	I2C_Cmd(I2C1,ENABLE);  //开启外设IIC模块
}


/*
* 函数名：IIC_AT24C02_BufferRead
* 描述  ：从EEPROM里面读取一块数据。 
* 输入  ：-pBuffer 存放从EEPROM读取的数据的缓冲区指针。
*         -ReadAddr 接收数据的EEPROM的地址。 
*         -NumByte 要从EEPROM读取的字节数。
* 输出  ：无
* 返回  ：无
* 调用  ：外部调用
*/
void IIC_AT24C02_BufferRead(uint8_t *pBuffer,uint8_t ReadAddr,uint8_t NumByte)
{
	uint16_t  flag1,flag2;

	//判断BUSY是否为1,BUSY=1总线处于忙状态
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));  //如果BUSY=1则等待一直等到BUSY=0

	//主机产生起始条件
	I2C_GenerateSTART(I2C1,ENABLE);  

	//判断SB是否为1,SB=1起始条件已经发送 即判断EV5
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);  //当产生了起始条件,设备就进入了主模式
	
	//发送eepROM的地址0XA0
	I2C_Send7bitAddress(I2C1,0XA0,I2C_Direction_Transmitter); //软件读取SR1寄存器后,写数据寄存器的操作将清除SB位
	
	//判断ADDR是否为1,ADDR=1地址发送结束 即判断EV6
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
	flag1=I2C1->SR2; //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位
	(void)flag1;

	//发送想要读取字节的地址
	I2C_SendData(I2C1,ReadAddr); 
	
	//判断TXE是否为1,TXE=1移位寄存器不为空,数据寄存器为空 即判断EV8
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);

	//主机重新发送起始信号
	I2C_GenerateSTART(I2C1,ENABLE);  
	
	//判断SB是否为1,SB=1起始条件已经发送 即判断EV5
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);  //当产生了起始条件,设备就进入了主模式
	
	//主机重新发送AT24C02的地址,这是R/W位置1
  I2C_Send7bitAddress(I2C1,0XA0,I2C_Direction_Receiver); 

	//判断ADDR是否为1,ADDR=1地址发送结束 即判断EV6
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)==ERROR);
		flag2=I2C1->SR2; //软件读取SR1寄存器后,对SR2寄存器的读操作将清除ADDR位
	(void)flag2;
	//接收AT24C02发送的数据
	while(NumByte)
	{
		//检查RXNE是否为1,即判断EV7
		if(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_RECEIVED)==SUCCESS)
		{
			/* Read a byte from the EEPROM */
			*pBuffer = I2C_ReceiveData(I2C1);  //软件读取数据寄存器的值,讲清除RXNE位
   
			/* Point to the next location where the byte read will be saved */
			pBuffer++; 
   
			/* Decrement the read bytes counter */
			NumByte--;   
			
			if(NumByte==0)   //等于1时数据已经发送完成
			{
				//主机不再发送响应信号
				I2C_AcknowledgeConfig(I2C1,DISABLE);
				//主机产生停止信号
				I2C_GenerateSTOP(I2C1,ENABLE);
			}
			else
			{
				I2C_AcknowledgeConfig(I2C1, ENABLE); //打开应答信号使其回到初始状态
			}
		}
		
	}
	
	I2C_AcknowledgeConfig(I2C1, ENABLE); //打开应答信号使其回到初始状态
} 	

void IIC_Write_AT24C02_OneByte(uint8_t addr,uint8_t data)
{
	uint16_t flag;
	
	while(I2C_GetFlagStatus(I2C1,I2C_FLAG_BUSY));  
	 
	I2C_GenerateSTART(I2C1,ENABLE);  
	
	
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_MODE_SELECT)==ERROR);		


	I2C_Send7bitAddress(I2C1,0XA0,I2C_Direction_Transmitter); 
	
	//EV6
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)==ERROR);
 	 flag=I2C1->SR2;			
	(void)flag;
	I2C_SendData(I2C1,addr);
		
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTING)==ERROR);

	I2C_SendData(I2C1,data);		
	
	while(I2C_CheckEvent(I2C1,I2C_EVENT_MASTER_BYTE_TRANSMITTED)==ERROR);

	I2C_GenerateSTOP(I2C1,ENABLE);
}

/*
* 函数名：IIC_AT24C02_BufferWrite
* 描述  ：将缓冲区中的数据写到I2C EEPROM中
* 输入  ：-pBuffer 缓冲区指针
*         -WriteAddr 接收数据的EEPROM的地址
*         -NumByteToWrite 要写入EEPROM的字节数
* 输出  ：无
* 返回  ：无
*/

void IIC_AT24C02_BufferWrite(uint8_t *pBuffer,uint8_t WriteAddr,uint8_t NumByteToWrite )
{
	uint8_t i;
	for(i=0;i<NumByteToWrite;i++)
	{
		IIC_Write_AT24C02_OneByte(WriteAddr+i,pBuffer[i]);
		delay_ms(10);
	}
}

