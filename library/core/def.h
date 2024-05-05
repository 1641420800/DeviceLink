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


typedef enum
{
  CORE_OK = 0x00U,      /*!< �ɹ� */
  CORE_ERROR = 0x01U,   /*!< ʧ�� */
  CORE_BUSY = 0x02U,    /*!< æµ */
  CORE_TIMEOUT = 0x03U, /*!< ��ʱ */
} CORE_StatusTypeDef;


#ifdef __cplusplus
}
#endif

#endif
