#ifndef DATA_H
#define DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"
#include <stdint.h>

    // ����������ṹ��
    typedef struct DATANODE
    {
        struct DATANODE *next;
        const char *key; // �������
        uint8_t *data;   // ����ָ��
        size_t size;     // ���ݴ�С
    } data_node_t;

    /**
     * @brief ���ݳ�ʼ��
     *
     */
    void data_init(void);

    /**
     * @brief ���������
     *
     * @param key �������
     * @param data ����ָ��
     * @param size ���ݴ�С
     *
     * @return dataItem_t* ������ָ��
     */
    data_node_t *data_add(const char *key, uint8_t *data, size_t size);

    /**
     * @brief ��ȡ������
     *
     * @param key �������
     *
     * @return dataItem_t* ������ָ��
     */
    data_node_t *data_get(const char *key);

    /**
     * @brief �ͷ�������
     *
     * @param item ������ָ��
     */
    void data_free(data_node_t *item);

#ifdef __cplusplus
}
#endif

#endif
