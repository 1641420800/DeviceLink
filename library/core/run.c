#include "run.h"

run_task_node_t * run_task_list = NULL;

/**
 * @brief ��������ѭ������ڵ�
 * 
 * @param task ������
 * @param arg ����������
 * 
 * @return run_task_node_t* ����ڵ�
 * 
 */
run_task_node_t * run_create_task_node(run_task_t task, void *arg)
{
    run_task_node_t * node = (run_task_node_t *)malloc(sizeof(run_task_node_t));
    if(!node) return NULL;
    node->task = task;
    node->arg = arg;
    node->next = NULL;
    return node;
}

/**
 * @brief �ͷ�����ѭ������ڵ�
 * 
 * @param node ����ڵ�
 * 
 */
void run_free_task_node(run_task_node_t * node)
{
    if(node) free(node);
}

/**
 * @brief ����ѭ������
 * 
 */
void run_loop(void)
{
    run_task_node_t * node = run_task_list;
    while(node)
    {
        node->task(node->arg);
        node = node->next;
    }
}

/**
 * @brief �������ѭ������
 * 
 * @param task ������
 * @param arg ����������
 * 
 * @return run_task_node_t* ����ڵ�
 * 
 */
run_task_node_t * run_add_task(run_task_t task, void *arg)
{
    CORE_StatusTypeDef status;
    run_task_node_t * node = run_create_task_node(task, arg);
    if(!node) return NULL;
    status = CORE_add_node((void**)&run_task_list,node);
    if(status == CORE_OK) return node;
    run_free_task_node(node);
    return NULL;
}

/**
 * @brief �Ƴ�����ѭ������
 * 
 * @param task ����ڵ�
 * 
 */
void run_remove_task(run_task_node_t * node)
{
    CORE_del_node((void**)&run_task_list,node);
    run_free_task_node(node);
}
