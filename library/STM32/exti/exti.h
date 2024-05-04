#ifndef _EXTI_H_
#define _EXTI_H_

#include "stm32f10x_gpio.h"
#include "stm32f10x_exti.h"

#ifndef WEAK
#	ifdef __CC_ARM
#		define WEAK __weak
#	elif defined(__IAR_SYSTEMS_ICC__)
#		define WEAK __weak
#	elif defined(__GNUC__)
#		define WEAK __attribute__((weak))
#	elif defined(__ADSPBLACKFIN__)
#		define WEAK __attribute__((weak))
#	elif defined(_MSC_VER)
#		define WEAK
#	elif defined(__TI_COMPILER_VERSION__)
#		define WEAK
#	else
#		error not supported tool chain
#	endif
#endif

#define ServiceEXTI 1

void EXTIx_Init(GPIO_TypeDef * GPIO, uint16_t PIN);


#if ServiceEXTI
typedef struct _ServiceExti ServiceExti_t;
typedef void(*extiCallback_t)(ServiceExti_t * serviceExti);
struct _ServiceExti
{
	uint16_t PIN;
	extiCallback_t extiCallback;
	struct _ServiceExti * next;
};

void registerExti(GPIO_TypeDef * GPIO,uint16_t PIN,extiCallback_t extiCallback);

#endif

#endif
