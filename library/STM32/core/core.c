#include "core.h"

#include <stdio.h>
#define CORE_malloc malloc
#define CORE_free free


typedef struct CORE_msg_list
{
    struct CORE_msg_list * next;
    size_t data_len;
    uint8_t data[0];// ��������
} CORE_msg_list_t;

typedef struct CORE_callback_list
{
    struct CORE_callback_list * next;
    CORE_callback_t callback;
} CORE_callback_list_t;

typedef struct CORE_task_list
{
    struct CORE_task_list * next;
    char topic[CORE_TOPIC_MAX_LEN + 1];
    CORE_callback_list_t * callback_list;
    CORE_msg_list_t * msg_list;
    
    uint16_t msg_count;
    float msg_speed;
} CORE_task_list_t;

CORE_task_list_t * CORE_task_list = NULL;


/**
 * @brief �½��ص�����ڵ�
 * 
 * @param callback �ص�����
 * 
 * @return CORE_callback_list_t *
 * 
 */
CORE_callback_list_t * CORE_new_callback_list(CORE_callback_t callback)
{
    CORE_callback_list_t * callback_list;
    if(!callback) return NULL;
    callback_list = (CORE_callback_list_t *)CORE_malloc(sizeof(CORE_callback_list_t));
    if(!callback_list) return NULL;
    memset(callback_list,0,sizeof(CORE_callback_list_t));
    callback_list->callback = callback;
    return callback_list;
}

/**
 * @brief �ͷŻص�����ڵ�
 * 
 * @param callback_list �ص�����ڵ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_free_callback_list(CORE_callback_list_t * callback_list)
{
    if(!callback_list || callback_list->next) return CORE_ERROR;
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
CORE_msg_list_t * CORE_new_msg(uint8_t *data, size_t data_len)
{
    CORE_msg_list_t * msg;
    if(!data || data_len <= 0) return NULL;
    msg = (CORE_msg_list_t *)CORE_malloc(sizeof(CORE_msg_list_t) + data_len);
    if(!msg) return NULL;
    msg->next = NULL;
    msg->data_len = data_len;
    memcpy(msg->data,data,data_len);
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
CORE_StatusTypeDef CORE_free_msg(CORE_msg_list_t * msg)
{
    if(!msg || msg->next) return CORE_ERROR;
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
CORE_task_list_t * CORE_new_task(const char *topic)
{
    CORE_task_list_t * task;
    if(!topic) return NULL;
    task = (CORE_task_list_t *)CORE_malloc(sizeof(CORE_task_list_t));
    if(!task) return NULL;
    memset(task,0,sizeof(CORE_task_list_t));
    strncpy(task->topic,topic,CORE_TOPIC_MAX_LEN);
    task->topic[CORE_TOPIC_MAX_LEN] = '\0';
    return task;
}

/**
 * @brief �ͷ�����ڵ�
 * 
 * @param task ����
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_free_task(CORE_task_list_t * task)
{
    CORE_callback_list_t * callback_list;
    CORE_msg_list_t * msg_list;
    if(!task || task->next) return CORE_ERROR;

    while(task->callback_list)
    {
        callback_list = task->callback_list;
        task->callback_list = task->callback_list->next;
        task->callback_list->next = NULL;
        CORE_free_callback_list(callback_list);
    }
    while(task->msg_list)
    {
        msg_list = task->msg_list;
        task->msg_list = task->msg_list->next;
        task->msg_list->next = NULL;
        CORE_free_msg(msg_list);
    }

    CORE_free(task);
    return CORE_OK;
}

/**
 * @brief ��ӽڵ�
 * 
 * @param list ����ͷ
 * @param node �ڵ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_add_node(void **list,void *node)
{
    if(!list || !node) return CORE_ERROR;
    

    return CORE_OK;
}

/**
 * @brief ɾ���ڵ�
 * 
 * @param list ����ͷ
 * @param node �ڵ�
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_del_node(void **list,void *node)
{
    return CORE_OK;
}


/**
 * @brief ������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param callback ��Ϣ�ص�����
 * @param len ���ն��г���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback)
{
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
    return CORE_OK;
}

/**
 * @brief ������Ϣ
 * 
 * @param topic ��Ϣ����
 * @param arg ��Ϣ����
 * @param siz ��Ϣ���ݳ���
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t siz)
{
    return CORE_OK;
}

/**
 * @brief ��ȡ��������
 * 
 * @param topics ��������
 * @param len �������鳤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_topics(char **topics, size_t *len)
{
    return CORE_OK;
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
CORE_StatusTypeDef CORE_speed(const char *topic, float *speed)
{
    return CORE_OK;
}

/**
 * @brief ��ȡ����ʣ�೤��
 * 
 * @param topic ����
 * @param len ����ʣ�೤��
 * 
 * @return CORE_StatusTypeDef
 * 
 */
CORE_StatusTypeDef CORE_queue_remain(const char *topic, size_t *len)
{
    return CORE_OK;
}
