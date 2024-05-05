#include "run.h"

run_task_node_t * run_task_list = NULL;

/**
 * @brief 创建运行循环任务节点
 * 
 * @param task 任务函数
 * @param arg 任务函数参数
 * 
 * @return run_task_node_t* 任务节点
 * 
 */
run_task_node_t * run_create_task_node(run_task_t task, void *arg)
{
    run_task_node_t * node = (run_task_node_t *)CORE_malloc(sizeof(run_task_node_t));
    if(!node) return NULL;
    node->task = task;
    node->arg = arg;
    node->next = NULL;
    return node;
}

/**
 * @brief 释放运行循环任务节点
 * 
 * @param node 任务节点
 * 
 */
void run_free_task_node(run_task_node_t * node)
{
    if(node) CORE_free(node);
}

/**
 * @brief 运行循环函数
 * 
 */
void CORE_Run_loop(void)
{
    run_task_node_t * node = run_task_list;
    while(node)
    {
        node->task(node->arg);
        node = node->next;
    }
}

/**
 * @brief 添加运行循环任务
 * 
 * @param task 任务函数
 * @param arg 任务函数参数
 * 
 * @return run_task_node_t* 任务节点
 * 
 */
run_task_node_t * CORE_Run_add_task(run_task_t task, void *arg)
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
 * @brief 移除运行循环任务
 * 
 * @param task 任务节点
 * 
 */
void CORE_Run_remove_task(run_task_node_t * node)
{
    CORE_del_node((void**)&run_task_list,node);
    run_free_task_node(node);
}
