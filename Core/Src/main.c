/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

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
void TIM2_IRQHandler(void);
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

	// Set up the clock
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;
  SystemClock_Config();

	// Configure the LEDs
	GPIOC->MODER |= (1<<16) | (1<<18) | (1<<13) | (1<<15);
	GPIOC->MODER &= ~((1<<17) | (1<<19) | (1<<12) | (1<<14)); // red and blue are in alternative function mode
	GPIOC->OTYPER &= ~((1<<6) | (1<<7) | (1<<8) | (1<<9));
	GPIOC->OSPEEDR &= ~((1<<12) | (1<<14) | (1<<16) | (1<<18));
	GPIOC->PUPDR &= ~((1<<12) | (1<<14) | (1<<16) | (1<<18)
									| (1<<13) | (1<<15) | (1<<17) | (1<<19));
	GPIOC->AFR[0] &= ~((1<<24) | (1<<25) | (1<<26) | (1<<27)
									| (1<<28) | (1<<29) | (1<<30) | (1<<31));
	
	
	// Set up the timer to trigger a UEV interrupt
//  TIM2->PSC = 7999;
//	TIM2->ARR = 250;
//	TIM2->DIER |= (1<<0);
//	TIM2->CR2 |= (1<<4);
//	TIM2->CR1 |= (1<<0);
	
	// Set up the timer to trigger the LEDs based on the Capture/Compare register
	TIM3->PSC = 99;//999;
	TIM3->ARR = 100;//10;
	TIM3->CCMR1 &= ~((1<<0) | (1<<1) | (1<<8) | (1<<9));
	TIM3->CCMR1 |= (1<<4) | (1<<5) | (1<<6);
	TIM3->CCMR1 |= (1<<14) | (1<<13);
	TIM3->CCMR1 &= ~(1<<12);
	TIM3->CCMR1 |= (1<<3) | (1<<11);
	TIM3->CCER |= (1<<0) | (1<<4);
	TIM3->CCR1 = 80;
	TIM3->CCR2 = 80;
	TIM3->CR1 |= 1;
	
	
	// Enable the interrupt
//	NVIC_EnableIRQ(15);
//	NVIC_SetPriority(15,1);
	
	// Turn on the green LED
//	GPIOC->ODR |= (1<<8);
//	GPIOC->ODR &= ~(1<<9);

	
  while (1)
  {
    
  }
 
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void TIM2_IRQHandler(void)
{
	// Toggle the leds
	GPIOC->ODR ^= (1<<8);
	GPIOC->ODR ^= (1<<9);
	
	// Disable the flag
	TIM2->SR &= ~(1<<0);
}
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


