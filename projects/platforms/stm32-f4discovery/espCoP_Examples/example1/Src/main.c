/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  ** This notice applies to any and all portions of this file
  * that are not between comment pairs USER CODE BEGIN and
  * USER CODE END. Other portions of this file, whether 
  * inserted by the user or by software development tools
  * are owned by their respective copyright owners.
  *
  * COPYRIGHT(c) 2019 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "dma.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stack/esp_basic.h>
#include <stack/esp_wifi.h>
#include <stack/esp_tcpip.h>
#include "debug.h"
#include "slre.h"
#include "esp_drv.h"
#include "esp_port.h"
#include "assert.h"

#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
//#define SSID	"HUAWEI-DUge\0"
//#define PASS	"485754439FED179D\0"
#define SSID	"AlVik\0"
#define PASS	"xPo#h0c2016\0"
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
static char message[] = "Hello\r\n";
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static bool receice_handlle(esp_conn_id_t id, const char data[], size_t size)
{
	if(data == NULL || size == 0)
	{
		debug_error("[TCP]: Received from id: %c...FAULT\r\n", id);
		return false;
	}

	debug_info("[TCP]: Received from id: %c\r\n", id);
	debug_info("\r\n");
	debug_dump((const void*)data, (unsigned long)&data[0], size, 1);
	debug_info("\r\n");
	debug_info("\r\n");

	esp_tcp_transmit(id, message, sizeof(message));
	return true;
}

static bool disconnect_handlle(esp_conn_id_t id)
{
	debug_info("[TCP]: Connection %c close\r\n", id);
	return true;
}

static bool connect_handlle(esp_conn_id_t id)
{
	debug_info("[TCP]: Connection %c open\r\n", id);
	return true;
}

