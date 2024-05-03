#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"

/**
 * @brief 查找指向某节点的指针
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return void **
 * 
 */
void ** CORE_get_last(void **list,void *node);


/**
 * @brief 添加节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_add_node(void **list_head,void *new_node);


/**
 * @brief 删除节点
 * 
 * @param list 链表头
 * @param node 节点
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_del_node(void **list_head,void *node);

#ifdef __cplusplus
}
#endif

#endif
