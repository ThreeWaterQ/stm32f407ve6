#include "main.h"

//中断向量配置
void Nvic_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//优先级组1，抢占1bit，响应3bit
	
	NVIC_InitTypeDef nvic_struct;
	//按键1
	nvic_struct.NVIC_IRQChannel = EXTI4_IRQn;//设置中断通道
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//使能
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&nvic_struct);
	//按键2，3
	nvic_struct.NVIC_IRQChannel = EXTI9_5_IRQn;//设置中断通道
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//使能
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&nvic_struct);
	//按键4
	nvic_struct.NVIC_IRQChannel = EXTI15_10_IRQn;//设置中断通道
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//使能
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//响应优先级
	NVIC_Init(&nvic_struct);
	
//	nvic_struct.NVIC_IRQChannel = TIM4_IRQn;//设置中断通道
//	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//使能
//	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
//	nvic_struct.NVIC_IRQChannelSubPriority = 0;//响应优先级
//	NVIC_Init(&nvic_struct);
	
	nvic_struct.NVIC_IRQChannel = TIM5_IRQn;//设置中断通道
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//使能
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
	nvic_struct.NVIC_IRQChannelSubPriority = 3;//响应优先级
	NVIC_Init(&nvic_struct);
	
	//USART2
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;//定时器6中断通道
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//使能定时器通道
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//抢占式优先级
	nvic_struct.NVIC_IRQChannelSubPriority=5;//响应式优先级
	NVIC_Init(&nvic_struct);
	
	//USART1
	nvic_struct.NVIC_IRQChannel = USART1_IRQn;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_struct);	
	
		//8中断配置
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;
	nvic_struct.NVIC_IRQChannelSubPriority=5;
	NVIC_Init(&nvic_struct);    
}

//外部中断4配置
void EXTI4_Config(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);//中断映射 PE4配置成外部中断4
	EXTI_InitTypeDef exti_4_struct;
	exti_4_struct.EXTI_Line = EXTI_Line4;//引脚4
	exti_4_struct.EXTI_LineCmd = ENABLE;//开启中断
	exti_4_struct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	exti_4_struct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti_4_struct);
}

void EXTI_5_6_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5);//重映射 PE5配置成外部中断5
	EXTI_InitTypeDef exti_5_struct;
	exti_5_struct.EXTI_Line = EXTI_Line5;//引脚5
	exti_5_struct.EXTI_LineCmd = ENABLE;//开启中断
	exti_5_struct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	exti_5_struct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti_5_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource6);//重映射 PE6配置成外部中断6
	
	EXTI_InitTypeDef exti_6_struct;
	exti_6_struct.EXTI_Line = EXTI_Line6;//引脚6
	exti_6_struct.EXTI_LineCmd = ENABLE;//开启中断
	exti_6_struct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	exti_6_struct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti_6_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);//重映射 PE7配置成外部中断7
	
	EXTI_InitTypeDef exti_13_struct;
	exti_13_struct.EXTI_Line = EXTI_Line13;//引脚7
	exti_13_struct.EXTI_LineCmd = ENABLE;//开启中断
	exti_13_struct.EXTI_Mode = EXTI_Mode_Interrupt;//中断
	exti_13_struct.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿
	EXTI_Init(&exti_13_struct);
}


unsigned int key1_flag=0;
unsigned int key2_flag=0;
unsigned int key3_flag=0;
unsigned int key4_flag=0;



