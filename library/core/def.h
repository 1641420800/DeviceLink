#ifndef _DEF_H_
#define _DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

// 版本号
#define CORE_VERSION "0.0.1-alpha"


#define CORE_malloc malloc
#define CORE_free free

// 主题最大长度
#define CORE_TOPIC_MAX_LEN 32

// 接收缓冲队列最大长度
#define CORE_RECV_QUEUE_MAX_LEN 16


typedef enum
{
  CORE_OK = 0x00U,      /*!< 成功 */
  CORE_ERROR = 0x01U,   /*!< 失败 */
  CORE_BUSY = 0x02U,    /*!< 忙碌 */
  CORE_TIMEOUT = 0x03U, /*!< 超时 */
} CORE_StatusTypeDef;


#ifdef __cplusplus
}
#endif

#endif
