#include "com.h"
#include "circularQueue.h"

/*

# 通信流程

 - 邮件
 - 发件箱
 - 收件箱
 - 邮件ID 0 -> 255 循环
 - 发件地址
 - 收件地址
 - 邮件主题
 - 邮件内容
 
## 初始化
 - 初始化邮箱配置
 - 设置当前邮箱

## 发送流程
循环中或中断里  -  添加邮件到邮箱,然后等待发送 - 需要等待总线空闲
接口:
 - 添加邮件到邮箱的接口(收件地址,主题,内容,重传次数) - 需要支持重入 - 返回邮箱状态
 - 立即发送的接口 
 - 发送完成的接口
 - 发送失败的接口 (对方无响应,总线忙)

## 接收流程
接收中断 尝试解析邮件,然后添加到邮箱

### 收到邮件并回信
立即回信 或 间隔n毫秒后回信
提前绑定回信内容
接口:
 - 收到邮件的中断接口
 - 收到邮件的普通接口
 - 绑定回信内容的接口(立即回信)
 - 绑定回信内容的接口(间隔n毫秒后回信)
 - 准备回信的中断接口

### 收到回信
接口:
 - 收到回信的中断接口
 - 收到回信的普通接口




*/


/**
 * @todo
 * 
 * 		- 添加对邮件发送优先级的支持
 * 		- 优化接收队列
 * 		- 优化收发缓冲
 *		- 扩充地址为 16位
 *		- 数据长度普通帧最大32 巨型帧最大128 (最高位为1表示不是完整的数据帧)
 * 
 */
const uint16_t crc16_table[256] = {
	0x0000, 0x1189, 0x2312, 0x329b, 0x4624, 0x57ad, 0x6536, 0x74bf,
	0x8c48, 0x9dc1, 0xaf5a, 0xbed3, 0xca6c, 0xdbe5, 0xe97e, 0xf8f7,
	0x1081, 0x0108, 0x3393, 0x221a, 0x56a5, 0x472c, 0x75b7, 0x643e,
	0x9cc9, 0x8d40, 0xbfdb, 0xae52, 0xdaed, 0xcb64, 0xf9ff, 0xe876,
	0x2102, 0x308b, 0x0210, 0x1399, 0x6726, 0x76af, 0x4434, 0x55bd,
	0xad4a, 0xbcc3, 0x8e58, 0x9fd1, 0xeb6e, 0xfae7, 0xc87c, 0xd9f5,
	0x3183, 0x200a, 0x1291, 0x0318, 0x77a7, 0x662e, 0x54b5, 0x453c,
	0xbdcb, 0xac42, 0x9ed9, 0x8f50, 0xfbef, 0xea66, 0xd8fd, 0xc974,
	0x4204, 0x538d, 0x6116, 0x709f, 0x0420, 0x15a9, 0x2732, 0x36bb,
	0xce4c, 0xdfc5, 0xed5e, 0xfcd7, 0x8868, 0x99e1, 0xab7a, 0xbaf3,
	0x5285, 0x430c, 0x7197, 0x601e, 0x14a1, 0x0528, 0x37b3, 0x263a,
	0xdecd, 0xcf44, 0xfddf, 0xec56, 0x98e9, 0x8960, 0xbbfb, 0xaa72,
	0x6306, 0x728f, 0x4014, 0x519d, 0x2522, 0x34ab, 0x0630, 0x17b9,
	0xef4e, 0xfec7, 0xcc5c, 0xddd5, 0xa96a, 0xb8e3, 0x8a78, 0x9bf1,
	0x7387, 0x620e, 0x5095, 0x411c, 0x35a3, 0x242a, 0x16b1, 0x0738,
	0xffcf, 0xee46, 0xdcdd, 0xcd54, 0xb9eb, 0xa862, 0x9af9, 0x8b70,
	0x8408, 0x9581, 0xa71a, 0xb693, 0xc22c, 0xd3a5, 0xe13e, 0xf0b7,
	0x0840, 0x19c9, 0x2b52, 0x3adb, 0x4e64, 0x5fed, 0x6d76, 0x7cff,
	0x9489, 0x8500, 0xb79b, 0xa612, 0xd2ad, 0xc324, 0xf1bf, 0xe036,
	0x18c1, 0x0948, 0x3bd3, 0x2a5a, 0x5ee5, 0x4f6c, 0x7df7, 0x6c7e,
	0xa50a, 0xb483, 0x8618, 0x9791, 0xe32e, 0xf2a7, 0xc03c, 0xd1b5,
	0x2942, 0x38cb, 0x0a50, 0x1bd9, 0x6f66, 0x7eef, 0x4c74, 0x5dfd,
	0xb58b, 0xa402, 0x9699, 0x8710, 0xf3af, 0xe226, 0xd0bd, 0xc134,
	0x39c3, 0x284a, 0x1ad1, 0x0b58, 0x7fe7, 0x6e6e, 0x5cf5, 0x4d7c,
	0xc60c, 0xd785, 0xe51e, 0xf497, 0x8028, 0x91a1, 0xa33a, 0xb2b3,
	0x4a44, 0x5bcd, 0x6956, 0x78df, 0x0c60, 0x1de9, 0x2f72, 0x3efb,
	0xd68d, 0xc704, 0xf59f, 0xe416, 0x90a9, 0x8120, 0xb3bb, 0xa232,
	0x5ac5, 0x4b4c, 0x79d7, 0x685e, 0x1ce1, 0x0d68, 0x3ff3, 0x2e7a,
	0xe70e, 0xf687, 0xc41c, 0xd595, 0xa12a, 0xb0a3, 0x8238, 0x93b1,
	0x6b46, 0x7acf, 0x4854, 0x59dd, 0x2d62, 0x3ceb, 0x0e70, 0x1ff9,
	0xf78f, 0xe606, 0xd49d, 0xc514, 0xb1ab, 0xa022, 0x92b9, 0x8330,
	0x7bc7, 0x6a4e, 0x58d5, 0x495c, 0x3de3, 0x2c6a, 0x1ef1, 0x0f78,
};
com_Hardware_t comHardware;
NEW_queue(comReadBuf,uint8_t,512)
uint8_t comSendBuf[72];
uint8_t comReadBuf[72];
/**
 * @brief COM发送函数 弱定义
 * 
 * @param buf 待发送数据
 * @param bufLen 待发送数据长度
 * 
 * @return void
 * 
 */
