#include "button.h"

button_t *button_list = NULL;

button_t *button_find(const char *name)
{
    button_t *node = button_list;

    while (node != NULL)
    {
        if (!strcmp(node->name, name))
            return node;
        node = node->next;
    }
    return NULL;
}

button_t *button_new(const char *name)
{
    button_t *node = (button_t *)malloc(sizeof(button_t));
    if (!node)
        return NULL;
    memset(node, 0, sizeof(button_t));
    strcpy(node->name, name);
    node->state = 1;
    node->next = button_list;
    button_list = node;
    return node;
}

void CORE_callback_button(const char *topic, void *arg, size_t siz)
{
    key_msg_t *key = (key_msg_t *)arg;

    button_t *node = button_find(key->name);
    if (!node)
    {
        node = button_new(key->name);
        if (!node)
            return;
    }

    if (key->event == KEY_DOWN)
    {
        node->click_count++;
        if (node->state == 0 && node->time < SINGLE_CLICK_TIMEOUT)
        {
            node->state = 3;
            // 双击
            button_msg_t msg = {node->name, BUTTON_DOUBLE_CLICKED, node->click_count};
            CORE_publish("input/button", &msg, sizeof(msg), 0);
        }
        else
        {
            node->state = 1;
        }
        node->time = 0;
    }
    else if (key->event == KEY_UP)
    {
        if (node->state == 1 && node->time < LONG_PRESSED_TIMEOUT)
        {
            // 单击
            button_msg_t msg = {node->name, BUTTON_CLICKED, node->click_count};
            CORE_publish("input/button", &msg, sizeof(msg), 0);
        }
        node->state = 0;
        node->time = 0;
    }
}

void button_tick(void *arg)
{
    button_t *node = button_list;
    while (node != NULL)
    {
        node->time += 100;
        if ((node->state == 1 || node->state == 2) && node->time >= LONG_PRESSED_TIMEOUT)
        {
            node->state = 2;
            node->time = LONG_PRESSED_TIMEOUT - LONG_PRESSED_MSG_INTERVAL;
            // 长按
            button_msg_t msg = {node->name, BUTTON_LONG_PRESSED, node->click_count};
            CORE_publish("input/button", &msg, sizeof(msg), 0);
        }
        else if (node->state == 0 && node->time >= SINGLE_CLICK_TIMEOUT)
        {
            node->click_count = 0;
        }
        node = node->next;
    }
}

void button_init(void)
{
    static uint8_t button_inited = 0;
    if (button_inited)
        return;
    button_inited = 1;
    CORE_subscribe("input/key", CORE_callback_button);
    CORE_Timer_add_task(button_tick, NULL, 100);
}
