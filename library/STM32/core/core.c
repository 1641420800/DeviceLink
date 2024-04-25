#include "core.h"


/**
 * @brief ������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 * @param len ���ն��г���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback, size_t len)
{
    return CORE_OK;
}

/**
 * @brief ȡ��������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback)
{
    return CORE_OK;
}

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
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t siz)
{
    return CORE_OK;
}

/**
 * @brief ��ȡ��������
 * 
 * @param topics ��������
 * @param len �������鳤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_topics(char **topics, size_t *len)
{
    return CORE_OK;
}

/**
 * @brief ��ȡͨ���ٶ�
 * 
 * @param topic ����
 * @param speed ͨ���ٶ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_speed(const char *topic, float *speed)
{
    return CORE_OK;
}

/**
 * @brief ��ȡ���г���
 * 
 * @param topic ����
 * @param len ���г���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_len(const char *topic, size_t *len)
{
    return CORE_OK;
}

/**
 * @brief ��ȡ����ʣ�೤��
 * 
 * @param topic ����
 * @param len ����ʣ�೤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_remain(const char *topic, size_t *len)
{
    return CORE_OK;
}
