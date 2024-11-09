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
	//CSS����  W25Q_CS����
	gpio_struct.GPIO_Mode = GPIO_Mode_OUT;
	gpio_struct.GPIO_OType = GPIO_OType_PP;
	gpio_struct.GPIO_Pin = W25Q_CS_PIN;
	gpio_struct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&gpio_struct);
	
	W25Q_CS_Write(1);//���øߵ�ƽ-->����״̬,������������ӻ���ʼͨѶ
	//�ܽŸ���
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1);
	
	SPI_InitTypeDef spi1_struct;
	spi1_struct.SPI_Direction = SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	spi1_struct.SPI_Mode = SPI_Mode_Master;//����ģʽ
	spi1_struct.SPI_DataSize = SPI_DataSize_8b;//8bit
	//�ӻ�ģʽ ģʽ3  ʱ�Ӽ��� 1 ʱ������ 1    
	spi1_struct.SPI_CPOL = SPI_CPOL_High;//SCK����״̬Ϊ�ߵ�ƽ
	spi1_struct.SPI_CPHA = SPI_CPHA_2Edge;//�ڶ��������زɼ�
	spi1_struct.SPI_NSS = SPI_NSS_Soft;//NSS����Ϊ�������
	spi1_struct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;//4��Ƶ  84MHz/4 = 21MHz
	spi1_struct.SPI_FirstBit = SPI_FirstBit_MSB;//��λ��ǰ
	SPI_Init(SPI1,&spi1_struct);
	
	SPI_Cmd(SPI1,ENABLE);
}

u16 W25Q128_SPI_READ_WRITE(u16 data)//���������ֽ�
{
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_TXE)==RESET);//�ȴ����ݴ������
	SPI_SendData(SPI1,data);
	
	while(SPI_GetFlagStatus(SPI1,SPI_FLAG_RXNE)==RESET);
	
	return SPI_ReceiveData(SPI1);
}

u8 W25Q128_SendByte(u8 byte)//����һ���ֽ�
{
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_TXE)==RESET);//�ȴ����ݴ������
	SPI_I2S_SendData(SPI1,byte);
	
	while(SPI_I2S_GetFlagStatus(SPI1,SPI_I2S_FLAG_RXNE)==RESET);
	
	return SPI_I2S_ReceiveData(SPI1);
}

u16 W25Q128_ReadDeviceID(void)//��ȡоƬID
{
	u16 ID = 0;
	W25Q_CS_Write(0);//���Ϳ�ʼ
	W25Q128_SPI_READ_WRITE(0x90);//����ָ��
	
	//���͵�ַ 24bit
	W25Q128_SPI_READ_WRITE(0x00);
	W25Q128_SPI_READ_WRITE(0x00);
	W25Q128_SPI_READ_WRITE(0x00);	
	
	//���ճ���ID���豸ID  ,����������������
	ID = W25Q128_SPI_READ_WRITE(0x00) << 8;
	ID |= W25Q128_SPI_READ_WRITE(0x00);
	
	//���߽���
	W25Q_CS_Write(1);
	return ID;
}