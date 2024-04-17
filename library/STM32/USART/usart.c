#include "usart.h"

/*
	uart1_init(9600);		// ���ڳ�ʼ��
	uart2_init(9600);		// ���ڳ�ʼ��
	uart3_init(9600);		// ���ڳ�ʼ��
*/

WEAK	// Ԥ������жϻص�����
void usart1_Rx_IRQ(uint8_t c)
{
	switch(c)
	{
	default:
		break;
	}
}

WEAK	// Ԥ������жϻص�����
void usart2_Rx_IRQ(uint8_t c)
{
	switch(c)
	{
	default:
		break;
	}
}

WEAK	// Ԥ������жϻص�����
void usart3_Rx_IRQ(uint8_t c)
{
	switch(c)
	{
	default:
		break;
	}
}
  
#if 1
#include <stdio.h>
#pragma import(__use_no_semihosting)
struct __FILE 
{ 
	int handle;
}; 

FILE __stdout;       

void _sys_exit(int x) 
{ 
	x = x; 
}
void _ttywrch(int ch)
{
	ch = ch;
}

int fputc(int ch, FILE *f)
{      
	while((USART1->SR&0X40)==0){}
    USART1->DR = (uint8_t)ch;      
	return ch;
}
#endif 

#if USART1_enable
void uart1_init(uint32_t bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// ʹ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);		// ʹ��ʱ��

	// USART_TX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;			// �����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��GPIO

	// USART_RX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	// ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��GPIO

	// Usart NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel						= USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;		// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 3;		// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	// USART ��ʼ������
	USART_InitStructure.USART_BaudRate				= bound;							// ���ڲ�����
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;				// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;					// һ��ֹͣλ
	USART_InitStructure.USART_Parity				= USART_Parity_No;					// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	// ��Ӳ������������
	USART_InitStructure.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;	// �շ�ģʽ

	USART_Init(USART1, &USART_InitStructure);		// ��ʼ������
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);	// �������ڽ����ж�
	USART_Cmd(USART1, ENABLE);						// ʹ�ܴ���
}

void USART1_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		usart1_Rx_IRQ(USART_ReceiveData(USART1));	//��ȡ���յ�������
	}
} 
#endif
#if USART2_enable
void uart2_init(uint32_t bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		// ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);		// ʹ��ʱ��

	// USART_TX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;			// �����������
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��GPIO

	// USART_RX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	// ��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);						// ��ʼ��GPIO

	// Usart NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel						= USART2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;		// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 3;		// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	// USART ��ʼ������
	USART_InitStructure.USART_BaudRate				= bound;							// ���ڲ�����
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;				// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;					// һ��ֹͣλ
	USART_InitStructure.USART_Parity				= USART_Parity_No;					// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	// ��Ӳ������������
	USART_InitStructure.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;	// �շ�ģʽ

	USART_Init(USART2, &USART_InitStructure);		// ��ʼ������
	USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);	// �������ڽ����ж�
	USART_Cmd(USART2, ENABLE);						// ʹ�ܴ���
}

void USART2_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		usart2_Rx_IRQ(USART_ReceiveData(USART2));	//��ȡ���յ�������
	}
}
#endif
#if USART3_enable
void uart3_init(uint32_t bound)
{
	// GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		// ʹ��ʱ��
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);		// ʹ��ʱ��

	// USART_TX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;			// �����������
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// ��ʼ��GPIO

	// USART_RX
	GPIO_InitStructure.GPIO_Pin		= GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_IN_FLOATING;	// ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);						// ��ʼ��GPIO

	// Usart NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel						= USART3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority	= 3;		// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority			= 3;		// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd					= ENABLE;	// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);

	// USART ��ʼ������
	USART_InitStructure.USART_BaudRate				= bound;							// ���ڲ�����
	USART_InitStructure.USART_WordLength			= USART_WordLength_8b;				// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits				= USART_StopBits_1;					// һ��ֹͣλ
	USART_InitStructure.USART_Parity				= USART_Parity_No;					// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl	= USART_HardwareFlowControl_None;	// ��Ӳ������������
	USART_InitStructure.USART_Mode					= USART_Mode_Rx | USART_Mode_Tx;	// �շ�ģʽ

	USART_Init(USART3, &USART_InitStructure);		// ��ʼ������
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);	// �������ڽ����ж�
	USART_Cmd(USART3, ENABLE);						// ʹ�ܴ���
}

void USART3_IRQHandler(void)                	//����1�жϷ������
{
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
		usart3_Rx_IRQ(USART_ReceiveData(USART3));	//��ȡ���յ�������
	}
}
#endif

void USART_sendBuf(USART_TypeDef * USARTx, uint8_t * buf, uint16_t bufLen)
{
	while(bufLen--)
	{
		while((USARTx->SR&0X40)==0){}
		USARTx->DR = *buf;
		++buf;
	}
}

#include <stdarg.h>
int USART_printf(USART_TypeDef * USARTx,char *fmt,...)
{
	char buffer[BUF_LEN];
	int  ret;
	va_list arg_ptr;
	va_start(arg_ptr,fmt);
	ret = vsnprintf(buffer,BUF_LEN,fmt,arg_ptr);
	va_end(arg_ptr);
	
	USART_sendBuf(USARTx,(uint8_t*)buffer,ret);
	
	return ret;
}
