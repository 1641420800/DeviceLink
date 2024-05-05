#ifndef _RUN_H_
#define _RUN_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"

    typedef struct RUN_TASK_NODE
    {
        struct RUN_TASK_NODE *next;
        run_task_t task;
        void *arg;
    } run_task_node_t;

    /**
     * @brief 运行循环函数
     *
     */
    void CORE_Run_loop(void);

    /**
     * @brief 添加运行循环任务
     *
     * @param task 任务函数
     * @param arg 任务函数参数
     *
     * @return run_task_node_t* 任务节点
     *
     */
    run_task_node_t *CORE_Run_add_task(run_task_t task, void *arg);

    /**
     * @brief 移除运行循环任务
     *
     * @param task 任务节点
     *
     */
    void CORE_Run_remove_task(run_task_node_t *node);

#ifdef __cplusplus
}
#endif

#endif
