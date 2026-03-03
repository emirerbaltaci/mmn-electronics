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

#include <math.h>

#include "usbd_cdc_if.h"

#include "FreeRTOS.h"
#include "queue.h"
#include "semphr.h"
#include "task.h"

#include "auvcontrol.h"
#include "auvmath.h"

#include "auvflags.h"

#include "ncom_handlers.h"
#include "ncom_rx.h"
#include "ncom_tx.h"

#include "auvconfig.h"
#include "bar30_config.h"
#include "ekf_config.h"
#include "imu_config.h"
#include "magnetometer_config.h"
#include "pid_config.h"
#include "task_config.h"

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
ADC_HandleTypeDef hadc1;
ADC_HandleTypeDef hadc3;
DMA_HandleTypeDef hdma_adc1;

CORDIC_HandleTypeDef hcordic;

CRC_HandleTypeDef hcrc;

FDCAN_HandleTypeDef hfdcan1;

I2C_HandleTypeDef hi2c2;

IWDG_HandleTypeDef hiwdg;

SPI_HandleTypeDef hspi1;
SPI_HandleTypeDef hspi2;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;
TIM_HandleTypeDef htim8;
TIM_HandleTypeDef htim16;

UART_HandleTypeDef huart4;
UART_HandleTypeDef huart5;
UART_HandleTypeDef huart1;
UART_HandleTypeDef huart2;
UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */

TaskHandle_t xStateEstimateTaskHandle = NULL;
TaskHandle_t xControlTaskHandle = NULL;
TaskHandle_t xNCOMTaskHandle = NULL;
TaskHandle_t xSensorTaskHandle = NULL;
TaskHandle_t xSysMonitorTaskHandle = NULL;

static StackType_t xEKFStack[2048] __attribute__((section(".ccmram")));
static StaticTask_t xEKFTCB;

SemaphoreHandle_t xStateMutex = NULL;
SemaphoreHandle_t xCommandMutex = NULL;

NCOM_RX_t ncomRx __attribute__((section(".sram2_data")));

float ekfState[12] __attribute__((section(".ccmram")));
Setpoint_t setPoint __attribute__((section(".ccmram")));
Setspeed_t setSpeed __attribute__((section(".ccmram")));
bool axisSpeedMode[6] __attribute__((section(".ccmram")));

volatile uint8_t hbStateEstimateTask = AUV_TASK_DEAD;
volatile uint8_t hbControlTask = AUV_TASK_DEAD;
volatile uint8_t hbNCOMTask = AUV_TASK_DEAD;
volatile uint8_t armStatus = AUV_DISARMED;

IMU_Handler_t icm __attribute__((section(".sram2_data")));
MAG_Handler_t lis __attribute__((section(".sram2_data")));
BAR30_Handler_t bar __attribute__((section(".sram2_data")));

uint8_t imuBufDMATx[15] __attribute__((section(".sram2_data")));
uint8_t imuBufDMARx[15] __attribute__((section(".sram2_data")));
uint8_t imuBuf[15] __attribute__((section(".ccmram")));
uint8_t magBufDMATx[9] __attribute__((section(".sram2_data")));
uint8_t magBufDMARx[9] __attribute__((section(".sram2_data")));
uint8_t magBuf[9] __attribute__((section(".ccmram")));

volatile bool isDepthUpdated = false;
volatile bool isMagUpdated = false;

volatile uint32_t ulHighWordOverflows = 0;
float cpuLoad = 0.0f;

extern AUV_Config_t auvConfig;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_DMA_Init(void);
static void MX_IWDG_Init(void);
static void MX_ADC1_Init(void);
static void MX_CRC_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC3_Init(void);
static void MX_FDCAN1_Init(void);
static void MX_I2C2_Init(void);
static void MX_SPI1_Init(void);
static void MX_SPI2_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM8_Init(void);
static void MX_UART4_Init(void);
static void MX_UART5_Init(void);
static void MX_USART1_UART_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_CORDIC_Init(void);
static void MX_TIM7_Init(void);
static void MX_TIM16_Init(void);
/* USER CODE BEGIN PFP */

void Task_StateEstimate(void *pvParameters);
void Task_Control(void *pvParameters);
void Task_NCOM(void *pvParameters);
void Task_Sensor(void *pvParameters);
void Task_SysMonitor(void *pvParameters);

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void) {

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_IWDG_Init();
  MX_ADC1_Init();
  MX_CRC_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_USB_Device_Init();
  MX_ADC3_Init();
  MX_FDCAN1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  MX_TIM3_Init();
  MX_TIM8_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART1_UART_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_CORDIC_Init();
  MX_TIM7_Init();
  MX_TIM16_Init();
  /* USER CODE BEGIN 2 */

  AUV_Config_Init();

  memset(&setPoint, 0, sizeof(Setpoint_t));
  memset(&setSpeed, 0, sizeof(Setspeed_t));
  memset(axisSpeedMode, 0, sizeof(axisSpeedMode));
  memset(imuBuf, 0, sizeof(imuBuf));

  NCOM_RX_Init(&ncomRx);
  NCOM_TX_Init();

  icm.pSPIx = &hspi2;
  icm.pGPIOx = GPIOB;
  icm.GPIO_PIN_x = GPIO_PIN_12;
  icm.GPIO_PIN_x_INT = GPIO_PIN_8;
  lis.pSPIx = &hspi1;
  lis.pGPIOx = GPIOC;
  lis.GPIO_PIN_x = GPIO_PIN_4;
  lis.GPIO_PIN_x_INT = GPIO_PIN_4;

  IMU_SPI_Init(&icm);
  MAG_SPI_Init(&lis);

  xStateMutex = xSemaphoreCreateMutex();
  xCommandMutex = xSemaphoreCreateMutex();

  if (xStateMutex != NULL && xCommandMutex != NULL) {
    xStateEstimateTaskHandle = xTaskCreateStatic(
        Task_StateEstimate, "EKF_TASK", 2048, NULL, 11, xEKFStack, &xEKFTCB);
    xTaskCreate(Task_Control, "CTRL_TASK", 1024, NULL, 10, &xControlTaskHandle);
    xTaskCreate(Task_NCOM, "NCOM_TASK", 4096, NULL, 9, &xNCOMTaskHandle);
    xTaskCreate(Task_Sensor, "SENSOR_TASK", 256, NULL, 8, &xSensorTaskHandle);
    xTaskCreate(Task_SysMonitor, "MONITOR_TASK", 256, NULL, 7,
                &xSysMonitorTaskHandle);

    vTaskStartScheduler();
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1) {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1_BOOST);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48 |
                                     RCC_OSCILLATORTYPE_LSI |
                                     RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.LSIState = RCC_LSI_ON;
  RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV2;
  RCC_OscInitStruct.PLL.PLLN = 85;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = RCC_PLLQ_DIV2;
  RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK |
                                RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK) {
    Error_Handler();
  }
}

