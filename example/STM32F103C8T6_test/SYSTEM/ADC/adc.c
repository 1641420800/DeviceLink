#include "adc.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"

/*---------ADת��--------
˵����c8t6����10���ⲿADת�� PA0-PA7 PB0��PB1 �ֱ��Ӧͨ��0-ͨ��9  ������ΪPA0��PA1��PB0��PB1
		��Ҫ�����ڱ��ļ����ļ����޸���Ҫʹ�õ�ͨ�� CLOSE���رգ� �� OPEN���򿪣�
		
ͨ��˵����	//����	GET_ADC_PA3��GET_ADC_PA4��GET_ADC_PA5��GET_ADC_PA6��GET_ADC_PA7��
			GET_ADC_PA8��GET_ADC_PA9��GET_ADC_PA5��GET_ADC_PB0��GET_ADC_PB1
���ã�
	#include "adc.h"
	
	int adcx1,adcx2,adcx3,adcx4;  			//ADת���Ľ��
	float adc1,adc2,adc3,adc4;
//--------------------------------ADC��ʼ��Ĭ�Ͽ���PA0��PA1��ͨ��������Ҫ����ͨ�����·����п���----

	Adc_Init();							//ADCת����ʼ��
//--------------------------------ADC��ȡ���ݲ�OLED������ʾ--------------------------------------
	adcx1=GET_ADC_PA0;						//��ȡPA0��ADCֵ
	adcx2=GET_ADC_PA1;						//��ȡPA1��ADCֵ
	adcx3=GET_ADC_PA2;						//��ȡPA2��ADCֵ			Ĭ��δ�����������·�����
	adcx4=GET_ADC_PA3;						//��ȡPA3��ADCֵ			Ĭ��δ�����������·�����
	adc1 = 100-(adcx1/40);  				//���Ʊ仯��Χ��0-100
	adc2 = 100-(adcx2/40);					//���Ʊ仯��Χ��0-100
	adc3 = 100-(adcx3/40);					//���Ʊ仯��Χ��0-100
	adc4 = 100-(adcx4/40);					//���Ʊ仯��Χ��0-100
	OLED_ShowString(0,0,"adc1:",16);   	    //��ʾ���� 
	OLED_ShowNum(55,0,adc1,3,16);           //��ʾ����ʾadc1 
	OLED_ShowString(0,2,"adc2:",16);   	    //��ʾ���� 
	OLED_ShowNum(55,2,adc2,3,16);           //��ʾ����ʾadc2
	OLED_ShowString(0,4,"adc3:",16);   	    //��ʾ����  
	OLED_ShowNum(55,4,adc3,3,16);           //��ʾ����ʾadc3 
	OLED_ShowString(0,6,"adc4:",16);   	    //��ʾ������ 
	OLED_ShowNum(55,6,adc4,3,16);           //��ʾ����ʾadc4 	

*/
#define CLOSE 0
#define OPEN  1

#define ADC1_CH0_PA0_CHOOSE	OPEN
#define ADC1_CH1_PA1_CHOOSE	OPEN
#define ADC1_CH2_PA2_CHOOSE	CLOSE
#define ADC1_CH3_PA3_CHOOSE	CLOSE
#define ADC1_CH4_PA4_CHOOSE	CLOSE
#define ADC1_CH5_PA5_CHOOSE	CLOSE
#define ADC1_CH6_PA6_CHOOSE	CLOSE
#define ADC1_CH7_PA7_CHOOSE	CLOSE
#define ADC1_CH8_PB0_CHOOSE	CLOSE
#define ADC1_CH9_PB1_CHOOSE	CLOSE

#define ADC1_DMA CLOSE

#if ADC1_DMA
#include "stm32f10x_dma.h"
void ADC_DMA_Init(uint32_t * data, uint16_t dataSize)
{
	DMA_InitTypeDef DMA_InitStructure;
	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE); 				//ʹ��MDA1ʱ��
	/* DMA channel1 configuration */
	DMA_DeInit(DMA1_Channel1); 										//ָ��DMAͨ��
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&(ADC1->DR);		//ADC1��ַ---����ADC1����ת��ֵ�ļĴ���
	DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)&data;	//����DMA�ڴ��ַ��ADCת�����ֱ�ӷ���õ�ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC; 				//����Ϊ����Ϊ���ݴ������Դ
	DMA_InitStructure.DMA_BufferSize = dataSize;						//����������---2ͨ����Ҫ����2������
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable; 	//�����ַ�̶�
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;			//�ڴ��ַ����---�����ʾʼ�մ�����ADC1��ַ��ȡֵ---���α��浽�����������ڴ������---
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord; //�ڴ洫�����ݵ�Ԫ---����16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;					//ѭ��ģʽ---2����������ѭ�����մ�����ADC1���������ADCֵ---
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);

	/* Enable DMA channel1 */
	DMA_Cmd(DMA1_Channel1, ENABLE); 									//ʹ��DMAͨ��
	
	Adc_Init();
	
}
#endif

