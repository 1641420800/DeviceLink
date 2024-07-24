#include "core.h"

typedef struct CORE_MSG_LIST
{
    struct CORE_MSG_LIST *next; // 指向下一个消息节点的指针
    size_t data_len;            // 数据长度
    uint8_t data[0];            // 动态存储数据，向下增长
} CORE_msg_list_t;

typedef struct CORE_CALLBACK_LIST
{
    struct CORE_CALLBACK_LIST *next; // 指向下一个回调节点的指针
    CORE_callback_t callback;        // 回调函数
    uint16_t runTime_us;             // 回调函数运行时间
} CORE_callback_list_t;

typedef struct CORE_TASK_LIST
{
    struct CORE_TASK_LIST *next;         // 指向下一个任务节点的指针
    char topic[CORE_TOPIC_MAX_LEN + 1];  // 主题字符串
    CORE_callback_list_t *callback_list; // 回调函数列表指针
    CORE_msg_list_t *msg_list;           // 消息列表指针
    uint16_t msg_list_len;               // 消息列表长度

    uint16_t msg_count; // 消息计数
    uint16_t msg_speed; // 消息速率
} CORE_task_list_t;

// 全局任务列表指针
CORE_task_list_t *CORE_task_list = NULL;
char currentTopic[CORE_TOPIC_MAX_LEN + 1]; // 主题字符串
uint16_t currentNodeCount = 0;

/**
 * @brief 新建回调链表节点
 *
 * @param callback 回调函数
 *
 * @return CORE_callback_list_t *
 *
 */
CORE_callback_list_t *CORE_new_callback_list(CORE_callback_t callback)
{
    CORE_callback_list_t *callback_list;
    if (!callback)
        return NULL;
    callback_list = (CORE_callback_list_t *)CORE_malloc(sizeof(CORE_callback_list_t));
    if (!callback_list)
        return NULL;
    memset(callback_list, 0, sizeof(CORE_callback_list_t));
    callback_list->callback = callback;
    ++currentNodeCount;
    return callback_list;
}

/**
 * @brief 查找回调链表节点
 *
 * @param callback_list 回调链表节点
 * @param callback 回调函数
 *
 * @return CORE_callback_list_t *
 *
 */
CORE_callback_list_t *CORE_find_callback_list(CORE_callback_list_t *callback_list, CORE_callback_t callback)
{
    while (callback_list)
    {
        if (callback_list->callback == callback)
            return callback_list;
        callback_list = callback_list->next;
    }
    return NULL;
}

/**
 * @brief 释放回调链表节点
 *
 * @param callback_list 回调链表节点
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_callback_list(CORE_callback_list_t *callback_list)
{
    if (!callback_list)
        return CORE_ERROR;
    CORE_free(callback_list);
    --currentNodeCount;
    return CORE_OK;
}

/**
 * @brief 新建消息节点
 *
 * @param data 消息数据
 * @param data_len 消息数据长度
 *
 * @return CORE_msg_t *
 *
 */
CORE_msg_list_t *CORE_new_msg(const char *topic, uint8_t *data, size_t data_len)
{
    CORE_msg_list_t *msg;
    if (!topic || !data || data_len <= 0)
        return NULL;
    msg = (CORE_msg_list_t *)CORE_malloc(sizeof(CORE_msg_list_t) + data_len);
    if (!msg)
        return NULL;
    msg->next = NULL;
    msg->data_len = data_len;
    memcpy(msg->data, data, data_len);
    ++currentNodeCount;
    return msg;
}

/**
 * @brief 释放消息节点
 *
 * @param msg 消息节点
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_msg(CORE_msg_list_t *msg)
{
    if (!msg)
        return CORE_ERROR;
    CORE_free(msg);
    --currentNodeCount;
    return CORE_OK;
}

/**
 * @brief 新建任务节点
 *
 * @param topic 消息主题
 *
 * @return CORE_task_t *
 *
 */
CORE_task_list_t *CORE_new_task(const char *topic)
{
    CORE_task_list_t *task;
    if (!topic)
        return NULL;
    task = (CORE_task_list_t *)CORE_malloc(sizeof(CORE_task_list_t));
    if (!task)
        return NULL;
    memset(task, 0, sizeof(CORE_task_list_t));
    strncpy(task->topic, topic, CORE_TOPIC_MAX_LEN); // 复制主题字符串 超过最大长度时截断
    task->topic[CORE_TOPIC_MAX_LEN] = '\0';
    ++currentNodeCount;
    return task;
}

