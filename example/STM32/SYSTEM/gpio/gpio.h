#ifndef __GPIO_H
#define __GPIO_H
#include "sys.h"

#define LED PCout(13)

#include "core.h"
#include "u8g2_ui.h"

typedef struct {
    GPIO_TypeDef* GPIOx;
    uint16_t GPIO_Pin;
    char name[10];
    uint8_t valLog;
} key_t;

typedef enum { KEY_DOWN = 0, KEY_UP } key_event;
typedef struct {
    char *name;
    key_event event;
} key_msg_t;



void CORE_key_init(char *key_name, GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin);
void gpio_init(void);

#endif
