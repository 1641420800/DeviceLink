#include "list.h"

/**
 * @brief ����ָ��ĳ�ڵ��ָ��
 * 
 * @param list ����ͷ
 * @param node �ڵ�
 * 
 * @return void **
 * 
 */
void ** CORE_get_last(void **list,void *node)
{
    void ** ret = list;
    if(!list) return NULL;
    while(*ret != node) ret = *ret;
    return ret;
}

/**
 * @brief ��ӽڵ�
 * 
 * @param list ����ͷ
 * @param node �ڵ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_add_node(void **list_head,void *new_node)
{
    void ** current_tail;
    if(!list_head || !new_node) return CORE_ERROR;
    current_tail = CORE_get_last(list_head, NULL);
    if(!current_tail) return CORE_ERROR;
    *current_tail = new_node;
    return CORE_OK;
}

/**
 * @brief ɾ���ڵ�
 * 
 * @param list ����ͷ
 * @param node �ڵ�
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
