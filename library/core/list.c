#include "list.h"

/**
 * @brief 查找指向某节点的指针
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return void **
 * 
 */
void ** CORE_get_last(void **list,void *node)
{
    void ** ret = list;
    if(!list) return NULL;
    while(ret && *ret != node) ret = *ret;
    return ret;
}

/**
 * @brief 添加节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_add_node(void **list_head,void *new_node)
{
    void ** current_tail;
    static uint8_t lock = 0;
    if(!list_head || !new_node) return CORE_ERROR;
    if(lock) 
    {
        return CORE_BUSY;
    }
    lock = 1;
    current_tail = CORE_get_last(list_head, NULL);
    if(!current_tail)
    {
        lock = 0;
        return CORE_ERROR;
    }
    *current_tail = new_node;
    lock = 0;
    return CORE_OK;
}

/**
 * @brief 删除节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_del_node(void **list_head,void *node)
{
    void ** current_tail;
    if(!list_head || !node) return CORE_ERROR;
    current_tail = CORE_get_last(list_head, node);
    if(!current_tail || *current_tail != node) return CORE_ERROR;
    *current_tail = *(void**)node;
    return CORE_OK;
}