/**
 * @brief ADC1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC1_Init(void) {

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
   */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.GainCompensation = 0;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.ContinuousConvMode = ENABLE;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.DMAContinuousRequests = ENABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc1.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc1) != HAL_OK) {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
   */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Regular Channel
   */
  sConfig.Channel = ADC_CHANNEL_TEMPSENSOR_ADC1;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_640CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */
}

/**
 * @brief ADC3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_ADC3_Init(void) {

  /* USER CODE BEGIN ADC3_Init 0 */

  /* USER CODE END ADC3_Init 0 */

  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC3_Init 1 */

  /* USER CODE END ADC3_Init 1 */

  /** Common config
   */
  hadc3.Instance = ADC3;
  hadc3.Init.ClockPrescaler = ADC_CLOCK_SYNC_PCLK_DIV4;
  hadc3.Init.Resolution = ADC_RESOLUTION_12B;
  hadc3.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc3.Init.GainCompensation = 0;
  hadc3.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc3.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc3.Init.LowPowerAutoWait = DISABLE;
  hadc3.Init.ContinuousConvMode = DISABLE;
  hadc3.Init.NbrOfConversion = 1;
  hadc3.Init.DiscontinuousConvMode = DISABLE;
  hadc3.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc3.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc3.Init.DMAContinuousRequests = DISABLE;
  hadc3.Init.Overrun = ADC_OVR_DATA_PRESERVED;
  hadc3.Init.OversamplingMode = DISABLE;
  if (HAL_ADC_Init(&hadc3) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Regular Channel
   */
  sConfig.Channel = ADC_CHANNEL_14;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SamplingTime = ADC_SAMPLETIME_2CYCLES_5;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc3, &sConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC3_Init 2 */

  /* USER CODE END ADC3_Init 2 */
}

/**
 * @brief CORDIC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CORDIC_Init(void) {

  /* USER CODE BEGIN CORDIC_Init 0 */

  /* USER CODE END CORDIC_Init 0 */

  /* USER CODE BEGIN CORDIC_Init 1 */

  /* USER CODE END CORDIC_Init 1 */
  hcordic.Instance = CORDIC;
  if (HAL_CORDIC_Init(&hcordic) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN CORDIC_Init 2 */

  /* USER CODE END CORDIC_Init 2 */
}

/**
 * @brief CRC Initialization Function
 * @param None
 * @retval None
 */
static void MX_CRC_Init(void) {

  /* USER CODE BEGIN CRC_Init 0 */

  /* USER CODE END CRC_Init 0 */

  /* USER CODE BEGIN CRC_Init 1 */

  /* USER CODE END CRC_Init 1 */
  hcrc.Instance = CRC;
  hcrc.Init.DefaultPolynomialUse = DEFAULT_POLYNOMIAL_DISABLE;
  hcrc.Init.DefaultInitValueUse = DEFAULT_INIT_VALUE_DISABLE;
  hcrc.Init.GeneratingPolynomial = 4129;
  hcrc.Init.CRCLength = CRC_POLYLENGTH_16B;
  hcrc.Init.InitValue = 0xFFFF;
  hcrc.Init.InputDataInversionMode = CRC_INPUTDATA_INVERSION_NONE;
  hcrc.Init.OutputDataInversionMode = CRC_OUTPUTDATA_INVERSION_DISABLE;
  hcrc.InputDataFormat = CRC_INPUTDATA_FORMAT_BYTES;
  if (HAL_CRC_Init(&hcrc) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN CRC_Init 2 */

  /* USER CODE END CRC_Init 2 */
}

/**
 * @brief FDCAN1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_FDCAN1_Init(void) {

  /* USER CODE BEGIN FDCAN1_Init 0 */

  /* USER CODE END FDCAN1_Init 0 */

  /* USER CODE BEGIN FDCAN1_Init 1 */

  /* USER CODE END FDCAN1_Init 1 */
  hfdcan1.Instance = FDCAN1;
  hfdcan1.Init.ClockDivider = FDCAN_CLOCK_DIV1;
  hfdcan1.Init.FrameFormat = FDCAN_FRAME_CLASSIC;
  hfdcan1.Init.Mode = FDCAN_MODE_NORMAL;
  hfdcan1.Init.AutoRetransmission = DISABLE;
  hfdcan1.Init.TransmitPause = DISABLE;
  hfdcan1.Init.ProtocolException = DISABLE;
  hfdcan1.Init.NominalPrescaler = 16;
  hfdcan1.Init.NominalSyncJumpWidth = 1;
  hfdcan1.Init.NominalTimeSeg1 = 1;
  hfdcan1.Init.NominalTimeSeg2 = 1;
  hfdcan1.Init.DataPrescaler = 1;
  hfdcan1.Init.DataSyncJumpWidth = 1;
  hfdcan1.Init.DataTimeSeg1 = 1;
  hfdcan1.Init.DataTimeSeg2 = 1;
  hfdcan1.Init.StdFiltersNbr = 0;
  hfdcan1.Init.ExtFiltersNbr = 0;
  hfdcan1.Init.TxFifoQueueMode = FDCAN_TX_FIFO_OPERATION;
  if (HAL_FDCAN_Init(&hfdcan1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN FDCAN1_Init 2 */

  /* USER CODE END FDCAN1_Init 2 */
}

/**
 * @brief I2C2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_I2C2_Init(void) {

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x40B285C2;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Analogue filter
   */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK) {
    Error_Handler();
  }

  /** Configure Digital filter
   */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */
}

