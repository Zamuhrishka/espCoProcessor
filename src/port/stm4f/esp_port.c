// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_port.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief
*
********************************************************************************/
//_____ C O N F I G S  __________________________________________________________
//! Max size of queue buffer
#define ESP_RX_QUEUE_SIZE                 				(10U)

//! Max size of each of element of queue
#define ESP_RX_PAYLOAD_LENGTH							(200U)

//_____ I N C L U D E S _______________________________________________________
#include "esp_port.h"

#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "stm32f4xx_hal_dma.h"
//_____ D E F I N I T I O N ___________________________________________________
//! @brief Static queue structure
//! @{
typedef struct
{
	struct {
		size_t size;
		char buffer[ESP_RX_PAYLOAD_LENGTH];
	}	data[ESP_RX_QUEUE_SIZE];																//!array of data
    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	esp_rqueue_t;
//! @}
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!UART receive buffer
static char esp_hardware_buffer[BUFFER_SIZE] = {0};

//!Receive Queue
static esp_rqueue_t rqueue = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
/**
* @brief 	This function disable UART interrupt.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_disable_irq(void)
{
	extern UART_HandleTypeDef huart3;
	__HAL_UART_DISABLE_IT(&huart3, UART_IT_IDLE);
}

/**
* @brief 	This function enable UART interrupt.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_enable_irq(void)
{
	extern UART_HandleTypeDef huart3;
	__HAL_UART_ENABLE_IT(&huart3, UART_IT_IDLE);
}

/**
* @brief 	This function clear UART interrupt flag.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_clear_irq_flag(void)
{
	uint8_t byte = 0;
	extern UART_HandleTypeDef huart3;
	__HAL_UART_CLEAR_FLAG(&huart3, UART_FLAG_IDLE);

	byte = huart3.Instance->DR;
}

/**
* @brief 	This function test UART interrupt flag.
*
* @param	none.
*
* @return 	none.
*/
inline static bool esp_uart_test_irq(void)
{
	extern UART_HandleTypeDef huart3;
	return (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_IDLE) == SET) ? true : false;
}

/**
* @brief 	This function disable UART DMA interrupt.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_disable_irq(void)
{
	extern UART_HandleTypeDef huart3;

	/* Disable all the transfer interrupts */
	huart3.hdmarx->Instance->CR  &= ~(DMA_IT_TC | DMA_IT_TE | DMA_IT_DME);
	huart3.hdmarx->Instance->FCR &= ~(DMA_IT_FE);
	huart3.hdmarx->Instance->CR  &= ~(DMA_IT_HT);
}

/**
* @brief 	This function enable UART DMA interrupt.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_enable_irq(void)
{
}

/**
* @brief 	This function test UART DMA interrupt flag.
*
* @param	none.
*
* @return 	none.
*/
inline static bool esp_uart_dma_test_irq(void)
{
	extern DMA_HandleTypeDef hdma_usart3_rx;
	return (__HAL_DMA_GET_FLAG(&hdma_usart3_rx, DMA_FLAG_TCIF1_5) == SET) ? true : false;
}

/**
* @brief 	This function clear UART DMA interrupt flag.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_clear_irq_flag(void)
{
	extern DMA_HandleTypeDef hdma_usart3_rx;
	__HAL_DMA_CLEAR_FLAG(&hdma_usart3_rx, DMA_FLAG_TCIF1_5);
}

/**
* @brief 	This function disable UART DMA module.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_disable(void)
{
	extern DMA_HandleTypeDef hdma_usart3_rx;
	__HAL_DMA_DISABLE(&hdma_usart3_rx);
}

/**
* @brief 	This function enable UART DMA module.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_enable(void)
{
	extern DMA_HandleTypeDef hdma_usart3_rx;
	__HAL_DMA_ENABLE(&hdma_usart3_rx);
}

/**
* @brief 	This function return number of received bytes.
*
* @param	none.
*
* @return 	number of received bytes.
*/
inline static size_t esp_uart_dma_get_nbm(void)
{
	extern DMA_HandleTypeDef hdma_usart3_rx;
	return __HAL_DMA_GET_COUNTER((&hdma_usart3_rx));
}

