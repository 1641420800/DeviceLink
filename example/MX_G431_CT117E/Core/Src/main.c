/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include "lcd.h"
#include "core.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void CORE_callback_1(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line0,(u8*)outBuf);
}
void CORE_callback_2(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line1,(u8*)outBuf);
}
void CORE_callback_3(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line2,(u8*)outBuf);
}
void CORE_callback_4(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line3,(u8*)outBuf);
}
void CORE_callback_5(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line4,(u8*)outBuf);
}
void CORE_callback_6(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line5,(u8*)outBuf);
}
void CORE_callback_7(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line6,(u8*)outBuf);
}
void CORE_callback_8(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line7,(u8*)outBuf);
}
void CORE_callback_9(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed(topic,&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line8,(u8*)outBuf);
}
void CORE_callback_all2(const char *topic, void *arg, size_t siz)
{
  char outBuf[64];
  uint16_t speed;
  CORE_speed("*",&speed);
  sprintf(outBuf,"%d %s : %d",siz, (char*)arg,speed);
  
	LCD_DisplayStringLine(Line9,(u8*)outBuf);
}
void CORE_callback_all(const char *topic, void *arg, size_t siz)
{
	CORE_subscribe(arg,CORE_callback_all2);
}

void timer_callback_1(void *arg)
{
  char outBuf[64];
  sprintf(outBuf,"%u",CORE_Timer_GetCurrentTime_ms());
  CORE_publish_str("789",outBuf,0);

}

uint32_t CORE_Timer_GetTime_us(void)
{
    return TIM7->CNT;
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if(htim->Instance == TIM7)
  {
    CORE_Timer_IRQHandler(1);
  }
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  char outBuf[64];
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM3_Init();
  MX_USART1_UART_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  LCD_Init();
  LCD_Clear(Black);
  
	CORE_init();
	CORE_subscribe("CORE_subscribe",CORE_callback_all);
	CORE_subscribe("123",CORE_callback_1);
	CORE_subscribe("456",CORE_callback_2);
	CORE_subscribe("123",CORE_callback_3);
	CORE_subscribe("456",CORE_callback_4);
//	CORE_subscribe("123",CORE_callback_5);
//	CORE_subscribe("456",CORE_callback_6);
//	CORE_subscribe("456",CORE_callback_7);
//	CORE_subscribe("123",CORE_callback_8);
	CORE_subscribe("789",CORE_callback_9);
  
  CORE_Timer_add_task(timer_callback_1,NULL,100);
  
  HAL_TIM_Base_Start_IT(&htim7);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
  #if 1
    sprintf(outBuf,"%u",CORE_Timer_GetCurrentTime_us());
		CORE_publish_str("123",outBuf,0);
    sprintf(outBuf,"%u",CORE_Timer_GetCurrentTime_ms());
		CORE_publish_str("456",outBuf,0);
		CORE_Run_loop();
  #else
    CORE_callback_1("123","abc",4);
    CORE_callback_2("456","def",4);
    CORE_callback_3("123","abc",4);
    CORE_callback_4("456","def",4);
    CORE_callback_5("123","abc",4);
    CORE_callback_6("456","def",4);
    CORE_callback_7("123","abc",4);
    CORE_callback_8("456","def",4);
    CORE_callback_9("123","abc",4);
    CORE_callback_all("123","abc",4);
    CORE_callback_all("456","def",4);
  #endif
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV6;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
