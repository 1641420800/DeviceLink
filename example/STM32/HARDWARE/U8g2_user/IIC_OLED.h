#ifndef __IIC_OLED_H__
#define __IIC_OLED_H__

#include "STM32f10x.h"
#include "u8g2.h"
#include "u8g2_menu.h"

#define oled_u8g2_DeviceLink_on 1

#if oled_u8g2_DeviceLink_on
#include "core.h"
void oled_u8g2Menu_init(void);
#else
void oled_u8g2_init(u8g2_t *u8g2);
#endif

#endif
