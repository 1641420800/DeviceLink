#include "data.h"

data_node_t *data_list = NULL;

/**
 * @brief 创建数据节点
 * 
 * @param key 节点key
 * @param data 节点数据
 * @param size 节点数据大小
 * 
 * @return data_node_t* 节点指针
 * 
 */
data_node_t *data_new(const char *key, void *data, size_t size)
{
    if(!key || !data || size <= 0) return NULL;
    data_node_t *node = (data_node_t*)CORE_malloc(sizeof(data_node_t));
    if(node == NULL) return NULL;
    uint16_t len = strlen(key) + 1;
    if(len > CORE_KEY_MAX_LEN) len = CORE_KEY_MAX_LEN;
    node->key = CORE_malloc(len);
    if(node->key == NULL)
    {
        CORE_free(node);
        return NULL;
    }
    strncpy(node->key, key, len);
    node->data = data;
    node->size = size;
    CORE_StatusTypeDef status = CORE_add_node(&data_list, node);
    if(status != CORE_OK)
    {
        CORE_free(node->key);
        CORE_free(node);
        return NULL;
    }
    return node;
}

/**
 * @brief 查找数据节点
 * 
 * @param key 节点key
 * 
 * @return data_node_t* 节点
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
 * @brief 删除数据节点
 * 
 * @param key 节点key
 * 
 * @return CORE_StatusTypeDef 状态
 * 
 */
CORE_StatusTypeDef data_delete(const char *key)
{
    if(!key) return CORE_ERROR;
    data_node_t *node = data_find(key);
    if(!node) return CORE_ERROR;
    
    CORE_StatusTypeDef status = CORE_del_node(&data_list, node);

    if(status != CORE_OK) return CORE_ERROR;

    CORE_free(node->key);
    CORE_free(node);

    return CORE_OK;
}

/**
 * @brief 数据查询回调
 *
 * @param topic 订阅主题
 * @param arg 参数
 * @param siz 数据大小
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
 * @brief 数据初始化
 *
 */
void data_init(void)
{

    CORE_subscribe("CDataQuery", CORE_callback_data_query);
}

/**
 * @brief 添加数据项
 *
 * @param key 数据项键
 * @param data 数据指针
 * @param size 数据大小
 *
 * @return dataItem_t* 数据项指针
 */
data_node_t *data_add(const char *key, uint8_t *data, size_t size)
{
}

/**
 * @brief 获取数据项
 *
 * @param key 数据项键
 *
 * @return data_node_t* 数据项指针
 */
data_node_t *data_get(const char *key)
{
}

/**
 * @brief 释放数据项
 *
 * @param item 数据项指针
 */
void data_free(data_node_t *item)
{
}
