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
#include "esp_sockets.h"
#include "esp_port.h"
#include "assert.h"

#include <string.h>
#include "esp_server.h"
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
static uint8_t message[] = "Hello\r\n";
//static uint8_t message1[] = "Hello1\r\n";
static uint8_t buffer[50];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
static void server_data_handle(esocket_t* socket)
{
	uint8_t data[50] = {0};

	assert(NULL != socket);

	size_t size = esp_socket_get_rx_data_num(socket);
	esp_conn_id_t id = esp_socket_get_connection_id(socket);

	debug_info("[Server RX]: len = %d, id = %d\r\n", size, CONVER_TO_NUMBER(id));

	for(size_t i = 0; i < size; i++)
	{
		uint8_t ch = '\0';

		if(esp_socket_rx_data_denqueue(socket, &ch))
		{
			data[i] = ch;
			debug_info("%c", ch);
		}
	}

	debug_info("\r\n");

	esp_server_transmit(socket, data, size);

}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	esocket_t* socket = NULL;
	const char ssid[] = SSID;
	const char password[] = PASS;
	char ssid_cur[35] = {0};
	ip4addr_t ip = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	esp_tcp_cfg_t connParam;
	char ipStr[] = "192.168.100.57\0";
	char _ip[] = "000.000.000.000\0";
	char _gw[] = "000.000.000.000\0";
	char _msk[] = "000.000.000.000\0";
	esp_at_version_t at_version = {0};
	esp_sdk_version_t sdk_version = {0};
	char at_string[10] = {0};
	char sdk_string[10] = {0};
	uint32_t count = 3;
	esp_status_t res = ESP_INNER_ERR;
	int32_t len = 0;
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
  esp_drv_init();

  convert_string_to_ip4addr(&connParam.remoteIp, ipStr);
    connParam.remotePort = 9000;
    connParam.keepAlive = 0;
    connParam.id = ESP_ID_NONE;

    debug_info("Example #1\r\n");
    debug_info("\t- WiFi in station mode\r\n");
    debug_info("\t- TCP server\r\n");
    debug_info("\t- Multi connection mode\r\n");
    debug_info("\t- Normal transmit mode\r\n");
    debug_info("\r\n");
    debug_info("\r\n");

    esp_get_version(&at_version, &sdk_version, 2000);
    esp_at_version_to_string(&at_version, at_string);
    esp_sdk_version_to_string(&sdk_version, sdk_string);

    debug_info("AT command version: %s\r\n", at_string);
    debug_info("SDK version: %s\r\n", sdk_string);

    debug_info("Setup WiFi station mode...");
    if(esp_wifi_mode_setup(ESP_WIFI_STATION, false, 5000u))
    {
  	  debug_info(" PASS\r\n");

  	  debug_info("Check connection to Wifi AP: \r\n");
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

			  esp_server_init(333, 1, 0);
			  esp_register_server_receive_cb(server_data_handle);

//  		  	  if(esp_socket_init())
//  		  	  {
//  		  		  debug_info(" PASS\r\n");
//  		  		  debug_info("Opening tcp socket...");
//  				  socket = esp_socket_open(connParam.remoteIp, connParam.remotePort);
//  				  if(socket != NULL)
//  				  {
//  					  debug_info(" PASS\r\n");
//  				  }
//  			  	  else
//  			  	  {
//  			  		  debug_error(" FAULT\r\n");
//  			  	  }
//  				  esp_socket_transmit(socket, message, sizeof(message));
//  		  	  }
//  		  	  else
//  		  	  {
//  		  		  debug_error(" FAULT\r\n");
//  		  	  }
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
	esp_drv_receive_handle();
	esp_drv_transmit_handle();

	esp_server_receive_handle();
	esp_server_transmit_handle();

//	len = esp_socket_receive(socket, buffer, sizeof(buffer));
//	if(len > 0) {
//		esp_socket_transmit(socket, buffer, len);
//	}

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
	debug_critical("Wrong parameters value: file %s on line %d\r\n", file, line);
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
