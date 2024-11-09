#include "main.h"

void IIC_Init(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_OD;
	GPIO_struct.GPIO_Pin = IIC_SDA_PIN|IIC_SCL_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
}

void SET_SDA_OUT(void)//SDA输出模式
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_OD;
	GPIO_struct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
}

void SET_SDA_IN(void)//SDA输入模式
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_struct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
}

void Start_signal(void)//起始信号,先同步高电平，拉低SDA,再拉低SCL
{
	SET_SDA_OUT();//输出模式
	
	//拉高SCL、SDA
	SCL_Write(1);
	SDA_Write(1);
	delay_us(5);
	//拉低SDA
	SDA_Write(0);
	delay_us(5);
	//拉低SCL
	SCL_Write(0);
}

void Stop_signal(void)//停止信号,先同步低电平，拉高SCL,再拉高SDA
{
	SET_SDA_OUT();//输出模式
	
	//拉低SCL、SDA
	SCL_Write(0);
	SDA_Write(0);
	delay_us(5);
	//拉高SCL
	SCL_Write(1);
	delay_us(5);
	//拉高SDA
	SDA_Write(1);
}

u8 IIC_get_Ask(void)//等待从机应答 0应答，1不应答
{
	SCL_Write(0);//拉低SCL,数据无效化
	
	SET_SDA_IN();//输入模式
	//拉高SCL
	SCL_Write(1);
	delay_us(5);//保持
	
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN) == RESET)
	{
		//应答
		//拉低SCL
		SCL_Write(0);
		delay_us(5);
		return 0;
	}
	
	return 1;
}

void IIC_Send_Ask(u8 ask)//主机发送应答信号
{
	SET_SDA_OUT();//SDA输出
	SCL_Write(0);//拉低SCL
	delay_us(5);
	
	if(ask)//0应答，1不应答
	{
		SDA_Write(1);
	}
	else
	{
		SDA_Write(0);
	}
	
	SCL_Write(1);
	delay_ms(5);
	SCL_Write(0);
	delay_us(5);
}

void IIC_Send_Byte(u8 Data)//发送一个字节
{
	int i;
	SET_SDA_OUT();//SDA输出,SCL、SDA保持高电平
	SDA_Write(0);
	SCL_Write(0);
	for(i = 0;i < 8;i++)
	{
		if(Data & 1<<(7-i))
		{
			SDA_Write(1);
		}
		else
		{
			SDA_Write(0);
		}
		
		//拉高SCL,数据发送
		SCL_Write(1);
		delay_us(5);
		//拉低SCL等待
		SCL_Write(0);
		delay_us(5);
	}
}

u8 IIC_Read_Byte(void)//读取一个字节
{
	u8 Data = 0;
	int i;
	SCL_Write(0);
	SET_SDA_IN();//输入模式
	SCL_Write(0);//SCL保持低电平
	delay_us(5);
	
	for(i = 7;i >= 0;i--)
	{
		SCL_Write(1);//拉高读数据
		delay_us(2);
		if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN) == SET)
		{
			Data |= 1<<i;
		}
		SCL_Write(0);//拉低交换
		delay_us(2);
	}
	SCL_Write(1);
	delay_us(2);
	return Data;
}

void IIC_AT24C02_write_data(u8 addr,u8 data)//指定地址写一个字节
{
	Start_signal();//起始信号
	IIC_Send_Byte(0xA0);//从器件地址
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
		return;
	}
	
	IIC_Send_Byte(addr);//写地址
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
		return;
	}
	
	IIC_Send_Byte(data);//写数据
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
		return;
	}
	
	Stop_signal();
	
}

u8 AT24C02_read_data(u8 addr)//随机读
{
	Start_signal();//起始信号
	IIC_Send_Byte(0xA0);//从器件地址,写
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
	}
	
	IIC_Send_Byte(addr);//写地址
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
	}
	
	Start_signal();//起始信号
	IIC_Send_Byte(0xA1);//从器件地址,读
	if(IIC_get_Ask())//等待应答
	{
		Stop_signal();
	}
	
	//读数据
	u8 data = IIC_Read_Byte();
	IIC_Send_Ask(1);//主机发送，不应答	
	
	Stop_signal();
	return data;
}