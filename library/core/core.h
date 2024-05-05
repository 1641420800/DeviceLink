#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "list.h"
#include "run.h"
#include "timer.h"

/**
 * @brief 订阅消息
 * 
 * @param topic 消息主题。如果为 `"*"`，表示订阅所有主题。
 * @param callback 消息回调函数
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback);

/**
 * @brief 取消订阅消息
 * 
 * @param topic 消息主题
 * @param callback 消息回调函数
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback);

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
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t messageSize, uint8_t sendImmediately);

/**
 * @brief 获取所有主题
 * 
 * @param topics 主题数组
 * @param len 主题数组长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_topics(char **topics, size_t *len);

/**
 * @brief 获取通信速度
 * 
 * @param topic 主题
 * @param speed 通信速度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_speed(const char *topic, float *speed);

/**
 * @brief 获取队列剩余长度
 * 
 * @param topic 主题
 * @param len 队列剩余长度
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_remain(const char *topic, size_t *len);

/**
 * @brief 初始化
 * 
 * @note 必须在 `main()` 函数中调用。
 * 
 */
void CORE_init(void);


#ifdef __cplusplus
}
#endif

#endif
