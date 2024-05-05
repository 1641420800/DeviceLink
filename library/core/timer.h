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
     * @brief timer��ʼ��
     *
     */
    void CORE_Timer_Init(void);

    /**
     * @brief ��ȡ��ǰʱ�� ��λus
     *
     * @note �������Ӧ����Ӳ��ƽ̨ʵ�� �����ṩus����ʱ���׼
     *
     * @return uint32_t ��ǰʱ�� ��λus
     *
     */
    WEAK uint32_t CORE_Timer_GetTime_us(void);

    /**
     * @brief timer IRQHandler
     *
     * @param ms ��ʱ����ʱ���� ��λms
     *
     * @note �������Ӧ���ڶ�ʱ���ж��е���
     *
     */
    void CORE_Timer_IRQHandler(uint32_t ms);

    /**
     * @brief ��һ����ʱ���ص����� �����ж��е���
     *
     * @param callback ��ʱ���ص�����
     * @param arg ��ʱ���ص������Ĳ���
     * @param interval ��ʱ��ʱ���� ��λms
     *
     * @return timer_node_t* ��ʱ���ڵ�
     *
     */
    timer_node_t *CORE_Timer_add_task(timer_callback_t callback, void *arg, uint32_t interval);

    /**
     * @brief ��һ����ʱ���ص����� ����ѭ���е���
     *
     * @param callback ��ʱ���ص�����
     * @param arg ��ʱ���ص������Ĳ���
     * @param interval ��ʱ��ʱ���� ��λms
     *
     * @return timer_node_t* ��ʱ���ڵ�
     *
     */
    timer_node_t *CORE_TimerService_add_task(run_task_t callback, void *arg, uint32_t interval);

    /**
     * @brief ���һ����ʱ���ص�����
     *
     * @param node ��ʱ���ڵ�
     *
     * @return CORE_StatusTypeDef ����״̬
     *
     */
    CORE_StatusTypeDef CORE_Timer_remove_task(timer_node_t *node);

    /**
     * @brief ������ǰִ���еĶ�ʱ��ʱ����
     *
     * @param interval ��ʱ��ʱ���� ��λms
     *
     * @note �������Ӧ���ڶ�ʱ���ص��е���
     *
     * @return CORE_StatusTypeDef ����״̬
     *
     */
    CORE_StatusTypeDef CORE_Timer_AdjustInterval(uint32_t interval);

    /**
     * @brief ���ö�ʱ��ʱ����
     *
     * @param node ��ʱ���ڵ�
     * @param interval ��ʱ��ʱ���� ��λms
     *
     * @return CORE_StatusTypeDef ����״̬
     *
     */
    CORE_StatusTypeDef CORE_Timer_SetInterval(timer_node_t *node, uint32_t interval);

    /**
     * @brief ��ȡ��ǰִ���еĶ�ʱ���ڵ�
     *
     * @return timer_node_t* ��ʱ���ڵ�
     *
     */
    timer_node_t *CORE_Timer_GetCurrentNode(void);

    /**
     * @brief ��ȡ��ǰִ���еĶ�ʱ��ʱ��(ms)
     *
     * @return uint32_t ��ǰִ���еĶ�ʱ��ʱ��(ms)
     *
     */
    uint32_t CORE_Timer_GetCurrentTime_ms(void);

    /**
     * @brief ��ȡ��ǰ����ʱ��(us)
     *
     * @note ������ CORE_Timer_GetTime_us
     *
     * @return uint32_t ��ǰ����ʱ��(us)
     *
     */
    uint32_t CORE_Timer_GetCurrentTime_us(void);

    /**
     * @brief ��ͣһ���ڵ�
     *
     * @param node ��ʱ���ڵ�
     *
     */
    CORE_StatusTypeDef CORE_Timer_Pause(timer_node_t *node);

    /**
     * @brief �ָ�һ���ڵ�
     *
     * @param node ��ʱ���ڵ�
     *
     */
    CORE_StatusTypeDef CORE_Timer_Resume(timer_node_t *node);

#ifdef __cplusplus
}
#endif

#endif // !_TIMER_H_
