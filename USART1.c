#include "main.h"

void Usart1_Config(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);
	
	//管脚复用
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1);
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1);
	
	//GPIO
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode = GPIO_Mode_AF;//复用
	gpio_struct.GPIO_OType = GPIO_OType_PP;//推挽
	gpio_struct.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_10;
	gpio_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&gpio_struct);
	
	//串口配置
	USART_InitTypeDef usart1;
	usart1.USART_BaudRate = 115200;//波特率
	usart1.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件控制流：无
	usart1.USART_Mode = USART_Mode_Rx|USART_Mode_Tx;//接收、发送
	usart1.USART_Parity = USART_Parity_No;//无校验
	usart1.USART_StopBits = USART_StopBits_1;
	usart1.USART_WordLength = USART_WordLength_8b;
		
	
	USART_Init(USART1,&usart1);
	
	USART_Cmd(USART1,ENABLE);
	
	USART_ITConfig(USART1,USART_IT_RXNE,ENABLE);
}

void myprintf(const char * str)
{
	int i;
	for(i = 0;*str;i++)
	{
		USART_SendData(USART1,*str);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
		str++;
	}
}

int fputc(int ch,FILE *fp)
{
		USART_SendData(USART1,ch);
		while(USART_GetFlagStatus(USART1,USART_FLAG_TXE) == RESET);
}


