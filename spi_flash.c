#include "main.h"


//SPI��ʼ��
void SPI1_Init(void)
{
	//1.ʹ��ʱ��  W25Q_CS :PB2  SPI_SCK :PB3    SPI_MISO :PB4   SPI_MOSI: PB5  
	//GPIOB λ��AHB1���� SPI1λ��APB2����
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.����GPIO����
		GPIO_InitTypeDef SPIGPIOstruct;
		//��д����   //
		SPIGPIOstruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
		//��д�ٶ�  ����
		SPIGPIOstruct.GPIO_Speed = GPIO_Speed_50MHz;
		//��д����
		SPIGPIOstruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;//��������
		//���ģʽ
		SPIGPIOstruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOB,&SPIGPIOstruct); 
	
		//CS����
		SPIGPIOstruct.GPIO_Pin = GPIO_Pin_2;
		//��д�ٶ�  ����
		SPIGPIOstruct.GPIO_Speed = GPIO_Speed_50MHz;
		//��д����
		SPIGPIOstruct.GPIO_OType = GPIO_OType_PP;//
		//���ģʽ
		SPIGPIOstruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init(GPIOB,&SPIGPIOstruct); 
		
		//��Ƭѡ��������/�ͣ��������ģʽѡ��  ����Ϊ����״̬ ��ʾ��ͨ��
		W25Q128_CS(1);
		

	//3����ģ��
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4����Ϊ SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5����Ϊ SPI1
	//4.����SPIģ��
	SPI_InitTypeDef spi_struct;
	//����
	spi_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//ȫ˫��
	spi_struct.SPI_Mode=SPI_Mode_Master;//����ģʽ
	spi_struct.SPI_DataSize=SPI_DataSize_8b;//���ݴ�С��8λ����
	//���spi_falsh����Ϊģʽ3
	spi_struct.SPI_CPOL=SPI_CPOL_High;//ʱ�Ӽ��ԣ��ߵ�ƽ����
	spi_struct.SPI_CPHA=SPI_CPHA_2Edge;//�ڶ���ʱ�������زɼ�����
	spi_struct.SPI_FirstBit=SPI_FirstBit_MSB;//��λ��ǰ ����λ�ں�
	spi_struct.SPI_NSS=SPI_NSS_Soft;//���Ƭѡ ����Ӳ��Ƭѡ ��ѡ�����Ƭѡ
	//��ΪSPI1λ��APB2���� APB2����ʱ��Ϊ84MHZ ,spi���fenp
	spi_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;//Ԥ��Ƶ 84/2=21MHZ
	SPI_Init(SPI1,&spi_struct);
	//5.ʹ��SPI
	SPI_Cmd(SPI1,ENABLE);
}
//ͨ��SPI���跢��һ���ֽ����� ͬʱ���յ�һ���ֽ�
uint8_t W25Q128_SendByte(uint8_t byte)
{
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

 
  SPI_I2S_SendData(SPI1, byte);


  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);


  return SPI_I2S_ReceiveData(SPI1);
}

//�����Ҫ����W251Q128оƬ  �ͱ���ͨ���������Ͳ�ͬ��ָ�Ȼ��оƬ����ָ��������ͬ�Ĳ���
//ָ��90 Ϊ��ȡID

//��ȡFlash�����ID
uint16_t W25Q128_ReadDeviceId(void)
{
	uint16_t ID = 0;
	//1.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0); 
	
	//2.����ָ��
	W25Q128_SendByte(0x90);
	
	//3.���͵�ַ  24bit
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);

	//4.���ճ���ID���豸ID  ���������������ݼ���
	ID  = W25Q128_SendByte(0x00) << 8; 
	ID |= W25Q128_SendByte(0x00);
	
	//5.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1); 
	
	return ID;
}

//ʹ��д��
void W25Q128_WriteEnable(void)
{
	//1.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);
	
	//2.����ָ��
	W25Q128_SendByte(0x06);
	
	//3.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);
}


//��ֹд��
void W25Q128_WriteDisable(void)
{
	//1.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);
	
	//2.����ָ��
	W25Q128_SendByte(0x04);
	
	//3.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);
}

//��ȡ״̬��Busyλλ0��ʾ���У�1��ʾæµ
uint8_t	W25Q128_ReadStatusRegister1(void)
{
	uint8_t status = 0;
	//1.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);

	//2.����ָ��
	W25Q128_SendByte(0x05);
	
	//3.������������  Ŀ���ǽ���һ���ֽڵķ���ֵ
	status = W25Q128_SendByte(0xFF);

	//4.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);

	return status;
}

//����һ������
void W25Q128_SectorErase(uint32_t Address)
{
	//1.����дʹ��
	W25Q128_WriteEnable();
	delay_ms(1);  	
	
	//2.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);
	
	//3.����ָ��
	W25Q128_SendByte(0x20);

	//4.���͵�ַ   �����Ǹ�λ�ȳ���MSB��
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  

	//5.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);
	
	//6.�ȴ��������  BUSYλ = 1 ��ʾ���ڹ���  BUSYλ=0 ��ʾ��������
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//7.�ر�дʹ��
	W25Q128_WriteDisable();
}

//��ȡ����֮������0������
void W25Q128_ReadData(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen)
{
	//1.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);

	//2.����ָ��
	W25Q128_SendByte(0x03);
	
	//3.���͵�ַ
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  
	
	//4.��������
	while(ReadLen--)
	{
		*RecvBuf++ = W25Q128_SendByte(0xFF);
	}
	
	//5.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);
}

//����д��  ���д��256���ֽ�
void W25Q128_PageProgram(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen)
{
	//1.����дʹ��
	W25Q128_WriteEnable();
	delay_ms(1);  
	
	//2.��CSƬѡ��������  ��ʾѡ��
	W25Q128_CS(0);
	
	//3.����ָ��
	W25Q128_SendByte(0x02);
	
	//4.���͵�ַ   �����Ǹ�λ�ȳ���MSB��
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  
	
	//5.��������
	while(ReadLen--)
		{
		W25Q128_SendByte(*RecvBuf++);
	}
	
	//6.��CSƬѡ��������  ��ʾ��ͨ��
	W25Q128_CS(1);
	
	//7.�ȴ�д�����  BUSYλ = 1 ��ʾæµ  BUSYλ=0 ��ʾ����
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//8.�ر�дʹ��
	W25Q128_WriteDisable();
}
