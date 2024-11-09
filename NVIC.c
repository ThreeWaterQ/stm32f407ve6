#include "main.h"

//�ж���������
void Nvic_Config(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//���ȼ���1����ռ1bit����Ӧ3bit
	
	NVIC_InitTypeDef nvic_struct;
	//����1
	nvic_struct.NVIC_IRQChannel = EXTI4_IRQn;//�����ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�
	NVIC_Init(&nvic_struct);
	//����2��3
	nvic_struct.NVIC_IRQChannel = EXTI9_5_IRQn;//�����ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�
	NVIC_Init(&nvic_struct);
	//����4
	nvic_struct.NVIC_IRQChannel = EXTI15_10_IRQn;//�����ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 2;//��ռ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority = 2;//��Ӧ���ȼ�
	NVIC_Init(&nvic_struct);
	
//	nvic_struct.NVIC_IRQChannel = TIM4_IRQn;//�����ж�ͨ��
//	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
//	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;//��ռ���ȼ�
//	nvic_struct.NVIC_IRQChannelSubPriority = 0;//��Ӧ���ȼ�
//	NVIC_Init(&nvic_struct);
	
	nvic_struct.NVIC_IRQChannel = TIM5_IRQn;//�����ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;//ʹ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 3;//��ռ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority = 3;//��Ӧ���ȼ�
	NVIC_Init(&nvic_struct);
	
	//USART2
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;//��ʱ��6�ж�ͨ��
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;//ʹ�ܶ�ʱ��ͨ��
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;//��ռʽ���ȼ�
	nvic_struct.NVIC_IRQChannelSubPriority=5;//��Ӧʽ���ȼ�
	NVIC_Init(&nvic_struct);
	
	//USART1
	nvic_struct.NVIC_IRQChannel = USART1_IRQn;
	nvic_struct.NVIC_IRQChannelCmd = ENABLE;
	nvic_struct.NVIC_IRQChannelPreemptionPriority = 0;
	nvic_struct.NVIC_IRQChannelSubPriority = 0;
	NVIC_Init(&nvic_struct);	
	
		//8�ж�����
	nvic_struct.NVIC_IRQChannel=USART2_IRQn;
	nvic_struct.NVIC_IRQChannelCmd=ENABLE;
	nvic_struct.NVIC_IRQChannelPreemptionPriority=0;
	nvic_struct.NVIC_IRQChannelSubPriority=5;
	NVIC_Init(&nvic_struct);    
}

//�ⲿ�ж�4����
void EXTI4_Config(void)
{
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource4);//�ж�ӳ�� PE4���ó��ⲿ�ж�4
	EXTI_InitTypeDef exti_4_struct;
	exti_4_struct.EXTI_Line = EXTI_Line4;//����4
	exti_4_struct.EXTI_LineCmd = ENABLE;//�����ж�
	exti_4_struct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	exti_4_struct.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti_4_struct);
}

void EXTI_5_6_Config(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG,ENABLE);
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource5);//��ӳ�� PE5���ó��ⲿ�ж�5
	EXTI_InitTypeDef exti_5_struct;
	exti_5_struct.EXTI_Line = EXTI_Line5;//����5
	exti_5_struct.EXTI_LineCmd = ENABLE;//�����ж�
	exti_5_struct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	exti_5_struct.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti_5_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource6);//��ӳ�� PE6���ó��ⲿ�ж�6
	
	EXTI_InitTypeDef exti_6_struct;
	exti_6_struct.EXTI_Line = EXTI_Line6;//����6
	exti_6_struct.EXTI_LineCmd = ENABLE;//�����ж�
	exti_6_struct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	exti_6_struct.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti_6_struct);
	
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOC,EXTI_PinSource13);//��ӳ�� PE7���ó��ⲿ�ж�7
	
	EXTI_InitTypeDef exti_13_struct;
	exti_13_struct.EXTI_Line = EXTI_Line13;//����7
	exti_13_struct.EXTI_LineCmd = ENABLE;//�����ж�
	exti_13_struct.EXTI_Mode = EXTI_Mode_Interrupt;//�ж�
	exti_13_struct.EXTI_Trigger = EXTI_Trigger_Falling;//�½���
	EXTI_Init(&exti_13_struct);
}


unsigned int key1_flag=0;
unsigned int key2_flag=0;
unsigned int key3_flag=0;
unsigned int key4_flag=0;



