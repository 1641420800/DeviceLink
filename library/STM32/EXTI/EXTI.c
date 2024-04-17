#include "EXTI.h"
#include <stdlib.h>

WEAK	// 预定义的中断回调函数
void exti_0_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_1_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_2_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_3_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_4_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_5_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_6_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_7_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_8_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_9_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_10_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_11_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_12_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_13_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_14_IRQ(void)
{

}
WEAK	// 预定义的中断回调函数
void exti_15_IRQ(void)
{

}

#if ServiceEXTI
ServiceExti_t * SExti = NULL;

void registerExti(GPIO_TypeDef * GPIO,uint16_t PIN,extiCallback_t extiCallback)
{
	ServiceExti_t ** p = &SExti;
	
	if(!extiCallback) return;
	
	while(*p) p = &(*p)->next;
	
	(*p) = (ServiceExti_t*)malloc(sizeof(ServiceExti_t));
	
	(*p)->PIN = PIN;
	(*p)->extiCallback = extiCallback;
	(*p)->next = NULL;
	
	EXTIx_Init(GPIO,PIN);
}
void EXTI_Events(uint16_t PIN)
{
	ServiceExti_t ** p = &SExti;
	
	while(*p)
	{
		if((*p)->PIN == PIN)
		{
			if((*p)->extiCallback) (*p)->extiCallback((*p));
		}
		p = &(*p)->next;
	}
}

#endif

void EXTIx_Init(GPIO_TypeDef * GPIO, uint16_t PIN)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXIT_InitStrue;
	NVIC_InitTypeDef NVIC_InitStrue;
	uint8_t GPIO_PortSource = 0;
	uint8_t GPIO_PinSource  = 0;
	uint32_t EXTI_Line = 0;
	IRQn_Type IRQn;
	if(GPIO == GPIOA)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		GPIO_PortSource = GPIO_PortSourceGPIOA;
	}
	if(GPIO == GPIOB)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		GPIO_PortSource = GPIO_PortSourceGPIOB;
	}
	if(GPIO == GPIOC)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
		GPIO_PortSource = GPIO_PortSourceGPIOC;
	}
	
	if(PIN == GPIO_Pin_0)
	{
		GPIO_PinSource = GPIO_PinSource0;
		EXTI_Line = EXTI_Line0;
		IRQn = EXTI0_IRQn;
	}
	if(PIN == GPIO_Pin_1)
	{
		GPIO_PinSource = GPIO_PinSource1;
		EXTI_Line = EXTI_Line1;
		IRQn = EXTI1_IRQn;
	}
	if(PIN == GPIO_Pin_2)
	{
		GPIO_PinSource = GPIO_PinSource2;
		EXTI_Line = EXTI_Line2;
		IRQn = EXTI2_IRQn;
	}
	if(PIN == GPIO_Pin_3)
	{
		GPIO_PinSource = GPIO_PinSource3;
		EXTI_Line = EXTI_Line3;
		IRQn = EXTI3_IRQn;
	}
	if(PIN == GPIO_Pin_4)
	{
		GPIO_PinSource = GPIO_PinSource4;
		EXTI_Line = EXTI_Line4;
		IRQn = EXTI4_IRQn;
	}
	if(PIN == GPIO_Pin_5)
	{
		GPIO_PinSource = GPIO_PinSource5;
		EXTI_Line = EXTI_Line5;
		IRQn = EXTI9_5_IRQn;
	}
	if(PIN == GPIO_Pin_6)
	{
		GPIO_PinSource = GPIO_PinSource6;
		EXTI_Line = EXTI_Line6;
		IRQn = EXTI9_5_IRQn;
	}
	if(PIN == GPIO_Pin_7)
	{
		GPIO_PinSource = GPIO_PinSource7;
		EXTI_Line = EXTI_Line7;
		IRQn = EXTI9_5_IRQn;
	}
	if(PIN == GPIO_Pin_8)
	{
		GPIO_PinSource = GPIO_PinSource8;
		EXTI_Line = EXTI_Line8;
		IRQn = EXTI9_5_IRQn;
	}
	if(PIN == GPIO_Pin_9)
	{
		GPIO_PinSource = GPIO_PinSource9;
		EXTI_Line = EXTI_Line9;
		IRQn = EXTI9_5_IRQn;
	}
	if(PIN == GPIO_Pin_10)
	{
		GPIO_PinSource = GPIO_PinSource10;
		EXTI_Line = EXTI_Line10;
		IRQn = EXTI15_10_IRQn;
	}
	if(PIN == GPIO_Pin_11)
	{
		GPIO_PinSource = GPIO_PinSource11;
		EXTI_Line = EXTI_Line11;
		IRQn = EXTI15_10_IRQn;
	}
	if(PIN == GPIO_Pin_12)
	{
		GPIO_PinSource = GPIO_PinSource12;
		EXTI_Line = EXTI_Line12;
		IRQn = EXTI15_10_IRQn;
	}
	if(PIN == GPIO_Pin_13)
	{
		GPIO_PinSource = GPIO_PinSource13;
		EXTI_Line = EXTI_Line13;
		IRQn = EXTI15_10_IRQn;
	}
	if(PIN == GPIO_Pin_14)
	{
		GPIO_PinSource = GPIO_PinSource14;
		EXTI_Line = EXTI_Line14;
		IRQn = EXTI15_10_IRQn;
	}
	if(PIN == GPIO_Pin_15)
	{
		GPIO_PinSource = GPIO_PinSource15;
		EXTI_Line = EXTI_Line15;
		IRQn = EXTI15_10_IRQn;
	}
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin  = PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; 
	GPIO_Init(GPIO, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSource, GPIO_PinSource);
	
	EXIT_InitStrue.EXTI_Line    = EXTI_Line;
	EXIT_InitStrue.EXTI_LineCmd = ENABLE;
	EXIT_InitStrue.EXTI_Mode    = EXTI_Mode_Interrupt;
	EXIT_InitStrue.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
	EXTI_Init(&EXIT_InitStrue);


	NVIC_InitStrue.NVIC_IRQChannel						= IRQn;
	NVIC_InitStrue.NVIC_IRQChannelCmd					= ENABLE;
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority	= 0;
	NVIC_InitStrue.NVIC_IRQChannelSubPriority			= 0;
	NVIC_Init(&NVIC_InitStrue);

}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0))
	{
		exti_0_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_0);
