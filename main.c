#include "main.h"


//温
uint8_t HZ_wen[]={
0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,
};

//度
uint8_t HZ_du[]={
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

//湿
uint8_t HZ_shi[]={
0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00,
};

//光
uint8_t HZ_guang[]={
0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00,
0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,
};

//照
uint8_t HZ_zhao[]={
0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x42,0xA2,0x9E,0x82,0xA2,0xC2,0xBE,0x00,0x00,
0x80,0x6F,0x08,0x08,0x28,0xCF,0x00,0x00,0x2F,0xC8,0x08,0x08,0x28,0xCF,0x00,0x00,
};


u8 wendu[2];//温度
u8 shidu[2];//湿度
u16 val;
u8 guangzhao_gao,guangzhao_di;
u8 OLED_FLAG = 1;
u8 LED_HUXI_FALG = 0;
u8 BEEP_FALG = 0;
int wifi_rec_flag = 0;
int wifi_work_flag = 0;
u8 wifi_buf[4096] = {0};
int wifi_len = 0;


void Set_Server_Mode(void)
{
	char *ESP8266_AP_Server[] = {
    "AT\r\n",                                // 1. 检查ESP8266模块是否工作正常
    "ATE1\r\n",                              // 2. 1打开/0关闭回显功能，使得ESP8266模块发送的指令本身输出到串口
    "AT+CWMODE=2\r\n",                       // 3. 设置ESP8266模块为AP模式
    "AT+RST\r\n",                            // 4. 重启ESP8266模块
    "ATE1\r\n",                              //同第二条指令一样，回显功能。
    "AT+CWSAP=\"yjq\",\"77777777\",1,4\r\n",  // 6. 设置AP模式下的SSID、密码、加密方式和信道
    "AT+CIPMUX=1\r\n",                       // 7. 打开多连接模式
    "AT+CIPSERVER=1,8089\r\n",               // 8. 开启服务器功能，监听8089端口
    "AT+CIFSR\r\n"                           // 9. 查询ESP8266模块的IP地址
	};
	for(int i=0;i < 8;i++)
	{
//		printf("配置%d\r\n",i);
		Usart2_Send_str(ESP8266_AP_Server[i]);
		delay_ms(200);
		if(i == 8)
		{
			delay_ms(1000);
		}
		wifi_rec_flag = 1;
	}
	wifi_work_flag = 1;
//	printf("模式设置完成\r\n");
}

u16 usart_data_get;
u16 usart_data_set;
void USART1_IRQHandler(void)
{
	if(USART_GetFlagStatus(USART1,USART_IT_RXNE) == SET)
	{
		usart_data_get = USART_ReceiveData(USART1);
		usart_data_set = usart_data_get;
		USART_SendData(USART1,usart_data_get);
		USART_ClearITPendingBit(USART1,USART_IT_RXNE);
		if(usart_data_set == '1')
		{	
			if(BEEP_FALG == 1)
			BEEP_FALG =0;
			else
			BEEP_FALG =1;
		}
		else if(usart_data_set == '2')
		{
			if(LED_HUXI_FALG == 1)
			LED_HUXI_FALG =0;
			else
			LED_HUXI_FALG =1;
		}
	}
}

int sssss = 0;

void USART2_IRQHandler(void)
{
	u16 us2_get_data;
	u16 us2_b;
	if(USART_GetITStatus(USART2,USART_IT_RXNE))
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);

		us2_get_data = USART_ReceiveData(USART2);
		us2_b = us2_get_data;
		USART_SendData(USART1,us2_b);
		wifi_buf[wifi_len++] = us2_b;
		if(wifi_work_flag)
		{
			switch(us2_b)
			{
				case 'A':
//									GPIO_ToggleBits(GPIOB,GPIO_Pin_10);
					if(BEEP_FALG == 1)
					BEEP_FALG =0;
					else
					BEEP_FALG =1;
					break;
				case 'B':
//					GPIO_ToggleBits(GPIOB,GPIO_Pin_10);
					if(LED_HUXI_FALG == 1)
					LED_HUXI_FALG =0;
					else
					LED_HUXI_FALG =1;					
					break;
			}
		}
	}
}


