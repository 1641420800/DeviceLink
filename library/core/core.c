#include "core.h"

typedef struct CORE_MSG_LIST
{
    struct CORE_MSG_LIST *next; // ָ����һ����Ϣ�ڵ��ָ��
    size_t data_len;            // ���ݳ���
    uint8_t data[0];            // ��̬�洢���ݣ���������
} CORE_msg_list_t;

typedef struct CORE_CALLBACK_LIST
{
    struct CORE_CALLBACK_LIST *next; // ָ����һ���ص��ڵ��ָ��
    CORE_callback_t callback;        // �ص�����
    uint16_t runTime_us;             // �ص���������ʱ��
} CORE_callback_list_t;

typedef struct CORE_TASK_LIST
{
    struct CORE_TASK_LIST *next;         // ָ����һ������ڵ��ָ��
    char topic[CORE_TOPIC_MAX_LEN + 1];  // �����ַ���
    CORE_callback_list_t *callback_list; // �ص������б�ָ��
    CORE_msg_list_t *msg_list;           // ��Ϣ�б�ָ��
    uint16_t msg_list_len;               // ��Ϣ�б���

    uint16_t msg_count; // ��Ϣ����
    uint16_t msg_speed; // ��Ϣ����
} CORE_task_list_t;

// ȫ�������б�ָ��
CORE_task_list_t *CORE_task_list = NULL;
char currentTopic[CORE_TOPIC_MAX_LEN + 1]; // �����ַ���

/**
 * @brief �½��ص�����ڵ�
 *
 * @param callback �ص�����
 *
 * @return CORE_callback_list_t *
 *
 */
CORE_callback_list_t *CORE_new_callback_list(CORE_callback_t callback)
{
    CORE_callback_list_t *callback_list;
    if (!callback)
        return NULL;
    callback_list = (CORE_callback_list_t *)CORE_malloc(sizeof(CORE_callback_list_t));
    if (!callback_list)
        return NULL;
    memset(callback_list, 0, sizeof(CORE_callback_list_t));
    callback_list->callback = callback;
    return callback_list;
}

/**
 * @brief ���һص�����ڵ�
 *
 * @param callback_list �ص�����ڵ�
 * @param callback �ص�����
 *
 * @return CORE_callback_list_t *
 *
 */
CORE_callback_list_t *CORE_find_callback_list(CORE_callback_list_t *callback_list, CORE_callback_t callback)
{
    while (callback_list)
    {
        if (callback_list->callback == callback)
            return callback_list;
        callback_list = callback_list->next;
    }
    return NULL;
}

/**
 * @brief �ͷŻص�����ڵ�
 *
 * @param callback_list �ص�����ڵ�
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_callback_list(CORE_callback_list_t *callback_list)
{
    if (!callback_list)
        return CORE_ERROR;
    CORE_free(callback_list);
    return CORE_OK;
}

/**
 * @brief �½���Ϣ�ڵ�
 *
 * @param data ��Ϣ����
 * @param data_len ��Ϣ���ݳ���
 *
 * @return CORE_msg_t *
 *
 */
CORE_msg_list_t *CORE_new_msg(const char *topic, uint8_t *data, size_t data_len)
{
    CORE_msg_list_t *msg;
    if (!topic || !data || data_len <= 0)
        return NULL;
    msg = (CORE_msg_list_t *)CORE_malloc(sizeof(CORE_msg_list_t) + data_len);
    if (!msg)
        return NULL;
    msg->next = NULL;
    msg->data_len = data_len;
    memcpy(msg->data, data, data_len);
    return msg;
}

/**
 * @brief �ͷ���Ϣ�ڵ�
 *
 * @param msg ��Ϣ�ڵ�
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_msg(CORE_msg_list_t *msg)
{
    if (!msg)
        return CORE_ERROR;
    CORE_free(msg);
    return CORE_OK;
}

/**
 * @brief �½�����ڵ�
 *
 * @param topic ��Ϣ����
 *
 * @return CORE_task_t *
 *
 */
CORE_task_list_t *CORE_new_task(const char *topic)
{
    CORE_task_list_t *task;
    if (!topic)
        return NULL;
    task = (CORE_task_list_t *)CORE_malloc(sizeof(CORE_task_list_t));
    if (!task)
        return NULL;
    memset(task, 0, sizeof(CORE_task_list_t));
    strncpy(task->topic, topic, CORE_TOPIC_MAX_LEN); // ���������ַ��� ������󳤶�ʱ�ض�
    task->topic[CORE_TOPIC_MAX_LEN] = '\0';
    return task;
}

