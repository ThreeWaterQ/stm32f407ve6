#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H
void SPI1_Init(void);
//通过SPI外设发送一个字节数据 同时会收到一个字节
uint8_t W25Q128_SendByte(uint8_t byte);
//读取Flash闪存的ID
uint16_t W25Q128_ReadDeviceId(void);
//使能写入
void W25Q128_WriteEnable(void);
//禁止写入
void W25Q128_WriteDisable(void);
//读取状态，Busy位位0表示空闲，1表示忙碌
uint8_t	W25Q128_ReadStatusRegister1(void);
//擦除一个扇区
void W25Q128_SectorErase(uint32_t Address);
//读取擦除之后扇区0的内容
void W25Q128_ReadData(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen);

//扇区写入  最多写入256个字节
void W25Q128_PageProgram(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen);
#endif