/**
 * @brief IWDG Initialization Function
 * @param None
 * @retval None
 */
static void MX_IWDG_Init(void) {

  /* USER CODE BEGIN IWDG_Init 0 */

  /* USER CODE END IWDG_Init 0 */

  /* USER CODE BEGIN IWDG_Init 1 */

  /* USER CODE END IWDG_Init 1 */
  hiwdg.Instance = IWDG;
  hiwdg.Init.Prescaler = IWDG_PRESCALER_8;
  hiwdg.Init.Window = 4095;
  hiwdg.Init.Reload = 4095;
  if (HAL_IWDG_Init(&hiwdg) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN IWDG_Init 2 */

  /* USER CODE END IWDG_Init 2 */
}

/**
 * @brief SPI1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI1_Init(void) {

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
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 7;
  hspi1.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi1.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */
}

/**
 * @brief SPI2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_SPI2_Init(void) {

  /* USER CODE BEGIN SPI2_Init 0 */

  /* USER CODE END SPI2_Init 0 */

  /* USER CODE BEGIN SPI2_Init 1 */

  /* USER CODE END SPI2_Init 1 */
  /* SPI2 parameter configuration*/
  hspi2.Instance = SPI2;
  hspi2.Init.Mode = SPI_MODE_MASTER;
  hspi2.Init.Direction = SPI_DIRECTION_2LINES;
  hspi2.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi2.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi2.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi2.Init.NSS = SPI_NSS_SOFT;
  hspi2.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
  hspi2.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi2.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi2.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi2.Init.CRCPolynomial = 7;
  hspi2.Init.CRCLength = SPI_CRC_LENGTH_DATASIZE;
  hspi2.Init.NSSPMode = SPI_NSS_PULSE_ENABLE;
  if (HAL_SPI_Init(&hspi2) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI2_Init 2 */

  /* USER CODE END SPI2_Init 2 */
}

/**
 * @brief TIM1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM1_Init(void) {

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 170 - 1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 19999;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);
}

/**
 * @brief TIM2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM2_Init(void) {

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 170 - 1;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 19999;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);
}

/**
 * @brief TIM3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM3_Init(void) {

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 170 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 19999;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);
}

/**
 * @brief TIM7 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM7_Init(void) {

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 170 - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 65535;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */
}

/**
 * @brief TIM8 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM8_Init(void) {

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 170 - 1;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 19999;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK) {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim8) != HAL_OK) {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK) {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim8, &sConfigOC, TIM_CHANNEL_1) != HAL_OK) {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.BreakAFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.Break2AFMode = TIM_BREAK_AFMODE_INPUT;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim8, &sBreakDeadTimeConfig) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */

  /* USER CODE END TIM8_Init 2 */
  HAL_TIM_MspPostInit(&htim8);
}

/**
 * @brief TIM16 Initialization Function
 * @param None
 * @retval None
 */
static void MX_TIM16_Init(void) {

  /* USER CODE BEGIN TIM16_Init 0 */

  /* USER CODE END TIM16_Init 0 */

  /* USER CODE BEGIN TIM16_Init 1 */

  /* USER CODE END TIM16_Init 1 */
  htim16.Instance = TIM16;
  htim16.Init.Prescaler = 17000 - 1;
  htim16.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim16.Init.Period = 65535;
  htim16.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim16.Init.RepetitionCounter = 0;
  htim16.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim16) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM16_Init 2 */

  /* USER CODE END TIM16_Init 2 */
}

/**
 * @brief UART4 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART4_Init(void) {

  /* USER CODE BEGIN UART4_Init 0 */

  /* USER CODE END UART4_Init 0 */

  /* USER CODE BEGIN UART4_Init 1 */

  /* USER CODE END UART4_Init 1 */
  huart4.Instance = UART4;
  huart4.Init.BaudRate = 115200;
  huart4.Init.WordLength = UART_WORDLENGTH_8B;
  huart4.Init.StopBits = UART_STOPBITS_1;
  huart4.Init.Parity = UART_PARITY_NONE;
  huart4.Init.Mode = UART_MODE_TX_RX;
  huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart4.Init.OverSampling = UART_OVERSAMPLING_16;
  huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart4.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart4) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart4, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart4, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart4) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN UART4_Init 2 */

  /* USER CODE END UART4_Init 2 */
}

/**
 * @brief UART5 Initialization Function
 * @param None
 * @retval None
 */
