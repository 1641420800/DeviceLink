#include "gpio.h"

#if GPIO_DeviceLink_on && GPIO_u8g2Menu_on
void task_gpio_keys(void *arg)
{
	static uint8_t keyLog[4] = {0};
	uint8_t key[4];

	key[0] = KEY_1;
	key[1] = KEY_2;
	key[2] = KEY_3;

	if (key[0] == 0 && keyLog[0] != 0)
	{
		CORE_publish_int("key/value", MENU_Key_Up, 0);
	}
	if (key[1] == 0 && keyLog[1] != 0)
	{
		CORE_publish_int("key/value", MENU_Key_Down, 0);
	}
	if (key[2] == 0 && keyLog[2] != 0)
	{
		CORE_publish_int("key/value", MENU_Key_Enter, 0);
	}

	keyLog[0] = key[0];
	keyLog[1] = key[1];
	keyLog[2] = key[2];
}
#endif

void gpio_init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE); // 使能PA端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); // 使能PB端口时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); // 使能PC端口时钟

	GPIO_SetBits(GPIOC, GPIO_Pin_13); // 输出高

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;		  // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  // 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; // IO口速度为50MHz
	GPIO_Init(GPIOC, &GPIO_InitStructure);			  // 根据设定参数初始化GPIOC

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2; // 端口配置
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;						// 推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;					// IO口速度为50MHz
	GPIO_Init(GPIOA, &GPIO_InitStructure);								// 根据设定参数初始化GPIOC

#if GPIO_DeviceLink_on && GPIO_u8g2Menu_on
	CORE_Timer_add_task(task_gpio_keys, NULL, 20);
#endif
}
