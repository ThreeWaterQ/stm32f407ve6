#include "main.h"

void SPI1_Init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	GPIO_InitTypeDef gpio_struct;
	gpio_struct.GPIO_Mode = GPIO_Mode_AF;
	gpio_struct.GPIO_Pin = SPI_SCK1_PIN|SPI_MISO1_PIN|SPI_MOSI1_PIN;
	gpio_struct.GPIO_PuPd = GPIO_PuPd_NOPULL;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	//CSS引脚  W25Q_CS引脚
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_struct.GPIO_OType = GPIO_OType_PP;
	gpio_struct.GPIO_Pin = W25Q_CS_PIN;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	
	W25Q_CS_Write(1);//设置高电平-->空闲状态,拉低则主机与从机开始通讯
	//管脚复用
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	SPI_InitTypeDef spi1_struct;
	spi1_struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//全双工
	spi1_struct.SPI_Mode = SPI_Mode_Master;//主机模式
	spi1_struct.SPI_DataSize = SPI_DataSize_8b;//8bit
	//从机模式 模式3  时钟极性 1 时钟相性 1    
	spi1_struct.SPI_CPOL = SPI_CPOL_High;//SCK空闲状态为高电平
	spi1_struct.SPI_CPHA = SPI_CPHA_2Edge;//第二个跳变沿采集
	spi1_struct.SPI_NSS = SPI_NSS_Soft;//NSS设置为软件管理
	spi1_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//4分频  84MHz/4 = 21MHz
	spi1_struct.SPI_FirstBit = SPI_FirstBit_MSB;//高位在前
	SPI_Init(SPI1,&spi1_struct);
	
	SPI_Cmd(SPI1,ENABLE);
}

u16 W25Q128_SPI_READ_WRITE(u16 data)//发送两个字节
{
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);//等待数据传输完成
	SPI_SendData(SPI1,data);
	
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE)==RESET);
	
	return SPI_ReceiveData(SPI1);
}

u8 W25Q128_SendByte(u8 byte)//发送一个字节
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//等待数据传输完成
	SPI_I2S_SendData(SPI1,byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}

u16 W25Q128_ReadDeviceID(void)//获取芯片ID
{
	u16 ID = 0;
	W25Q_CS_Write(0);//拉低开始
	W25Q128_SPI_READ_WRITE(0x90);//发送指令
	
	//发送地址 24bit
	W25Q128_SPI_READ_WRITE(0x00);
	W25Q128_SPI_READ_WRITE(0x00);
	W25Q128_SPI_READ_WRITE(0x00);	
	
	//接收厂商ID和设备ID  ,主机发送任意数据
	ID = W25Q128_SPI_READ_WRITE(0x00) << 8;
	ID |= W25Q128_SPI_READ_WRITE(0x00);
	
	//拉高结束
	W25Q_CS_Write(1);
	return ID;
}