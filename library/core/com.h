#ifndef _COM_H_
#define _COM_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdlib.h>

#ifdef __CC_ARM /* ARM Compiler */
    #define WEAK __weak
#elif defined(__IAR_SYSTEMS_ICC__) /* for IAR Compiler */
    #define WEAK __weak
#elif defined(__GNUC__) /* GNU GCC Compiler */
    #define WEAK __attribute__((weak))
#elif defined(__ADSPBLACKFIN__) /* for VisualDSP++ Compiler */
    #define WEAK __attribute__((weak))
#elif defined(_MSC_VER)
    #define WEAK
#elif defined(__TI_COMPILER_VERSION__)
    #define WEAK
#else
    #error not supported tool chain
#endif


// �㲥��ַ
#define COM_BROADCAST_ADDR 0xFF
// ���Ͷ��г���
#define COM_QUEUE_LEN 128


typedef enum {
    COM_FRAME_EMPTY = 0,    // ����Ϣ ��Ч����Ϣ
    COM_FRAME_PREPARE,      // �����͵���Ϣ
	COM_FRAME_SEND,         // ���ڷ��͵���Ϣ
    COM_FRAME_RETRY,		// ��ʱ�ط�����Ϣ

} COM_FRAME_STATE;


typedef struct
{
	uint8_t addr;					// Ŀ���ַ
	uint8_t addr_in;				// ������ַ
	uint8_t ID;						// ��ϢID
	uint8_t cmd;					// ����
	uint8_t len;					// ���ݳ���
	uint8_t data[32];				// ����
	uint16_t crc;					// У��

	// ���½������ڲ�ʹ�� ������
	COM_FRAME_STATE state;		// ״̬
	uint8_t retryCount;				// ���Դ���
} com_frame_t;

typedef struct
{
	com_frame_t com[COM_QUEUE_LEN];		// ���ͻ��峤��
	
	uint8_t com_rx;					// ���յ���com
	uint8_t com_tx;					// ���͵�com
	
	uint8_t address;				// ������ַ
	uint8_t ID;						// ��ǰ��Ծ����ϢID
	
	uint16_t rTim;					// �ط���ʱ��
	
	uint16_t surplus;				// ʣ��ռ�
	
	int8_t receiveTim;				// ����ռ�ü�ʱ��

	uint16_t timedOutResendTime;	// ��ʱ�ط�ʱ��

	// ͨ���ٶ���������
	uint16_t baud_RXTX_timer;
	uint16_t baud_RX_temp;
	uint16_t baud_TX_temp;
	uint16_t baud_RX;
	uint16_t baud_TX;
	uint32_t baud_RX_total;
	uint32_t baud_TX_total;

} com_Hardware_t;

void com_init(uint8_t address);

void com_loop(void);

void com_receive_irq(uint8_t c);

void com_tim_irq(uint8_t ms);

/* ------------------------------------------- */

char com_add(com_frame_t * com);

uint16_t com_surplus(void);


#ifdef __cplusplus
}
#endif

#endif
