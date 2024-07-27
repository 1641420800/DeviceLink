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


// 广播地址
#define COM_BROADCAST_ADDR 0xFF
// 发送队列长度
#define COM_QUEUE_LEN 128


typedef enum {
    COM_FRAME_EMPTY = 0,    // 空消息 无效的消息
    COM_FRAME_PREPARE,      // 待发送的消息
	COM_FRAME_SEND,         // 正在发送的消息
    COM_FRAME_RETRY,		// 超时重发的消息

} COM_FRAME_STATE;


typedef struct
{
	uint8_t addr;					// 目标地址
	uint8_t addr_in;				// 本机地址
	uint8_t ID;						// 消息ID
	uint8_t cmd;					// 命令
	uint8_t len;					// 数据长度
	uint8_t data[32];				// 数据
	uint16_t crc;					// 校验

	// 以下仅用于内部使用 不发送
	COM_FRAME_STATE state;		// 状态
	uint8_t retryCount;				// 重试次数
} com_frame_t;

typedef struct
{
	com_frame_t com[COM_QUEUE_LEN];		// 发送缓冲长度
	
	uint8_t com_rx;					// 接收到的com
	uint8_t com_tx;					// 发送的com
	
	uint8_t address;				// 本机地址
	uint8_t ID;						// 当前活跃的消息ID
	
	uint16_t rTim;					// 重发计时器
	
	uint16_t surplus;				// 剩余空间
	
	int8_t receiveTim;				// 总线占用计时器

	uint16_t timedOutResendTime;	// 超时重发时间

	// 通信速度与总流量
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
