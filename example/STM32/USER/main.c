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

u8g2Ui_starrySky_t * u8g2Ui_starrySky;

void CORE_callback(const char *topic, void *arg, size_t siz)
{
	if (!strcmp(topic, "input/key"))
	{
		switch(*(int*)arg)
		{
			case 0:
				u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky,2);
				break;
			case 1:
				u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky,20);
				break;
			case 2:
				u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky,40);
				break;
		}
	}
}

u8g2Ui_t* u8g2Ui_found()
{
	u8g2Ui_t* u8g2Ui = new_u8g2Ui();
	
	u8g2Ui_starrySky = new_u8g2Ui_starrySky(u8g2Ui,40);

	return u8g2Ui;
}

int main(void)
{
	deviceLink_init();
	delay_init();
	gpio_init();
	oled_u8g2Ui_init(u8g2Ui_found());
	matrixKey_init_def(); // 矩阵键盘初始化

	CORE_subscribe("input/key",CORE_callback);
	while (1)
	{
		deviceLink_loop();
	}
}
