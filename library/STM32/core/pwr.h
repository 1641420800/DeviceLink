#ifndef _PWR_H_
#define _PWR_H_

#include "stm32f10x_pwr.h"
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

#define PVD_flashStart(data,voltage) PVD_flash_start((uint32_t*)&data,sizeof(data)/sizeof(uint32_t),voltage)

void powerFailureDetection_init(uint16_t v_10);
int PVD_flash_start(uint32_t * data, uint32_t number, uint16_t v_10);
void PVD_flash_save(void);

#endif
