/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

typedef struct 
{
  /* data */
  uint8_t state;
  uint16_t press_time;
  uint16_t release_time;
} Key;

typedef struct 
{
  /* data */
  uint8_t polarity;
  uint8_t state;
  uint16_t count;
  uint16_t pulse_time;
} Pulse;

enum KeyState
{
  Key_None,
  Key_Press,
  Key_Release
};

enum PulseState
{
  Pulse_None,
  Pulse_NoWork,
  Pulse_Work
};
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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOC
#define A_Pulse_Pin GPIO_PIN_0
#define A_Pulse_GPIO_Port GPIOA
#define B_Pulse_Pin GPIO_PIN_1
#define B_Pulse_GPIO_Port GPIOA
#define KEY_UP_Pin GPIO_PIN_13
#define KEY_UP_GPIO_Port GPIOD
#define KEY_DOWN_Pin GPIO_PIN_0
#define KEY_DOWN_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
#define KEY_UP()  !HAL_GPIO_ReadPin(KEY_UP_GPIO_Port, KEY_UP_Pin)
#define KEY_DOWN()  !HAL_GPIO_ReadPin(KEY_DOWN_GPIO_Port, KEY_DOWN_Pin)
#define A_High()  HAL_GPIO_WritePin(A_Pulse_GPIO_Port, A_Pulse_Pin, GPIO_PIN_SET)
#define A_Low()  HAL_GPIO_WritePin(A_Pulse_GPIO_Port, A_Pulse_Pin, GPIO_PIN_RESET)

#define B_High()  HAL_GPIO_WritePin(B_Pulse_GPIO_Port, B_Pulse_Pin, GPIO_PIN_SET)
#define B_Low()  HAL_GPIO_WritePin(B_Pulse_GPIO_Port, B_Pulse_Pin, GPIO_PIN_RESET)

#define Led_Toggle() HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
