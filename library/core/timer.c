#include "timer.h"

timer_node_t *timer_list = NULL;
timer_node_t *timer_current = NULL;

uint32_t timerTim = 0;

/**
 * @brief �½�timer�ڵ�
 *
 * @return timer_node_t* �½���timer�ڵ�
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
 * @brief �ͷ�timer�ڵ�
 *
 * @param node timer�ڵ�
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
 * @brief timerѭ��
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
 * @brief timer��ʼ��
 *
 */
void CORE_Timer_Init(void)
{
    CORE_Run_add_task(timer_loop, NULL);
}

/**
 * @brief ��ȡ��ǰʱ�� ��λus
 *
 * @note �������Ӧ����Ӳ��ƽ̨ʵ�� �����ṩus����ʱ���׼
 *
 * @return uint32_t ��ǰʱ�� ��λus
 *
 */
WEAK uint32_t CORE_Timer_GetTime_us(void)
{
    return 0;
}

/**
 * @brief timer IRQHandler
 *
 * @param ms ��ʱ����ʱ���� ��λms
 *
 * @note �������Ӧ���ڶ�ʱ���ж��е���
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
 * @brief ��һ����ʱ���ص����� �����ж��е���
 *
 * @param callback ��ʱ���ص�����
 * @param arg ��ʱ���ص������Ĳ���
 * @param interval ��ʱ��ʱ���� ��λms
 *
 * @return timer_node_t* ��ʱ���ڵ�
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
 * @brief ��һ����ʱ���ص����� ����ѭ���е���
 *
 * @param callback ��ʱ���ص�����
 * @param arg ��ʱ���ص������Ĳ���
 * @param interval ��ʱ��ʱ���� ��λms
 *
 * @return timer_node_t* ��ʱ���ڵ�
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
 * @brief ���һ����ʱ���ص�����
 *
 * @param node ��ʱ���ڵ�
 *
 * @return CORE_StatusTypeDef ����״̬
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
 * @brief ������ǰִ���еĶ�ʱ��ʱ����
 *
 * @param interval ��ʱ��ʱ���� ��λms
 *
 * @note �������Ӧ���ڶ�ʱ���ص��е���
 *
 * @return CORE_StatusTypeDef ����״̬
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
 * @brief ���ö�ʱ��ʱ����
 *
 * @param node ��ʱ���ڵ�
 * @param interval ��ʱ��ʱ���� ��λms
 *
 * @return CORE_StatusTypeDef ����״̬
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
 * @brief ��ȡ��ǰִ���еĶ�ʱ���ڵ�
 *
 * @return timer_node_t* ��ʱ���ڵ�
 *
 */
timer_node_t *CORE_Timer_GetCurrentNode(void)
{
    return timer_current;
}

/**
 * @brief ��ȡ��ǰִ���еĶ�ʱ��ʱ��(ms)
 *
 * @return uint32_t ��ǰִ���еĶ�ʱ��ʱ��(ms)
 *
 */
uint32_t CORE_Timer_GetCurrentTime_ms(void)
{
    return timerTim;
}

/**
 * @brief ��ȡ��ǰ����ʱ��(us)
 *
 * @note ������ CORE_Timer_GetTime_us
 *
 * @return uint32_t ��ǰ����ʱ��(us)
 *
 */
uint32_t CORE_Timer_GetCurrentTime_us(void)
{
    return timerTim * 1000 + CORE_Timer_GetTime_us();
}

/**
 * @brief ��ͣһ���ڵ�
 *
 * @param node ��ʱ���ڵ�
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
 * @brief �ָ�һ���ڵ�
 *
 * @param node ��ʱ���ڵ�
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
