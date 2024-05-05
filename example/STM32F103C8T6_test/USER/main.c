#include <stdio.h>
#include <stdlib.h>

#include "sys.h"
#include "adc.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"

#include "led.h"

#include "core.h"


int main(void)
{	
	delay_init();	   		// 延时函数初始化	  
	LED_Init();		  		// 初始化与外设连接的硬件接口
	
	CORE_init();
	while(1)
	{
		run_loop();
		LED;
	}
}
