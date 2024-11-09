#include "main.h"

void PWM_TIM1_CH1_LED(void)
{
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;


	//1TIM1ʱ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE); 
	//2E��ʱ������
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE); 
	
	//3��������ʱ��1
	TIM_DeInit(TIM1);
	
	//4E���9�����Ÿ���Ϊ��ʱ��1
	GPIO_PinAFConfig(GPIOE,GPIO_PinSource9,GPIO_AF_TIM1); 
	
	//5����E����������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;   
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//5��ʼ������
	GPIO_Init(GPIOE,&GPIO_InitStructure);  
	
	//6�߼���ʱ����ʼ��
	TIM_TimeBaseStructure.TIM_Prescaler=400;  //��ʱ����Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=100;   //�Զ���װ��ֵ
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1; 
	//TIM_TimeBaseStructure.TIM_RepetitionCounter = 1; 
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseStructure);
		
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 	        
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_OCIdleState = TIM_OCIdleState_Reset;
	//7����Tָ���Ĳ�����ʼ������TIM1 4OC1
//	TIM_OCInitStructure.TIM_Pulse = 100;
	TIM_OC1Init(TIM1, &TIM_OCInitStructure); 
	//8.ʹ�ܶ�ʱ��
	TIM_Cmd(TIM1, ENABLE);  	
	//9�������ģʽʹ��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
}

void PWM_TIM2_CH3_BEEP()
{		 					 
	//�˲������ֶ��޸�IO������	
	GPIO_InitTypeDef GPIO_InitStructure;
	//��ʱ���ṹ���ʼ����
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	//1��ʱ��2ʱ�ӳ�ʼ��ʹ�ܣ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);  	
	
	////2GPIOʱ�����ã�B��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE); 	
	
	//3GPIOB ��10����Ա ���óɶ�ʱ��2��
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_TIM2);
	
	//GPIOB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;   
	//���ù���
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;  
	//�ٶ�100MHz
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;  
	//���츴�����
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;   
	//����
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;  
	//4��ʼ��PB10
	GPIO_Init(GPIOB,&GPIO_InitStructure);              
	  					
	TIM_TimeBaseStructure.TIM_Prescaler=84-1;  //��ʱ����Ƶ  ȷ������Ƶ�ʼ��� һ����ֵ����ռʱ�䡣��λ�������ڡ�
	TIM_TimeBaseStructure.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseStructure.TIM_Period=1000;  	 //�Զ���װ��ֵȷ��������ʱʱ�䡣��ʱʱ�䣬�����ڡ�1000 * 2us = 2000us = 2ms//
	TIM_TimeBaseStructure.TIM_ClockDivision=TIM_CKD_DIV1;   //ʱ�ӷָ
	//5��ʼ����ʱ��2��
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseStructure); 
		
	//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1  ģʽ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;  
	//�Ƚ����ʹ�� 
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;  
	//�������:TIM����Ƚϼ��Ը�    ���ԣ���0���趨ֵ���ֵ֮�䣬������͵�ƽ���Ǹߵ�ƽ������ߵ�ƽ�� 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  
	//6ͨ��3��ʼ���� 
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);   
	
	//7��ʱ��2��ͨ��3�ıȽϲ���Ĵ���ʹ�ܡ� 
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);   
	//8�Զ�װ��ֵ�Ĵ���ʹ��  
	TIM_ARRPreloadConfig(TIM2,ENABLE); 
	//9ʹ��TIM2 
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
	//�����ò�������
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