#ifndef DATA_H
#define DATA_H

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"
#include <stdint.h>

    // 定义数据项结构体
    typedef struct DATANODE
    {
        struct DATANODE *next;
        const char *key; // 数据项键
        uint8_t *data;   // 数据指针
        size_t size;     // 数据大小
    } data_node_t;

    /**
     * @brief 数据初始化
     *
     */
    void data_init(void);

    /**
     * @brief 添加数据项
     *
     * @param key 数据项键
     * @param data 数据指针
     * @param size 数据大小
     *
     * @return dataItem_t* 数据项指针
     */
    data_node_t *data_add(const char *key, uint8_t *data, size_t size);

    /**
     * @brief 获取数据项
     *
     * @param key 数据项键
     *
     * @return dataItem_t* 数据项指针
     */
    data_node_t *data_get(const char *key);

    /**
     * @brief 释放数据项
     *
     * @param item 数据项指针
     */
    void data_free(data_node_t *item);

#ifdef __cplusplus
}
#endif

#endif
