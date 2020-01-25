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
static char receive_data[BUFFER_SIZE] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
/**
* @brief 	This function disable UART interrupt.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_UartIrqDisable(void)
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
inline static void ESP_UartIrqEnable(void)
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
inline static void ESP_UartClrFlag(void)
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
inline static bool ESP_IsUartIrq(void)
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
inline static void ESP_DmaIrqDisable(void)
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
inline static void ESP_DmaIrqEnable(void)
{
}

/**
* @brief 	This function test UART DMA interrupt flag.
*
* @param	none.
*
* @return 	none.
*/
inline static bool ESP_IsDmaIrq(void)
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
inline static void ESP_ClrDmaIrq(void)
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
inline static void ESP_DmaDisable(void)
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
inline static void ESP_DmaEnable(void)
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
inline static size_t ESP_DmaGetNbm(void)
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
inline static void ESP_ReceiveDmaCfg(uint8_t *pData, uint16_t Size)
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
* @brief 	This function init hardware unit.
*
* @param	none.
*
* @return 	none.
*/
void ESP_HardWareInit(void)
{
	ESP_HardWarePowerOn();
	ESP_HardWareSwitchMode(BLOCK);
}

/**
* @brief 	This function switch between block and unblock modes.
*
* @param	mode[in] - type of mode.
*
* @return 	none.
*/
void ESP_HardWareSwitchMode(Blocking_t mode)
{
	if(mode == BLOCK)
	{
		ESP_UartIrqDisable();
		ESP_DmaDisable();
	}
	else
	{
		ESP_DmaDisable();
		ESP_UartClrFlag();
		ESP_UartIrqEnable();
		ESP_ReceiveDmaCfg((uint8_t*)receive_data, sizeof(receive_data));
	}
}

/**
* @brief 	Low level function for enable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_HardWarePowerOn(void)
{
	return false;
}

/**
* @brief 	Low level function for disable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_HardWarePowerOff(void)
{
	return false;
}

/**
* @brief 	Low level function for transmit data throw UART.
*
* @param	data[in] - array of data.
* @param	size[in] - size of data.
*
* @return 	none.
*/
bool ESP_HardWareTransmitUartBlock(const char data[], uint16_t size)
{
	return HAL_UART_Transmit_DMA(&huart3, (uint8_t*)data, size) == HAL_OK ? true : false;
}

/**
* @brief 	Low level function for transmit data throw UART in block mode.
*
* @param	data[in] - array of data.
* @param	size[in] - size of data.
* @param	timeout[in] - timeout in msec. for waiting data.
*
* @return 	none.
*/
ESPStatus_t ESP_HardWareReceiveUartBlock(uint8_t* data, uint16_t size, uint32_t timeout)
{
	uint32_t _timeout = 0ul;

	ESP_HardWareSwitchMode(BLOCK);

	ESP_UartIrqDisable();
	ESP_DmaDisable();
	ESP_ReceiveDmaCfg(data, size);

	_timeout = HAL_GetTick() + timeout;
	while(!ESP_IsUartIrq())
	{
		 if(HAL_GetTick() > _timeout) {
		  return ESP_TIMEOUT;
		 }

		 if(ESP_IsDmaIrq())
		 {
			 ESP_ClrDmaIrq();
			 return ESP_SIZE_ERR;
		 }
	};

	ESP_UartClrFlag();
	ESP_DmaIrqDisable();
	ESP_DmaDisable();

	return ESP_PASS;
}

/**
* @brief 	This function is interrupt handler for received
*			data throw UART.
*
* @warning	This function need to be put in the RX UART
* 			interrupt vector handler.
*
* @param	none.
*
* @return 	none.
*/
void ESP_HardWareReceiveIrq(void)
{
	size_t size = 0;

	if(ESP_IsUartIrq())
	{
		size = BUFFER_SIZE - ESP_DmaGetNbm();

		if(!ESP_RxQueueEnqueue(receive_data, size)) {
			//TODO: Add error handler
		}

		ESP_UartClrFlag();
		ESP_DmaIrqDisable();
		ESP_ReceiveDmaCfg((uint8_t*)receive_data, sizeof(receive_data));
	}
}
