#include "pwr.h"
#include <stdlib.h>
#include "stm32f10x_flash.h"
#include "delay.h"
#define PVD_delay(ms) delay_ms(ms)

/*
#include "pwr.h"

	// 需要在 3.3V上并联一颗大电容
	PVD_flashStart(data,29);
*/

WEAK
void PVD_irq(void)
{
	// 断电中断 尽快退出
	
}

void powerFailureDetection_init(uint16_t v_10)
{
	NVIC_InitTypeDef NVIC_InitStruct;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStruct.NVIC_IRQChannel 					= PVD_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority	= 0;			// 抢占式优先级
	NVIC_InitStruct.NVIC_IRQChannelSubPriority			= 0;			// 响应式优先级
	NVIC_InitStruct.NVIC_IRQChannelCmd					= ENABLE;		// 使能通道
	NVIC_Init(&NVIC_InitStruct);										// NVIC初始
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP, ENABLE);
	
	EXTI_ClearITPendingBit(EXTI_Line16);
	EXTI_StructInit(&EXTI_InitStructure);
	EXTI_InitStructure.EXTI_Line						= EXTI_Line16; 
	EXTI_InitStructure.EXTI_Mode						= EXTI_Mode_Interrupt; 
	EXTI_InitStructure.EXTI_Trigger						= EXTI_Trigger_Rising;	//EXTI_Trigger_Falling   ;//EXTI_Trigger_Rising; 
	EXTI_InitStructure.EXTI_LineCmd						= ENABLE; 
	EXTI_Init(&EXTI_InitStructure);

	if(v_10 >= 29)		PWR_PVDLevelConfig(PWR_PVDLevel_2V9);
	else if(v_10 >= 28)	PWR_PVDLevelConfig(PWR_PVDLevel_2V8);
	else if(v_10 >= 27)	PWR_PVDLevelConfig(PWR_PVDLevel_2V7);
	else if(v_10 >= 26)	PWR_PVDLevelConfig(PWR_PVDLevel_2V6);
	else if(v_10 >= 25)	PWR_PVDLevelConfig(PWR_PVDLevel_2V5);
	else if(v_10 >= 24)	PWR_PVDLevelConfig(PWR_PVDLevel_2V4);
	else if(v_10 >= 23)	PWR_PVDLevelConfig(PWR_PVDLevel_2V3);
	else /*v_10 < 23*/	PWR_PVDLevelConfig(PWR_PVDLevel_2V2);

	PWR_PVDCmd(ENABLE);
}


#if STM32F10X_MD
#define FLASH_ADDR_A 0x0800FC00
#define FLASH_ADDR_B 0x0800F800
#elif STM32F10X_HD
#define FLASH_ADDR_A 0x0801FC00
#define FLASH_ADDR_B 0x0801F800
#else
#error "undefin STM32F10X_XX"
#endif


uint32_t * Data = NULL;
uint32_t Number = 0;

void PVD_flash_save(void)
{
	uint32_t Address = FLASH_ADDR_B;
	uint16_t position;
	FLASH_Status FLASHStatus;
	uint8_t err_time = 0;
	/* 解锁 */
	FLASH_Unlock();
	
	/* 清空所有标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* 尝试擦除*/
	do
	{
		PVD_delay(10);
		FLASHStatus = FLASH_ErasePage(FLASH_ADDR_B);
	} while(FLASHStatus != FLASH_COMPLETE && ++err_time < 50);
		
	/* 擦除失败*/
	if (err_time >= 50) return;
	
	/* 向内部 FLASH 写入数据 */
	for(position = 0; position < Number; ++position)
	{
		FLASH_ProgramWord(Address, Data[position]);
		Address = Address + 4;
	}
	FLASH_ProgramWord(Address, 0);

	/* 锁定*/
	FLASH_Lock();
}

/*
	@return 
		2 数据丢失恢复失败 启动成功
		1 数据丢失 尝试恢复成功
		0 启动成功
		-1 参数错误
		-2 擦除失败
*/
int PVD_flash_start(uint32_t * data, uint32_t number, uint16_t v_10)
{
	FLASH_Status FLASHStatus;
	uint8_t err_time = 0;
	uint32_t Address;
	uint16_t position;
	int ret = 0;
	// 参数检查
	if(data == NULL || number == 0) return -1;
	if(number >= 0x100) return -1;
	
	// 延时防止抖动
	PVD_delay(500);

	if(((__IO uint32_t*) FLASH_ADDR_A)[number])
	{
		// 数据丢失
		if(((__IO uint32_t*) FLASH_ADDR_B)[number])
		{
			ret = 2;
		}
		else
		{
			ret = 1;
		}
		
	}
	else
	{
		// 数据未丢失 备份到B区

		/* 解锁 */
		FLASH_Unlock();
		
		/* 清空所有标志位 */
		FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
		
		/* 尝试擦除*/
		do
		{
			PVD_delay(10);
			FLASHStatus = FLASH_ErasePage(FLASH_ADDR_B);
		} while(FLASHStatus != FLASH_COMPLETE && ++err_time < 50);
		
		/* 擦除失败*/
		if (err_time >= 50) return -2;
		
		Address = FLASH_ADDR_B;
		
		/* 向内部 FLASH 写入数据 */
		for(position = 0; position < number; ++position)
		{
			FLASH_ProgramWord(Address, ((__IO uint32_t*) FLASH_ADDR_A)[position]);
			Address = Address + 4;
		}
		FLASH_ProgramWord(Address, 0);
		
		/* 锁定*/
		FLASH_Lock();
	}
	
	if(((__IO uint32_t*) FLASH_ADDR_B)[number] == 0)
	{
		Address = FLASH_ADDR_B;
		for(position = 0; position < number; ++position)
		{
			data[position] = ((__IO uint32_t*) FLASH_ADDR_B)[position];
		}
	}

	/* 解锁 */
	FLASH_Unlock();
	
	/* 清空所有标志位 */
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);
	
	/* 尝试擦除*/
	do
	{
		PVD_delay(10);
		FLASHStatus = FLASH_ErasePage(FLASH_ADDR_A);
	} while(FLASHStatus != FLASH_COMPLETE && ++err_time < 50);
		
	/* 擦除失败*/
	if (err_time >= 50) return -2;
	
	/* 锁定*/
	FLASH_Lock();

	Data = data;
	Number = number;
	powerFailureDetection_init(v_10);

	return ret;
}

void PVD_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line16) != RESET)
	{
		PVD_irq();

		if(Data)
		{
			uint32_t Address = FLASH_ADDR_A;
			uint16_t position;
			/* 解锁 */
			FLASH_Unlock();
	
			/* 清空所有标志位 */
			FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_PGERR | FLASH_FLAG_WRPRTERR);

			/* 向内部 FLASH 写入数据 */
			for(position = 0; position < Number; ++position)
			{
				FLASH_ProgramWord(Address, Data[position]);
				Address = Address + 4;
			}
			FLASH_ProgramWord(Address, 0);

			/* 锁定*/
			FLASH_Lock();
		}
		
        EXTI_ClearITPendingBit(EXTI_Line16); // 清除中断标志位
	}
}

#undef FLASH_ADDR
