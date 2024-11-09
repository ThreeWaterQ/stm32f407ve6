#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm32f4xx.h"
#include "misc.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "cstring"
//#include "spi_flah.h"

void LED_Config();//����LED
void LED_Open(unsigned int led_n);//��ָ��LED
void LED_Close(unsigned int led_n);//�ر�ָ��LED
void LED_3_Normal();//��������˸1��

void BEEP_Config();//���÷�����
void BEEP_Open();//�򿪷�����
void BEEP_Close();//�رշ�����
void BEEP_WORRING();

void KEY_123_Config();//����1,2,3����
void KEY_4_Config();//����4����	

void Scan_KEY(void);//�������


void Nvic_Config(void);//�ж���������
void EXTI4_Config(void);//�ⲿ�ж�4����
void EXTI_5_6_Config(void);//�ⲿ�ж�5_9����
extern unsigned int key1_flag;
extern unsigned int key2_flag;
extern unsigned int key3_flag;
extern unsigned int key4_flag;


void Systick_Init(void);
void my_delay_ms(unsigned int n);//��ʱ
void SysTick_Handler(void);

void SysTick_Init(void);
void Delay_us(uint16_t nus);
void Delay_ms(uint16_t nms);

void TIM6_Config(void);
void TIM4_Config(void);//TIM4��������
void TIM5_Configuration(void);
	
void PWM_TIM1_CH1_LED(void);
void PWM_TIM2_CH3_BEEP(void);
void LED_HUXI(u16 *led0pwmval,u8 *dir);
void BEEP_PWM(u16 *led0pwmval,u8 *dir);

void Usart1_Config(void);
void myprintf(const char * str);


void DHT11_Config_IN(void);//����
void DHT11_Config_OUT(void);//���
int Start_ask(void);
u8 Read_Bit(void);
u8 Read_Byte(void);
int DHT11_WORK(u8 *humi,u8 *temp);


#define IIC_PORT GPIOB//IIC�˿�
#define IIC_SDA_PIN GPIO_Pin_7//SDA����
#define IIC_SCL_PIN GPIO_Pin_6//SCL����

//SCLʱ����д����
#define SCL_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SCL_PIN):GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN)

//SDA������д����
#define SDA_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SDA_PIN):GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN)
void IIC_Init(void);
void SET_SDA_OUT(void);//SDA���ģʽ
void SET_SDA_IN(void);//SDA����ģʽ
void Start_signal(void);//��ʼ�ź�,��ͬ���ߵ�ƽ������SDA,������SCL
void Stop_signal(void);//ֹͣ�ź�,��ͬ���͵�ƽ������SCL,������SDA
u8 IIC_get_Ask(void);//�ȴ��ӻ�Ӧ�� 0Ӧ��1��Ӧ��
void IIC_Send_Ask(u8 ask);//��������Ӧ���ź�
void IIC_Send_Byte(u8 Data);//����һ���ֽ�
u8 IIC_Read_Byte(void);//��ȡһ���ֽ�
void IIC_AT24C02_write_data(u8 addr,u8 data);//ָ����ַдһ���ֽ�
u8 AT24C02_read_data(u8 addr);//�����
#define wendu_addr 0x00
#define shidu_addr 0x10
#define guangzhao_addr_gao 0x30
#define guangzhao_addr_di  0x50

//OLED
//�����Ļ���
#define X_WIDTH   128

//1.oledд�����
void WriteOLedCmd(uint8_t cmd);

//2 OLEDд���ݺ���
void WriteOLedData(uint8_t data);

//3 oled��㺯��
void OLed_Fill(unsigned char bmp_data);

void OLed_SetPos(unsigned char x, unsigned char y);

//5 OLED��ʼ������
void InitOLed(void);

//6.��ʾ��ĸ����
void Oled_print(uint8_t x, uint8_t y,char *str);

//7.��ʾ���ֺ���
void OLed_ShowChina(uint8_t x,uint8_t y,uint8_t *buf);


//#define W25Q_CS_PIN GPIO_Pin_2
//#define SPI_SCK1_PIN GPIO_Pin_3
//#define SPI_MISO1_PIN GPIO_Pin_4
//#define SPI_MOSI1_PIN GPIO_Pin_5

//#define W25Q_CS_Write(N) (N)?GPIO_SetBits(GPIOB,W25Q_CS_PIN):GPIO_ResetBits(GPIOB,W25Q_CS_PIN)
//void SPI1_Init(void);
//u16 W25Q128_SPI_READ_WRITE(u16 data);//���������ֽ�
//u8 W25Q128_SendByte(u8 byte);//����һ���ֽ�
//u16 W25Q128_ReadDeviceID(void);//��ȡоƬID
#define W25Q128_CS(n)  (n)?GPIO_SetBits(GPIOB,GPIO_Pin_2):GPIO_ResetBits(GPIOB,GPIO_Pin_2)
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



void PHtores_ADC_Init();
u16 get_adc_val();//���ADC��ֵ
unsigned short int getAdcAverage1(unsigned char ch, unsigned char times);


void Usart2_Config(void);
void Usart2_Send_str(char *s);
void Set_Server_Mode(void);
void Set_Cilent_Mode(void);


void fan_init();
void fan_open();
void fan_close();

void dma_Init(u32 *src,u32 *drc,u32 size);//��src��drc����size��word��С����
void dma_compare(void);


#endif 