#include "main.h"


//��
uint8_t HZ_wen[]={
0x10,0x60,0x02,0x8C,0x00,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x40,0x7E,0x42,0x42,0x7E,0x42,0x7E,0x42,0x42,0x7E,0x40,0x00,
};

//��
uint8_t HZ_du[]={
0x00,0x00,0xFC,0x24,0x24,0x24,0xFC,0x25,0x26,0x24,0xFC,0x24,0x24,0x24,0x04,0x00,
0x40,0x30,0x8F,0x80,0x84,0x4C,0x55,0x25,0x25,0x25,0x55,0x4C,0x80,0x80,0x80,0x00,
};

//ʪ
uint8_t HZ_shi[]={
0x10,0x60,0x02,0x8C,0x00,0xFE,0x92,0x92,0x92,0x92,0x92,0x92,0xFE,0x00,0x00,0x00,
0x04,0x04,0x7E,0x01,0x44,0x48,0x50,0x7F,0x40,0x40,0x7F,0x50,0x48,0x44,0x40,0x00,
};

//��
uint8_t HZ_guang[]={
0x40,0x40,0x42,0x44,0x58,0xC0,0x40,0x7F,0x40,0xC0,0x50,0x48,0x46,0x40,0x40,0x00,
0x80,0x80,0x40,0x20,0x18,0x07,0x00,0x00,0x00,0x3F,0x40,0x40,0x40,0x40,0x78,0x00,
};

//��
uint8_t HZ_zhao[]={
0x00,0xFE,0x42,0x42,0x42,0xFE,0x00,0x42,0xA2,0x9E,0x82,0xA2,0xC2,0xBE,0x00,0x00,
0x80,0x6F,0x08,0x08,0x28,0xCF,0x00,0x00,0x2F,0xC8,0x08,0x08,0x28,0xCF,0x00,0x00,
};


u8 wendu[2];//�¶�
u8 shidu[2];//ʪ��
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
    "AT\r\n",                                // 1. ���ESP8266ģ���Ƿ�������
    "ATE1\r\n",                              // 2. 1��/0�رջ��Թ��ܣ�ʹ��ESP8266ģ�鷢�͵�ָ������������
    "AT+CWMODE=2\r\n",                       // 3. ����ESP8266ģ��ΪAPģʽ
    "AT+RST\r\n",                            // 4. ����ESP8266ģ��
    "ATE1\r\n",                              //ͬ�ڶ���ָ��һ�������Թ��ܡ�
    "AT+CWSAP=\"yjq\",\"77777777\",1,4\r\n",  // 6. ����APģʽ�µ�SSID�����롢���ܷ�ʽ���ŵ�
    "AT+CIPMUX=1\r\n",                       // 7. �򿪶�����ģʽ
    "AT+CIPSERVER=1,8089\r\n",               // 8. �������������ܣ�����8089�˿�
    "AT+CIFSR\r\n"                           // 9. ��ѯESP8266ģ���IP��ַ
	};
	for(int i=0;i < 8;i++)
	{
//		printf("����%d\r\n",i);
		Usart2_Send_str(ESP8266_AP_Server[i]);
		delay_ms(200);
		if(i == 8)
		{
			delay_ms(1000);
		}
		wifi_rec_flag = 1;
	}
	wifi_work_flag = 1;
//	printf("ģʽ�������\r\n");
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
		EXTI_ClearITPendingBit(EXTI_Line4);//����жϱ�־λ
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
		EXTI_ClearITPendingBit(EXTI_Line5);//����жϱ�־λ
		if(OLED_FLAG == 1)
		{
			OLED_FLAG =0;
		}
		OLed_Fill(0x00);//�������������
	}
	else if(EXTI_GetITStatus(EXTI_Line6))
	{
		EXTI_ClearITPendingBit(EXTI_Line6);//����жϱ�־λ
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
		EXTI_ClearITPendingBit(EXTI_Line13);//����жϱ�־λ
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
        // ����¶��Ƿ���ڵ���30
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
	else     //dir==0 led0pwmval�ݼ� 
		(*led0pwmval)-=10;
	//led0pwmval����300�󣬷���Ϊ�ݼ�
	if((*led0pwmval)>100)
		*dir=0;
	//led0pwmval�ݼ���0�󣬷����Ϊ����
	if((*led0pwmval)==0)
		*dir=1;	
	//�޸ıȽ�ֵ���޸�ռ�ձ�	
	TIM_SetCompare1(TIM1,*led0pwmval);
}

