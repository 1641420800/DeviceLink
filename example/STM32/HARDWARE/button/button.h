#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "gpio.h"

typedef enum
{
    BUTTON_CLICKED = 0,
    BUTTON_LONG_PRESSED,
    BUTTON_DOUBLE_CLICKED
} button_event;
typedef struct
{
    char *name;
    button_event event;
    uint8_t click_count;
} button_msg_t;
typedef struct BUTTON
{
    struct BUTTON *next;
    char name[10];
    uint8_t state;
    uint16_t click_count;
    uint32_t time;
} button_t;


#define SINGLE_CLICK_TIMEOUT 400
#define LONG_PRESSED_TIMEOUT 800
#define LONG_PRESSED_MSG_INTERVAL 300

void button_init(void);

#endif
