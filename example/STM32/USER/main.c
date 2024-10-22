#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sys.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"

#include "deviceLink.h"
#include "u8g2_ui.h"
#include "matrixKey.h"


u8g2Ui_t* u8g2Ui_found()
{
	u8g2Ui_t* u8g2Ui = new_u8g2Ui();
	
	new_u8g2Ui_starrySky(u8g2Ui,20);

	return u8g2Ui;
}

int main(void)
{
	deviceLink_init();
	delay_init();
	gpio_init();
	oled_u8g2Ui_init(u8g2Ui_found());
	matrixKey_init_def(); // 矩阵键盘初始化

	while (1)
	{
		deviceLink_loop();
	}
}
