#include "main.h"
void BEEP_Config()//���÷�����
{
	//ʹ��ʱ�� ������ ����                  GPIOB     
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
	//�������ṹ�����ó�ʼ��
	GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;//����
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;//ģʽ�����
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//������ͣ��������
    GPIO_InitStructure.GPIO_Speed = GPIO_Low_Speed;//����

    GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_RESET);	
}

void BEEP_Open()//�򿪷�����
{
	GPIO_WriteBit(GPIOB,GPIO_Pin_10,Bit_SET);	
}

void BEEP_Close()//�رշ�����
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