#endif
		EXTI_ClearITPendingBit(EXTI_Line0); //清除中断标志位
	}
}
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line1))
	{
		exti_1_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_1);
#endif
		EXTI_ClearITPendingBit(EXTI_Line1); //清除中断标志位
	}
}
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line2))
	{
		exti_2_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_2);
#endif
		EXTI_ClearITPendingBit(EXTI_Line2); //清除中断标志位
	}
}
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line3))
	{
		exti_3_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_3);
#endif
		EXTI_ClearITPendingBit(EXTI_Line3); //清除中断标志位
	}
}
void EXTI4_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line4))
	{
		exti_4_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_4);
#endif
		EXTI_ClearITPendingBit(EXTI_Line4); //清除中断标志位
	}
}
void EXTI9_5_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line5))
	{
		exti_5_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_5);
#endif
		EXTI_ClearITPendingBit(EXTI_Line5); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line6))
	{
		exti_6_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_6);
#endif
		EXTI_ClearITPendingBit(EXTI_Line6); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line7))
	{
		exti_7_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_7);
#endif
		EXTI_ClearITPendingBit(EXTI_Line7); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line8))
	{
		exti_8_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_8);
#endif
		EXTI_ClearITPendingBit(EXTI_Line8); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line9))
	{
		exti_9_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_9);
#endif
		EXTI_ClearITPendingBit(EXTI_Line9); //清除中断标志位
	}
}
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line10))
	{
		exti_10_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_10);
#endif
		EXTI_ClearITPendingBit(EXTI_Line10); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line11))
	{
		exti_11_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_11);
#endif
		EXTI_ClearITPendingBit(EXTI_Line11); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line12))
	{
		exti_12_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_12);
#endif
		EXTI_ClearITPendingBit(EXTI_Line12); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line13))
	{
		exti_13_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_13);
#endif
		EXTI_ClearITPendingBit(EXTI_Line13); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line14))
	{
		exti_14_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_14);
#endif
		EXTI_ClearITPendingBit(EXTI_Line14); //清除中断标志位
	}
	if(EXTI_GetITStatus(EXTI_Line15))
	{
		exti_15_IRQ();
#if ServiceEXTI
		EXTI_Events(GPIO_Pin_15);
#endif
		EXTI_ClearITPendingBit(EXTI_Line15); //清除中断标志位
	}
}
