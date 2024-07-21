#include "data.h"

data_node_t *data_list = NULL;

/**
 * @brief �������ݽڵ�
 * 
 * @param key �ڵ�key
 * @param data �ڵ�����
 * @param size �ڵ����ݴ�С
 * 
 * @return data_node_t* �ڵ�ָ��
 * 
 */
data_node_t *data_new(const char *key, void *data, size_t size)
{
    if(!key || !data || size <= 0) return NULL;
    data_node_t *node = (data_node_t*)CORE_malloc(sizeof(data_node_t));
    if(node == NULL) return NULL;
    uint16_t len = strlen(key) + 1;
    if(len > CORE_KEY_MAX_LEN) len = CORE_KEY_MAX_LEN;
		char *_key = CORE_malloc(len);
    if(_key == NULL)
    {
        CORE_free(node);
        return NULL;
    }
    strncpy(_key, key, len);
    node->data = data;
    node->size = size;
		node->key  = _key;
    CORE_StatusTypeDef status = CORE_add_node((void**)&data_list, node);
    if(status != CORE_OK)
    {
        CORE_free(_key);
        CORE_free(node);
        return NULL;
    }
    return node;
}

/**
 * @brief �������ݽڵ�
 * 
 * @param key �ڵ�key
 * 
 * @return data_node_t* �ڵ�
 * 
 */
data_node_t *data_find(const char *key)
{
    data_node_t *data_node = data_list;

    while(data_node != NULL)
    {
        if(strcmp(data_node->key, key) == 0)
        {
            return data_node;
        }
        data_node = data_node->next;
    }
    return NULL;
} 

/**
 * @brief ɾ�����ݽڵ�
 * 
 * @param key �ڵ�key
 * 
 * @return CORE_StatusTypeDef ״̬
 * 
 */
CORE_StatusTypeDef data_delete(const char *key)
{
    if(!key) return CORE_ERROR;
    data_node_t *node = data_find(key);
    if(!node) return CORE_ERROR;
    
    CORE_StatusTypeDef status = CORE_del_node((void**)&data_list, node);

    if(status != CORE_OK) return CORE_ERROR;

    CORE_free((void*)node->key);
    CORE_free(node);

    return CORE_OK;
}

/**
 * @brief ���ݲ�ѯ�ص�
 *
 * @param topic ��������
 * @param arg ����
 * @param siz ���ݴ�С
 *
 */
void CORE_callback_data_query(const char *topic, void *arg, size_t siz)
{
    data_node_t *data = arg;
    data = data_find(data->key);
    if(data == NULL) 
    {
        data = arg; 
        data->data = NULL;
        data->size = 0;
        return;
    }
    
}

/**
 * @brief ���ݳ�ʼ��
 *
 */
void data_init(void)
{

    CORE_subscribe("CDataQuery", CORE_callback_data_query);
}

/**
 * @brief ���������
 *
 * @param key �������
 * @param data ����ָ��
 * @param size ���ݴ�С
 *
 * @return dataItem_t* ������ָ��
 */
data_node_t *data_add(const char *key, uint8_t *data, size_t size)
{
	return NULL;
}

/**
 * @brief ��ȡ������
 *
 * @param key �������
 *
 * @return data_node_t* ������ָ��
 */
data_node_t *data_get(const char *key)
{
	return NULL;
}

/**
 * @brief �ͷ�������
 *
 * @param item ������ָ��
 */
void data_free(data_node_t *item)
{
}