/**
 * @brief 查找任务节点
 *
 * @param topic 消息主题
 *
 * @return CORE_task_t *
 *
 */
CORE_task_list_t *CORE_find_task(const char *topic)
{
    CORE_task_list_t *task = CORE_task_list;
    if (!topic)
        return NULL;
    while (task)
    {
        if (strcmp(task->topic, topic) == 0)
            return task;
        task = task->next;
    }
    return NULL;
}

/**
 * @brief 释放任务节点
 *
 * @param task 任务
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_task(CORE_task_list_t *task)
{
    CORE_callback_list_t *callback_list;
    CORE_msg_list_t *msg_list;
    if (!task)
        return CORE_ERROR;

    while (task->callback_list)
    {
        callback_list = task->callback_list;
        task->callback_list = task->callback_list->next;
        CORE_free_callback_list(callback_list);
    }
    while (task->msg_list)
    {
        msg_list = task->msg_list;
        task->msg_list = task->msg_list->next;
        CORE_free_msg(msg_list);
    }

    CORE_free(task);
    --currentNodeCount;
    return CORE_OK;
}

/**
 * @brief 调用回调函数
 *
 * @param callback 回调函数
 * @param topic 消息主题
 * @param arg 消息数据
 * @param siz 消息数据长度
 *
 * @todo 计时部分
 *
 */
void CORE_call_callback(CORE_callback_list_t *callback, const char *topic, void *arg, size_t siz)
{
    uint32_t tim;
    if (!callback || !callback->callback)
        return;
    strcpy(currentTopic, topic);
    // 开始计时
    tim = CORE_Timer_GetCurrentTime_us();
    callback->callback(topic, arg, siz);
    tim = CORE_Timer_GetCurrentTime_us() - tim;
    // 溢出纠正 todo
    callback->runTime_us = tim;

    strcpy(currentTopic, "");

    // 结束计时
}

/**
 * @brief 订阅消息
 *
 * @param topic 消息主题
 * @param callback 消息回调函数
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback)
{
    CORE_task_list_t *task;
    if (!topic || !callback)
        return CORE_ERROR;
    if (strcmp(topic, "CORE_subscribe") && strcmp(currentTopic, "CORE_subscribe"))
        CORE_publish_str("CORE_subscribe", topic, 0);
    task = CORE_find_task(topic);
    if (!task)
    {
        task = CORE_new_task(topic);
        if (!task)
            return CORE_ERROR;
        CORE_add_node((void **)&CORE_task_list, (void *)task);
    }
    else
    {
        if (CORE_find_callback_list(task->callback_list, callback))
        {
            return CORE_EXIST;
        }
    }
    CORE_add_node((void **)&task->callback_list, (void *)CORE_new_callback_list(callback));
    return CORE_OK;
}

/**
 * @brief 取消订阅消息
 *
 * @param topic 消息主题
 * @param callback 消息回调函数
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback)
{
    CORE_task_list_t *task;
    CORE_callback_list_t *callback_list;
    if (!topic || !callback)
        return CORE_ERROR;
    if (strcmp(topic, "CORE_unsubscribe") && strcmp(currentTopic, "CORE_unsubscribe"))
        CORE_publish_str("CORE_unsubscribe", topic, 0);
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    callback_list = CORE_find_callback_list(task->callback_list, callback);
    if (!callback_list)
        return CORE_ERROR;
    CORE_del_node((void **)&task->callback_list, (void *)callback_list);
    CORE_free_callback_list(callback_list);
    if (!task->callback_list)
    {
        CORE_del_node((void **)&CORE_task_list, (void *)task);
        CORE_free_task(task);
    }
    return CORE_OK;
}

/**
 * @brief 发布消息
 *
 * @param topic 消息主题
 * @param arg 消息数据
 * @param messageSize 消息数据长度
 * @param sendImmediately  是否立即发送
 *
 * @note 如果 sendImmediately  为 true，则立即发送，否则将放入队列。
 *
 * @warning 在特权模式下(IRQ)下谨慎使用立即发送功能。
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t messageSize, uint8_t sendImmediately)
{
    CORE_msg_list_t *msg;
    CORE_task_list_t *task;
    CORE_callback_list_t *callback_list;
    CORE_StatusTypeDef status;

    if (!topic || !arg || !messageSize)
        return CORE_ERROR;
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    if (sendImmediately)
    { // 立即发送
        callback_list = task->callback_list;
        while (callback_list)
        {
            CORE_call_callback(callback_list, topic, arg, messageSize);
            callback_list = callback_list->next;
            ++task->msg_count;
        }
    }
    else
    { // 放入队列
        if (task->msg_list_len >= CORE_QUEUE_MAX_LEN)
            return CORE_BUSY;
        msg = CORE_new_msg(topic, arg, messageSize);
        if (!msg)
            return CORE_ERROR;
        status = CORE_add_node((void **)&task->msg_list, (void *)msg);
        if (status != CORE_OK)
        {
            CORE_free_msg(msg);
            return CORE_ERROR;
        }
        task->msg_list_len++;
    }

    return CORE_OK;
}

/**
 * @brief 发布字符串消息
 *
 * @param topic 消息主题
 * @param arg 消息数据
 * @param sendImmediately  是否立即发送
 *
 * @note 如果 sendImmediately  为 true，则立即发送，否则将放入队列。
 *
 * @warning 在特权模式下(IRQ)下谨慎使用立即发送功能。
 *
 */