static void transmit_pass_handle(void)
{
	debug_info("[TCP]: Transmit Pass\r\n");
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	const char ssid[] = SSID;
	const char password[] = PASS;
	char ssid_cur[35] = {0};
	ip4addr_t ip = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	esp_tcp_cfg_t connParam;
	char ipStr[] = "192.168.100.7\0";
	char _ip[] = "000.000.000.000\0";
	char _gw[] = "000.000.000.000\0";
	char _msk[] = "000.000.000.000\0";

	esp_at_version_t at_version = {0};
	esp_sdk_version_t sdk_version = {0};

	esp_conn_status_t status;
	esp_tx_mode_t transfer;
	esp_conn_mode_t mux;
	uint32_t count = 3;
	esp_status_t res = ESP_INNER_ERR;

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
  MX_DMA_Init();
  MX_USART3_UART_Init();
  MX_USART2_UART_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  debug_init();

  esp_register_receive_cb(&receice_handlle);
  esp_register_close_conn_cb(&disconnect_handlle);
  esp_register_open_conn_cb(&connect_handlle);
  esp_register_transmit_cb(&transmit_pass_handle);
  esp_init();

  convert_string_to_ip4addr(&connParam.remoteIp, ipStr);
  connParam.remotePort = 9000;
  connParam.keepAlive = 0;
  connParam.id = ESP_ID_NONE;

  debug_info("Example #1\r\n");
  debug_info("\t- WiFi in station mode\r\n");
  debug_info("\t- TCP client\r\n");
  debug_info("\t- Single connection mode\r\n");
  debug_info("\t- Normal transmit mode\r\n");
  debug_info("\r\n");
  debug_info("\r\n");


  esp_get_version(&at_version, &sdk_version, 2000);


  debug_info("Setup WiFi station mode...");
  if(esp_wifi_mode_setup(ESP_WIFI_STATION, false, 5000u))
  {
	  debug_info(" PASS\r\n");

	  debug_info("Check connection to AP status: \r\n");
	  if(esp_wifi_ap_ssid_request(ssid_cur, false, 5000u) == ESP_PASS)
	  {
		  if(strlen(ssid_cur) != 0)
		  {
			  debug_info("\tjoin to %s\r\n", ssid_cur);
			  if(esp_wifi_ap_unjoin(5000u))
			  {
				  debug_info("\tunjoin from %s\r\n", ssid_cur);
			  }
		  }
		  else
		  {
			  debug_info("\tdo not join\r\n");
		  }
	  }

	  debug_info("Join to SSID: %s...", ssid);
	  while(res != ESP_PASS && count > 0)
	  {
		  res = esp_wifi_ap_join(ssid, password, false, 5000u);
		  count--;
	  }

	  if(res == ESP_PASS)
	  {
		  debug_info(" PASS\r\n");
		  debug_info("Request station IP...");

		  res = ESP_INNER_ERR;
		  count = 1;
		  while(res != ESP_PASS && count != 0)
		  {
			  res = esp_wifi_station_ip_request(&ip, &gw, &mask, false, 5000u);
			  count++;
		  }
		  if(res == ESP_PASS)
		  {
			  debug_info(" PASS\r\n");

			  convert_ip4addr_to_string(ip, _ip);
			  convert_ip4addr_to_string(gw, _gw);
			  convert_ip4addr_to_string(mask, _msk);

			  debug_info("\tIP: %s\r\n", _ip);
			  debug_info("\tGW: %s\r\n", _gw);
			  debug_info("\tMASK: %s\r\n", _msk);
		  }
		  else
		  {
			  debug_error(" FAULT\r\n");
		  }

		  debug_info("Request multiplexor mode...");
		  res = ESP_INNER_ERR;
		  count = 0;

		  while(res != ESP_PASS)
		  {
			  res = esp_mux_cfg_request(&mux, 5000u);
			  count++;
		  }
		  if(res == ESP_PASS)
		  {
			  debug_info(" PASS\r\n");
			  if(mux != ESP_SINGLE_CONNECT)
			  {
				  debug_info("Setup single mode...");
				  if(esp_single_connection_enable(5000u) == ESP_SERVER_ON)
				  {
					  debug_info(" FAULT\r\n");
					  debug_info("\t Need disable TCP Server...");
					  if(esp_tcp_server_close(0, 5000u))
					  {
						  debug_info(" PASS\r\n");

						  debug_info("Setup single mode...");
						  if(esp_single_connection_enable(5000u))
						  {
							  debug_info(" PASS\r\n");
						  }
						  else
						  {
							  debug_error(" FAULT\r\n");
						  }
					  }
					  else
					  {
						  debug_error(" FAULT\r\n");
					  }
				  }
				  else
				  {
					  debug_info(" PASS\r\n");
				  }
			  }
			  else
			  {
				  debug_info("\tAlready in single mode...\r\n");
			  }

			  debug_info("Request transfer mode...");
			  res = ESP_INNER_ERR;
			  count = 0;
			  while(res != ESP_PASS)
			  {
				  res = esp_transmit_mode_request(&transfer, 5000u);
				  count++;
			  }
			  if(res == ESP_PASS)
			  {
				  debug_info(" PASS\r\n");
				  if(transfer != ESP_NORMAL_MODE)
				  {
					  debug_info("Setup normal transfer mode...");
					  if(esp_transmit_mode_setup(ESP_NORMAL_MODE, 5000u) == ESP_PASS)
					  {
						  debug_info(" PASS\r\n");
					  }
					  else
					  {
						  debug_error(" FAULT\r\n");
					  }
				  }
				  else
				  {
					  debug_info("\tAlready in normal transfer mode...\r\n");
				  }
			  }
			  else if(res == ESP_PASS)
			  {
				  debug_error(" TIMEOUT\r\n");
			  }
			  else
			  {
				  debug_error(" FAULT\r\n");
			  }

			  debug_info("Establish TCP connection...");
			  if(esp_tcp_connect(&connParam, 5000u) == ESP_PASS)
			  {
				  debug_info(" PASS\r\n");
				  if(esp_conn_status_request(&status, 5000u) == ESP_PASS)
				  {
					  debug_info("\tID: %c\r\n", status.id);
					  debug_info("\tlocalPort: %d\r\n", status.localPort);
					  convert_ip4addr_to_string(status.remoteIp, _ip);
					  debug_info("\tremoteIp: %s\r\n", _ip);
					  debug_info("\tremotePort: %d\r\n", status.remotePort);
					  debug_info("\tstat: %c\r\n", status.stat);
					  debug_info("\ttype: %x\r\n", status.type);
				  }
			  }
			  else
			  {
				  debug_error(" FAULT\r\n");
			  }

			  esp_hardware_switch_mode(UNBLOCK);
			  esp_tcp_transmit(ESP_ID_NONE, message, sizeof(message));
		  }
		  else
		  {
			  debug_error(" FAULT\r\n");
		  }
	  }
	  else
	  {
		  debug_error(" FAULT\r\n");
	  }
  }

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	esp_message_handle();
	esp_tcp_transmit_handle();
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

  /**Configure the main internal regulator output voltage 
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /**Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
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
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
