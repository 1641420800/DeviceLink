#include "stm32_hal_core.h"
#include "tim.h"

uint32_t CORE_Timer_GetTime_us(void)
{
    return TIM7->CNT;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM7)
    {
        CORE_Timer_IRQHandler(1);
    }
}

void core_init(void)
{
    CORE_init();
    CORE_Timer_Init();
    HAL_TIM_Base_Start_IT(&htim7);
}

void core_loop(void)
{
    CORE_Run_loop();
}