void Adc_Init(void)
{
	int order = 0;
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 ,ENABLE);
#if ADC1_CH0_PA0_CHOOSE || ADC1_CH1_PA1_CHOOSE || ADC1_CH2_PA2_CHOOSE || ADC1_CH3_PA3_CHOOSE || ADC1_CH4_PA4_CHOOSE || ADC1_CH5_PA5_CHOOSE || ADC1_CH6_PA6_CHOOSE || ADC1_CH7_PA7_CHOOSE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
#endif	
	
#if ADC1_CH8_PB0_CHOOSE||ADC1_CH9_PB1_CHOOSE
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_ADC1,ENABLE);
#endif
	
	RCC_ADCCLKConfig(RCC_PCLK2_Div6);	//����ADCΪ6��Ƶ 72M/6=12M  ADC���ʱ�� <= 14M	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_AIN;
#if ADC1_CH0_PA0_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH1_PA1_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH2_PA2_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_2;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH3_PA3_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_3;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH4_PA4_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_4;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH5_PA5_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_5;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH6_PA6_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH7_PA7_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_7;
	GPIO_Init(GPIOA,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH8_PB0_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	++order;
#endif
#if ADC1_CH9_PB1_CHOOSE 
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	++order;
#endif
	
	ADC_DeInit(ADC1);
	
	ADC_InitStructure.ADC_Mode					= ADC_Mode_Independent;			// ADC����ģʽ:ADC1��ADC2�����ڶ���ģʽ

#if ADC1_DMA
	ADC_InitStructure.ADC_ScanConvMode			= ENABLE;						// ģ��ת��������ɨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode	= ENABLE;						// ģ��ת������������ת��
#else
	ADC_InitStructure.ADC_ScanConvMode			= DISABLE;						// ģ��ת�������ڵ�ͨ��ģʽ
	ADC_InitStructure.ADC_ContinuousConvMode	= DISABLE;						// ģ��ת�������ڵ���ת��
#endif
	ADC_InitStructure.ADC_ExternalTrigConv		= ADC_ExternalTrigConv_None;	// ת��������������ⲿ��������
	ADC_InitStructure.ADC_DataAlign				= ADC_DataAlign_Right;			// ADC�����Ҷ���
	ADC_InitStructure.ADC_NbrOfChannel			= order;						// ˳����й���ת����ADCͨ������Ŀ
	ADC_Init(ADC1, &ADC_InitStructure);											// ��ʼ�� ADC 
	
	order = 0;
#if ADC1_CH0_PA0_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_0, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH1_PA1_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_1, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH2_PA2_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_2, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH3_PA3_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_3, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH4_PA4_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_4, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH5_PA5_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_5, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH6_PA6_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_6, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH7_PA7_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_7, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH8_PB0_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_8, ++order, ADC_SampleTime_1Cycles5);
#endif
#if ADC1_CH9_PB1_CHOOSE 
	ADC_RegularChannelConfig (ADC1, ADC_Channel_9, ++order, ADC_SampleTime_1Cycles5);
#endif	 

#if ADC1_DMA
	ADC_DMACmd ( ADC1, ENABLE ); //ʹ��ADC DMA����
#endif

	ADC_Cmd(ADC1,ENABLE);
	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));
}

uint16_t Get_Adc(uint8_t ch)						
{
	switch(ch)
	{
#if ADC1_CH0_PA0_CHOOSE 
		case ADC_Channel_0: break;
#endif
#if ADC1_CH1_PA1_CHOOSE 
		case ADC_Channel_1: break;
#endif
#if ADC1_CH2_PA2_CHOOSE 
		case ADC_Channel_2: break;
#endif
#if ADC1_CH3_PA3_CHOOSE 
		case ADC_Channel_3: break;
#endif
#if ADC1_CH4_PA4_CHOOSE 
		case ADC_Channel_4: break;
#endif
#if ADC1_CH5_PA5_CHOOSE 
		case ADC_Channel_5: break;
#endif
#if ADC1_CH6_PA6_CHOOSE 
		case ADC_Channel_6: break;
#endif
#if ADC1_CH7_PA7_CHOOSE 
		case ADC_Channel_7: break;
#endif
#if ADC1_CH8_PB0_CHOOSE 
		case ADC_Channel_8: break;
#endif
#if ADC1_CH9_PB1_CHOOSE 
		case ADC_Channel_9: break;
#endif
		default : return 0;
	}
 	ADC_RegularChannelConfig (ADC1, ch, 1, ADC_SampleTime_1Cycles5);
	
	ADC_SoftwareStartConvCmd(ADC1,ENABLE);
	while(!ADC_GetFlagStatus(ADC1,ADC_FLAG_EOC));
	return ADC_GetConversionValue(ADC1);
}

#include "delay.h"
uint16_t Get_Adc_Average(uint8_t ch,uint8_t times)	
{
	uint32_t temp_val=0;
	uint8_t t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc(ch);
		delay_ms(5);
	}
	return temp_val/times;
}
