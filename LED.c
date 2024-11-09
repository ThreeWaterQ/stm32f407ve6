#include "main.h"

void LED_Config()//≈‰÷√
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE,ENABLE);
	GPIO_InitTypeDef gpioe_led_struct;
	gpioe_led_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpioe_led_struct.GPIO_OType = GPIO_OType_PP;
	gpioe_led_struct.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10;
	gpioe_led_struct.GPIO_Speed = GPIO_Low_Speed;
	
	GPIO_Init(GPIOE,&gpioe_led_struct);
	GPIO_SetBits(GPIOE,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10);
}

void LED_Open(unsigned int led_n)//¥Úø™
{
	GPIO_ResetBits(GPIOE,led_n);
}

void LED_Close(unsigned int led_n)//πÿ±’
{
	GPIO_SetBits(GPIOE,led_n);
}

void LED_3_Normal()//¡¡√¡¡…¡À∏1¥Œ
{
	LED_Open(GPIO_Pin_10);
	delay_ms(250);
	LED_Close(GPIO_Pin_10);
	delay_ms(250);
}
