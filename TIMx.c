#include "main.h"



void TIM4_Config(void)//TIM4按键消抖
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	TIM_TimeBaseInitTypeDef tim4_struct;
	tim4_struct.TIM_CounterMode = TIM_CounterMode_Up;
	
	tim4_struct.TIM_Prescaler = 8400-1;
	tim4_struct.TIM_Period = 20;
	TIM_TimeBaseInit(TIM4,&tim4_struct);
	TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);
	TIM_Cmd(TIM4,ENABLE);
}

void TIM5_Configuration(void)
 {
	 TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	 NVIC_InitTypeDef NVIC_InitStructure;

	 // 使能TIM3时钟，假设TIM3挂载在APB1总线上
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	 // 定时器基本配置
	 TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // 自动重装载值，产生1kHz频率（假设系统时钟为1MHz，可根据实际调整）
	 TIM_TimeBaseStructure.TIM_Prescaler = 82 - 1; // 预分频值
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	 // 开启定时器更新中断
	 TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	 // 配置NVIC（嵌套向量中断控制器）
	 NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);

	 TIM_Cmd(TIM5, ENABLE);
 }
 
