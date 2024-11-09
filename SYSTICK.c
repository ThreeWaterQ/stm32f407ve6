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

void my_delay_ms(unsigned int n)//延时
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
	SysTick->CTRL &= ~(1<<0);//关闭定时器
	SysTick->LOAD = 21*nus -1;//设置重装载值 value-1
	SysTick->VAL = 0;//清空当前计数值
	SysTick->CTRL |= (1<<0);//开启定时器
	while((SysTick->CTRL & 0x10000)==0);//等待倒数完成
	SysTick->CTRL = 0;//关闭定时器
	SysTick->VAL = 0;//清空当前计数值
}

void Delay_ms(uint16_t nms)
{
	SysTick->CTRL &= ~(1<<0);//关闭定时器
	SysTick->LOAD = 21*1000*nms -1;//设置重装载值 value-1
	SysTick->VAL = 0;//清空当前计数值
	SysTick->CTRL |= (1<<0);//开启定时器
	while((SysTick->CTRL & 0x10000)==0);//等待倒数完成
	SysTick->CTRL = 0;//关闭定时器
	SysTick->VAL = 0;//清空当前计数值
}