void BEEP_PWM(u16 *led0pwmval,u8 *dir)
{
	if(*dir)
		(*led0pwmval)++;
	else     //dir==0 led0pwmval�ݼ� 
		(*led0pwmval)--;
	//led0pwmval����300�󣬷���Ϊ�ݼ�
	if((*led0pwmval)>60)
		*dir=0;
	//led0pwmval�ݼ���0�󣬷����Ϊ����
	if((*led0pwmval)==0)
		*dir=1;	
	//�޸ıȽ�ֵ���޸�ռ�ձ�	
	TIM_SetCompare3(TIM2,*led0pwmval); 
}
 


void sys_init()
{

	//��ʱ
	delay_init(168);
	//SPI
	SPI1_Init();
	//LED
	LED_Config();
	//BEEP
	BEEP_Config();
	//USART1����
	Usart1_Config();

	//ADC����
	PHtores_ADC_Init();
	//IIC
	IIC_Init();	
	//OLED
	InitOLed();
	//�ж�
	Nvic_Config();
	EXTI4_Config();
	EXTI_5_6_Config();
	TIM4_Config();
	TIM5_Configuration();
	//����
	KEY_123_Config();
	//����
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
		IIC_AT24C02_write_data(shidu_addr,shidu[0]);//����д��AT24C02
		char *str_shidu = (char *)malloc(4); // �������ռ�� 3 ���ַ���һ��������
		sprintf(str_shidu, "%d", shidu[0]);
		Oled_print(64,2,str_shidu);

		IIC_AT24C02_write_data(wendu_addr,wendu[0]);//����д��AT24C02
		char *str_wendu = (char *)malloc(4); // �������ռ�� 3 ���ַ���һ��������
		sprintf(str_wendu, "%d", wendu[0]);
		Oled_print(64,0,str_wendu);
		free(str_shidu);
		free(str_wendu);
	} 
}

void guangzhao()
{
	val = get_adc_val();//getAdcAverage1(0,5);//����
	guangzhao_gao = (u8)(val>>8);
	guangzhao_di = val & 0x00ff;
//	printf("%d\r\n",(u16)(guangzhao_gao<<8 | guangzhao_di));
	IIC_AT24C02_write_data(guangzhao_addr_gao,guangzhao_gao);//����д��AT24C02
	IIC_AT24C02_write_data(guangzhao_addr_di,guangzhao_di);//����д��AT24C02
	
	char *str_guangzhao = (char *)malloc(5); // �������ռ�� 4 ���ַ���һ��������
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
	OLed_Fill(0x00);//�������������
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
			OLed_ShowChina(16,0,HZ_wen);//��
			OLed_ShowChina(32,0,HZ_du);//��
			Oled_print(48,0,":");//��
			OLed_ShowChina(16,2,HZ_shi);//ʪ
			OLed_ShowChina(32,2,HZ_du);//��
			Oled_print(48,2,":");//��
			OLed_ShowChina(16,4,HZ_guang);//��
			OLed_ShowChina(32,4,HZ_zhao);//��
			Oled_print(48,4,":");//��
			wenshidu();
			guangzhao();
			LED_3_Normal(); 
			u8 wd,sd;
			u16 gz;
			wd = AT24C02_read_data(wendu_addr);
			sd = AT24C02_read_data(shidu_addr);
			gz = (u16)(AT24C02_read_data(guangzhao_addr_gao)<<8 | AT24C02_read_data(guangzhao_addr_di));
			printf("�¶�:%d\r\nʪ��:%d\r\n����:%d\r\n",wd,sd,gz);
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