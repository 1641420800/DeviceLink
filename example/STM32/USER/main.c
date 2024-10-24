#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "sys.h"
#include "tim.h"
#include "usart.h"
#include "delay.h"
#include "gpio.h"

#include "button.h"
#include "deviceLink.h"
#include "u8g2_ui.h"
#include "matrixKey.h"

u8g2Ui_starrySky_t *u8g2Ui_starrySky;
u8g2Ui_text_t *u8g2Ui_text;

void CORE_callback(const char *topic, void *arg, size_t siz)
{
	if (!strcmp(topic, "input/button"))
	{
		button_msg_t *button = (button_msg_t *)arg;
		u8g2Ui_text_set_text(u8g2Ui_text, "%s %d %d", button->name, button->event, button->click_count);
		if (!strcmp(button->name, "key0"))
		{
			u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky, 2);
		}
		else if (!strcmp(button->name, "key1"))
		{
			u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky, 20);
		}
		else if (!strcmp(button->name, "key2"))
		{
			u8g2Ui_starrySky_setMaximumQuantity(u8g2Ui_starrySky, 100);
		}
	}
}

u8g2Ui_t *u8g2Ui_found()
{
	u8g2Ui_t *u8g2Ui = new_u8g2Ui();

	u8g2Ui_text = new_u8g2Ui_textBuff(u8g2Ui, 40);
	u8g2Ui_starrySky = new_u8g2Ui_starrySky(u8g2Ui_text, 40);

	return u8g2Ui;
}

int main(void)
{
	deviceLink_init();
	delay_init();
	gpio_init();
	oled_u8g2Ui_init(u8g2Ui_found());
	matrixKey_init_def(); // 矩阵键盘初始化
	button_init();
	CORE_subscribe("input/button", CORE_callback);
	while (1)
	{
		deviceLink_loop();
	}
}
