#ifndef _DEF_H_
#define _DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

// 版本号
#define CORE_VERSION "0.0.1-alpha"

// 内存分配函数
#define CORE_malloc malloc

// 内存释放函数
#define CORE_free free

// 主题最大长度
#define CORE_TOPIC_MAX_LEN 32

// 消息队列最大长度
#define CORE_QUEUE_MAX_LEN 16

// 日志输出函数 todo
#define CORE_LOG(...) printf(__VA_ARGS__)

// WEAK 定义
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

  // 状态定义
  typedef enum
  {
    CORE_OK = 0x00U,      /*!< 成功 */
    CORE_ERROR = 0x01U,   /*!< 失败 */
    CORE_BUSY = 0x02U,    /*!< 忙碌 */
    CORE_TIMEOUT = 0x03U, /*!< 超时 */
    CORE_EXIST = 0x04U,   /*!< 已存在 */
  } CORE_StatusTypeDef;

  // 任务状态
  typedef enum
  {
    CORE_TASK_STOP = 0,     /* 停止 */
    CORE_TASK_RUNNING = 1,  /* 运行 */
    CORE_TASK_EXECUTING = 2 /* 执行中 */
  } CORE_TaskStatusTypeDef;

  // 回调函数
  // 订阅回调函数
  typedef void (*CORE_callback_t)(const char *topic, void *arg, size_t siz);
  // 运行回调函数
  typedef void (*run_task_t)(void *arg);
  // 定时回调函数
  typedef void (*timer_callback_t)(void *arg);

#ifdef __cplusplus
}
#endif

#endif