WEAK void COM_sendBuf(uint8_t * buf, uint16_t bufLen) {}
/**
 * @brief COM接收函数 弱定义
 * 
 * @param com 待接收数据
 * 
 * @return void
 * 
 */
WEAK void COM_receive(com_frame_t * com) {}
/**
 * @brief COM发送失败函数 弱定义
 * 
 * @param com 待发送数据
 * 
 * @return void
 * 
 */
WEAK void COM_failInSend(com_frame_t * com) {}
/**
 * @brief COM_failInReceive 接收消息错误 弱定义
 * 
 * @param c 错误代码
 * 			0:消息头重复校验出错,不应该出现的问题 一旦出现可能意味着单片机出现内存溢出的可能
 * 			1:CRC校验出错 可能通信受到干扰
 * 			2:不是当前帧的应答 可能是从机应答速度过慢 或者是有多个重复数据帧 应该已经触发过发送失败了
 * 			3:收到重复的数据帧 可能是从机应答速度过慢
 * 			4:收到了不应该使用的cmd
 * 
 * @return void
 * 
 */
WEAK void COM_failInReceive(com_frame_t * com, uint8_t c) {}
/**
 * @brief CRC16校验
 * 
 * @param data 待校验数据
 * @param length 待校验数据长度
 * 
 * @return uint16_t 校验结果
 * 
 */
uint16_t crc16_check(uint8_t* data, uint32_t length)
{
    uint16_t crc_reg = 0xFFFF;
    while (length--)
    {
        crc_reg = (crc_reg >> 8) ^ crc16_table[(crc_reg ^ *data++) & 0xff];
    }
    return (uint16_t)(~crc_reg) & 0xFFFF;
}
/**
 * @brief COM初始化
 * 
 * @param address 默认地址
 * 
 * @return void
 * 
 */
void com_init(uint8_t address)
{
	comHardware.com_rx = 0;
	comHardware.com_tx = 0;
	comHardware.address = address;
	comHardware.receiveTim = 0;
    comHardware.surplus = 0;
    comHardware.ID = 0;
	comHardware.timedOutResendTime = 100;
}
/**
 * @brief COM切换到下一个要发送的邮件
 * 
 * @return void
 * 
 */
void comTxNext()
{
	if(!comHardware.surplus) return;
	--comHardware.surplus;
	++comHardware.com_tx;
	if(comHardware.com_tx >= sizeof(comHardware.com) / sizeof(comHardware.com[0])) comHardware.com_tx = 0;
}
/**
 * @brief COM切换到下一个要填充的邮件
 * 
 * @return void
 * 
 */
