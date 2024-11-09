#include "main.h"

void PWM_TIM1_CH1_LED(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	//1TIM1时钟配置
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
	//2E组时钟配置
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	//3重启动定时器1
	TIM_DeInit(TIM1);
	
	//4E组第9个引脚复用为定时器1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	
	//5配置E组引脚配置
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//5初始化引脚
	GPIO_Init(GPIOE,&GPIO_InitStructure);  
	
	//6高级定时器初始化
	TIM_TimeBaseStructure.TIM_Prescaler=400;  //定时器分频
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=100;   //自动重装载值
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7根据T指定的参数初始化外设TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.使能定时器
	TIM_Cmd(TIM1, ENABLE);  	
	//9控制输出模式使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_TIM2_CH3_BEEP()
{		 					 
	//此部分需手动修改IO口设置	
	GPIO_InitTypeDef GPIO_InitStructure;
	//定时器结构体初始化。
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//1定时器2时钟初始化使能；
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	
	
	////2GPIO时钟配置；B组
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	
	//3GPIOB 第10个成员 复用成定时器2；
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	//GPIOB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   
	//复用功能
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//速度100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//推挽复用输出
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//上拉
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//4初始化PB10
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
	  					
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //定时器分频  确定工作频率即是 一个数值的所占时间。单位机器周期。
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseStructure.TIM_Period=1000;  	 //自动重装载值确定整个定时时间。定时时间，大周期。1000 * 2us = 2000us = 2ms//
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //时钟分割。
	//5初始化定时器2；
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
		
	//选择定时器模式:TIM脉冲宽度调制模式1  模式 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  
	//比较输出使能 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	//输出极性:TIM输出比较极性高    极性：从0到设定值最高值之间，是输出低电平还是高电平。输出高电平。 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	//6通道3初始化； 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);   
	
	//7定时器2的通道3的比较捕获寄存器使能。 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
	//8自动装载值寄存器使能  
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	//9使能TIM2 
	TIM_Cmd(TIM2, ENABLE);  
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
}  



void fan_init()
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode=GPIO_Mode_OUT;
	gpio_struct.GPIO_Pin=GPIO_Pin_15;
	gpio_struct.GPIO_OType=GPIO_OType_PP;
	gpio_struct.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//先设置不开风扇
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}

void fan_open()
{
	GPIO_SetBits(GPIOB,GPIO_Pin_15);
}
void fan_close()
{
	GPIO_ResetBits(GPIOB,GPIO_Pin_15);
}