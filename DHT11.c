#include "main.h"

void DHT11_Config_IN(void)//输入
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef DHT11_struct;
	DHT11_struct.GPIO_Mode = GPIO_Mode_IN;
	DHT11_struct.GPIO_Pin = GPIO_Pin_3;
	DHT11_struct.GPIO_PuPd = GPIO_PuPd_UP;
	DHT11_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&DHT11_struct);
}

void DHT11_Config_OUT(void)//输出
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE);
	GPIO_InitTypeDef DHT11_struct;
	DHT11_struct.GPIO_Mode = GPIO_Mode_OUT;
	DHT11_struct.GPIO_Pin = GPIO_Pin_3;
	DHT11_struct.GPIO_OType = GPIO_OType_PP;
	DHT11_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA,&DHT11_struct);
}

int Start_ask(void)
{

	DHT11_Config_OUT();
	GPIO_SetBits(GPIOA,GPIO_Pin_3);
	
	GPIO_ResetBits(GPIOA,GPIO_Pin_3);//拉低20ms
	delay_ms(20);
	
	GPIO_SetBits(GPIOA,GPIO_Pin_3);//拉高30us
	delay_us(30);
	
	DHT11_Config_IN();//输入模式响应
	int t1 = 1;
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)))
	{
		delay_us(1);
		t1++;
	}
	//printf("t1=%d\r\n",t1);
	int t = 0;
	while(!(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t<80)//等待变高
	{
		delay_us(1);
		t++;
	}
	if(t>= 80)
	{
		return 0;
	}
	
    t1 = 0;//等待变低
	while((GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3))&&t1<80)
	{
		delay_us(1);
		t1++; 
	}
	
	if(t1>=80)
	{	
		return 0;
	}
	return 1;
}

u8 Read_Bit(void)
{
	DHT11_Config_IN();
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET);//等待变低
	
	while(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==RESET);//等待变高
	delay_us(40);//40us后还是高电平，这1位即为1
	
	if(GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_3)==SET)
	{
		return 1;
	}
	return 0;
	
}

u8 Read_Byte(void)
{
	u8 dht11_data = 0;
	for(int i = 7;i>= 0;i--)
	{
		dht11_data|=Read_Bit()<<i;
	}
	return dht11_data;
}

u8 buf[5] = {0};

int DHT11_WORK(u8 *humi,u8 *temp)
{
	
	u8 data;
	if(Start_ask())
	{
		for(int i = 0;i<5;i++)
		{
			buf[i] = Read_Byte();
		}
		if((buf[0] + buf[1] + buf[2] + buf[3]) == buf[4])
		{
			humi[0] = buf[0];
			humi[1] = buf[1];
			temp[0] = buf[2];
			temp[1] = buf[3];
			return 1;
		}
		DHT11_Config_OUT();
	}
	return 0;
}