void comRxNext()
{
	++comHardware.surplus;
	++comHardware.com_rx;
	if(comHardware.com_rx >= sizeof(comHardware.com) / sizeof(comHardware.com[0])) comHardware.com_rx = 0;
}
/**
 * @brief COM剩余邮件数
 * 
 * @return uint16_t
 * 
 */
uint16_t com_surplus(void)
{
	return comHardware.surplus;
}
/**
 * @brief COM填充邮件
 * 
 * @param com 待填充邮件
 * 
 * @return uint8_t
 * 
 */
uint8_t com_fillBuff(com_frame_t * com)
{
	uint8_t siz = 0;
	uint8_t i;
	uint8_t ID = 0; // 防重复ID 连续同ID视为无效
	if(!com || com->state == COM_FRAME_EMPTY) return 0;
	if(com->cmd == 0x7F)
	{	// 不能发送 0x7F 会导致应答帧地址变为广播帧
		return 0;
	}
	if(com->ID && com->addr != COM_BROADCAST_ADDR)
	{
		if(com->state == COM_FRAME_PREPARE)
		{	// 消息首次发送参与消息ID计算
			comHardware.ID += 1;
			if(!comHardware.ID) comHardware.ID = 1;
		}
		ID = comHardware.ID;
	}
	comSendBuf[siz++] = 0xAA;
	comSendBuf[siz++] = 0x55;
	comSendBuf[siz++] = com->addr;
	comSendBuf[siz++] = comHardware.address;
	comSendBuf[siz++] = ID;
	comSendBuf[siz++] = com->cmd;
	comSendBuf[siz++] = com->len;
	for(i = 0; i < com->len && i < sizeof(comSendBuf) - siz - 3; ++i)
	{
		comSendBuf[siz++] = com->data[i];
	}
	com->crc = crc16_check(comSendBuf,siz);
	comSendBuf[siz++] = (com->crc >> 8) & 0xFF;
	comSendBuf[siz++] = (com->crc >> 0) & 0xFF;
	comSendBuf[siz++] = 0x00;
	return siz;
}
/**
 * @brief COM添加邮件
 * 
 * @param c 待添加邮件
 * 
 * @return char
 * 
 */
char com_add(com_frame_t * c)
{
	uint8_t i;
	if(!c) return 1;
	if(comHardware.surplus >= sizeof(comHardware.com) / sizeof(comHardware.com[0])) return 1;
	if(c->cmd == 0x7F) return 1;
	if(c->len > 32) return 1;
	com_frame_t * com = &comHardware.com[comHardware.com_rx];
	comRxNext();
	com->addr = c->addr;
	com->ID = 1;
	com->cmd = c->cmd;
	com->len = c->len;
	for(i = 0; i < c->len; ++i)
	{
		com->data[i] = c->data[i];
	}
	com->state = COM_FRAME_PREPARE;
	com->retryCount = c->retryCount;
	return 0;
}
/**
 * @brief COM返回响应函数
 * 
 * @param c 待发送数据
 * 
 * @return void
 * 
 * @todo 待优化
 * 
 */
void sendResponse(com_frame_t * c)
{
	uint8_t siz = 0;
	c->cmd = c->cmd | 0x80;	// 添加应答标志
	c->addr = c->addr_in;
	c->ID = 0; // 应答信号不参与防重复ID计算
	siz = com_fillBuff(c);
	if(siz) COM_sendBuf(comSendBuf,siz);
	comHardware.baud_TX_temp += siz;
}
/**
 * @brief COM检查函数
 * 
 * @return void
 * 
 */
void com_inspect()
{
	uint16_t crc;
	uint8_t siz = 0;
	uint8_t i;
	com_frame_t c;
	if(comReadBuf[siz++] != 0xAA)
	{
		COM_failInReceive(NULL,0);
		return;
	}
	if(comReadBuf[siz++] != 0x55)
	{
		COM_failInReceive(NULL,0);
		return;
	}
	c.addr = comReadBuf[siz++];
	c.addr_in = comReadBuf[siz++];
	c.ID = comReadBuf[siz++];
	c.cmd = comReadBuf[siz++];
	c.len = comReadBuf[siz++];
	for(i = 0; i < c.len; ++i)
	{
		c.data[i] = comReadBuf[siz++];
	}
	c.crc = crc16_check(comReadBuf,siz);
	crc = comReadBuf[siz] << 8 | comReadBuf[siz + 1] << 0;
	if(crc != c.crc)
	{
		COM_failInReceive(&c,1);
		return;
	}
	if(c.cmd == 0x7F)
	{
		// 不应该使用的地址
		COM_failInReceive(&c,4);
	}
	if(c.addr == comHardware.address && (c.cmd & 0x80))
	{
		// 应答帧
		if(c.cmd == (comHardware.com[comHardware.com_tx].cmd | 0x80))
		{
			comHardware.com[comHardware.com_tx].state = COM_FRAME_SEND;	// 收到应答 完成发送
			comTxNext();
			COM_receive(&c);
		}	
		else 
		{
			// 不是当前消息的应答 应该已经触发过发送失败了
			COM_failInReceive(&c,2);
		}
	}
	else if(c.addr == COM_BROADCAST_ADDR)
	{
		// 广播帧
		COM_receive(&c);
	}
	else
	{
		// 数据帧
		if(c.ID != comHardware.ID || c.ID == 0)
		{
			COM_receive(&c);
			sendResponse(&c);
			comHardware.ID = c.ID;
		}
		else
		{
			// 收到重复数据帧
			COM_failInReceive(&c,3);
		}
	}
}
/**
 * @brief COM循环函数
 * 
 * @return void
 * 
 */
