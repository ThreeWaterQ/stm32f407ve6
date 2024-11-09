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

void SET_SDA_OUT(void)//SDA���ģʽ
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_struct.GPIO_OType = GPIO_OType_OD;
	GPIO_struct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
}

void SET_SDA_IN(void)//SDA����ģʽ
{
	//RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	GPIO_InitTypeDef GPIO_struct;
	GPIO_struct.GPIO_Mode = GPIO_Mode_IN;
	GPIO_struct.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_struct.GPIO_Pin = IIC_SDA_PIN;
	GPIO_struct.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB,&GPIO_struct);
}

void Start_signal(void)//��ʼ�ź�,��ͬ���ߵ�ƽ������SDA,������SCL
{
	SET_SDA_OUT();//���ģʽ
	
	//����SCL��SDA
	SCL_Write(1);
	SDA_Write(1);
	delay_us(5);
	//����SDA
	SDA_Write(0);
	delay_us(5);
	//����SCL
	SCL_Write(0);
}

void Stop_signal(void)//ֹͣ�ź�,��ͬ���͵�ƽ������SCL,������SDA
{
	SET_SDA_OUT();//���ģʽ
	
	//����SCL��SDA
	SCL_Write(0);
	SDA_Write(0);
	delay_us(5);
	//����SCL
	SCL_Write(1);
	delay_us(5);
	//����SDA
	SDA_Write(1);
}

u8 IIC_get_Ask(void)//�ȴ��ӻ�Ӧ�� 0Ӧ��1��Ӧ��
{
	SCL_Write(0);//����SCL,������Ч��
	
	SET_SDA_IN();//����ģʽ
	//����SCL
	SCL_Write(1);
	delay_us(5);//����
	
	if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN) == RESET)
	{
		//Ӧ��
		//����SCL
		SCL_Write(0);
		delay_us(5);
		return 0;
	}
	
	return 1;
}

void IIC_Send_Ask(u8 ask)//��������Ӧ���ź�
{
	SET_SDA_OUT();//SDA���
	SCL_Write(0);//����SCL
	delay_us(5);
	
	if(ask)//0Ӧ��1��Ӧ��
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

void IIC_Send_Byte(u8 Data)//����һ���ֽ�
{
	int i;
	SET_SDA_OUT();//SDA���,SCL��SDA���ָߵ�ƽ
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
		
		//����SCL,���ݷ���
		SCL_Write(1);
		delay_us(5);
		//����SCL�ȴ�
		SCL_Write(0);
		delay_us(5);
	}
}

u8 IIC_Read_Byte(void)//��ȡһ���ֽ�
{
	u8 Data = 0;
	int i;
	SCL_Write(0);
	SET_SDA_IN();//����ģʽ
	SCL_Write(0);//SCL���ֵ͵�ƽ
	delay_us(5);
	
	for(i = 7;i >= 0;i--)
	{
		SCL_Write(1);//���߶�����
		delay_us(2);
		if(GPIO_ReadInputDataBit(IIC_PORT,IIC_SDA_PIN) == SET)
		{
			Data |= 1<<i;
		}
		SCL_Write(0);//���ͽ���
		delay_us(2);
	}
	SCL_Write(1);
	delay_us(2);
	return Data;
}

void IIC_AT24C02_write_data(u8 addr,u8 data)//ָ����ַдһ���ֽ�
{
	Start_signal();//��ʼ�ź�
	IIC_Send_Byte(0xA0);//��������ַ
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
		return;
	}
	
	IIC_Send_Byte(addr);//д��ַ
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
		return;
	}
	
	IIC_Send_Byte(data);//д����
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
		return;
	}
	
	Stop_signal();
	
}

u8 AT24C02_read_data(u8 addr)//�����
{
	Start_signal();//��ʼ�ź�
	IIC_Send_Byte(0xA0);//��������ַ,д
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
	}
	
	IIC_Send_Byte(addr);//д��ַ
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
	}
	
	Start_signal();//��ʼ�ź�
	IIC_Send_Byte(0xA1);//��������ַ,��
	if(IIC_get_Ask())//�ȴ�Ӧ��
	{
		Stop_signal();
	}
	
	//������
	u8 data = IIC_Read_Byte();
	IIC_Send_Ask(1);//�������ͣ���Ӧ��	
	
	Stop_signal();
	return data;
}