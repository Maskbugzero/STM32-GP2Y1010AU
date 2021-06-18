#ifndef _GY2Y1014AU_H_
#define _GY2Y1014AU_H_

#include "sys.h"
#include "delay.h"
#include "stdio.h"

#define GP2Y_High 	GPIO_SetBits(GPIOA,GPIO_Pin_0);	
#define GP2Y_Low	GPIO_ResetBits(GPIOA,GPIO_Pin_0);	

void GP2Y_Adc_Init(void);  //ADC通道初始化
u16 GP2Y_Get_Adc(u8 ch);   
u16 GetGP2YSingleValue(void);
u16 GPY2_Get_Adc_Aveurage(u8 ch,u8 times);

#endif
