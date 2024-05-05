#ifndef _LIST_H_
#define _LIST_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"

    /**
     * @brief ����ָ��ĳ�ڵ��ָ��
     *
     * @param list ����ͷ
     * @param node �ڵ�
     *
     * @return void **
     *
     */
    void **CORE_get_last(void **list, void *node);

    /**
     * @brief ��ӽڵ�
     *
     * @param list ����ͷ
     * @param node �ڵ�
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_add_node(void **list_head, void *new_node);

    /**
     * @brief ɾ���ڵ�
     *
     * @param list ����ͷ
     * @param node �ڵ�
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_del_node(void **list_head, void *node);

#ifdef __cplusplus
}
#endif

#endif