u8 temperature_high_flag = 0;

void EXTI4_IRQHandler(void)
{
	delay_us(25);
	if(EXTI_GetITStatus(EXTI_Line4))
	{
		EXTI_ClearITPendingBit(EXTI_Line4);//清除中断标志位
		if(OLED_FLAG == 0)
		{
			OLED_FLAG =1;
		}
	}
	delay_us(25);
}

void EXTI9_5_IRQHandler(void)
{
	delay_us(25);
	if(EXTI_GetITStatus(EXTI_Line5))
	{
		EXTI_ClearITPendingBit(EXTI_Line5);//清除中断标志位
		if(OLED_FLAG == 1)
		{
			OLED_FLAG =0;
		}
		OLed_Fill(0x00);//描点清屏函数；
	}
	else if(EXTI_GetITStatus(EXTI_Line6))
	{
		EXTI_ClearITPendingBit(EXTI_Line6);//清除中断标志位
		if(BEEP_FALG == 1)
		BEEP_FALG =0;
		else
		BEEP_FALG =1;
	}
	delay_us(25);
}

void EXTI15_10_IRQHandler(void)
{
	delay_us(25);
	if(EXTI_GetITStatus(EXTI_Line13))
	{
		EXTI_ClearITPendingBit(EXTI_Line13);//清除中断标志位
		if(LED_HUXI_FALG == 1)
		LED_HUXI_FALG =0;
		else
		LED_HUXI_FALG =1;
	}
	delay_us(25);
}

 void TIM5_IRQHandler(void)
 {
	 if (TIM_GetITStatus(TIM5, TIM_IT_Update)!= RESET)
    {
        // 检查温度是否大于等于30
        if (wendu[0] >= 30)
        {
            temperature_high_flag = 1;
        }
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
 }


void LED_HUXI(u16 *led0pwmval,u8 *dir)
{
	if(*dir)
		(*led0pwmval)+=10;
	else     //dir==0 led0pwmval递减 
		(*led0pwmval)-=10;
	//led0pwmval到达300后，方向为递减
	if((*led0pwmval)>100)
		*dir=0;
	//led0pwmval递减到0后，方向改为递增
	if((*led0pwmval)==0)
		*dir=1;	
	//修改比较值，修改占空比	
	TIM_SetCompare1(TIM1,*led0pwmval);
}

void BEEP_PWM(u16 *led0pwmval,u8 *dir)
{
	if(*dir)
		(*led0pwmval)++;
	else     //dir==0 led0pwmval递减 
		(*led0pwmval)--;
	//led0pwmval到达300后，方向为递减
	if((*led0pwmval)>60)
		*dir=0;
	//led0pwmval递减到0后，方向改为递增
	if((*led0pwmval)==0)
		*dir=1;	
	//修改比较值，修改占空比	
	TIM_SetCompare3(TIM2,*led0pwmval); 
}
 


void sys_init()
{

	//延时
	delay_init(168);
	//SPI
	SPI1_Init();
	//LED
	LED_Config();
	//BEEP
	BEEP_Config();
	//USART1串口
	Usart1_Config();

	//ADC光照
	PHtores_ADC_Init();
	//IIC
	IIC_Init();	
	//OLED
	InitOLed();
	//中断
	Nvic_Config();
	EXTI4_Config();
	EXTI_5_6_Config();
	TIM4_Config();
	TIM5_Configuration();
	//按键
	KEY_123_Config();
	//风扇
	fan_init();
	//USART2
	Usart2_Config();
	Set_Server_Mode();
//	Set_Cilent_Mode();	
	//PWM
	PWM_TIM1_CH1_LED();
	PWM_TIM2_CH3_BEEP();

	
}

void wenshidu()
{
	if(DHT11_WORK(shidu, wendu)) 
	{
		IIC_AT24C02_write_data(shidu_addr,shidu[0]);//数据写入AT24C02
		char *str_shidu = (char *)malloc(4); // 假设最多占用 3 个字符加一个结束符
		sprintf(str_shidu, "%d", shidu[0]);
		Oled_print(64,2,str_shidu);

		IIC_AT24C02_write_data(wendu_addr,wendu[0]);//数据写入AT24C02
		char *str_wendu = (char *)malloc(4); // 假设最多占用 3 个字符加一个结束符
		sprintf(str_wendu, "%d", wendu[0]);
		Oled_print(64,0,str_wendu);
		free(str_shidu);
		free(str_wendu);
	} 
}

void guangzhao()
{
	val = get_adc_val();//getAdcAverage1(0,5);//光照
	guangzhao_gao = (u8)(val>>8);
	guangzhao_di = val & 0x00ff;
//	printf("%d\r\n",(u16)(guangzhao_gao<<8 | guangzhao_di));
	IIC_AT24C02_write_data(guangzhao_addr_gao,guangzhao_gao);//数据写入AT24C02
	IIC_AT24C02_write_data(guangzhao_addr_di,guangzhao_di);//数据写入AT24C02
	
	char *str_guangzhao = (char *)malloc(5); // 假设最多占用 4 个字符加一个结束符
	sprintf(str_guangzhao, "%d", val);
	char *kkk = " ";
	strcat(str_guangzhao,kkk);
	Oled_print(64,4,str_guangzhao);
	free(str_guangzhao);
}

#if 0
int main(void)
{
	u16 led0pwmval=0;
	u8 dir=1;
	sys_init();
	OLed_Fill(0x00);//描点清屏函数；
	while(1)
	{
		if(LED_HUXI_FALG == 0)
		{
			TIM_SetCompare1(TIM1, 100); 
		}
		else if(LED_HUXI_FALG == 1)
		{
			LED_HUXI(&led0pwmval,&dir);
		}
		
		if(BEEP_FALG == 0)
		{
			TIM_SetCompare3(TIM2, 0); 
		}
		else if(BEEP_FALG == 1)
		{
			BEEP_PWM(&led0pwmval,&dir);
		}
		if(OLED_FLAG)
		{	
			OLed_ShowChina(16,0,HZ_wen);//温
			OLed_ShowChina(32,0,HZ_du);//度
			Oled_print(48,0,":");//：
			OLed_ShowChina(16,2,HZ_shi);//湿
			OLed_ShowChina(32,2,HZ_du);//度
			Oled_print(48,2,":");//：
			OLed_ShowChina(16,4,HZ_guang);//光
			OLed_ShowChina(32,4,HZ_zhao);//照
			Oled_print(48,4,":");//：
			wenshidu();
			guangzhao();
			LED_3_Normal(); 
			u8 wd,sd;
			u16 gz;
			wd = AT24C02_read_data(wendu_addr);
			sd = AT24C02_read_data(shidu_addr);
			gz = (u16)(AT24C02_read_data(guangzhao_addr_gao)<<8 | AT24C02_read_data(guangzhao_addr_di));
			printf("温度:%d\r\n湿度:%d\r\n光照:%d\r\n",wd,sd,gz);
			if(temperature_high_flag == 1)
			{
				fan_open();
				BEEP_WORRING();
				fan_close();
				temperature_high_flag = 0;
			}
		}

	}
	return 1;
}
#endif

u32 data_src[3] = {0x11111111,0x22222222,0x33333333};
u32 data_drc[3] = {0};

void dma_compare(void)
{
	int i = 0;
	for(i = 0;i < 10;i++)
	{
		if(data_drc[i] != data_src[i])
		{
			printf("data_err[%d] data = 0x%x\r\n",i,data_drc[i]);
			return;
		}
		printf("data_drc[%d] = 0x%x\r\n",i,data_drc[i]);
	}
}

int main()
{
	delay_init(168);
	Usart1_Config();
	dma_Init(data_src,data_drc,sizeof(data_src));
	while(DMA_GetFlagStatus(DMA2_Stream0,DMA_FLAG_TCIF0)==RESET);
	dma_compare();
	printf("hello 2407\r\n");
	while(1);
}