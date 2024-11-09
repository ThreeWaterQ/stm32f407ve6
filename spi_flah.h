#ifndef _SPI_FLASH_H
#define _SPI_FLASH_H
void SPI1_Init(void);
//ͨ��SPI���跢��һ���ֽ����� ͬʱ���յ�һ���ֽ�
uint8_t W25Q128_SendByte(uint8_t byte);
//��ȡFlash�����ID
uint16_t W25Q128_ReadDeviceId(void);
//ʹ��д��
void W25Q128_WriteEnable(void);
//��ֹд��
void W25Q128_WriteDisable(void);
//��ȡ״̬��Busyλλ0��ʾ���У�1��ʾæµ
uint8_t	W25Q128_ReadStatusRegister1(void);
//����һ������
void W25Q128_SectorErase(uint32_t Address);
//��ȡ����֮������0������
void W25Q128_ReadData(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen);

//����д��  ���д��256���ֽ�
void W25Q128_PageProgram(uint32_t Address,uint8_t *RecvBuf,uint32_t ReadLen);
#endif