void com_loop(void)
{
// 接收检测
	uint8_t c;
	static uint8_t schedule = 0;
	static uint8_t len = 0;
	if(comHardware.receiveTim > 0) return;
	while(Queue_Read(comReadBuf,&c))
	{
		comReadBuf[schedule] = c;
		switch(schedule)
		{
			case 0:
				if(c == 0xAA) schedule++;
				else schedule = 0;
				break;
			case 1:
				if(c == 0x55) schedule++;
				else schedule = 0;
				break;
			case 2:
				if(c == comHardware.address || c == COM_BROADCAST_ADDR) schedule++;
				else schedule = 0;
				break;
			case 3:
				schedule++;
				break;
			case 4:
				schedule++;
				break;
			case 5:
				schedule++;
				break;
			case 6:
				if(c < 64)
				{
					schedule++;
					len = c + 2;
				}
				else
				{
					schedule = 0;
				}
				break;
			default:
				if(schedule - 6 < len) schedule++;
				else
				{
					schedule = 0;
					com_inspect();
				}
		}
	}
// 发送检测
	uint8_t siz;
	while(comHardware.com[comHardware.com_tx].state == COM_FRAME_PREPARE || comHardware.com[comHardware.com_tx].state == COM_FRAME_RETRY)
	{
		if(comHardware.com[comHardware.com_tx].retryCount)
		{
			siz = com_fillBuff(&comHardware.com[comHardware.com_tx]);
			if(siz) COM_sendBuf(comSendBuf,siz);
			comHardware.baud_TX_temp += siz;
			if(comHardware.com[comHardware.com_tx].addr == COM_BROADCAST_ADDR)
			{	// 广播地址不重发 无应答
				comHardware.com[comHardware.com_tx].retryCount = 0;
			}
			else
			{
				comHardware.com[comHardware.com_tx].retryCount--;
			}
			comHardware.com[comHardware.com_tx].state = COM_FRAME_SEND;
			comHardware.rTim = 0;
		}
		else
		{
			comHardware.com[comHardware.com_tx].state = COM_FRAME_EMPTY;
			COM_failInSend(&comHardware.com[comHardware.com_tx]);
			comTxNext();
		}
	}
}
/**
 * @brief COM硬件接口接收函数
 * 
 * @param c 接收的数据
 * 
 */
void com_receive_irq(uint8_t c)
{
	Queue_Write(comReadBuf,c);
	comHardware.receiveTim = 2;
	++comHardware.baud_RX_temp;
}
/**
 * @brief COM硬件接口定时函数
 * 
 * @param ms 中断的周期 单位:毫秒
 * 
 */
void com_tim_irq(uint8_t ms)
{
	if(comHardware.receiveTim > 0) comHardware.receiveTim -= ms;
	comHardware.baud_RXTX_timer += ms;
	if(comHardware.baud_RXTX_timer >= 1000)
	{
		comHardware.baud_RXTX_timer -= 1000;
		comHardware.baud_TX = comHardware.baud_TX_temp;
		comHardware.baud_RX = comHardware.baud_RX_temp;
		comHardware.baud_TX_total += comHardware.baud_TX;
		comHardware.baud_RX_total += comHardware.baud_RX;
		comHardware.baud_TX_temp = 0;
		comHardware.baud_RX_temp = 0;
	}
	if(comHardware.com[comHardware.com_tx].state == COM_FRAME_SEND)
	{
		comHardware.rTim += ms;
		
		if(comHardware.rTim >= comHardware.timedOutResendTime)
		{	// 超时重发
			comHardware.com[comHardware.com_tx].state = COM_FRAME_RETRY;
		}
	}
}
