#include "core.h"

#include <stdio.h>
#define CORE_malloc malloc
#define CORE_free free


typedef struct CORE_msg_list
{
    struct CORE_msg_list * next;     // 指向下一个消息节点的指针
    size_t data_len;                 // 数据长度
    uint8_t data[0];                 // 动态存储数据，向下增长
} CORE_msg_list_t;

typedef struct CORE_callback_list
{
    struct CORE_callback_list * next; // 指向下一个回调节点的指针
    CORE_callback_t callback;         // 回调函数
} CORE_callback_list_t;

typedef struct CORE_task_list
{
    struct CORE_task_list * next;           // 指向下一个任务节点的指针
    char topic[CORE_TOPIC_MAX_LEN + 1];     // 主题字符串
    CORE_callback_list_t * callback_list;   // 回调函数列表指针
    CORE_msg_list_t * msg_list;             // 消息列表指针
    
    uint16_t msg_count;                     // 消息计数
    float msg_speed;                        // 消息速率
} CORE_task_list_t;

// 全局任务列表指针
CORE_task_list_t * CORE_task_list = NULL;


/**
 * @brief 新建回调链表节点
 * 
 * @param callback 回调函数
 * 
 * @return CORE_callback_list_t *
 * 
 */
CORE_callback_list_t * CORE_new_callback_list(CORE_callback_t callback)
{
    CORE_callback_list_t * callback_list;
    if(!callback) return NULL;
    callback_list = (CORE_callback_list_t *)CORE_malloc(sizeof(CORE_callback_list_t));
    if(!callback_list) return NULL;
    memset(callback_list,0,sizeof(CORE_callback_list_t));
    callback_list->callback = callback;
    return callback_list;
}

/**
 * @brief 释放回调链表节点
 * 
 * @param callback_list 回调链表节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_free_callback_list(CORE_callback_list_t * callback_list)
{
    if(!callback_list || callback_list->next) return CORE_ERROR;
    CORE_free(callback_list);
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
CORE_msg_list_t * CORE_new_msg(uint8_t *data, size_t data_len)
{
    CORE_msg_list_t * msg;
    if(!data || data_len <= 0) return NULL;
    msg = (CORE_msg_list_t *)CORE_malloc(sizeof(CORE_msg_list_t) + data_len);
    if(!msg) return NULL;
    msg->next = NULL;
    msg->data_len = data_len;
    memcpy(msg->data,data,data_len);
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
CORE_StatusTypeDef CORE_free_msg(CORE_msg_list_t * msg)
{
    if(!msg || msg->next) return CORE_ERROR;
    CORE_free(msg);
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
CORE_task_list_t * CORE_new_task(const char *topic)
{
    CORE_task_list_t * task;
    if(!topic) return NULL;
    task = (CORE_task_list_t *)CORE_malloc(sizeof(CORE_task_list_t));
    if(!task) return NULL;
    memset(task,0,sizeof(CORE_task_list_t));
    strncpy(task->topic,topic,CORE_TOPIC_MAX_LEN);
    task->topic[CORE_TOPIC_MAX_LEN] = '\0';
    return task;
}

/**
 * @brief 释放任务节点
 * 
 * @param task 任务
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_free_task(CORE_task_list_t * task)
{
    CORE_callback_list_t * callback_list;
    CORE_msg_list_t * msg_list;
    if(!task || task->next) return CORE_ERROR;

    while(task->callback_list)
    {
        callback_list = task->callback_list;
        task->callback_list = task->callback_list->next;
        task->callback_list->next = NULL;
        CORE_free_callback_list(callback_list);
    }
    while(task->msg_list)
    {
        msg_list = task->msg_list;
        task->msg_list = task->msg_list->next;
        task->msg_list->next = NULL;
        CORE_free_msg(msg_list);
    }

    CORE_free(task);
    return CORE_OK;
}

/**
 * @brief 添加节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_add_node(void **list,void *node)
{
    if(!list || !node) return CORE_ERROR;
    

    return CORE_OK;
}

/**
 * @brief 删除节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_del_node(void **list,void *node)
{
    return CORE_OK;
}


/**
 * @brief 订阅消息
 * 
 * @param topic 消息主题
 * @param callback 消息回调函数
 * @param len 接收队列长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback)
{
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
    return CORE_OK;
}

/**
 * @brief 获取所有主题
 * 
 * @param topics 主题数组
 * @param len 主题数组长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_topics(char **topics, size_t *len)
{
    return CORE_OK;
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
CORE_StatusTypeDef CORE_speed(const char *topic, float *speed)
{
    return CORE_OK;
}

/**
 * @brief 获取队列剩余长度
 * 
 * @param topic 主题
 * @param len 队列剩余长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_remain(const char *topic, size_t *len)
{
    return CORE_OK;
}
