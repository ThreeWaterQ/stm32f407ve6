#include "main.h"



void TIM4_Config(void)//TIM4��������
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

	 // ʹ��TIM3ʱ�ӣ�����TIM3������APB1������
	 RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);

	 // ��ʱ����������
	 TIM_TimeBaseStructure.TIM_Period = 1000 - 1; // �Զ���װ��ֵ������1kHzƵ�ʣ�����ϵͳʱ��Ϊ1MHz���ɸ���ʵ�ʵ�����
	 TIM_TimeBaseStructure.TIM_Prescaler = 82 - 1; // Ԥ��Ƶֵ
	 TIM_TimeBaseStructure.TIM_ClockDivision = 0;
	 TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	 TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);

	 // ������ʱ�������ж�
	 TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);

	 // ����NVIC��Ƕ�������жϿ�������
	 NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
	 NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	 NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	 NVIC_Init(&NVIC_InitStructure);

	 TIM_Cmd(TIM5, ENABLE);
 }
 
