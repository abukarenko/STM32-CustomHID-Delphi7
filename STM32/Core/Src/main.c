/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
#include "usb_device.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usbd_custom_hid_if.h"
#include "usbd_customhid.h"
#include "usb_device.h"
#include "ILI9488.h"
#include "gui.h"
#include "gui_state.h"
#include "hid_gui.h"
#include "xpt2046.h"
#include <stdio.h>
#include <string.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define DE_BUG 1
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
extern USBD_HandleTypeDef hUsbDeviceFS;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
static void MX_USART1_UART_Init(void);
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
  MX_SPI1_Init();
  XPT2046_Init();
  MX_USART1_UART_Init();
   uint8_t msg[] = "STM32 HID started\r\n";
   HAL_UART_Transmit(&huart1, msg, sizeof(msg) - 1, 100);

  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */



  lcd_init();
  HAL_Delay(100);
  fill_rectangle(0,0,480,320,COLOR_BLACK);
GUI_Draw();

//  TS_Point p;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
while (1)
{
  /* USER CODE END WHILE */

  /* USER CODE BEGIN 3 */

	static uint32_t heartbeat_tmr = 0;
	static uint32_t last_pc_rx_tick = 0;
	static uint8_t pc_connected = 1;

	if (HAL_GetTick() - heartbeat_tmr > 200)
	{
	    heartbeat_tmr = HAL_GetTick();

	    uint8_t report[8];

	    report[0] = 0x7F;   // HEARTBEAT
	    report[1] = GuiState.value1;
	    report[2] = GuiState.value2;
	    report[3] = GuiState.value3;
	    report[4] = GuiState.sw1;
	    report[5] = GuiState.sw2;
	    report[6] = GuiState.sw3;
	    report[7] =
	        report[0] +
	        report[1] +
	        report[2] +
	        report[3] +
	        report[4] +
	        report[5] +
	        report[6];

	    USBD_CUSTOM_HID_SendReport(&hUsbDeviceFS, report, 8);
	}

  if (pc_connected && ((HAL_GetTick() - last_pc_rx_tick) > 3000))
  {
      pc_connected = 0;
      GUI_Status("Disconnect");
  }
  uint16_t tx, ty;
  static uint8_t touch_pressed = 0;

  if (GUI_IsCalibrationMode())
  {
      TS_Point raw = XPT2046_GetPoint();

      if (raw.z > 100)
      {
          if (!touch_pressed)
          {
              GUI_CalibrationTouch(raw.x, raw.y);
              touch_pressed = 1;
          }
      }
      else
      {
          touch_pressed = 0;
      }
  }
  else if (GUI_IsTouchTestMode())
  {
      if (XPT2046_GetScreenPoint(&tx, &ty))
      {
          if (!touch_pressed)
          {
              GUI_TouchTestTouch(tx, ty);
              touch_pressed = 1;
          }
      }
      else
      {
          touch_pressed = 0;
      }
  }
  else
  {
      if (XPT2046_GetScreenPoint(&tx, &ty))
      {
          if (!touch_pressed)
          {
              GUI_Touch(tx, ty);
              touch_pressed = 1;
          }
      }
      else
      {
          touch_pressed = 0;
      }
  }

  if (GuiStateChanged)
  {
      GuiStateChanged = 0;
      SendGuiStateToPC();
  }

  if (HidRxReady)
  {
      HidRxReady = 0;

      uint8_t sum =
          HidRxBuffer[0] +
          HidRxBuffer[1] +
          HidRxBuffer[2] +
          HidRxBuffer[3] +
          HidRxBuffer[4] +
          HidRxBuffer[5] +
          HidRxBuffer[6];

      if (HidRxBuffer[7] != sum)
      {
          GUI_Status("RX BAD PACKET");
      }
      else if (HidRxBuffer[0] == 0x02)
      {
          GUI_STATE old_state = GuiState;

          last_pc_rx_tick = HAL_GetTick();
          if (!pc_connected)
          {
              pc_connected = 1;
              GUI_Status("Ready");
          }

          GuiState.value1 = HidRxBuffer[1];
          GuiState.value2 = HidRxBuffer[2];
          GuiState.value3 = HidRxBuffer[3];

          GuiState.sw1 = HidRxBuffer[4] ? 1 : 0;
          GuiState.sw2 = HidRxBuffer[5] ? 1 : 0;
          GuiState.sw3 = HidRxBuffer[6] ? 1 : 0;

          GUI_RefreshChangedFromState(&old_state);

          SendGuiStateToPC();
      }
      else if (HidRxBuffer[0] == 0x10)
      {
          last_pc_rx_tick = HAL_GetTick();
          if (!pc_connected)
          {
              pc_connected = 1;
              GUI_Status("Ready");
          }
          GUI_StartCalibration();
      }
      else if (HidRxBuffer[0] == 0x11)
      {
          last_pc_rx_tick = HAL_GetTick();
          if (!pc_connected)
          {
              pc_connected = 1;
              GUI_Status("Ready");
          }
          GUI_StopCalibration();
      }
      else if (HidRxBuffer[0] == 0x12)
      {
          last_pc_rx_tick = HAL_GetTick();
          if (!pc_connected)
          {
              pc_connected = 1;
              GUI_Status("Ready");
          }
      }
      else
      {
          GUI_Status("RX UNKNOWN CMD");
      }
  }

  GUI_Task();
  HAL_Delay(20);
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 7;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_4;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, TFT_CS_Pin|TFT_DC_Pin|TFT_RST_Pin|TOUCH_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : TFT_CS_Pin TFT_DC_Pin TFT_RST_Pin TOUCH_CS_Pin */
  GPIO_InitStruct.Pin = TFT_CS_Pin|TFT_DC_Pin|TFT_RST_Pin|TOUCH_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
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
#ifdef USE_FULL_ASSERT
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