/**
* @brief 	This function configure UART DMA module.
*
* @param	none.
*
* @return 	none.
*/
inline static void esp_uart_dma_receive_cfg(uint8_t *pData, uint16_t Size)
{
	extern UART_HandleTypeDef huart3;

	huart3.hdmarx->State = HAL_DMA_STATE_READY;
	huart3.RxState = HAL_UART_STATE_READY;
	huart3.hdmarx->Lock = HAL_UNLOCKED;

	HAL_UART_Receive_DMA(&huart3, pData, Size);
}

/**
* This function add data into receive queue.
*
* Public function defined in esp_queue.h
*/
static bool esp_rbuffer_enqueue(const char buffer[], size_t size)
{
	if(rqueue.size == ESP_RX_QUEUE_SIZE) {
		return false;
	}

	rqueue.data[rqueue.write].size = size;
	if(memcpy(&rqueue.data[rqueue.write].buffer, buffer, size) == NULL) {
		return false;
	}

	rqueue.size++;
	rqueue.write = (rqueue.write == ESP_RX_QUEUE_SIZE - 1ul) ? 0ul: (rqueue.write + 1ul);

	return true;
}

/**
* This function get data from receive queue.
*
* Public function defined in esp_queue.h
*/
static bool esp_rbuffer_denqueue(char **buffer, size_t *size)
{
	if(rqueue.size == 0) {
		return false;
	}

	*size = rqueue.data[rqueue.read].size;
	*buffer = rqueue.data[rqueue.read].buffer;

	rqueue.size--;
	rqueue.read = (rqueue.read == ESP_RX_QUEUE_SIZE - 1ul) ? 0ul : (rqueue.read + 1ul);

	return true;
}

static bool esp_rbuffer_is_empty(void)
{
	if(rqueue.size == 0) {
		return true;
	}

	return false;
}

//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function init hardware unit.
*
* Public function defined in esp_port.h
*/
void esp_harware_init(void)
{
	esp_hardware_power_on();
	esp_uart_dma_disable();
	esp_uart_clear_irq_flag();
	esp_uart_enable_irq();
	esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));
}

/**
* This function is low level function for enable power on module.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_power_on(void)
{
	return false;
}

/**
* This function is low level function for disable power on module.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_power_off(void)
{
	return false;
}

/**
* This function is low level function for transmit data throw UART.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_transmit_block(const char data[], uint16_t size)
{
	return HAL_UART_Transmit_DMA(&huart3, (uint8_t*)data, size) == HAL_OK ? true : false;
}

/**
* This function is low level function for receive data
* throw UART in block mode.
*
* Public function defined in esp_port.h
*/
esp_status_t esp_hardware_receive_block(char **msg, size_t *len, uint32_t timeout)
{
	uint32_t _timeout = 0ul;

	_timeout = HAL_GetTick() + timeout;

	while(esp_rbuffer_is_empty())
	{
		 if(HAL_GetTick() > _timeout) {
			 return ESP_TIMEOUT;
		 }
	}

	 if(!esp_rbuffer_denqueue(msg, len)) {
		 return ESP_INNER_ERR;
	 }

	return ESP_PASS;
}



bool esp_hardware_receive(char **msg, size_t *len)
{

	if(esp_rbuffer_is_empty()) {
		return false;
	}

	 if(!esp_rbuffer_denqueue(msg, len)) {
		 return false;
	 }

	return true;
}




/**
* This function is interrupt handler for received
* data throw UART
*
* Public function defined in esp_port.h
*/
void esp_hardware_receive_irq(void)
{
	if(esp_uart_test_irq())
	{
		size_t size = BUFFER_SIZE - esp_uart_dma_get_nbm();

		esp_rbuffer_enqueue(esp_hardware_buffer, size);

		esp_uart_clear_irq_flag();
		esp_uart_dma_disable_irq();
		esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));
	}
}
