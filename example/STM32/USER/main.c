#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sys.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"

#include "deviceLink.h"
#include "IIC_OLED.h"
#include "matrixKey.h"

int main(void)
{
	deviceLink_init();
	delay_init();
	gpio_init();
	oled_u8g2Menu_init();
	matrixKey_init_def(); // 矩阵键盘初始化

	while (1)
	{
		deviceLink_loop();
	}
}
