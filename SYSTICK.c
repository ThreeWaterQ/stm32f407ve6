#include "main.h"

unsigned int Timedalay = 0;
void Systick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK);
	if(SysTick_Config(SystemCoreClock/1000))
	{
		LED_Open(GPIO_Pin_8);
	}
}

void my_delay_ms(unsigned int n)//��ʱ
{
	Timedalay = n;
	while(Timedalay!=0);
}

void SysTick_Handler(void)
{
  //TimingDelay_Decrement();
	if(Timedalay!=0)
		Timedalay--;
} 

void SysTick_Init(void)
{
	SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);
}

void Delay_us(uint16_t nus)
{
	SysTick->CTRL &= ~(1<<0);//�رն�ʱ��
	SysTick->LOAD = 21*nus -1;//������װ��ֵ value-1
	SysTick->VAL = 0;//��յ�ǰ����ֵ
	SysTick->CTRL |= (1<<0);//������ʱ��
	while((SysTick->CTRL & 0x10000)==0);//�ȴ��������
	SysTick->CTRL = 0;//�رն�ʱ��
	SysTick->VAL = 0;//��յ�ǰ����ֵ
}

void Delay_ms(uint16_t nms)
{
	SysTick->CTRL &= ~(1<<0);//�رն�ʱ��
	SysTick->LOAD = 21*1000*nms -1;//������װ��ֵ value-1
	SysTick->VAL = 0;//��յ�ǰ����ֵ
	SysTick->CTRL |= (1<<0);//������ʱ��
	while((SysTick->CTRL & 0x10000)==0);//�ȴ��������
	SysTick->CTRL = 0;//�رն�ʱ��
	SysTick->VAL = 0;//��յ�ǰ����ֵ
}