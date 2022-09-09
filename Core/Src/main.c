/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <rtthread.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
Key g_key_up, g_key_down;
Pulse g_pulse;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define PULSE_CNT  2500
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);

uint16_t g_led_cnt;

/* USER CODE BEGIN PFP */
static void task_key_check(void *parameter)
{
    while (1)
    {
        if (g_key_up.state == Key_None)
        {
            if (KEY_UP())
            {
                g_key_up.press_time++;

                if (g_key_up.press_time >= 5 * 1)
                {
                    g_key_up.press_time = 0;

                    if (KEY_UP())
                    {
                        g_key_up.state = Key_Press;
                    }
                }
            }
            else
            {
                g_key_up.press_time = 0;
            }
        }

        if (g_key_down.state == Key_None)
        {
            if (KEY_DOWN())
            {
                g_key_down.press_time++;

                if (g_key_down.press_time >= 5 * 1)
                {
                    g_key_down.press_time = 0;

                    if (KEY_DOWN())
                    {
                        g_key_down.state = Key_Press;
                    }
                }
            }
            else
            {
                g_key_down.press_time = 0;
            }
        }

        if (g_key_up.state == Key_Press)
        {
            if (!KEY_UP())
            {
                g_key_up.press_time++;

                if (g_key_up.press_time >= 5 * 1)
                {
                    g_key_up.press_time = 0;

                    if (!KEY_UP())
                    {
                        g_key_up.state = Key_Release;
                    }
                }
            }
        }

        if (g_key_down.state == Key_Press)
        {
            if (!KEY_DOWN())
            {
                g_key_down.press_time++;

                if (g_key_down.press_time >= 5 * 1)
                {
                    g_key_down.press_time = 0;

                    if (!KEY_DOWN())
                    {
                        g_key_down.state = Key_Release;
                    }
                }
            }
        }

        if (g_key_up.state == Key_Release)
        {
            if (!KEY_UP())
            {
                g_key_up.press_time++;

                if (g_key_up.press_time >= 5 * 1)
                {
                    g_key_up.press_time = 0;

                    if (!KEY_UP())
                    {
                        g_key_up.state = Key_None;
                    }
                }
            }
        }

        if (g_key_down.state == Key_Release)
        {
            if (!KEY_DOWN())
            {
                g_key_down.press_time++;

                if (g_key_down.press_time >= 5 * 1)
                {
                    g_key_down.press_time = 0;

                    if (!KEY_DOWN())
                    {
                        g_key_down.state = Key_None;
                    }
                }
            }
        }

        g_led_cnt++;

        if (g_led_cnt >= 50)
        {
            g_led_cnt = 0;
            Led_Toggle();
        }

        rt_thread_delay(1);
    }
}

static void task_pulse_produce(void *parameter)
{
    while (1)
    {
        if (g_pulse.state == Pulse_None)
        {
            if (g_key_up.state == Key_Press)
            {
                g_pulse.state = Pulse_Work;
                g_pulse.polarity = 1;
                continue;
            }

            if (g_key_down.state == Key_Press)
            {
                g_pulse.state = Pulse_Work;
                g_pulse.polarity = 0;
                continue;
            }
        }

        if (g_pulse.state == Pulse_Work)
        {
            if (g_pulse.polarity)
            {
                if (g_pulse.pulse_time == 4)
                {
                    g_pulse.pulse_time = 0;

                    g_pulse.count++;

                    if (g_pulse.count == PULSE_CNT)
                    {
                        g_pulse.count = 0;
                        g_pulse.state = Pulse_NoWork;
                        continue;
                    }
                }

                if (g_pulse.pulse_time == 0)
                {
                    g_pulse.pulse_time = 0;
                    A_High();
                    B_Low();
                }

                if (g_pulse.pulse_time == 1)
                {
                    B_High();
                }

                if (g_pulse.pulse_time == 2)
                {
                    A_Low();
                }

                if (g_pulse.pulse_time == 3)
                {
                    B_Low();
                }

                g_pulse.pulse_time++;
            }
            else
            {

                if (g_pulse.pulse_time == 4)
                {
                    g_pulse.pulse_time = 0;
                    g_pulse.count++;

                    if (g_pulse.count == PULSE_CNT)
                    {
                        g_pulse.count = 0;
                        g_pulse.state = Pulse_NoWork;
                        continue;
                    }
                }

                if (g_pulse.pulse_time == 0)
                {
                    g_pulse.pulse_time = 0;
                    B_High();
                    A_Low();
                }

                if (g_pulse.pulse_time == 1)
                {
                    A_High();
                }

                if (g_pulse.pulse_time == 2)
                {
                    B_Low();
                }

                if (g_pulse.pulse_time == 3)
                {
                    A_Low();
                }

                g_pulse.pulse_time++;
            }
        }

        if (g_pulse.state == Pulse_NoWork)
        {
            if (g_key_up.state == Key_None && g_key_down.state == Key_None)
            {
                g_pulse.state = Pulse_None;
            }
        }


        rt_thread_delay(10);
    }
}
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static  rt_thread_t key_thread = RT_NULL;
static  rt_thread_t pulse_thread = RT_NULL;
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
    /* USER CODE BEGIN 1 */

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

    A_Low();
    B_Low();
    /* USER CODE BEGIN 2 */
    key_thread = rt_thread_create("key_check", task_key_check, RT_NULL, 512, 1, 1);

    if (key_thread != NULL)
    {
        rt_thread_startup(key_thread);
    }

    pulse_thread = rt_thread_create("pulse_produce", task_pulse_produce, RT_NULL, 512, 1, 1);

    if (pulse_thread != NULL)
    {
        rt_thread_startup(pulse_thread);
    }

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
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

    /** Initializes the RCC Oscillators according to the specified parameters
    * in the RCC_OscInitTypeDef structure.
    */
    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;

    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
    {
        Error_Handler();
    }

    /** Initializes the CPU, AHB and APB buses clocks
    */
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
                                  | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
    {
        Error_Handler();
    }
}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* GPIO Ports Clock Enable */
    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOE_CLK_ENABLE();

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, GPIO_PIN_RESET);

    /*Configure GPIO pin Output Level */
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0 | GPIO_PIN_1, GPIO_PIN_RESET);

    /*Configure GPIO pin : PC13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /*Configure GPIO pins : PA0 PA1 */
    GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /*Configure GPIO pin : PD13 */
    GPIO_InitStruct.Pin = GPIO_PIN_13;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /*Configure GPIO pin : PE0 */
    GPIO_InitStruct.Pin = GPIO_PIN_0;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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