static void MX_UART5_Init(void) {

  /* USER CODE BEGIN UART5_Init 0 */

  /* USER CODE END UART5_Init 0 */

  /* USER CODE BEGIN UART5_Init 1 */

  /* USER CODE END UART5_Init 1 */
  huart5.Instance = UART5;
  huart5.Init.BaudRate = 115200;
  huart5.Init.WordLength = UART_WORDLENGTH_8B;
  huart5.Init.StopBits = UART_STOPBITS_1;
  huart5.Init.Parity = UART_PARITY_NONE;
  huart5.Init.Mode = UART_MODE_TX_RX;
  huart5.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart5.Init.OverSampling = UART_OVERSAMPLING_16;
  huart5.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart5.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart5.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart5) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart5, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart5, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart5) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN UART5_Init 2 */

  /* USER CODE END UART5_Init 2 */
}

/**
 * @brief USART1 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART1_UART_Init(void) {

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
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart1, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart1, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart1) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */
}

/**
 * @brief USART2 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART2_UART_Init(void) {

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart2, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart2, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart2) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void) {

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) !=
      HAL_OK) {
    Error_Handler();
  }
  if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK) {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */
}

/**
 * Enable DMA controller clock
 */
static void MX_DMA_Init(void) {

  /* DMA controller clock enable */
  __HAL_RCC_DMAMUX1_CLK_ENABLE();
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* DMA interrupt init */
  /* DMA1_Channel1_IRQn interrupt configuration */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);
}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void) {
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4 | GPIO_PIN_5, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12,
                    GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE,
                    GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15,
                    GPIO_PIN_RESET);

  /*Configure GPIO pins : PC13 PC14 PC15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA4 */
  GPIO_InitStruct.Pin = GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PC4 PC5 */
  GPIO_InitStruct.Pin = GPIO_PIN_4 | GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : PB0 PB10 PB11 PB12 */
  GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PE12 PE13 PE14 PE15 */
  GPIO_InitStruct.Pin = GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PD8 */
  GPIO_InitStruct.Pin = GPIO_PIN_8;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pin : PB5 */
  GPIO_InitStruct.Pin = GPIO_PIN_5;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI4_IRQn, 5, 0);
  // HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 5, 0);
  // HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize) {
  static StaticTask_t xIdleTaskTCB;
  static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

  *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;
  *ppxIdleTaskStackBuffer = uxIdleTaskStack;
  *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize) {
  static StaticTask_t xTimerTaskTCB;
  static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

  *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;
  *ppxTimerTaskStackBuffer = uxTimerTaskStack;
  *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

void Task_StateEstimate(void *pvParameters) {
  // Nominal state: pos(3), quat(4), vel(3), gyro_bias(3), accel_bias(3)
  float nominal_x[16] = {
      0.0f, 0.0f, 0.0f,       // pos (0-2)
      1.0f, 0.0f, 0.0f, 0.0f, // quat (3-6) (w=1, x=0, y=0, z=0)
      0.0f, 0.0f, 0.0f,       // vel (7-9)
      0.0f, 0.0f, 0.0f,       // bg (10-12)
      0.0f, 0.0f, 0.0f        // ba (13-15)
  };
  float error_x[15] = {0};
  static float P[225] __attribute__((section(".ccmram")));
  static float Q[225] __attribute__((section(".ccmram")));

  // Simplified covariance initialization
  for (int i = 0; i < 225; i++) {
    P[i] = 0.0f; // Initial uncertainty
    Q[i] = 0.0f; // Process noise
  }

  P[0] = P[16] = P[32] = EKF_P_INIT_POS;
  P[48] = P[64] = P[80] = EKF_P_INIT_VEL;
  P[96] = P[112] = P[128] = EKF_P_INIT_ATT;
  P[144] = P[160] = P[176] = EKF_P_INIT_BG;
  P[192] = P[208] = P[224] = EKF_P_INIT_BA;

  Q[48] = Q[64] = Q[80] = EKF_Q_VEL_NOISE;
  Q[96] = Q[112] = Q[128] = EKF_Q_ATT_NOISE;
  Q[144] = Q[160] = Q[176] = EKF_Q_BG_NOISE;
  Q[192] = Q[208] = Q[224] = EKF_Q_BA_NOISE;

  float accel[3] = {0};
  float gyro[3] = {0};

  for (int i = 1; i < 15; i++)
    imuBufDMATx[i] = 0;
  imuBufDMATx[0] = 0x80 | IMU_REG_TEMP_DATA1;

  HAL_TIM_Base_Start(&htim7);
  uint16_t last_time_us = __HAL_TIM_GET_COUNTER(&htim7);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

  for (;;) {

    if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(5)) > 0) {

      hbStateEstimateTask = AUV_TASK_ALIVE;

      uint16_t curr_time_us = __HAL_TIM_GET_COUNTER(&htim7);
      uint16_t delta_us;
      if (curr_time_us >= last_time_us)
        delta_us = curr_time_us - last_time_us;
      else
        delta_us = (0xFFFF - last_time_us) + curr_time_us + 1;
      last_time_us = curr_time_us;
      float dt = (float)delta_us * 0.000001f;
      if (dt > 0.02f || dt <= 0.0f)
        dt = 0.001f;

      taskENTER_CRITICAL();
      accel[0] =
          (float)((int16_t)((imuBuf[3] << 8) | imuBuf[4])) * icm.accelMult;
      accel[1] =
          (float)((int16_t)((imuBuf[5] << 8) | imuBuf[6])) * icm.accelMult;
      accel[2] =
          (float)((int16_t)((imuBuf[7] << 8) | imuBuf[8])) * icm.accelMult;
      gyro[0] =
          (float)((int16_t)((imuBuf[9] << 8) | imuBuf[10])) * icm.gyroMult;
      gyro[1] =
          (float)((int16_t)((imuBuf[11] << 8) | imuBuf[12])) * icm.gyroMult;
      gyro[2] =
          (float)((int16_t)((imuBuf[13] << 8) | imuBuf[14])) * icm.gyroMult;
      taskEXIT_CRITICAL();

      const float r_imu_cg[3] = {EKF_IMU_LEVER_ARM_X, EKF_IMU_LEVER_ARM_Y,
                                 EKF_IMU_LEVER_ARM_Z};
      float w_x = gyro[0] - nominal_x[10];
      float w_y = gyro[1] - nominal_x[11];
      float w_z = gyro[2] - nominal_x[12];
      float v_tan_x = w_y * r_imu_cg[2] - w_z * r_imu_cg[1];
      float v_tan_y = w_z * r_imu_cg[0] - w_x * r_imu_cg[2];
      float v_tan_z = w_x * r_imu_cg[1] - w_y * r_imu_cg[0];
      float a_c_x = w_y * v_tan_z - w_z * v_tan_y;
      float a_c_y = w_z * v_tan_x - w_x * v_tan_z;
      float a_c_z = w_x * v_tan_y - w_y * v_tan_x;
      accel[0] -= a_c_x;
      accel[1] -= a_c_y;
      accel[2] -= a_c_z;

      eskf_predict(nominal_x, P, accel, gyro, Q, dt);

      float R[9], RT[9];
      quat_to_rot_matrix(&nominal_x[3], R);
      matrix_transpose(R, RT, 3, 3);

      float a_comp[3] = {accel[0] - nominal_x[13], accel[1] - nominal_x[14],
                         accel[2] - nominal_x[15]};
      float acc_norm = sqrtf(a_comp[0] * a_comp[0] + a_comp[1] * a_comp[1] +
                             a_comp[2] * a_comp[2]);

      if (acc_norm > EKF_GRAVITY_NORM_MIN && acc_norm < EKF_GRAVITY_NORM_MAX) {
        float g_earth[3] = {0.0f, 0.0f, -1.0f};

        float z_hat_acc[3];
        matrix_mult(RT, g_earth, z_hat_acc, 3, 3, 1);

        float z_acc[3] = {a_comp[0] / acc_norm, a_comp[1] / acc_norm,
                          a_comp[2] / acc_norm};

        float dz_acc[3] = {z_acc[0] - z_hat_acc[0], z_acc[1] - z_hat_acc[1],
                           z_acc[2] - z_hat_acc[2]};

        float H_acc[45] = {0}; // 3 rows * 15 cols
        H_acc[0 * 15 + 7] = -z_hat_acc[2];
        H_acc[0 * 15 + 8] = z_hat_acc[1];
        H_acc[1 * 15 + 6] = z_hat_acc[2];
        H_acc[1 * 15 + 8] = -z_hat_acc[0];
        H_acc[2 * 15 + 6] = -z_hat_acc[1];
        H_acc[2 * 15 + 7] = z_hat_acc[0];

        float R_acc[9] = {0};
        R_acc[0] = EKF_R_ACCEL;
        R_acc[4] = EKF_R_ACCEL;
        R_acc[8] = EKF_R_ACCEL;

        if (eskf_update(error_x, P, dz_acc, H_acc, R_acc, 3)) {
          eskf_inject(nominal_x, error_x, P);

          quat_to_rot_matrix(&nominal_x[3], R);
          matrix_transpose(R, RT, 3, 3);
        }
      }

      if (isMagUpdated) {

        if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {
          float mag_meas[3];

          mag_meas[0] = lis.data.magX;
          mag_meas[1] = lis.data.magY;
          mag_meas[2] = lis.data.magZ;
          isMagUpdated = false;
          xSemaphoreGive(xStateMutex);

          quat_to_rot_matrix(&nominal_x[3], R);
          matrix_transpose(R, RT, 3, 3);

          float m_earth[3] = {EKF_EARTH_MAG_X, EKF_EARTH_MAG_Y,
                              EKF_EARTH_MAG_Z};

          float z_hat_mag[3];
          matrix_mult(RT, m_earth, z_hat_mag, 3, 3, 1);

          float mag_norm =
              sqrtf(mag_meas[0] * mag_meas[0] + mag_meas[1] * mag_meas[1] +
                    mag_meas[2] * mag_meas[2]);

          if (mag_norm > 0.1f) { // Guard against division by zero
            float z_mag[3] = {mag_meas[0] / mag_norm, mag_meas[1] / mag_norm,
                              mag_meas[2] / mag_norm};

            float dz_mag[3] = {z_mag[0] - z_hat_mag[0], z_mag[1] - z_hat_mag[1],
                               z_mag[2] - z_hat_mag[2]};

            float H_mag[45] = {0}; // 3 rows * 15 cols
            H_mag[0 * 15 + 7] = -z_hat_mag[2];
            H_mag[0 * 15 + 8] = z_hat_mag[1];
            H_mag[1 * 15 + 6] = z_hat_mag[2];
            H_mag[1 * 15 + 8] = -z_hat_mag[0];
            H_mag[2 * 15 + 6] = -z_hat_mag[1];
            H_mag[2 * 15 + 7] = z_hat_mag[0];

            float R_mag[9] = {0};
            R_mag[0] = EKF_R_MAG;
            R_mag[4] = EKF_R_MAG;
            R_mag[8] = EKF_R_MAG;

            if (eskf_update(error_x, P, dz_mag, H_mag, R_mag, 3)) {
              eskf_inject(nominal_x, error_x, P);
            }
          }
        }
      }

      if (isDepthUpdated) {
        isDepthUpdated = false;
        float dz_depth = bar.depth - nominal_x[2];
        float H_depth[15] = {0};
        H_depth[2] = 1.0f;
        float R_depth = EKF_R_BARO;
        if (eskf_update(error_x, P, &dz_depth, H_depth, &R_depth, 1))
          eskf_inject(nominal_x, error_x, P);
      }

      bool isStationary = (armStatus == AUV_DISARMED);

      if (isStationary) {
        float dz_zupt[3] = {0.0f - nominal_x[7], 0.0f - nominal_x[8],
                            0.0f - nominal_x[9]};
        float H_zupt[45] = {0};
        H_zupt[3] = 1.0f;
        H_zupt[19] = 1.0f;
        H_zupt[35] = 1.0f;
        float R_zupt[9] = {0};
        R_zupt[0] = EKF_R_ZUPT;
        R_zupt[4] = EKF_R_ZUPT;
        R_zupt[8] = EKF_R_ZUPT;
        if (eskf_update(error_x, P, dz_zupt, H_zupt, R_zupt, 3))
          eskf_inject(nominal_x, error_x, P);
      }

      if (xSemaphoreTake(xStateMutex, 0) == pdTRUE) {

        ekfState[0] = nominal_x[0];
        ekfState[1] = nominal_x[1];
        ekfState[2] = nominal_x[2];

        quat_to_euler(&nominal_x[3], &ekfState[3], &ekfState[4], &ekfState[5]);

        ekfState[6] = nominal_x[7];
        ekfState[7] = nominal_x[8];
        ekfState[8] = nominal_x[9];

        ekfState[9] = gyro[0] - nominal_x[10];
        ekfState[10] = gyro[1] - nominal_x[11];
        ekfState[11] = gyro[2] - nominal_x[12];

        xSemaphoreGive(xStateMutex);
      }
    }
  }
}

