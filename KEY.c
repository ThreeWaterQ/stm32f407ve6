#include "main.h"
void KEY_123_Config()//∞¥º¸1,2,3≈‰÷√
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitTypeDef gpio_KEY_123_struct;
	gpio_KEY_123_struct.GPIO_Mode = GPIO_Mode_IN;
	gpio_KEY_123_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_KEY_123_struct.GPIO_Pin = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	gpio_KEY_123_struct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOE,&gpio_KEY_123_struct);
}

void KEY_4_Config()//∞¥º¸4≈‰÷√
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC,ENABLE);
	GPIO_InitTypeDef gpio_KEY_4_struct;
	gpio_KEY_4_struct.GPIO_Mode = GPIO_Mode_IN;
	gpio_KEY_4_struct.GPIO_PuPd = GPIO_PuPd_UP;
	gpio_KEY_4_struct.GPIO_Pin = GPIO_Pin_13;
	gpio_KEY_4_struct.GPIO_Speed = GPIO_Low_Speed;
	GPIO_Init(GPIOC,&gpio_KEY_4_struct);
}
