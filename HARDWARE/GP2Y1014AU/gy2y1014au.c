#include "gy2y1014au.h"

//初始化ADC
//这里我们仅以规则通道为例
//我们默认将开启通道0~3 
void GP2Y_Adc_Init(void)  //ADC通道初始化
{
	ADC_InitTypeDef ADC_InitStructure; 

	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_ADC1, ENABLE );        //使能GPIOA,ADC1通道时钟

	RCC_ADCCLKConfig(RCC_PCLK2_Div6);   //分频因子6时钟为72M/6=12MHz

	//PA1作为模拟通道输入引脚                         
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;         //模拟输入引脚
	GPIO_Init(GPIOA, &GPIO_InitStructure);        

	//PA0 作为脉冲输出引脚    
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;                         
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;           
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GP2Y_High;

	ADC_DeInit(ADC1);  //将外设 ADC1 的全部寄存器重设为缺省值

	ADC_InitStructure.ADC_Mode= ADC_Mode_Independent;        //ADC工作模式:ADC1和ADC2工作在独立模式
	ADC_InitStructure.ADC_ScanConvMode= DISABLE;   //模数转换工作在单通道模式
	ADC_InitStructure.ADC_ContinuousConvMode= DISABLE;        //模数转换工作在单次转换模式
	ADC_InitStructure.ADC_ExternalTrigConv= ADC_ExternalTrigConv_None;      //转换由软件而不是外部触发启动
	ADC_InitStructure.ADC_DataAlign= ADC_DataAlign_Right;       //ADC数据右对齐
	ADC_InitStructure.ADC_NbrOfChannel= 1; //顺序进行规则转换的ADC通道的数目
	ADC_Init(ADC1,&ADC_InitStructure);  //根据ADC_InitStruct中指定的参数初始化外设ADCx的寄存器

	ADC_Cmd(ADC1,ENABLE);    //使能指定的ADC1

	ADC_ResetCalibration(ADC1);       //重置指定的ADC1的复位寄存器

	while(ADC_GetResetCalibrationStatus(ADC1));        //获取ADC1重置校准寄存器的状态,设置状态则等待

	ADC_StartCalibration(ADC1);          //开启AD校准

	while(ADC_GetCalibrationStatus(ADC1));              //获取指定ADC1的校准程序,设置状态则等待
}

//获得ADC值
//ch:通道值 0~3
u16 GP2Y_Get_Adc(u8 ch)   
{
	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC1,ch, 1, ADC_SampleTime_239Cycles5 );  //ADC1,ADC通道3,第一个转换,采样时间为239.5周期                               

	ADC_SoftwareStartConvCmd(ADC1,ENABLE);             //使能指定的ADC1的软件转换启动功能

	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC1);      //返回最近一次ADC1规则组的转换结果
}

u16 GetGP2YSingleValue(void)
{
	uint16_t ADCVal;
	int dustVal = 0;
	float Voltage;

	GPIO_SetBits(GPIOA, GPIO_Pin_0);//置1  开启内部LED
	delay_us(280); 			 // 开启LED后的280us的等待时间
	ADCVal = GP2Y_Get_Adc(ADC_Channel_1);  //PA1 采样，读取AD值
	delay_us(19);			  //延时19us，因为这里AD采样的周期为239.5，所以AD转换一次需耗时21us，19加21再加280刚好是320us
	GPIO_ResetBits(GPIOA, GPIO_Pin_0);//置0
	delay_us(9680);			//需要脉宽比0.32ms/10ms的PWM信号驱动传感器中的LED
	
	Voltage = 3.3f * ADCVal / 4096.f * 2; //获得AO输出口的电压值
	
	dustVal = (0.17*Voltage-0.1)*1000;  //乘以1000单位换成ug/m3//

	if (dustVal < 0)
		dustVal = 0;            //限位//

	if (dustVal>500)        
		dustVal=500;

	printf("\r\n电压值:%fV\n",Voltage);

	printf("\r\n灰尘浓度:%d ug/m3\n",dustVal);
	
	return dustVal;
}

//获取通道ch的转换值
//取times次,然后平均
u16 GPY2_Get_Adc_Aveurage(u8 ch,u8 times)
{
	u32 pm_val=0;
	u8 t;

	for(t=0;t<times;t++)
	{
		pm_val+=GP2Y_Get_Adc(ch);
		delay_ms(5);
	}
	
	return pm_val/times;
}         











