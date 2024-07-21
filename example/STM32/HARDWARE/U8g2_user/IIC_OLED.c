#include "IIC_OLED.h"
#include "Delay.h"

/*
	SDA=PB8;
	SCL=PB9;
*/
#define IIC_OLED_SDA_Pin GPIO_Pin_8
#define IIC_OLED_SDA_GPIO GPIOB
#define IIC_OLED_SDA_GPIO_CLK RCC_APB2Periph_GPIOB

#define IIC_OLED_SCL_Pin GPIO_Pin_9
#define IIC_OLED_SCL_GPIO GPIOB
#define IIC_OLED_SCL_GPIO_CLK RCC_APB2Periph_GPIOB

void OLED12864_IoInit(void) // IO初始化
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};			   // 定义GPIO结构体
	RCC_APB2PeriphClockCmd(IIC_OLED_SDA_GPIO_CLK, ENABLE); // 开启GPIO模块的时钟
	RCC_APB2PeriphClockCmd(IIC_OLED_SCL_GPIO_CLK, ENABLE); // 开启GPIO模块的时钟

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE); // 关闭PA15的jtag调试功能
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SDA_Pin;	 // 配置SDA端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; // 设置GPIO的模式为输出模式					// 设置端口输出类型为：推免输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(IIC_OLED_SDA_GPIO, &GPIO_InitStructure); // 初始化GPIO为高速推免输出模式

	GPIO_InitStructure.GPIO_Pin = IIC_OLED_SCL_Pin;	   // 配置SCL端口
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   // 设置GPIO的模式为输出模式
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;  // 配置 I/O 口的速度为：50MHz，还可选：GPIO_Speed_2MHz，GPIO_Speed_10MHz
	GPIO_Init(IIC_OLED_SCL_GPIO, &GPIO_InitStructure); // 初始化GPIO为高速推免输出模式
}
void IIC_OLED_SDA_HIGH(void)
{
	GPIO_SetBits(IIC_OLED_SDA_GPIO, IIC_OLED_SDA_Pin);
}

void IIC_OLED_SDA_LOW(void)
{
	GPIO_ResetBits(IIC_OLED_SDA_GPIO, IIC_OLED_SDA_Pin);
}
void IIC_OLED_SCL_HIGH(void)
{
	GPIO_SetBits(IIC_OLED_SCL_GPIO, IIC_OLED_SCL_Pin);
}

void IIC_OLED_SCL_LOW(void)
{
	GPIO_ResetBits(IIC_OLED_SCL_GPIO, IIC_OLED_SCL_Pin);
}

uint8_t u8g2_gpio_and_delay_stm32(U8X8_UNUSED u8x8_t *u8x8, U8X8_UNUSED uint8_t msg, U8X8_UNUSED uint8_t arg_int, U8X8_UNUSED void *arg_ptr)
{
	switch (msg)
	{
	case U8X8_MSG_DELAY_MILLI: // Function which implements a delay, arg_int contains the amount of ms
		delay_ms(arg_int);
		break;

	case U8X8_MSG_DELAY_10MICRO: // Function which delays 10us
		delay_us(10);
		break;

	case U8X8_MSG_DELAY_100NANO: // Function which delays 100ns
		__NOP();
		break;
	case U8X8_MSG_GPIO_I2C_CLOCK:
		if (arg_int)
			IIC_OLED_SCL_HIGH();
		else
			IIC_OLED_SCL_LOW();
		break;
	case U8X8_MSG_GPIO_I2C_DATA:
		if (arg_int)
			IIC_OLED_SDA_HIGH();
		else
			IIC_OLED_SDA_LOW();
		break;
	default:
		return 0; // A message was received which is not implemented, return 0 to indicate an error
	}
	return 1; // command processed successfully.
}

void oled_u8g2_init(u8g2_t *u8g2)
{
	OLED12864_IoInit(); // IIC OLED 端口初始化
	u8g2_Setup_ssd1306_i2c_128x64_noname_f(u8g2, U8G2_R0, u8x8_byte_sw_i2c, u8g2_gpio_and_delay_stm32);
	u8g2_InitDisplay(u8g2);
	u8g2_SetPowerSave(u8g2, 0);
	u8g2_ClearDisplay(u8g2);
}

#if oled_u8g2_DeviceLink_on
u8g2_t u8g2;
u8g2_menu_t u8g2_menu;

void CORE_callback_u8g2Menu(const char *topic, void *arg, size_t siz)
{
	if (!strcmp(topic, "str/char"))
	{
		u8g2_MenuInChar(&u8g2_menu, *(int *)arg);
	}
	else if (!strcmp(topic, "key/value"))
	{
		u8g2_MenuKeys(&u8g2_menu, *(u8g2_menuKeyValue_t *)arg);
	}
}
void task_u8g2Menu(void *arg)
{
	if (!strcmp(arg, "run"))
	{
		u8g2_ClearBuffer(&u8g2);
		u8g2_DrawMenu(&u8g2_menu, 0, 0, 128, 64);
		u8g2_SendBuffer(&u8g2);
	}
	else if (!strcmp(arg, "timer"))
	{
		u8g2_MenuTime_ISR(&u8g2_menu, 50);
	}
}
void oled_u8g2Menu_init(menuItem_cb menuItem)
{
	oled_u8g2_init(&u8g2);
	u8g2_SetFont(&u8g2, u8g2_font_10x20_tf);
	// u8g2_SetFont(&u8g2,u8g2_font_8x13_mf);
	u8g2_CreateMenu(&u8g2, &u8g2_menu, menuItem);
	CORE_subscribe("key/value", CORE_callback_u8g2Menu);
	CORE_subscribe("str/char", CORE_callback_u8g2Menu);
	CORE_Run_add_task(task_u8g2Menu, "run");
	CORE_Timer_add_task(task_u8g2Menu, "timer", 50);
}
#endif
