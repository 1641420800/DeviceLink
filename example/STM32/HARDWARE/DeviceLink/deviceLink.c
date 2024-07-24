#include "deviceLink.h"


uint32_t CORE_Timer_GetTime_us(void)
{
	return get_CNT_value();
}

void deviceLink_tim_IRQ(ServiceTimer_t * serviceTimer)
{
	CORE_Timer_IRQHandler(serviceTimer->timeInterval);
}

void deviceLink_init(void)
{
	CORE_init();
	CORE_Timer_Init();
    registerTimer(1,deviceLink_tim_IRQ);
}

void deviceLink_loop(void)
{
	CORE_Run_loop();
}
