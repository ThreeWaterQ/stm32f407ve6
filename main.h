#ifndef _MAIN_H_
#define _MAIN_H_
#include "stm32f4xx.h"
#include "misc.h"
#include "stdio.h"
#include "delay.h"
#include "stdlib.h"
#include "cstring"
//#include "spi_flah.h"

void LED_Config();//配置LED
void LED_Open(unsigned int led_n);//打开指定LED
void LED_Close(unsigned int led_n);//关闭指定LED
void LED_3_Normal();//亮灭亮闪烁1次

void BEEP_Config();//配置蜂鸣器
void BEEP_Open();//打开蜂鸣器
void BEEP_Close();//关闭蜂鸣器
void BEEP_WORRING();

void KEY_123_Config();//按键1,2,3配置
void KEY_4_Config();//按键4配置	

void Scan_KEY(void);//按键检测


void Nvic_Config(void);//中断向量配置
void EXTI4_Config(void);//外部中断4配置
void EXTI_5_6_Config(void);//外部中断5_9配置
extern unsigned int key1_flag;
extern unsigned int key2_flag;
extern unsigned int key3_flag;
extern unsigned int key4_flag;


void Systick_Init(void);
void my_delay_ms(unsigned int n);//延时
void SysTick_Handler(void);

void SysTick_Init(void);
void Delay_us(uint16_t nus);
void Delay_ms(uint16_t nms);

void TIM6_Config(void);
void TIM4_Config(void);//TIM4按键消抖
void TIM5_Configuration(void);
	
void PWM_TIM1_CH1_LED(void);
void PWM_TIM2_CH3_BEEP(void);
void LED_HUXI(u16 *led0pwmval,u8 *dir);
void BEEP_PWM(u16 *led0pwmval,u8 *dir);

void Usart1_Config(void);
void myprintf(const char * str);


void DHT11_Config_IN(void);//输入
void DHT11_Config_OUT(void);//输出
int Start_ask(void);
u8 Read_Bit(void);
u8 Read_Byte(void);
int DHT11_WORK(u8 *humi,u8 *temp);


#define IIC_PORT GPIOB//IIC端口
#define IIC_SDA_PIN GPIO_Pin_7//SDA引脚
#define IIC_SCL_PIN GPIO_Pin_6//SCL引脚

//SCL时钟线写操作
#define SCL_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SCL_PIN):GPIO_ResetBits(IIC_PORT,IIC_SCL_PIN)

//SDA数据线写操作
#define SDA_Write(N) (N)?GPIO_SetBits(IIC_PORT,IIC_SDA_PIN):GPIO_ResetBits(IIC_PORT,IIC_SDA_PIN)
void IIC_Init(void);
void SET_SDA_OUT(void);//SDA输出模式
void SET_SDA_IN(void);//SDA输入模式
void Start_signal(void);//起始信号,先同步高电平，拉低SDA,再拉低SCL
void Stop_signal(void);//停止信号,先同步低电平，拉高SCL,再拉高SDA
u8 IIC_get_Ask(void);//等待从机应答 0应答，1不应答
void IIC_Send_Ask(u8 ask);//主机发送应答信号
void IIC_Send_Byte(u8 Data);//发送一个字节
u8 IIC_Read_Byte(void);//读取一个字节
void IIC_AT24C02_write_data(u8 addr,u8 data);//指定地址写一个字节
u8 AT24C02_read_data(u8 addr);//随机读
#define wendu_addr 0x00
#define shidu_addr 0x10
#define guangzhao_addr_gao 0x30
#define guangzhao_addr_di  0x50

//OLED
//描点屏幕宽度
#define X_WIDTH   128

//1.oled写命令函数
void WriteOLedCmd(uint8_t cmd);

//2 OLED写数据函数
void WriteOLedData(uint8_t data);

//3 oled描点函数
void OLed_Fill(unsigned char bmp_data);

void OLed_SetPos(unsigned char x, unsigned char y);

//5 OLED初始化函数
void InitOLed(void);

//6.显示字母函数
void Oled_print(uint8_t x, uint8_t y,char *str);

//7.显示汉字函数
void OLed_ShowChina(uint8_t x,uint8_t y,uint8_t *buf);


//#define W25Q_CS_PIN GPIO_Pin_2
//#define SPI_SCK1_PIN GPIO_Pin_3
//#define SPI_MISO1_PIN GPIO_Pin_4
//#define SPI_MOSI1_PIN GPIO_Pin_5

//#define W25Q_CS_Write(N) (N)?GPIO_SetBits(GPIOB,W25Q_CS_PIN):GPIO_ResetBits(GPIOB,W25Q_CS_PIN)
//void SPI1_Init(void);
//u16 W25Q128_SPI_READ_WRITE(u16 data);//发送两个字节
//u8 W25Q128_SendByte(u8 byte);//发送一个字节
//u16 W25Q128_ReadDeviceID(void);//获取芯片ID
#define W25Q128_CS(n)  (n)?GPIO_SetBits(GPIOB,GPIO_Pin_2):GPIO_ResetBits(GPIOB,GPIO_Pin_2)
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



void PHtores_ADC_Init();
u16 get_adc_val();//获得ADC的值
unsigned short int getAdcAverage1(unsigned char ch, unsigned char times);


void Usart2_Config(void);
void Usart2_Send_str(char *s);
void Set_Server_Mode(void);
void Set_Cilent_Mode(void);


void fan_init();
void fan_open();
void fan_close();

void dma_Init(u32 *src,u32 *drc,u32 size);//从src到drc传输size个word大小数据
void dma_compare(void);


#endif 