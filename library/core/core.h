#ifndef _CORE_H_
#define _CORE_H_

#ifdef __cplusplus
extern "C"
{
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "def.h"
#include "list.h"
#include "run.h"
#include "timer.h"

    /**
     * @brief ������Ϣ
     *
     * @param topic ��Ϣ���⡣���Ϊ `"*"`����ʾ�����������⡣
     * @param callback ��Ϣ�ص�����
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_subscribe(const char *topic, CORE_callback_t callback);

    /**
     * @brief ȡ��������Ϣ
     *
     * @param topic ��Ϣ����
     * @param callback ��Ϣ�ص�����
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_unsubscribe(const char *topic, CORE_callback_t callback);

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
    CORE_StatusTypeDef CORE_publish(const char *topic, void *arg, size_t messageSize, uint8_t sendImmediately);

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
    CORE_StatusTypeDef CORE_publish_str(const char *topic, const char *arg, uint8_t sendImmediately);

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
    CORE_StatusTypeDef CORE_publish_int(const char *topic, int arg, uint8_t sendImmediately);

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
    CORE_StatusTypeDef CORE_publish_float(const char *topic, float arg, uint8_t sendImmediately);

    /**
     * @brief ��ȡͨ���ٶ�
     *
     * @param topic ����
     * @param speed ͨ���ٶ�
     *
     * @return CORE_StatusTypeDef
     *
     */
    CORE_StatusTypeDef CORE_speed(const char *topic, uint16_t *speed);

    /**
     * @brief ��ʼ��
     *
     * @note ������ `main()` �����е��á�
     *
     */
    void CORE_init(void);

#ifdef __cplusplus
}
#endif

#endif
