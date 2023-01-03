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
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <bme680.h>
 #include "stdio.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DELAY_PERIOD_MS (1*1000)
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
struct bme680_dev gas_sensor;
char i2c_reading_buf[100];
int8_t rslt = BME680_OK;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
static void UARTSendString(char *msg, ...);
void uart_transmit(char *string);
int8_t bme680I2cRead(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t bme680I2cWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
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
  MX_I2C1_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  //HAL_GPIO_WritePin(BuzzerPin_GPIO_Port, BuzzerPin_Pin, GPIO_PIN_RESET);
  // Configure the BME680 driver
  gas_sensor.dev_id = BME680_I2C_ADDR_SECONDARY;
  gas_sensor.intf = BME680_I2C_INTF;
  gas_sensor.read = bme680I2cRead;
  gas_sensor.write = bme680I2cWrite;
  gas_sensor.delay_ms = HAL_Delay;
  gas_sensor.amb_temp = 25;

  // Initialize the driver
  if (bme680_init(&gas_sensor) != BME680_OK) {
  char bme_msg[] = "BME680 Initialization Error\r\n";
  UARTSendString(bme_msg);
  } else {
  char bme_msg[] = "BME680 Initialized and Ready\r\n";
  UARTSendString(bme_msg);
  }

  // Select desired oversampling rates
  gas_sensor.tph_sett.os_hum = BME680_OS_2X;
  gas_sensor.tph_sett.os_pres = BME680_OS_4X;
  gas_sensor.tph_sett.os_temp = BME680_OS_8X;

  // Set sensor to "always on"
  gas_sensor.power_mode = BME680_FORCED_MODE;

  // Set oversampling settings
  uint8_t required_settings = (BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL);
  rslt = bme680_set_sensor_settings(required_settings, &gas_sensor);

  // Set sensor mode
  rslt = bme680_set_sensor_mode(&gas_sensor);

  // Query minimum sampling period
  uint16_t min_sampling_period;
  bme680_get_profile_dur(&min_sampling_period, &gas_sensor);

  // Sampling results variable
  struct bme680_field_data data;
  float lastTemp = 9999, lastHum = 9999;
  int i = 0, j =0, counter = 1;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  // Allow BME680 to sample environment
	  HAL_Delay(min_sampling_period);

	  // Query the sample data
	  rslt = bme680_get_sensor_data(&data, &gas_sensor);

	  // Format results into a human readable string

	  sprintf(i2c_reading_buf,
	  "T: %u.%u degC H: %u.%u %%rH \n",
	  (unsigned int)data.temperature / 100,
	  (unsigned int)data.temperature % 100,
	  (unsigned int)data.humidity / 1000,
	  (unsigned int)data.humidity % 1000);

	  // Publish Data to connected PC
	  UARTSendString(i2c_reading_buf);

	  // Check Breathing Function
	  float Temperature = data.temperature;
	  float Humidity = data.humidity;
	  counter++;

	  if(Temperature <= lastTemp) {
		  i++;
	  }
	  if(Humidity <= lastHum) {
		  j++;
	  }
	  else {
		  i = 0;
		  j = 0;
	  }

	  	if (((i>=5) || (j>=5)) && (counter > 10)) {
		UARTSendString("Respiration not Detected");

	  	while(1){
	  		HAL_GPIO_WritePin(BuzzerPin_GPIO_Port, BuzzerPin_Pin, 1);
	  		HAL_Delay(500);
	  		HAL_GPIO_WritePin(BuzzerPin_GPIO_Port, BuzzerPin_Pin, 0);
	  		HAL_Delay(500);
	  		}
	  }

	  	  lastTemp = Temperature;
	  	  lastHum = Humidity;
	  // Wait between samples
	  HAL_Delay(DELAY_PERIOD_MS);

	  // Request the next sample
	  if (gas_sensor.power_mode == BME680_FORCED_MODE) {
	  rslt = bme680_set_sensor_mode(&gas_sensor);


	  }
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
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

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
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
int8_t bme680I2cRead(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
	int8_t result;

	if (HAL_I2C_Master_Transmit(&hi2c1, (dev_id << 1), &reg_addr, 1, HAL_MAX_DELAY) != HAL_OK) {
		result = -1;
	} else if (HAL_I2C_Master_Receive (&hi2c1, (dev_id << 1) | 0x01, reg_data, len, HAL_MAX_DELAY) != HAL_OK) {
	result = -1;
} else {
	result = 0;
}

return result;
}

int8_t bme680I2cWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len) {
	int8_t result;
	int8_t *buf;

// Allocate and load I2C transmit buffer
buf = malloc(len + 1);
buf[0] = reg_addr;
memcpy(buf + 1, reg_data, len);

if (HAL_I2C_Master_Transmit(&hi2c1, (dev_id << 1), (uint8_t *) buf, len + 1, HAL_MAX_DELAY) != HAL_OK) {
	result = -1;
} else {
	result = 0;
}

	free(buf);
	return result;
}

static void UARTSendString(char *msg, ...)
{
	char buff[50];

	va_list args;
	va_start(args, msg);
	vsprintf(buff,msg,args);

	uart_transmit(buff);
}

void uart_transmit(char *string)
{
	uint8_t len = strlen(string);
	HAL_UART_Transmit(&huart1, (uint8_t*) string, len, 2000);
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
