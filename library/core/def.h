#ifndef _DEF_H_
#define _DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

// �汾��
#define CORE_VERSION "0.0.1-alpha"

#define CORE_malloc malloc
#define CORE_free free

// ������󳤶�
#define CORE_TOPIC_MAX_LEN 32

// ���ջ��������󳤶�
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
    CORE_OK = 0x00U,      /*!< �ɹ� */
    CORE_ERROR = 0x01U,   /*!< ʧ�� */
    CORE_BUSY = 0x02U,    /*!< æµ */
    CORE_TIMEOUT = 0x03U, /*!< ��ʱ */
  } CORE_StatusTypeDef;

  typedef enum
  {
    CORE_TASK_STOP = 0,     /* ֹͣ */
    CORE_TASK_RUNNING = 1,  /* ���� */
    CORE_TASK_EXECUTING = 2 /* ִ���� */
  } CORE_TaskStatusTypeDef;

  typedef void (*CORE_callback_t)(const char *topic, void *arg, size_t siz);
  typedef void (*run_task_t)(void *arg);
  typedef void (*timer_callback_t)(void *arg);

#ifdef __cplusplus
}
#endif

#endif
