#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"

#define LED PCout(13)

#define KEY_1 PAin(0)
#define KEY_2 PAin(1)
#define KEY_3 PAin(2)

#define GPIO_DeviceLink_on 1
#define GPIO_u8g2Menu_on 1

#if GPIO_DeviceLink_on
#include "core.h"
#endif
#if GPIO_u8g2Menu_on
#include "u8g2_menu.h"
#endif

void gpio_init(void);

#endif