void Task_Control(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(TASK_CONTROL_SLEEP_MS);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  PID_Controller_t pidsSetpoint[6];
  PID_Controller_t pidsSetspeed[6];
  Controller_State ctrl = {0};

  float torque[6] = {0};
  float force[8] = {0};
  uint16_t pwmArr[8] = {1500, 1500, 1500, 1500, 1500, 1500, 1500, 1500};

  for (int i = 0; i < 6; i++) {
    // Initial dummy init to clear structs and sets wrap bounds
    PID_Init(&pidsSetpoint[i], 0, 0, 0, 0, 0, 0);
    PID_Init(&pidsSetspeed[i], 0, 0, 0, 0, 0, 0);
  }

  pidsSetpoint[5].wrap_bound = (float)M_PI;

  for (;;) {

    hbControlTask = AUV_TASK_ALIVE;

    static float currPosition[6] = {0};
    static float currSpeed[6] = {0};
    static Setpoint_t currSetpoint = {0};
    static Setspeed_t currSetspeed = {0};
    static bool currMode[6] = {false};
    static bool prevMode[6] = {false};

    if (xSemaphoreTake(xStateMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      memcpy(currPosition, &ekfState[0], sizeof(float) * 6);
      memcpy(currSpeed, &ekfState[6], sizeof(float) * 6);
      xSemaphoreGive(xStateMutex);
    }

    if (xSemaphoreTake(xCommandMutex, pdMS_TO_TICKS(1)) == pdTRUE) {
      currSetpoint = setPoint;
      currSetspeed = setSpeed;
      memcpy(currMode, axisSpeedMode, sizeof(bool) * 6);
      xSemaphoreGive(xCommandMutex);
    }

    if (armStatus == AUV_ARMED) {

      // Update PID gains dynamically from auvConfig
      if (AUV_Config_Lock(2)) {
        // Surge (X)
        pidsSetpoint[0].kp = auvConfig.pid.xy.p;
        pidsSetpoint[0].ki = auvConfig.pid.xy.i;
        pidsSetpoint[0].kd = auvConfig.pid.xy.d;
        // Sway (Y)
        pidsSetpoint[1].kp = auvConfig.pid.xy.p;
        pidsSetpoint[1].ki = auvConfig.pid.xy.i;
        pidsSetpoint[1].kd = auvConfig.pid.xy.d;
        // Heave (Depth / Z)
        pidsSetpoint[2].kp = auvConfig.pid.depth.p;
        pidsSetpoint[2].ki = auvConfig.pid.depth.i;
        pidsSetpoint[2].kd = auvConfig.pid.depth.d;
        // Roll
        pidsSetpoint[3].kp = auvConfig.pid.roll.p;
        pidsSetpoint[3].ki = auvConfig.pid.roll.i;
        pidsSetpoint[3].kd = auvConfig.pid.roll.d;
        // Pitch
        pidsSetpoint[4].kp = auvConfig.pid.pitch.p;
        pidsSetpoint[4].ki = auvConfig.pid.pitch.i;
        pidsSetpoint[4].kd = auvConfig.pid.pitch.d;
        // Yaw
        pidsSetpoint[5].kp = auvConfig.pid.yaw.p;
        pidsSetpoint[5].ki = auvConfig.pid.yaw.i;
        pidsSetpoint[5].kd = auvConfig.pid.yaw.d;

        // Note: If Setspeed requires separate tuning, you would map it here.
        // Defaults could just mirror the positional tunings or fall back to xy
        // if unspecified.
        for (int i = 0; i < 6; i++) {
          pidsSetspeed[i].kp = auvConfig.pid.xy.p;
          pidsSetspeed[i].ki = auvConfig.pid.xy.i;
          pidsSetspeed[i].kd = auvConfig.pid.xy.d;
        }
        AUV_Config_Unlock();
      }

      for (int i = 0; i < 6; i++) {
        if (currMode[i] != prevMode[i]) {
          if (currMode[i] == true) {
            PID_Reset(&pidsSetspeed[i]);
          } else {
            PID_Reset(&pidsSetpoint[i]);
          }
          prevMode[i] = currMode[i];
        }
      }

      PID_CalculateHybrid(currSetpoint, currSetspeed, currMode, currPosition,
                          currSpeed, torque, pidsSetpoint, pidsSetspeed, &ctrl,
                          TASK_CONTROL_PID_DT);
      Thrust_Allocate(torque, force);
      Process_All_Thrusters(force, pwmArr, 8);
    } else {
      for (int i = 0; i < 8; i++) {
        pwmArr[i] = 1500;
        force[i] = 0.0f;
      }
      for (int i = 0; i < 6; i++) {
        torque[i] = 0.0f;
        PID_Reset(&pidsSetpoint[i]);
        PID_Reset(&pidsSetspeed[i]);
      }
      memcpy(prevMode, currMode, sizeof(bool) * 6);
    }

    taskENTER_CRITICAL();
    TIM1->CCR1 = pwmArr[0];
    TIM1->CCR2 = pwmArr[1];
    TIM1->CCR3 = pwmArr[2];
    TIM1->CCR4 = pwmArr[3];
    TIM2->CCR1 = pwmArr[4];
    TIM2->CCR2 = pwmArr[5];
    TIM2->CCR3 = pwmArr[6];
    TIM2->CCR4 = pwmArr[7];
    taskEXIT_CRITICAL();

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void Task_NCOM(void *pvParameters) {
  TickType_t xPacketTimeout =
      pdMS_TO_TICKS(auvConfig.task.ncom_packet_timeout_ms);
  TickType_t xHandshakeTimeout =
      pdMS_TO_TICKS(auvConfig.task.ncom_handshake_timeout_ms);
  bool isConnected = false;
  uint8_t byteBuf;

  uint8_t seq = NCOM_TX_SendPacket(NCOM_MSG_CONFIG_REQ_STARTUP, NULL, 0);

  while (!isConnected) {
    hbNCOMTask = AUV_TASK_ALIVE;

    if (ulTaskNotifyTake(pdTRUE, xHandshakeTimeout) > 0) {
      while (NCOM_RX_RingBuffer_Read(&ncomRx, &byteBuf)) {
        if (NCOM_RX_ParseByte(&ncomRx, byteBuf)) {
          if (ncomRx.parser.msgId == NCOM_MSG_CONFIG_SET_STARTUP)
            isConnected = NCOM_Handlers_Config_Set_Startup(&ncomRx, &auvConfig);
          else if (ncomRx.parser.msgId == NCOM_MSG_ACKNOWLEDGEMENT) {
            NCOM_Payload_ACKNOWLEDGEMENT_t msg;
            ncom_unpack_acknowledgement(ncomRx.parser.payloadBuf, &msg);
            if (msg.requested_msg_id == NCOM_MSG_CONFIG_REQ_STARTUP &&
                msg.requested_seq == seq &&
                msg.response == NCOM_ACKNOWLEDGEMENT_RESPONSE_NACK) {
              isConnected = true;
            }
          }
        }
      }
    } else
      seq = NCOM_TX_SendPacket(NCOM_MSG_CONFIG_REQ_STARTUP, NULL, 0);
  }

  for (;;) {

    hbNCOMTask = AUV_TASK_ALIVE;

    if (ulTaskNotifyTake(pdTRUE, xPacketTimeout) > 0) {
      while (NCOM_RX_RingBuffer_Read(&ncomRx, &byteBuf)) {
        if (NCOM_RX_ParseByte(&ncomRx, byteBuf))
          NCOM_Handlers_Selector(&ncomRx);
      }
    } else {
      if (ncomRx.parser.state != NCOM_RX_STATE_SYNC_1) {
        ncomRx.stats.timeoutErrors++;
        ncomRx.parser.state = NCOM_RX_STATE_SYNC_1;
      }
    }
  }
}

void Task_Sensor(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(TASK_SENSOR_SLEEP_MS);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  float mag[4];

  for (;;) {

    if (ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(20)) > 0) {
      taskENTER_CRITICAL();
      mag[0] = (float)((int16_t)((magBuf[2] << 8) | magBuf[1])) * lis.mult;
      mag[1] = (float)((int16_t)((magBuf[4] << 8) | magBuf[3])) * lis.mult;
      mag[2] = (float)((int16_t)((magBuf[6] << 8) | magBuf[5])) * lis.mult;
      mag[3] = (float)((int16_t)((magBuf[8] << 8) | magBuf[7])) * 0.125f;
      taskEXIT_CRITICAL();

      if (xSemaphoreTake(xStateMutex, pdMS_TO_TICKS(5)) == pdTRUE) {
        lis.data.magX = mag[0];
        lis.data.magY = mag[1];
        lis.data.magZ = mag[2];
        lis.data.tempC = mag[3];
        isMagUpdated = true;
        xSemaphoreGive(xStateMutex);
      }
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void Task_SysMonitor(void *pvParameters) {
  const TickType_t xFrequency = pdMS_TO_TICKS(TASK_SYSMONITOR_SLEEP_MS);
  TickType_t xLastWakeTime = xTaskGetTickCount();

  uint32_t oneSecondTimer = 0;

  uint8_t missedHbStateEstimateTask = 0;
  uint8_t missedHbControlTask = 0;
  uint8_t missedHbNCOMTask = 0;

  uint8_t missedHbTolStateEstimateTask = 2;
  uint8_t missedHbTolControlTask = 2;
  uint8_t missedHbTolNCOMTask = 10;

  static TaskStatus_t pxTaskStatusArray[16];
  volatile UBaseType_t uxAllocatedArraySize = 16, x;
  TaskHandle_t xIdleTaskHandle = xTaskGetIdleTaskHandle();

  static uint32_t prevIdleCounter = 0;
  static uint32_t prevTotalRunTime = 0;

  for (;;) {

    if (hbStateEstimateTask == AUV_TASK_ALIVE) {
      missedHbStateEstimateTask = 0;
      hbStateEstimateTask = AUV_TASK_DEAD;
    } else
      missedHbStateEstimateTask++;

    if (hbControlTask == AUV_TASK_ALIVE) {
      missedHbControlTask = 0;
      hbControlTask = AUV_TASK_DEAD;
    } else
      missedHbControlTask++;

    if (hbNCOMTask == AUV_TASK_ALIVE) {
      missedHbNCOMTask = 0;
      hbNCOMTask = AUV_TASK_DEAD;
    } else
      missedHbNCOMTask++;

    if ((missedHbStateEstimateTask <= missedHbTolStateEstimateTask) &&
        (missedHbControlTask <= missedHbTolControlTask) &&
        (missedHbNCOMTask <= missedHbTolNCOMTask)) {
      HAL_IWDG_Refresh(&hiwdg);
    } else {

      HAL_IWDG_Refresh(&hiwdg);

      uint8_t deadTaskBitmask = 0;
      if (missedHbStateEstimateTask > missedHbTolStateEstimateTask)
        deadTaskBitmask |= (1 << 0);
      if (missedHbControlTask > missedHbTolControlTask)
        deadTaskBitmask |= (1 << 1);
      if (missedHbNCOMTask > missedHbTolNCOMTask)
        deadTaskBitmask |= (1 << 2);

      NCOM_TX_SendPacketUnsafe(NCOM_MSG_DYING_GASP, &deadTaskBitmask, 1);
      HAL_Delay(5);

      __disable_irq();
      while (1) {
        ;
      }
    }

    oneSecondTimer += TASK_SYSMONITOR_SLEEP_MS;
    if (oneSecondTimer >= 1000) {
      oneSecondTimer = 0;

      uint32_t ulTotalRunTime;
      UBaseType_t uxReturnedTaskCount;

      uxReturnedTaskCount = uxTaskGetSystemState(
          pxTaskStatusArray, uxAllocatedArraySize, &ulTotalRunTime);

      if (ulTotalRunTime > 0) {
        for (x = 0; x < uxReturnedTaskCount; x++) {
          if (pxTaskStatusArray[x].xHandle == xIdleTaskHandle) {
            uint32_t currIdleCounter = pxTaskStatusArray[x].ulRunTimeCounter;
            uint32_t deltaIdle = currIdleCounter - prevIdleCounter;
            uint32_t deltaTotal = ulTotalRunTime - prevTotalRunTime;
            prevIdleCounter = currIdleCounter;
            prevTotalRunTime = ulTotalRunTime;

            if (deltaTotal > 0) {
              float idlePct = (float)deltaIdle / (float)deltaTotal * 100.0f;
              cpuLoad = 100.0f - idlePct;
              if (cpuLoad < 0.0f)
                cpuLoad = 0.0f;
              else if (cpuLoad > 100.0f)
                cpuLoad = 100.0f;
            }
            break;
          }
        }
      }
    }

    vTaskDelayUntil(&xLastWakeTime, xFrequency);
  }
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin) {
  if (GPIO_Pin == icm.GPIO_PIN_x_INT) { // IMU
    HAL_GPIO_WritePin(icm.pGPIOx, icm.GPIO_PIN_x, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_DMA(icm.pSPIx, imuBufDMATx, imuBufDMARx, 15);
  } else if (GPIO_Pin == lis.GPIO_PIN_x_INT) {
    HAL_GPIO_WritePin(lis.pGPIOx, lis.GPIO_PIN_x, GPIO_PIN_RESET);
    HAL_SPI_TransmitReceive_DMA(&hspi1, magBufDMATx, magBufDMARx, 9);
  }
}

void HAL_SPI_TxRxCpltCallback(SPI_HandleTypeDef *hspi) {
  if (hspi == icm.pSPIx) {
    HAL_GPIO_WritePin(icm.pGPIOx, icm.GPIO_PIN_x, GPIO_PIN_SET);
    memcpy(imuBuf, imuBufDMARx, 15);
    if (xStateEstimateTaskHandle == NULL)
      return;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xStateEstimateTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }

  else if (hspi == lis.pSPIx) {
    HAL_GPIO_WritePin(lis.pGPIOx, lis.GPIO_PIN_x, GPIO_PIN_SET);
    memcpy(magBuf, magBufDMARx, 9);
    if (xSensorTaskHandle == NULL)
      return;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    vTaskNotifyGiveFromISR(xSensorTaskHandle, &xHigherPriorityTaskWoken);
    portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
  }
}

void ConfigureTimerForRunTimeStats(void) { HAL_TIM_Base_Start_IT(&htim16); }

unsigned long GetTimerForRunTimeStats(void) {
  uint32_t highBits;
  uint32_t lowBits;
  do {
    highBits = ulHighWordOverflows;
    lowBits = __HAL_TIM_GET_COUNTER(&htim16);
  } while (highBits != ulHighWordOverflows);

  return (highBits << 16) | lowBits;
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM6 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM6) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  else if (htim->Instance == TIM16)
    ulHighWordOverflows++;

  /* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void) {
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1) {
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
void assert_failed(uint8_t *file, uint32_t line) {
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line
     number, ex: printf("Wrong parameters value: file %s on line %d\r\n", file,
     line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