/**
 * @brief ��������ڵ�
 *
 * @param topic ��Ϣ����
 *
 * @return CORE_task_t *
 *
 */
CORE_task_list_t *CORE_find_task(const char *topic)
{
    CORE_task_list_t *task = CORE_task_list;
    if (!topic)
        return NULL;
    while (task)
    {
        if (strcmp(task->topic, topic) == 0)
            return task;
        task = task->next;
    }
    return NULL;
}

/**
 * @brief �ͷ�����ڵ�
 *
 * @param task ����
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_free_task(CORE_task_list_t *task)
{
    CORE_callback_list_t *callback_list;
    CORE_msg_list_t *msg_list;
    if (!task)
        return CORE_ERROR;

    while (task->callback_list)
    {
        callback_list = task->callback_list;
        task->callback_list = task->callback_list->next;
        CORE_free_callback_list(callback_list);
    }
    while (task->msg_list)
    {
        msg_list = task->msg_list;
        task->msg_list = task->msg_list->next;
        CORE_free_msg(msg_list);
    }

    CORE_free(task);
    return CORE_OK;
}

/**
 * @brief ���ûص�����
 *
 * @param callback �ص�����
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param siz ��Ϣ���ݳ���
 *
 * @todo ��ʱ����
 *
 */
void CORE_call_callback(CORE_callback_list_t *callback, const char *topic, void *arg, size_t siz)
{
    uint32_t tim;
    if (!callback || !callback->callback)
        return;
    strcpy(currentTopic, topic);
    // ��ʼ��ʱ
    tim = CORE_Timer_GetCurrentTime_us();
    callback->callback(topic, arg, siz);
    tim = CORE_Timer_GetCurrentTime_us() - tim;
    // ������� todo
    callback->runTime_us = tim;

    strcpy(currentTopic, "");

    // ������ʱ
}

/**
 * @brief ������Ϣ
 *
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback)
{
    CORE_task_list_t *task;
    if (!topic || !callback)
        return CORE_ERROR;
    if (strcmp(topic, "CORE_subscribe") && strcmp(currentTopic, "CORE_subscribe"))
        CORE_publish_str("CORE_subscribe", topic, 0);
    task = CORE_find_task(topic);
    if (!task)
    {
        task = CORE_new_task(topic);
        if (!task)
            return CORE_ERROR;
        CORE_add_node((void **)&CORE_task_list, (void *)task);
    }
    else
    {
        if (CORE_find_callback_list(task->callback_list, callback))
        {
            return CORE_EXIST;
        }
    }
    CORE_add_node((void **)&task->callback_list, (void *)CORE_new_callback_list(callback));
    return CORE_OK;
}

/**
 * @brief ȡ��������Ϣ
 *
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback)
{
    CORE_task_list_t *task;
    CORE_callback_list_t *callback_list;
    if (!topic || !callback)
        return CORE_ERROR;
    if (strcmp(topic, "CORE_unsubscribe") && strcmp(currentTopic, "CORE_unsubscribe"))
        CORE_publish_str("CORE_unsubscribe", topic, 0);
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    callback_list = CORE_find_callback_list(task->callback_list, callback);
    if (!callback_list)
        return CORE_ERROR;
    CORE_del_node((void **)&task->callback_list, (void *)callback_list);
    CORE_free_callback_list(callback_list);
    if (!task->callback_list)
    {
        CORE_del_node((void **)&CORE_task_list, (void *)task);
        CORE_free_task(task);
    }
    return CORE_OK;
}

/**
 * @brief ������Ϣ
 *
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param messageSize ��Ϣ���ݳ���
 * @param sendImmediately  �Ƿ���������
 *
 * @note ��� sendImmediately  Ϊ true�����������ͣ����򽫷�����С�
 *
 * @warning ����Ȩģʽ��(IRQ)�½���ʹ���������͹��ܡ�
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t messageSize, uint8_t sendImmediately)
{
    CORE_msg_list_t *msg;
    CORE_task_list_t *task;
    CORE_callback_list_t *callback_list;
    CORE_StatusTypeDef status;

    if (!topic || !arg || !messageSize)
        return CORE_ERROR;
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    if (sendImmediately)
    { // ��������
        callback_list = task->callback_list;
        while (callback_list)
        {
            CORE_call_callback(callback_list, topic, arg, messageSize);
            callback_list = callback_list->next;
            ++task->msg_count;
        }
    }
    else
    { // �������
        if (task->msg_list_len >= CORE_QUEUE_MAX_LEN)
            return CORE_BUSY;
        msg = CORE_new_msg(topic, arg, messageSize);
        if (!msg)
            return CORE_ERROR;
        status = CORE_add_node((void **)&task->msg_list, (void *)msg);
        if (status != CORE_OK)
        {
            CORE_free_msg(msg);
            return CORE_ERROR;
        }
        task->msg_list_len++;
    }

    return CORE_OK;
}

/**
 * @brief �����ַ�����Ϣ
 *
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param sendImmediately  �Ƿ���������
 *
 * @note ��� sendImmediately  Ϊ true�����������ͣ����򽫷�����С�
 *
 * @warning ����Ȩģʽ��(IRQ)�½���ʹ���������͹��ܡ�
 *
 */