CORE_StatusTypeDef CORE_publish_str(const char *topic, const char *arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)arg, strlen(arg) + 1, sendImmediately);
}

/**
 * @brief 发布整型消息
 *
 * @param topic 消息主题
 * @param arg 消息数据
 * @param sendImmediately  是否立即发送
 *
 * @note 如果 sendImmediately  为 true，则立即发送，否则将放入队列。
 *
 * @warning 在特权模式下(IRQ)下谨慎使用立即发送功能。
 *
 */
CORE_StatusTypeDef CORE_publish_int(const char *topic, int arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)&arg, sizeof(int), sendImmediately);
}

/**
 * @brief 发布浮点型消息
 *
 * @param topic 消息主题
 * @param arg 消息数据
 * @param sendImmediately  是否立即发送
 *
 * @note 如果 sendImmediately  为 true，则立即发送，否则将放入队列。
 *
 * @warning 在特权模式下(IRQ)下谨慎使用立即发送功能。
 *
 */
CORE_StatusTypeDef CORE_publish_float(const char *topic, float arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)&arg, sizeof(float), sendImmediately);
}

/**
 * @brief 获取通信速度
 *
 * @param topic 主题
 * @param speed 通信速度
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_speed(const char *topic, uint16_t *speed)
{
    CORE_task_list_t *task;
    if (!topic || !speed)
        return CORE_ERROR;
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    *speed = task->msg_speed;
    return CORE_OK;
}

/**
 * @brief 执行核心运行循环，处理任务列表中的消息。
 *
 * @param arg 传递给函数的参数，本函数中未使用。
 * 
 */
void CORE_run(void *arg)
{
    // 遍历任务列表
    CORE_task_list_t *task = CORE_task_list;

    while (task)
    {
        CORE_msg_list_t *msg = task->msg_list;
        task->msg_list = NULL;
        while (msg)
        {
            CORE_msg_list_t *msg_free = msg;
            CORE_callback_list_t *callback_list = task->callback_list;
            msg = msg->next;
            while (callback_list)
            {
                CORE_call_callback(callback_list, task->topic, msg_free->data, msg_free->data_len);
                callback_list = callback_list->next;
                ++task->msg_count;
            }
            CORE_free_msg(msg_free);
            task->msg_list_len--;
        }
        task = task->next;
    }
}

/**
 * @brief 运行计时器
 *
 * @param arg 传递给函数的参数，本函数中未使用。
 *
 */
void CORE_run_timer(void *arg)
{
    CORE_task_list_t *task = CORE_task_list;
    while (task)
    {
        task->msg_speed = task->msg_count;
        task->msg_count = 0;
        task = task->next;
    }
}

/**
 * @brief 初始化
 *
 * @note 必须在 `main()` 函数中调用。
 *
 */
void CORE_init(void)
{
    CORE_Run_add_task(CORE_run, NULL);
    CORE_Timer_add_task(CORE_run_timer, NULL, 1000);
}
