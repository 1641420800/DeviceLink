#include "timer.h"

timer_node_t *timer_list = NULL;
timer_node_t *timer_current = NULL;

uint32_t timerTim = 0;

/**
 * @brief 新建timer节点
 *
 * @return timer_node_t* 新建的timer节点
 *
 */
timer_node_t *timer_new_node()
{
    timer_node_t *node = (timer_node_t *)CORE_malloc(sizeof(timer_node_t));
    if (node == NULL)
    {
        return NULL;
    }
    node->next = NULL;
    node->callback = NULL;
    node->task_callback = NULL;
    node->arg = NULL;
    node->interval = 0;
    node->last_time = 0;
    node->status = CORE_TASK_STOP;
    return node;
}

/**
 * @brief 释放timer节点
 *
 * @param node timer节点
 *
 */
void timer_free_node(timer_node_t *node)
{
    if (node != NULL)
    {
        CORE_free(node);
    }
}

/**
 * @brief timer循环
 *
 */
void timer_loop(void *arg)
{
    timer_node_t *node = timer_list;
    timer_node_t *node_free = NULL;
    while (node != NULL)
    {
        if (node->callback == NULL && node->task_callback == NULL)
        {
            node_free = node;
            node = node->next;
            timer_free_node(node_free);
            continue;
        }
        else if (node->status == CORE_TASK_EXECUTING && node->task_callback != NULL)
        {
            timer_current = node;
            node->status = CORE_TASK_RUNNING;
            node->task_callback(node->arg);
            timer_current = NULL;
        }
        node = node->next;
    }
}

/**
 * @brief timer初始化
 *
 */
void CORE_Timer_Init(void)
{
    CORE_Run_add_task(timer_loop, NULL);
}

/**
 * @brief 获取当前时间 单位us
 *
 * @note 这个函数应该由硬件平台实现 用来提供us级的时间基准
 *
 * @return uint32_t 当前时间 单位us
 *
 */
WEAK uint32_t CORE_Timer_GetTime_us(void)
{
    return 0;
}

/**
 * @brief timer IRQHandler
 *
 * @param ms 定时器的时间间隔 单位ms
 *
 * @note 这个函数应该在定时器中断中调用
 *
 */
void CORE_Timer_IRQHandler(uint32_t ms)
{
    timer_node_t *node = timer_list;
    timer_node_t *current = timer_current;
    timerTim += ms;
    while (node != NULL)
    {
        if (node->callback == NULL && node->task_callback == NULL)
        {
            node = node->next;
            continue;
        }
        if (node->status == CORE_TASK_STOP)
        {
            node = node->next;
            continue;
        }
        if (timerTim - node->last_time >= node->interval)
        {
            node->last_time += node->interval;
            node->status = CORE_TASK_EXECUTING;
            timer_current = node;
            if (node->callback)
                node->callback(node->arg);
            timer_current = current;
        }
        if (node->last_time > timerTim)
        {
            node->last_time = timerTim;
        }
        node = node->next;
    }
}

/**
 * @brief 绑定一个定时器回调函数 将在中断中调用
 *
 * @param callback 定时器回调函数
 * @param arg 定时器回调函数的参数
 * @param interval 定时器时间间隔 单位ms
 *
 * @return timer_node_t* 定时器节点
 *
 */
timer_node_t *CORE_Timer_add_task(timer_callback_t callback, void *arg, uint32_t interval)
{
    timer_node_t *node = timer_new_node();
    CORE_StatusTypeDef status;
    if (node == NULL)
    {
        return NULL;
    }
    node->callback = callback;
    node->arg = arg;
    node->interval = interval;
    node->last_time = timerTim;
    node->status = CORE_TASK_RUNNING;
    status = CORE_add_node((void **)&timer_list, node);
    if (status != CORE_OK)
    {
        timer_free_node(node);
        return NULL;
    }
    return node;
}

/**
 * @brief 绑定一个定时器回调函数 将在循环中调用
 *
 * @param callback 定时器回调函数
 * @param arg 定时器回调函数的参数
 * @param interval 定时器时间间隔 单位ms
 *
 * @return timer_node_t* 定时器节点
 *
 */
timer_node_t *CORE_TimerService_add_task(run_task_t callback, void *arg, uint32_t interval)
{
    timer_node_t *node = timer_new_node();
    CORE_StatusTypeDef status;
    if (node == NULL)
    {
        return NULL;
    }
    node->task_callback = callback;
    node->arg = arg;
    node->interval = interval;
    node->last_time = timerTim;
    node->status = CORE_TASK_RUNNING;
    status = CORE_add_node((void **)&timer_list, node);
    if (status != CORE_OK)
    {
        timer_free_node(node);
        return NULL;
    }
    return node;
}

/**
 * @brief 解绑一个定时器回调函数
 *
 * @param node 定时器节点
 *
 * @return CORE_StatusTypeDef 操作状态
 *
 */
CORE_StatusTypeDef CORE_Timer_remove_task(timer_node_t *node)
{
    node->status = CORE_TASK_STOP;
    node->callback = NULL;
    node->task_callback = NULL;
    return CORE_OK;
}

/**
 * @brief 调整当前执行中的定时器时间间隔
 *
 * @param interval 定时器时间间隔 单位ms
 *
 * @note 这个函数应该在定时器回调中调用
 *
 * @return CORE_StatusTypeDef 操作状态
 *
 */
CORE_StatusTypeDef CORE_Timer_AdjustInterval(uint32_t interval)
{
    if (timer_current == NULL)
    {
        return CORE_ERROR;
    }
    timer_current->interval = interval;
    return CORE_OK;
}

/**
 * @brief 设置定时器时间间隔
 *
 * @param node 定时器节点
 * @param interval 定时器时间间隔 单位ms
 *
 * @return CORE_StatusTypeDef 操作状态
 *
 */
CORE_StatusTypeDef CORE_Timer_SetInterval(timer_node_t *node, uint32_t interval)
{
    if (node == NULL)
    {
        return CORE_ERROR;
    }
    node->interval = interval;
    return CORE_OK;
}

/**
 * @brief 获取当前执行中的定时器节点
 *
 * @return timer_node_t* 定时器节点
 *
 */
timer_node_t *CORE_Timer_GetCurrentNode(void)
{
    return timer_current;
}

/**
 * @brief 获取当前执行中的定时器时间(ms)
 *
 * @return uint32_t 当前执行中的定时器时间(ms)
 *
 */
uint32_t CORE_Timer_GetCurrentTime_ms(void)
{
    return timerTim;
}

/**
 * @brief 获取当前运行时间(us)
 *
 * @note 依赖于 CORE_Timer_GetTime_us
 *
 * @return uint32_t 当前运行时间(us)
 *
 */
uint32_t CORE_Timer_GetCurrentTime_us(void)
{
    return timerTim * 1000 + CORE_Timer_GetTime_us();
}

/**
 * @brief 暂停一个节点
 *
 * @param node 定时器节点
 *
 */
CORE_StatusTypeDef CORE_Timer_Pause(timer_node_t *node)
{
    if (node == NULL)
    {
        return CORE_ERROR;
    }
    node->status = CORE_TASK_STOP;
    return CORE_OK;
}

/**
 * @brief 恢复一个节点
 *
 * @param node 定时器节点
 *
 */
CORE_StatusTypeDef CORE_Timer_Resume(timer_node_t *node)
{
    if (node == NULL)
    {
        return CORE_ERROR;
    }
    node->last_time = timerTim;
    node->status = CORE_TASK_RUNNING;
    return CORE_OK;
}
