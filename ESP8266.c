#include "main.h"
void Usart2_Config(void)
{
GPIO_InitTypeDef GPIO_InitStructure;
USART_InitTypeDef USART_InitStructure;
NVIC_InitTypeDef NVIC_InitStructure;
//1 ����ʱ������
RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
//2 D��ʱ������
RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD,ENABLE);
    
//3���Ź�������
GPIO_PinAFConfig(GPIOD,GPIO_PinSource5,GPIO_AF_USART2);
GPIO_PinAFConfig(GPIOD,GPIO_PinSource6,GPIO_AF_USART2);
    
//4.GPIO��ʼ��
GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6;
GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
//������Ͳ�Ҫ����
GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;
GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
GPIO_Init(GPIOD, &GPIO_InitStructure);

//5.//���ڳ�ʼ��
USART_InitStructure.USART_BaudRate=115200;
USART_InitStructure.USART_HardwareFlowControl= USART_HardwareFlowControl_None;
USART_InitStructure.USART_Mode=USART_Mode_Rx|USART_Mode_Tx;
USART_InitStructure.USART_Parity=USART_Parity_No;
USART_InitStructure.USART_StopBits=USART_StopBits_1;
USART_InitStructure.USART_WordLength=USART_WordLength_8b;


USART_Init(USART2,&USART_InitStructure);
//6����1ʹ��
USART_Cmd(USART2,ENABLE);
//7�����ж�ʹ��
USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);

}




void Usart2_Send_str(char *s)
{	
    while(*s)
    {
		USART_SendData(USART2, *s++);
        while(USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET);
    }
}


void Set_Cilent_Mode(void)
{
	char *ESP8266_STA_Cilent[] = {
	"AT\r\n",                                // 1. ���ESP8266ģ���Ƿ�������
    "ATE1\r\n",                              // 2. 1��/0�رջ��Թ��ܣ�ʹ��ESP8266ģ�鷢�͵�ָ������������
    "AT+CWMODE=1\r\n",                       // 3. ����ESP8266ģ��ΪSTAģʽ
    "AT+RST\r\n",                            // 4. ����ESP8266ģ��
    "ATE1\r\n",                              // 5. ͬ�ڶ���ָ��һ�������Թ��ܡ�
	"AT+CWLAP\r\n",							 // 6. ��ѯ�����ӵ�wifi
    "AT+CWJAP=\"yjq_sever\",\"77777777\"\r\n",     // 7. ����ָ��WIFI
    "AT+CIPMUX=0\r\n",                       // 8. ������ģʽ
    "AT+CIPMODE=1\r\n",                      // 9. ͸��ģʽ
    "AT+CIPSTART=\"TCP\",\"192.168.137.1\",8087r\n"                           // 10. ��������
		
	};
	for(int i = 0;i < 10;i++)
	{
		printf("%d\r\n",i);
		Usart2_Send_str(ESP8266_STA_Cilent[i]);
		delay_ms(1000);
		if(i == 9)
		{
			delay_ms(2000);
		}
	}
}
