/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f3xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern ADC_HandleTypeDef*  p_hadc1;
extern DMA_HandleTypeDef*  p_hdma_adc1;
extern DAC_HandleTypeDef*  p_hdac;
extern I2C_HandleTypeDef*  p_hi2c1;
extern TIM_HandleTypeDef*  p_htim2;
extern TIM_HandleTypeDef*  p_htim15;
extern UART_HandleTypeDef* p_huart3;
extern DMA_HandleTypeDef*  p_hdma_usart3_tx;

extern void I2C1_IRQHandler();
void I2C1_ClearBusyFlagErratum();

extern void newCharRxUART3(void);
extern void waitForNewCharRxUART3(void);

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define TIM15_PERIOD_TICKS 4800
#define TIM2_PERIOD_TICKS 2400
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define USART_TX_Pin GPIO_PIN_2
#define USART_TX_GPIO_Port GPIOA
#define USART_RX_Pin GPIO_PIN_3
#define USART_RX_GPIO_Port GPIOA
#define LD2_Pin GPIO_PIN_13
#define LD2_GPIO_Port GPIOB
#define CmdQout_Pin GPIO_PIN_7
#define CmdQout_GPIO_Port GPIOC
#define EN_ACT_Pin GPIO_PIN_10
#define EN_ACT_GPIO_Port GPIOA
#define IN1_ACT_Pin GPIO_PIN_11
#define IN1_ACT_GPIO_Port GPIOA
#define IN2_ACT_Pin GPIO_PIN_12
#define IN2_ACT_GPIO_Port GPIOA
#define TMS_Pin GPIO_PIN_13
#define TMS_GPIO_Port GPIOA
#define TCK_Pin GPIO_PIN_14
#define TCK_GPIO_Port GPIOA
#define SWO_Pin GPIO_PIN_3
#define SWO_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
