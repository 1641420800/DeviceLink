#include "gpio.h"

void task_gpio_key(void *arg)
{
	key_t *key = (key_t *)arg;
	key_msg_t msg;

	msg.name = key->name;

	uint8_t val = GPIO_ReadInputDataBit(key->GPIOx, key->GPIO_Pin);

	if (val == 0 && key->valLog != 0)
	{
		msg.event = KEY_DOWN;
		CORE_publish("input/key", &msg, sizeof(msg), 0);
	}
	if (val != 0 && key->valLog == 0)
	{
		msg.event = KEY_UP;
		CORE_publish("input/key", &msg, sizeof(msg), 0);
	}

	key->valLog = val;
}

void CORE_key_init(char *key_name, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
	// 检查输入参数是否为空
	if (!key_name || !GPIOx)
		return;
	// 验证GPIOx和GPIO_Pin参数的有效性
	assert_param(IS_GPIO_ALL_PERIPH(GPIOx));
	assert_param(IS_GPIO_PIN(GPIO_Pin));

	key_t *key = (key_t *)CORE_malloc(sizeof(key_t));
	// 检查内存分配是否成功
	if (!key)
		return;

	key->GPIOx = GPIOx;
	key->GPIO_Pin = GPIO_Pin;
	key->valLog = 1;
	strcpy(key->name, key_name);

	// 使能PA、PB、PC端口时钟以及AFIO时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	// 禁用JTAG，释放PA13, PA14, PA15引脚
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_StructInit(&GPIO_InitStruct);
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	// 根据设定参数初始化GPIO
	GPIO_Init(GPIOx, &GPIO_InitStruct);

	// 将按键任务添加到定时器任务列表中
	CORE_Timer_add_task(task_gpio_key, key, 20);
}
void gpio_init(void)
{
	CORE_key_init("key0", GPIOA, GPIO_Pin_0);
	CORE_key_init("key1", GPIOA, GPIO_Pin_1);
	CORE_key_init("key2", GPIOA, GPIO_Pin_2);
}
