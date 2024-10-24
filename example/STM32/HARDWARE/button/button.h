#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "gpio.h"

typedef enum
{
    BUTTON_RELEASED = 0,
    BUTTON_CLICKED,
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
    /*
    state:
        0 : 初始或空闲状态
        1 : 按键按下开始状态
        2 : 长按状态
        3 : 单击确认状态
        4 : 双击确认状态
    */
    uint8_t state;
    uint16_t click_count;
    uint32_t time;
    uint32_t idle_time;
} button_t;

#define SINGLE_CLICK_TIMEOUT 200
#define LONG_PRESSED_TIMEOUT 800
#define LONG_PRESSED_MSG_INTERVAL 300
#define MAX_TIMEOUT (SINGLE_CLICK_TIMEOUT + LONG_PRESSED_TIMEOUT + LONG_PRESSED_MSG_INTERVAL)

void button_init(void);

#endif
