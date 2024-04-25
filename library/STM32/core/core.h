#include <stddef.h>
#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

// �汾��
#define CORE_VERSION "0.0.1-alpha"

// ������󳤶�
#define CORE_TOPIC_MAX_LEN 32


// ��������ģ��״̬
typedef enum
{
  CORE_OK = 0x00U,      /*!< �ɹ� */
  CORE_ERROR = 0x01U,   /*!< ʧ�� */
  CORE_BUSY = 0x02U,    /*!< æµ */
  CORE_TIMEOUT = 0x03U, /*!< ��ʱ */
} CORE_StatusTypeDef;

typedef void (*CORE_callback_t)(const char *topic, void *arg, size_t siz);

/**
 * @brief ������Ϣ
 * 
 * @param topic ��Ϣ���⡣���Ϊ `"*"`����ʾ�����������⡣
 * @param callback ��Ϣ�ص�����
 * @param len ���ն��г���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback, size_t len);

/**
 * @brief ȡ��������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback);

/**
 * @brief ������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param siz ��Ϣ���ݳ���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t siz);

/**
 * @brief ��ȡ��������
 * 
 * @param topics ��������
 * @param len �������鳤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_topics(char **topics, size_t *len);

/**
 * @brief ��ȡͨ���ٶ�
 * 
 * @param topic ����
 * @param speed ͨ���ٶ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_speed(const char *topic, float *speed);

/**
 * @brief ��ȡ���г���
 * 
 * @param topic ����
 * @param len ���г���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_len(const char *topic, size_t *len);

/**
 * @brief ��ȡ����ʣ�೤��
 * 
 * @param topic ����
 * @param len ����ʣ�೤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_remain(const char *topic, size_t *len);

#ifdef __cplusplus
}
#endif

#endif
