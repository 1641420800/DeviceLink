#include <stdio.h>
#include <stdlib.h>

#include "sys.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"

#include "led.h"

#include "core.h"

void CORE_callback_1(const char *topic, void *arg, size_t siz)
{
	
}
void CORE_callback_2(const char *topic, void *arg, size_t siz)
{
	
}
void CORE_callback_all(const char *topic, void *arg, size_t siz)
{
	
}
int main(void)
{	
	delay_init();	   		// 延时函数初始化	  
	LED_Init();		  		// 初始化与外设连接的硬件接口
	
	CORE_init();
	CORE_subscribe("123",CORE_callback_1);
	CORE_subscribe("456",CORE_callback_2);
	CORE_subscribe("*",CORE_callback_all);
	
	while(1)
	{
  
		CORE_publish_str("123","1122334",0);
		CORE_Run_loop();
		LED;
	}
}
