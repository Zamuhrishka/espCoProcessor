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
//_____ I N C L U D E S _______________________________________________________
#include "esp_port.h"

#include <string.h>
#include <stdio.h>
#include "usart.h"
#include "stm32f4xx_hal_dma.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!UART receive buffer
static char esp_hardware_buffer[BUFFER_SIZE] = {0};
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
	esp_hardware_switch_mode(BLOCK);
}

/**
* This function switch between block and unblock modes.
*
* Public function defined in esp_port.h
*/
void esp_hardware_switch_mode(esp_blocking_t mode)
{
	if(mode == BLOCK)
	{
		esp_uart_disable_irq();
		esp_uart_dma_disable();
	}
	else
	{
		esp_uart_dma_disable();
		esp_uart_clear_irq_flag();
		esp_uart_enable_irq();
		esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));
	}
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
esp_status_t esp_hardware_receive_block(uint8_t* data, uint16_t size, uint32_t timeout)
{
	uint32_t _timeout = 0ul;

	esp_hardware_switch_mode(BLOCK);

	esp_uart_disable_irq();
	esp_uart_dma_disable();
	esp_uart_dma_receive_cfg(data, size);

	_timeout = HAL_GetTick() + timeout;
	while(!esp_uart_test_irq())
	{
		 if(HAL_GetTick() > _timeout) {
		  return ESP_TIMEOUT;
		 }

		 if(esp_uart_dma_test_irq())
		 {
			 esp_uart_dma_clear_irq_flag();
			 return ESP_SIZE_ERR;
		 }
	};

	esp_uart_clear_irq_flag();
	esp_uart_dma_disable_irq();
	esp_uart_dma_disable();

	return ESP_PASS;
}

/**
* This function is interrupt handler for received
* data throw UART
*
* Public function defined in esp_port.h
*/
void esp_hardware_receive_irq(void)
{
	size_t size = 0;

	if(esp_uart_test_irq())
	{
		size = BUFFER_SIZE - esp_uart_dma_get_nbm();

		if(!esp_rbuffer_enqueue(esp_hardware_buffer, size)) {
			//TODO: Add error handler
		}

		esp_uart_clear_irq_flag();
		esp_uart_dma_disable_irq();
		esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));
	}
}
