#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "stdio.h"
#include "oled.h"
#include "gy2y1014au.h"
#include "dht11.h"

int main(void)
{
	char str[] = "";
	u16 PM = 0; 
	DHT11_Data_TypeDef DHT11_Data;
	
	delay_init();
	NVIC_Configuration();
	uart_init(115200);
	GP2Y_Adc_Init();  //ADC��ʼ��     
	OLED_Init();
	OLED_ColorTurn(0);//0������ʾ��1 ��ɫ��ʾ
	OLED_DisplayTurn(0);//0������ʾ 1 ��Ļ��ת��ʾ

	OLED_Refresh();
	OLED_ShowString(2,2,"PM2.5:",16); 
	OLED_ShowString(12,20,"TEM:",16); 
	OLED_ShowString(12,38,"HUM:",16); 
	
	OLED_ShowString(90,2,"ug/m3",12); //PM2.5��λ ug/m3
	OLED_ShowChinese(100,20,0,16); //�¶ȵ�λ ��
	OLED_ShowChar(100,38,'%',16);  //ʪ�ȵ�λ %
	
	while(1)
	{
		/* �۳���������ȡ����*/
		PM = GetGP2YSingleValue();       //�õ�pm2.5ֵ 
		if(PM < 10)
			sprintf(str, " %d ",PM);
		else if(PM < 100)
			sprintf(str, "%d ",PM);
		else
			sprintf(str, "%d",PM);
		OLED_ShowString(60,2,(u8 *)str,16);
		
		/* ��ʪ�ȴ�������ȡ����*/
		if( Read_DHT11(&DHT11_Data)==SUCCESS)
		{
			sprintf(str, "%d.%d �� ",DHT11_Data.temp_int,DHT11_Data.temp_deci);
			OLED_ShowString(60,20,(u8 *)str,16);
			sprintf(str, "%d.%d",DHT11_Data.humi_int,DHT11_Data.humi_deci);
			OLED_ShowString(60,38,(u8 *)str,16);
		}
		else
		{
			printf("Read DHT11 ERROR!\r\n");//��ȡ����ʧ�ܣ����ڴ�ӡ��Read DHT11 ERROR.
		}

		OLED_Refresh();
		delay_ms(1000);
	}
}

