#include "PWM.h"

#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"

/*
注意事项：尽量不要使用PB3、PB4和PA15引脚，如果当做普通引脚使用需要禁用JTAG，导致后续这个板子不能下载程序；
调用方法：
#include "PWM.h"
	
	TIM_PWM_Init(1000-1,72-1);		//定时器初始化 对应的频率计算方法 72MHz / 1000 = 72kHz  ,  72kHz / 72 = 1kHz
		
		PA0_PWM(500);				//对应的占空比计算方法 初始化时第一个参数为 1000 - 1 占空比为 500 / 1000 = 50%
	
	
硬件定时器:
	说明 : 
		TIM2 共计4通道
		CH1 ~ CH4
		有8个引脚可用 (同时只能选下面四组中的一组)
		
		通道    组1     组2     组3     组4
		CH1     PA0     PA15    PA0     PA15
		CH2     PA1     PB3     PA1     PB3
		CH3     PA2     PA2     PB10    PB10
		CH4     PA3     PA3     PB11    PB11
		
		
		TIM3 共计4通道
		CH1 ~ CH4
		有6个引脚可用 (同时只能选下面两组中的一组)
		
		通道    组1     组2
		CH1     PA6     PB4
		CH2     PA7     PB5
		CH3     PB0     PB0
		CH4     PB1     PB1
		
		
		TIM4 共计4通道   !!! 注意 : STM32F103C6T6 没有 TIM4 !!!
		CH1 ~ CH4
		有4个引脚可用
		
		通道    组1
		CH1     PB6
		CH2     PB7
		CH3     PB8
		CH4     PB9
		
	使用步骤 - PWM输出:
		1. 选择对应的定时器引脚组 0 表示不使用
			#define TIM2_PWM_INIT 1		// 选择 定时器2 的引脚
			#define TIM3_PWM_INIT 0		// 选择 定时器3 的引脚
			#define TIM4_PWM_INIT 0		// 选择 定时器4 的引脚
		
		2. 选择要打开的引脚 在对应的组别里
			#if TIM2_PWM_INIT == 1
			#define PWM_PA0  OPEN
			#define PWM_PA1  OPEN
			#define PWM_PA2  OPEN
			#define PWM_PA3  OPEN
			#endif
			
		3. 初始化定时器		-  对应的频率计算方法 72MHz / 1000 = 72kHz  ,  72kHz / 72 = 1kHz
			TIM_PWM_Init(1000-1,72-1);
		
		4. 使用				-  对应的占空比计算方法 初始化时第一个参数为 1000 - 1 占空比为 500 / 1000 = 50%
			PA0_PWM(500);
			PA1_PWM(500);
			PA2_PWM(500);
			PA3_PWM(500);
			
			
	使用步骤 - 舵机:
		1. 与 PWM输出 一样
		
		2. 与 PWM输出 一样
			
		3. 初始化定时器
			Servo_init();
		
		4. 使用
			Servo_PA0(90);
			
*/ 



#define OPEN  1				// 打开引脚
#define CLOSE 0				// 关闭引脚

#define TIM2_PWM_INIT 0		// 选择 定时器2 的引脚
#define TIM3_PWM_INIT 1		// 选择 定时器3 的引脚
#define TIM4_PWM_INIT 0		// 选择 定时器4 的引脚

#if TIM2_PWM_INIT == 1
#define PWM_PA0  OPEN
#define PWM_PA1  OPEN
#define PWM_PA2  OPEN
#define PWM_PA3  OPEN
#endif
#if TIM2_PWM_INIT == 2
#define PWM_PA15 OPEN
#define PWM_PB3  OPEN
#define PWM_PA2  OPEN
#define PWM_PA3  OPEN
#endif
#if TIM2_PWM_INIT == 3
#define PWM_PA0  OPEN
#define PWM_PA1  OPEN
#define PWM_PB10 OPEN
#define PWM_PB11 OPEN
#endif
#if TIM2_PWM_INIT == 4
#define PWM_PA15 OPEN
#define PWM_PB3  OPEN
#define PWM_PB10 OPEN
#define PWM_PB11 OPEN
#endif
#if TIM3_PWM_INIT == 1
#define PWM_PA6  OPEN
#define PWM_PA7  OPEN
#define PWM_PB0  OPEN
#define PWM_PB1  OPEN
#endif
#if TIM3_PWM_INIT == 2
#define PWM_PB4  OPEN
#define PWM_PB5  OPEN
#define PWM_PB0  OPEN
#define PWM_PB1  OPEN
#endif
#if TIM4_PWM_INIT == 1
#define PWM_PB6  OPEN
#define PWM_PB7  OPEN
#define PWM_PB8  OPEN
#define PWM_PB9  OPEN
#endif

void TIM_PWM_Init(uint16_t arr,uint16_t psc)
{  
	GPIO_InitTypeDef         GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef        TIM_OCInitStructure;
	
#if TIM2_PWM_INIT
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);							//使能定时器2时钟
#endif
#if TIM3_PWM_INIT
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);							//使能定时器3时钟
#endif
#if TIM4_PWM_INIT
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);							//使能定时器4时钟
#endif

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);							//使能复用功能时钟
	
