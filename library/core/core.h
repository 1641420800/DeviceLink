#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define _DeviceLink_

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "list.h"
#include "run.h"
#include "timer.h"
#include "data.h"

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
    CORE_StatusTypeDef CORE_publish_str(const char *topic, const char *arg, uint8_t sendImmediately);

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
    CORE_StatusTypeDef CORE_publish_int(const char *topic, int arg, uint8_t sendImmediately);

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
    CORE_StatusTypeDef CORE_publish_float(const char *topic, float arg, uint8_t sendImmediately);

    /**
     * @brief 获取通信速度
     *
     * @param topic 主题
     * @param speed 通信速度
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_speed(const char *topic, uint16_t *speed);

    /**
     * @brief 初始化
     *
     * @note 必须在 `main()` 函数中调用。
     *
     */
    void CORE_init(void);

    /**
     * @brief 获取所有话题的列表
     *
     * @param head 话题需要包含的字符串 NULL为继续匹配
     *
     * @return char * 本次匹配到的话题
     *
     */
    CORE_task_list_t *CORE_getTopic_list(char *head);
#ifdef __cplusplus
}
#endif

#endif
