#ifndef _RUN_H_
#define _RUN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"

typedef void (*run_task_t)(void * arg);

typedef struct RUN_TASK_NODE
{
    struct RUN_TASK_NODE *next;
    run_task_t task;
    void *arg;
} run_task_node_t;

/**
 * @brief ����ѭ������
 * 
 */
void run_loop(void);

/**
 * @brief �������ѭ������
 * 
 * @param task ������
 * @param arg ����������
 * 
 * @return run_task_node_t* ����ڵ�
 * 
 */
run_task_node_t * run_add_task(run_task_t task, void *arg);

/**
 * @brief �Ƴ�����ѭ������
 * 
 * @param task ����ڵ�
 * 
 */
void run_remove_task(run_task_node_t * node);

#ifdef __cplusplus
}
#endif

#endif
