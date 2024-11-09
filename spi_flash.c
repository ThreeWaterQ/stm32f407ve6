#include "main.h"


//SPI初始化
void SPI1_Init(void)
{
	//1.使能时钟  W25Q_CS :PB2  SPI_SCK :PB3    SPI_MISO :PB4   SPI_MOSI: PB5  
	//GPIOB 位于AHB1总线 SPI1位于APB2总线
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,ENABLE);
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	//2.配置GPIO引脚
		GPIO_InitTypeDef SPIGPIOstruct;
		//填写引脚   //
		SPIGPIOstruct.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5 ;
		//填写速度  高速
		SPIGPIOstruct.GPIO_Speed = GPIO_Speed_50MHz;
		//填写电阻
		SPIGPIOstruct.GPIO_PuPd = GPIO_PuPd_NOPULL ;//无上下拉
		//输出模式
		SPIGPIOstruct.GPIO_Mode = GPIO_Mode_AF;
		GPIO_Init(GPIOB,&SPIGPIOstruct); 
	
		//CS引脚
		SPIGPIOstruct.GPIO_Pin = GPIO_Pin_2;
		//填写速度  高速
		SPIGPIOstruct.GPIO_Speed = GPIO_Speed_50MHz;
		//填写电阻
		SPIGPIOstruct.GPIO_OType = GPIO_OType_PP;//
		//输出模式
		SPIGPIOstruct.GPIO_Mode = GPIO_Mode_OUT;
		GPIO_Init(GPIOB,&SPIGPIOstruct); 
		
		//将片选引脚拉高/低（具体根据模式选择）  设置为空闲状态 表示不通信
		W25Q128_CS(1);
		

	//3复用模块
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource3,GPIO_AF_SPI1); //PB3复用为 SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource4,GPIO_AF_SPI1); //PB4复用为 SPI1
		GPIO_PinAFConfig(GPIOB,GPIO_PinSource5,GPIO_AF_SPI1); //PB5复用为 SPI1
	//4.配置SPI模块
	SPI_InitTypeDef spi_struct;
	//方向
	spi_struct.SPI_Direction=SPI_Direction_2Lines_FullDuplex;//全双工
	spi_struct.SPI_Mode=SPI_Mode_Master;//主机模式
	spi_struct.SPI_DataSize=SPI_DataSize_8b;//数据大小：8位数据
	//如果spi_falsh配置为模式3
	spi_struct.SPI_CPOL=SPI_CPOL_High;//时钟极性：高电平空闲
	spi_struct.SPI_CPHA=SPI_CPHA_2Edge;//第二个时钟跳变沿采集数据
	spi_struct.SPI_FirstBit=SPI_FirstBit_MSB;//高位在前 ，低位在后
	spi_struct.SPI_NSS=SPI_NSS_Soft;//软件片选 还是硬件片选 ：选择软件片选
	//因为SPI1位于APB2总线 APB2总线时钟为84MHZ ,spi最高fenp
	spi_struct.SPI_BaudRatePrescaler=SPI_BaudRatePrescaler_4;//预分频 84/2=21MHZ
	SPI_Init(SPI1,&spi_struct);
	//5.使能SPI
	SPI_Cmd(SPI1,ENABLE);
}
//通过SPI外设发送一个字节数据 同时会收到一个字节
uint8_t W25Q128_SendByte(uint8_t byte)
{
 
  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_TXE) == RESET);

 
  SPI_I2S_SendData(SPI1, byte);


  while (SPI_I2S_GetFlagStatus(SPI1, SPI_I2S_FLAG_RXNE) == RESET);


  return SPI_I2S_ReceiveData(SPI1);
}

//如果想要控制W251Q128芯片  就必须通过主机发送不同的指令，然后芯片根据指令来做不同的操作
//指令90 为读取ID

//读取Flash闪存的ID
uint16_t W25Q128_ReadDeviceId(void)
{
	uint16_t ID = 0;
	//1.把CS片选引脚拉低  表示选中
	W25Q128_CS(0); 
	
	//2.发送指令
	W25Q128_SendByte(0x90);
	
	//3.发送地址  24bit
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);
	W25Q128_SendByte(0x00);

	//4.接收厂商ID和设备ID  主机发送任意数据即可
	ID  = W25Q128_SendByte(0x00) << 8; 
	ID |= W25Q128_SendByte(0x00);
	
	//5.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1); 
	
	return ID;
}

//使能写入
void W25Q128_WriteEnable(void)
{
	//1.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);
	
	//2.发送指令
	W25Q128_SendByte(0x06);
	
	//3.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);
}


//禁止写入
void W25Q128_WriteDisable(void)
{
	//1.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);
	
	//2.发送指令
	W25Q128_SendByte(0x04);
	
	//3.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);
}

//读取状态，Busy位位0表示空闲，1表示忙碌
uint8_t	W25Q128_ReadStatusRegister1(void)
{
	uint8_t status = 0;
	//1.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);

	//2.发送指令
	W25Q128_SendByte(0x05);
	
	//3.发送任意数据  目的是接收一个字节的返回值
	status = W25Q128_SendByte(0xFF);

	//4.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);

	return status;
}

//擦除一个扇区
void W25Q128_SectorErase(uint32_t Address)
{
	//1.开启写使能
	W25Q128_WriteEnable();
	delay_ms(1);  	
	
	//2.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);
	
	//3.发送指令
	W25Q128_SendByte(0x20);

	//4.发送地址   数据是高位先出（MSB）
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  

	//5.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);
	
	//6.等待擦除完成  BUSY位 = 1 表示正在工作  BUSY位=0 表示工作结束
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//7.关闭写使能
	W25Q128_WriteDisable();
}

//读取擦除之后扇区0的内容
void W25Q128_ReadData(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen)
{
	//1.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);

	//2.发送指令
	W25Q128_SendByte(0x03);
	
	//3.发送地址
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  
	
	//4.接收数据
	while(ReadLen--)
	{
		*RecvBuf++ = W25Q128_SendByte(0xFF);
	}
	
	//5.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);
}

//扇区写入  最多写入256个字节
void W25Q128_PageProgram(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen)
{
	//1.开启写使能
	W25Q128_WriteEnable();
	delay_ms(1);  
	
	//2.把CS片选引脚拉低  表示选中
	W25Q128_CS(0);
	
	//3.发送指令
	W25Q128_SendByte(0x02);
	
	//4.发送地址   数据是高位先出（MSB）
	W25Q128_SendByte( (Address & 0xFF0000)>>16 );
	W25Q128_SendByte( (Address & 0xFF00)>>8 ); 
	W25Q128_SendByte( Address & 0xFF );  
	
	//5.发送数据
	while(ReadLen--)
		{
		W25Q128_SendByte(*RecvBuf++);
	}
	
	//6.把CS片选引脚拉高  表示不通信
	W25Q128_CS(1);
	
	//7.等待写入完成  BUSY位 = 1 表示忙碌  BUSY位=0 表示空闲
	while( W25Q128_ReadStatusRegister1() & 0x01 );
	
	//8.关闭写使能
	W25Q128_WriteDisable();
}