#if PWM_PA15 || PWM_PB3 || PWM_PB4
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);						// 禁用JTAG调试
#endif

	// 使能端口时钟
#if PWM_PA0 || PWM_PA1 || PWM_PA2 || PWM_PA3 || PWM_PA6 || PWM_PA7 || PWM_PA15
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
#endif
#if PWM_PB0 || PWM_PB1 || PWM_PB3 || PWM_PB4 || PWM_PB5 || PWM_PB6 || PWM_PB7 || PWM_PB8 || PWM_PB9 || PWM_PB10 || PWM_PB11
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
#endif

	// 定时器复用功能
#if TIM2_PWM_INIT == 2
	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
#endif
#if TIM2_PWM_INIT == 3
	GPIO_PinRemapConfig(GPIO_PartialRemap2_TIM2, ENABLE);
#endif
#if TIM2_PWM_INIT == 4
	GPIO_PinRemapConfig(GPIO_FullRemap_TIM2, ENABLE);
#endif
#if TIM3_PWM_INIT == 2
	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE);
#endif
	
	// 配置引脚
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_AF_PP;									// 复用推挽输出
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
#if PWM_PA0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA2
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA7 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PA15 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;	
	GPIO_Init(GPIOA, &GPIO_InitStructure);		
#endif
#if PWM_PB0
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB1
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB3
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB4
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB7
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB8
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB10
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif
#if PWM_PB11
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;	
	GPIO_Init(GPIOB, &GPIO_InitStructure);		
#endif

	TIM_TimeBaseStructure.TIM_Period		= arr;									// 定时器周期 ARR
	TIM_TimeBaseStructure.TIM_Prescaler		= psc;									// 预分频系数 PSC 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;									// 外部输入时钟分频，基本定时器用不到
	TIM_TimeBaseStructure.TIM_CounterMode	= TIM_CounterMode_Up;					// 计数模式，基本定时器只能向上递增计数
	/*
		TIM_OCMode_PWM1
		TIM_OCMode_PWM2
	*/ 
	TIM_OCInitStructure.TIM_OCMode			= TIM_OCMode_PWM1;						// PWM输出模式
	TIM_OCInitStructure.TIM_OutputState		= TIM_OutputState_Enable;				// 使能PWM输出
	/*
		TIM_OCPolarity_High
		TIM_OCPolarity_Low
	*/
	TIM_OCInitStructure.TIM_OCPolarity		= TIM_OCPolarity_High;					// 输出极性
	
#if TIM2_PWM_INIT
	// 配置定时器2
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	// 配置定时器2通道
#if PWM_PA0 || PWM_PA15
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
#if PWM_PA1 || PWM_PB3
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
#if PWM_PA2 || PWM_PB10
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
#if PWM_PA3 || PWM_PB11
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);
#endif
	TIM_Cmd(TIM2, ENABLE);
#endif

#if TIM3_PWM_INIT
	// 配置定时器3
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
	// 配置定时器3通道
#if PWM_PA6 || PWM_PB4
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if PWM_PA7 || PWM_PB5
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if PWM_PB0
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
#if PWM_PB1
	TIM_OC4Init(TIM3, &TIM_OCInitStructure);
	TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);
#endif
	TIM_Cmd(TIM3, ENABLE);	
#endif
	
#if TIM4_PWM_INIT
	// 配置定时器4
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
#if PWM_PB6
	TIM_OC1Init(TIM4, &TIM_OCInitStructure);										//????T?????????????????TIM3 OC1
	TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);								//???TIM3??CCR2???????????
#endif
#if PWM_PB7
	TIM_OC2Init(TIM4, &TIM_OCInitStructure);										//????T?????????????????TIM3 OC1
	TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);								//???TIM3??CCR2???????????
#endif
#if PWM_PB8
	TIM_OC3Init(TIM4, &TIM_OCInitStructure);										//????T?????????????????TIM3 OC1
	TIM_OC3PreloadConfig(TIM4, TIM_OCPreload_Enable);								//???TIM3??CCR2???????????
#endif
#if PWM_PB9
	TIM_OC4Init(TIM4, &TIM_OCInitStructure);										//????T?????????????????TIM3 OC1
	TIM_OC4PreloadConfig(TIM4, TIM_OCPreload_Enable);								//???TIM3??CCR2???????????
#endif
	TIM_Cmd(TIM4, ENABLE);	
#endif	
}


#undef OPEN
#undef CLOSE
#undef PWM_PA0
#undef PWM_PA1
#undef PWM_PA2
#undef PWM_PA3
#undef PWM_PA6
#undef PWM_PA7
#undef PWM_PA15
#undef PWM_PB0
#undef PWM_PB1
#undef PWM_PB3
#undef PWM_PB4
#undef PWM_PB5
#undef PWM_PB6
#undef PWM_PB7
#undef PWM_PB8
#undef PWM_PB9
#undef PWM_PB10
#undef PWM_PB11
#undef TIM2_PWM_INIT
#undef TIM3_PWM_INIT
#undef TIM4_PWM_INIT