CORE_StatusTypeDef CORE_publish_str(const char *topic, const char *arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)arg, strlen(arg) + 1, sendImmediately);
}

/**
 * @brief ����������Ϣ
 *
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param sendImmediately  �Ƿ���������
 *
 * @note ��� sendImmediately  Ϊ true�����������ͣ����򽫷�����С�
 *
 * @warning ����Ȩģʽ��(IRQ)�½���ʹ���������͹��ܡ�
 *
 */
CORE_StatusTypeDef CORE_publish_int(const char *topic, int arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)&arg, sizeof(int), sendImmediately);
}

/**
 * @brief ������������Ϣ
 *
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param sendImmediately  �Ƿ���������
 *
 * @note ��� sendImmediately  Ϊ true�����������ͣ����򽫷�����С�
 *
 * @warning ����Ȩģʽ��(IRQ)�½���ʹ���������͹��ܡ�
 *
 */
CORE_StatusTypeDef CORE_publish_float(const char *topic, float arg, uint8_t sendImmediately)
{
    return CORE_publish(topic, (void *)&arg, sizeof(float), sendImmediately);
}

/**
 * @brief ��ȡͨ���ٶ�
 *
 * @param topic ����
 * @param speed ͨ���ٶ�
 *
 * @return CORE_StatusTypeDef
 *
 */
CORE_StatusTypeDef CORE_speed(const char *topic, uint16_t *speed)
{
    CORE_task_list_t *task;
    if (!topic || !speed)
        return CORE_ERROR;
    task = CORE_find_task(topic);
    if (!task)
        return CORE_ERROR;
    *speed = task->msg_speed;
    return CORE_OK;
}

/**
 * @brief ִ�к�������ѭ�������������б��е���Ϣ��
 *
 * @param arg ���ݸ������Ĳ�������������δʹ�á�
 * 
 */
void CORE_run(void *arg)
{
    // ���������б�
    CORE_task_list_t *task = CORE_task_list;

    while (task)
    {
        CORE_msg_list_t *msg = task->msg_list;
        task->msg_list = NULL;
        while (msg)
        {
            CORE_msg_list_t *msg_free = msg;
            CORE_callback_list_t *callback_list = task->callback_list;
            msg = msg->next;
            while (callback_list)
            {
                CORE_call_callback(callback_list, task->topic, msg_free->data, msg_free->data_len);
                callback_list = callback_list->next;
                ++task->msg_count;
            }
            CORE_free_msg(msg_free);
            task->msg_list_len--;
        }
        task = task->next;
    }
}

/**
 * @brief ���м�ʱ��
 *
 * @param arg ���ݸ������Ĳ�������������δʹ�á�
 *
 */
void CORE_run_timer(void *arg)
{
    CORE_task_list_t *task = CORE_task_list;
    while (task)
    {
        task->msg_speed = task->msg_count;
        task->msg_count = 0;
        task = task->next;
    }
}

/**
 * @brief ��ʼ��
 *
 * @note ������ `main()` �����е��á�
 *
 */
void CORE_init(void)
{
    CORE_Run_add_task(CORE_run, NULL);
    CORE_Timer_add_task(CORE_run_timer, NULL, 1000);
}
