
#include "main.h"
#include "i2c.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "string.h"
#include "stdlib.h"
#include "stdarg.h"
#include <bme680.h>
#include "stdio.h"

#define DELAY_PERIOD_MS (1*1000)

struct bme680_dev gas_sensor;
char i2c_reading_buf[100];
int8_t rslt = BME680_OK;
void SystemClock_Config(void);
static void UARTSendString(char *msg, ...);
void uart_transmit(char *string);
int8_t bme680I2cRead(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);
int8_t bme680I2cWrite(uint8_t dev_id, uint8_t reg_addr, uint8_t *reg_data, uint16_t len);

int main(void)
{

  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_I2C1_Init();
  MX_TIM6_Init();
  MX_USART1_UART_Init();
  gas_sensor.dev_id = BME680_I2C_ADDR_SECONDARY;
  gas_sensor.intf = BME680_I2C_INTF;
  gas_sensor.read = bme680I2cRead;
  gas_sensor.write = bme680I2cWrite;
  gas_sensor.delay_ms = HAL_Delay;
  gas_sensor.amb_temp = 25;
  if (bme680_init(&gas_sensor) != BME680_OK) {
  char bme_msg[] = "BME680 Initialization Error\r\n";
  UARTSendString(bme_msg);
  } else {
  char bme_msg[] = "BME680 Initialized and Ready\r\n";
  UARTSendString(bme_msg);
  }
  gas_sensor.tph_sett.os_hum = BME680_OS_2X;
  gas_sensor.tph_sett.os_pres = BME680_OS_4X;
  gas_sensor.tph_sett.os_temp = BME680_OS_8X;
	
  gas_sensor.power_mode = BME680_FORCED_MODE;
	
  uint8_t required_settings = (BME680_OST_SEL | BME680_OSP_SEL | BME680_OSH_SEL);
	
  rslt = bme680_set_sensor_settings(required_settings, &gas_sensor);
  rslt = bme680_set_sensor_mode(&gas_sensor);
  uint16_t min_sampling_period;
	
  bme680_get_profile_dur(&min_sampling_period, &gas_sensor);

  struct bme680_field_data data;
  float lastTemp = 9999, lastHum = 9999;
  int i = 0, j =0, counter = 1;

  while (1)
  {
	  HAL_Delay(min_sampling_period);

	  rslt = bme680_get_sensor_data(&data, &gas_sensor);

	  sprintf(i2c_reading_buf,
	  "T: %u.%u degC H: %u.%u %%rH \n",
	  (unsigned int)data.temperature / 100,
	  (unsigned int)data.temperature % 100,
	  (unsigned int)data.humidity / 1000,
	  (unsigned int)data.humidity % 1000);

	  UARTSendString(i2c_reading_buf);

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
	  
	  HAL_Delay(DELAY_PERIOD_MS);

	  if (gas_sensor.power_mode == BME680_FORCED_MODE) {
	  rslt = bme680_set_sensor_mode(&gas_sensor);


	  }

  }
}


void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

	
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

void Error_Handler(void)
{

  __disable_irq();
  while (1)
  {
  }

}

#ifdef  USE_FULL_ASSERT

void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif
