#include "main.h"
void BEEP_Config()//配置蜂鸣器
{
	//使能时钟 蜂鸣器 挂载                  GPIOB     
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//蜂鸣器结构体配置初始化
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//引脚
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//模式：输出
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//输出类型：推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;//速率

    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);	
}

void BEEP_Open()//打开蜂鸣器
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);	
}

void BEEP_Close()//关闭蜂鸣器
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);	
}

void BEEP_WORRING()
{
	for (int i = 0; i < 3; i++)
	{
		BEEP_Open();
		delay_ms(100);
		BEEP_Close();
		delay_ms(100);
	}
}