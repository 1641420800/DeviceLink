#ifndef _TIMER_H_
#define _TIMER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include "core.h"

    typedef struct TIMER_NODE
    {
        struct TIMER_NODE *next;
        timer_callback_t callback;
        run_task_t task_callback;
        void *arg;
        uint32_t interval;
        uint32_t last_time;
        CORE_TaskStatusTypeDef status;
    } timer_node_t;

    /**
     * @brief timer初始化
     *
     */
    void CORE_Timer_Init(void);

    /**
     * @brief 获取当前时间 单位us
     *
     * @note 这个函数应该由硬件平台实现 用来提供us级的时间基准
     *
     * @return uint32_t 当前时间 单位us
     *
     */
    WEAK uint32_t CORE_Timer_GetTime_us(void);

    /**
     * @brief timer IRQHandler
     *
     * @param ms 定时器的时间间隔 单位ms
     *
     * @note 这个函数应该在定时器中断中调用
     *
     */
    void CORE_Timer_IRQHandler(uint32_t ms);

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
    timer_node_t *CORE_Timer_add_task(timer_callback_t callback, void *arg, uint32_t interval);

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
    timer_node_t *CORE_TimerService_add_task(run_task_t callback, void *arg, uint32_t interval);

    /**
     * @brief 解绑一个定时器回调函数
     *
     * @param node 定时器节点
     *
     * @return CORE_StatusTypeDef 操作状态
     *
     */
    CORE_StatusTypeDef CORE_Timer_remove_task(timer_node_t *node);

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
    CORE_StatusTypeDef CORE_Timer_AdjustInterval(uint32_t interval);

    /**
     * @brief 设置定时器时间间隔
     *
     * @param node 定时器节点
     * @param interval 定时器时间间隔 单位ms
     *
     * @return CORE_StatusTypeDef 操作状态
     *
     */
    CORE_StatusTypeDef CORE_Timer_SetInterval(timer_node_t *node, uint32_t interval);

    /**
     * @brief 获取当前执行中的定时器节点
     *
     * @return timer_node_t* 定时器节点
     *
     */
    timer_node_t *CORE_Timer_GetCurrentNode(void);

    /**
     * @brief 获取当前执行中的定时器时间(ms)
     *
     * @return uint32_t 当前执行中的定时器时间(ms)
     *
     */
    uint32_t CORE_Timer_GetCurrentTime_ms(void);

    /**
     * @brief 获取当前运行时间(us)
     *
     * @note 依赖于 CORE_Timer_GetTime_us
     *
     * @return uint32_t 当前运行时间(us)
     *
     */
    uint32_t CORE_Timer_GetCurrentTime_us(void);

    /**
     * @brief 暂停一个节点
     *
     * @param node 定时器节点
     *
     */
    CORE_StatusTypeDef CORE_Timer_Pause(timer_node_t *node);

    /**
     * @brief 恢复一个节点
     *
     * @param node 定时器节点
     *
     */
    CORE_StatusTypeDef CORE_Timer_Resume(timer_node_t *node);

#ifdef __cplusplus
}
#endif

#endif // !_TIMER_H_
