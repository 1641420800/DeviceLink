#include "core.h"


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
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback, size_t len)
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
 * @param siz 消息数据长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t siz)
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
 * @brief 获取队列长度
 * 
 * @param topic 主题
 * @param len 队列长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_len(const char *topic, size_t *len)
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
