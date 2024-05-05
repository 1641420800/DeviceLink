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

#ifndef WEAK
#ifdef __CC_ARM
#define WEAK __weak
#elif defined(__IAR_SYSTEMS_ICC__)
#define WEAK __weak
#elif defined(__GNUC__)
#define WEAK __attribute__((weak))
#elif defined(__ADSPBLACKFIN__)
#define WEAK __attribute__((weak))
#elif defined(_MSC_VER)
#define WEAK
#elif defined(__TI_COMPILER_VERSION__)
#define WEAK
#else
#error not supported tool chain
#endif
#endif

  typedef enum
  {
    CORE_OK = 0x00U,      /*!< 成功 */
    CORE_ERROR = 0x01U,   /*!< 失败 */
    CORE_BUSY = 0x02U,    /*!< 忙碌 */
    CORE_TIMEOUT = 0x03U, /*!< 超时 */
  } CORE_StatusTypeDef;

  typedef enum
  {
    CORE_TASK_STOP = 0,     /* 停止 */
    CORE_TASK_RUNNING = 1,  /* 运行 */
    CORE_TASK_EXECUTING = 2 /* 执行中 */
  } CORE_TaskStatusTypeDef;

  typedef void (*CORE_callback_t)(const char *topic, void *arg, size_t siz);
  typedef void (*run_task_t)(void *arg);
  typedef void (*timer_callback_t)(void *arg);

#ifdef __cplusplus
}
#endif

#endif
