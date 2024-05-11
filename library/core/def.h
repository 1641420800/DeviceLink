#ifndef _DEF_H_
#define _DEF_H_

#ifdef __cplusplus
extern "C"
{
#endif

// �汾��
#define CORE_VERSION "0.0.1-alpha"

// �ڴ���亯��
#define CORE_malloc malloc

// �ڴ��ͷź���
#define CORE_free free

// ������󳤶�
#define CORE_TOPIC_MAX_LEN 32

// ��Ϣ������󳤶�
#define CORE_QUEUE_MAX_LEN 16

// ���ݼ�ֵ��󳤶�
#define CORE_KEY_MAX_LEN 32

// ��־������� todo
#define CORE_LOG(...) printf(__VA_ARGS__)

// WEAK ����
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

  // ״̬����
  typedef enum
  {
    CORE_OK = 0x00U,      /*!< �ɹ� */
    CORE_ERROR = 0x01U,   /*!< ʧ�� */
    CORE_BUSY = 0x02U,    /*!< æµ */
    CORE_TIMEOUT = 0x03U, /*!< ��ʱ */
    CORE_EXIST = 0x04U,   /*!< �Ѵ��� */
  } CORE_StatusTypeDef;

  // ����״̬
  typedef enum
  {
    CORE_TASK_STOP = 0,     /* ֹͣ */
    CORE_TASK_RUNNING = 1,  /* ���� */
    CORE_TASK_EXECUTING = 2 /* ִ���� */
  } CORE_TaskStatusTypeDef;

  // �ص�����
  // ���Ļص�����
  typedef void (*CORE_callback_t)(const char *topic, void *arg, size_t siz);
  // ���лص�����
  typedef void (*run_task_t)(void *arg);
  // ��ʱ�ص�����
  typedef void (*timer_callback_t)(void *arg);

#ifdef __cplusplus
}
#endif

#endif
