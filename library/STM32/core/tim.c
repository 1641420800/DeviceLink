#include "tim.h"
#include <stdlib.h>

/*
	tim1_init(1000-1,72-1);	// ��ʱ����ʼ��
	tim2_init(1000-1,72-1);	// ��ʱ����ʼ��
	tim3_init(1000-1,72-1);	// ��ʱ����ʼ��
	tim4_init(1000-1,72-1);	// ��ʱ����ʼ��
*/
#if ServiceTimer != 1
WEAK	// Ԥ������жϻص�����
void tim1_IRQ(void)
{

}
#endif

#if ServiceTimer != 2
WEAK	// Ԥ������жϻص�����
void tim2_IRQ(void)
{

}
#endif

#if ServiceTimer != 3
WEAK	// Ԥ������жϻص�����
void tim3_IRQ(void)
{

}
#endif

#if ServiceTimer != 4
WEAK	// Ԥ������жϻص�����
void tim4_IRQ(void)
{

}
#endif

#if ServiceTimer != 0
ServiceTimer_t * STimer = NULL;
uint32_t currentTime = 0;

ServiceTimer_t * registerTimer(uint32_t timeInterval,timerCallback_t timerCallback)
{
	ServiceTimer_t ** p = &STimer;
	
	if(!timeInterval) return NULL;
	if(!timerCallback) return NULL;
	
	while(*p) p = &(*p)->next;
	
	(*p) = (ServiceTimer_t*)malloc(sizeof(ServiceTimer_t));
	
	(*p)->timeInterval = timeInterval;
	(*p)->lastTime = 0;
	(*p)->timerCallback = timerCallback;
	(*p)->next = NULL;
	
#if ServiceTimer == 1
	tim1_init(1000-1,72-1);
#endif
#if ServiceTimer == 2
	tim2_init(1000-1,72-1);
#endif
#if ServiceTimer == 3
	tim3_init(1000-1,72-1);
#endif
#if ServiceTimer == 4
	tim4_init(1000-1,72-1);
#endif

	return *p;
}

uint16_t get_CNT_value(void)
{
#if ServiceTimer == 1
	return TIM1->CNT;
#endif
#if ServiceTimer == 2
	return TIM2->CNT;
#endif
#if ServiceTimer == 3
	return TIM3->CNT;
#endif
#if ServiceTimer == 4
	return TIM4->CNT;
#endif
}

#if ServiceTimer == 1
void tim1_IRQ(void)
#endif
#if ServiceTimer == 2
void tim2_IRQ(void)
#endif
#if ServiceTimer == 3
void tim3_IRQ(void)
#endif
#if ServiceTimer == 4
void tim4_IRQ(void)
#endif
{
	ServiceTimer_t ** p = &STimer;

	++currentTime;
	while(*p)
	{
		
		if((*p)->lastTime + (*p)->timeInterval <= currentTime)
		{
			(*p)->lastTime = currentTime;
			if((*p)->timerCallback) (*p)->timerCallback((*p));
		}
		
		if((*p)->lastTime > currentTime)
		{
			(*p)->lastTime = 0;
		}
		
		p = &(*p)->next;
	}
}

#endif

#if TIM1_enable || ServiceTimer == 1
void tim1_init(uint16_t Pre,uint16_t Psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);				// ʹ�ܶ�Ӧʱ��
	
	TIM_TimeBaseInitStruct.TIM_Period			= Pre;					// ��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler		= Psc;					// Ԥ��Ƶֵ����ʱʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;			// ϵͳʱ�ӷ�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;	// ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStruct);					// ��ʱ����ʼ��
	
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);							// ѡ���ж����Ͳ�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel 					= TIM1_UP_IRQn;	// stm32f103.h�ļ���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;			// ��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 1;			// ��Ӧʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		// ʹ��ͨ��
	NVIC_Init(&NVIC_InitStruct);										// NVIC��ʼ��
	
	TIM_Cmd(TIM1,ENABLE);												// ��ʱ��ʹ��
}

void TIM1_UP_IRQHandler()
{
	if(TIM_GetITStatus(TIM1,TIM_IT_Update) == 1)
	{
		tim1_IRQ();
	}
	TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
}
#endif

#if TIM2_enable || ServiceTimer == 2
void tim2_init(uint16_t Pre,uint16_t Psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);				// ʹ�ܶ�Ӧʱ��
	
	TIM_TimeBaseInitStruct.TIM_Period			= Pre;					// ��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler		= Psc;					// Ԥ��Ƶֵ����ʱʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;			// ϵͳʱ�ӷ�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;	// ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStruct);					// ��ʱ����ʼ��
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);							// ѡ���ж����Ͳ�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel 					= TIM2_IRQn;	// stm32f103.h�ļ���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;			// ��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 1;			// ��Ӧʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		// ʹ��ͨ��
	NVIC_Init(&NVIC_InitStruct);										// NVIC��ʼ��
	
	TIM_Cmd(TIM2,ENABLE);												// ��ʱ��ʹ��
}

void TIM2_IRQHandler()
{
	if(TIM_GetITStatus(TIM2,TIM_IT_Update) == 1)
	{
		tim2_IRQ();
	}
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
}
#endif

#if TIM3_enable || ServiceTimer == 3
void tim3_init(uint16_t Pre,uint16_t Psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);				// ʹ�ܶ�Ӧʱ��
	
	TIM_TimeBaseInitStruct.TIM_Period			= Pre;					// ��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler		= Psc;					// Ԥ��Ƶֵ����ʱʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;			// ϵͳʱ�ӷ�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;	// ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStruct);					// ��ʱ����ʼ��
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);							// ѡ���ж����Ͳ�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel						= TIM3_IRQn;	// stm32f103.h�ļ���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;			// ��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 1;			// ��Ӧʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		// ʹ��ͨ��
	NVIC_Init(&NVIC_InitStruct);										// NVIC��ʼ��
	
	TIM_Cmd(TIM3,ENABLE);												// ��ʱ��ʹ��
}

void TIM3_IRQHandler()
{
	if(TIM_GetITStatus(TIM3,TIM_IT_Update) == 1)
	{
		tim3_IRQ();
	}
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
}
#endif

#if TIM4_enable || ServiceTimer == 4
void tim4_init(uint16_t Pre,uint16_t Psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);				// ʹ�ܶ�Ӧʱ��
	
	TIM_TimeBaseInitStruct.TIM_Period			= Pre;					// ��װ��ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler		= Psc;					// Ԥ��Ƶֵ����ʱʱ��
	TIM_TimeBaseInitStruct.TIM_ClockDivision	= TIM_CKD_DIV1;			// ϵͳʱ�ӷ�Ƶϵ��
	TIM_TimeBaseInitStruct.TIM_CounterMode		= TIM_CounterMode_Up;	// ���ϼ���ģʽ
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseInitStruct);					// ��ʱ����ʼ��
	
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);							// ѡ���ж����Ͳ�ʹ��
	
	NVIC_InitStruct.NVIC_IRQChannel						= TIM4_IRQn;	// stm32f103.h�ļ���
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 1;			// ��ռʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 1;			// ��Ӧʽ���ȼ�
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		// ʹ��ͨ��
	NVIC_Init(&NVIC_InitStruct);										// NVIC��ʼ��
	
	TIM_Cmd(TIM4,ENABLE);												// ��ʱ��ʹ��
}

void TIM4_IRQHandler()
{
	if(TIM_GetITStatus(TIM4,TIM_IT_Update) == 1)
	{
		tim4_IRQ();
	}
	TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
}
#endif


