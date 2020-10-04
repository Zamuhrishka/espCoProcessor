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
#if !defined(UNIT_TESTING)
//_____ C O N F I G S  __________________________________________________________
//_____ I N C L U D E S _______________________________________________________
#include "esp_port.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "usart.h"
#include "stm32f4xx_hal_dma.h"
#include "queue.h"

#if defined(ESP_PORT_DEBUG)
#include "debug.h"
#endif
//_____ D E F I N I T I O N ___________________________________________________
#if defined(ESP_PORT_DEBUG)
	#define ESP_PORT_DEBUG_PARAM(fmt, ...)       debug_info(fmt, __VA_ARGS__)
	#define ESP_PORT_DEBUG(fmt)                  debug_info(fmt)
	#define ESP_PORT_ERROR_PARAM(fmt, ...)       debug_error(fmt, __VA_ARGS__)
	#define ESP_PORT_ERROR(fmt)                  debug_error(fmt)
#else
	#define ESP_PORT_DEBUG_PARAM(fmt, ...)
	#define ESP_PORT_DEBUG(fmt, ...)
	#define ESP_PORT_ERROR_PARAM(fmt, ...)
	#define ESP_PORT_ERROR(fmt)
#endif
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!UART receive buffer
static char esp_hardware_buffer[ESP_DRV_BUFFER_SIZE] = {0};

//!Receive Queue
queue_t* esp_hwrx_queue = NULL;

static volatile bool is_recv_message = false;
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

/** Retrieves a mask which contains the current state of the global interrupts for the device. This
 *  value can be stored before altering the global interrupt enable state, before restoring the
 *  flag(s) back to their previous values after a critical section using \ref SetGlobalInterruptMask().
 *
 *  \ingroup Group_GlobalInt
 *
 *  \return  Mask containing the current Global Interrupt Enable Mask bit(s).
 */
inline static uint32_t get_global_interrupt_mask(void)
{
//	GCC_MEMORY_BARRIER();
	return __get_PRIMASK();
}

/** Sets the global interrupt enable state of the microcontroller to the mask passed into the function.
 *  This can be combined with \ref GetGlobalInterruptMask() to save and restore the Global Interrupt Enable
 *  Mask bit(s) of the device after a critical section has completed.
 *
 *  \ingroup Group_GlobalInt
 *
 *  \param[in] GlobalIntState  Global Interrupt Enable Mask value to use
 */
inline static void set_global_interrupt_mask(const uint32_t GlobalIntState)
{
//	GCC_MEMORY_BARRIER();
	__set_PRIMASK(GlobalIntState);
//	GCC_MEMORY_BARRIER();
}

/** Enables global interrupt handling for the device, allowing interrupts to be handled.
 *
 *  \ingroup Group_GlobalInt
 */
inline static void global_interrupt_enable(void)
{
	//	GCC_MEMORY_BARRIER();
	__enable_irq();
	//	GCC_MEMORY_BARRIER();
}

/** Disabled global interrupt handling for the device, preventing interrupts from being handled.
 *
 *  \ingroup Group_GlobalInt
 */
inline static void global_interrupt_disable(void)
{
	//	GCC_MEMORY_BARRIER();
	__disable_irq();
	//	GCC_MEMORY_BARRIER();
}


inline static uint32_t critical_section_start(void)
{
	uint32_t int_state = get_global_interrupt_mask();
	global_interrupt_disable();
	return int_state;
}

inline static void critical_section_end(const uint32_t global_int_state)
{
	set_global_interrupt_mask(global_int_state);
	global_interrupt_enable();
}

//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function init hardware unit.
*
* Public function defined in esp_port.h
*/
bool esp_harware_init(void)
{
	queue_reg_mem_alloc_cb(malloc);
	queue_reg_mem_free_cb(free);

	esp_hwrx_queue = queue_create(ESP_DRV_BUFFER_SIZE, sizeof(char));
	if(esp_hwrx_queue == NULL) {
		return false;
	}

	esp_hardware_power_on();
	esp_uart_dma_disable();
	esp_uart_clear_irq_flag();
	esp_uart_enable_irq();
	esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));

	return true;
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
	if(HAL_UART_Transmit_DMA(&huart3, (uint8_t*)data, size) == HAL_OK)
	{
		ESP_PORT_DEBUG_PARAM("[ESP_PORT]: ---> %d, %s\r\n", size, data);
		return true;
	}
	else
	{
		ESP_PORT_ERROR("[ESP_PORT]: Transmit error!\r\n");
		return false;
	}

}

/**
* This function is low level function for receive data
* throw UART in block mode.
*
* Public function defined in esp_port.h
*/
int32_t esp_hardware_receive_block(char *msg, size_t len, uint32_t timeout)
{
	bool _is_recv_message = false;
	uint32_t irq_state = 0;
	uint32_t _timeout = HAL_GetTick() + timeout;

	while(!_is_recv_message)
	{
		irq_state = critical_section_start();
		_is_recv_message = is_recv_message;
		critical_section_end(irq_state);

		 if(HAL_GetTick() > _timeout)
		 {
			 if(timeout != 0)
			 {
				 ESP_PORT_ERROR("[ESP_PORT]: Timeout error!\r\n");
				 return (int32_t)ESP_PORT_TIMEOUT_ERR;
			 }
			 else
			 {
				 return (int32_t)ESP_PORT_PASS;
			 }
		 }
	}

	irq_state = critical_section_start();
	is_recv_message = false;
	critical_section_end(irq_state);

	size_t size = queue_size(esp_hwrx_queue);
	if(size > len)
	{
		ESP_PORT_ERROR("[ESP_PORT]: Rx buffer size error!\r\n");
		return ESP_PORT_SIZE_ERR;
	}

	for(size_t i = 0; i < size; i++) {
		queue_denqueue(esp_hwrx_queue, &msg[i]);
	}

	ESP_PORT_DEBUG_PARAM("[ESP_PORT]: <--- %d, %s\r\n", size, msg);

	return size;
}

//\warning Potential error situation when size of msg buffer is less then count of
// received message
esp_port_status_t esp_hardware_receive(char *msg, size_t* len, uint32_t timeout)
{
	bool _is_recv_message = false;
	uint32_t irq_state = 0;
	uint32_t _timeout = HAL_GetTick() + timeout;

	while(!_is_recv_message)
	{
		irq_state = critical_section_start();
		_is_recv_message = is_recv_message;
		critical_section_end(irq_state);

		if(HAL_GetTick() > _timeout)
		{
			if(timeout != 0)
			{
				ESP_PORT_ERROR("[ESP_PORT]: Timeout error!\r\n");
				return ESP_PORT_TIMEOUT_ERR;
			}
			else
			{
				return ESP_PORT_PASS;
			}
		}
	}

	irq_state = critical_section_start();
	is_recv_message = false;
	critical_section_end(irq_state);

	size_t size = queue_size(esp_hwrx_queue);

	for(size_t i = 0; i < size; i++)
	{
		if(queue_denqueue(esp_hwrx_queue, &msg[i])) {
			*len++;
		}
	}

	ESP_PORT_DEBUG_PARAM("[ESP_PORT]: <--- %d, %s\r\n", size, msg);

	return ESP_PORT_PASS;
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
		size_t len = esp_uart_dma_get_nbm();
		size_t size = (len <= ESP_DRV_BUFFER_SIZE) ?
					(ESP_DRV_BUFFER_SIZE - len) :
					ESP_DRV_BUFFER_SIZE;

		len = queue_free_space(esp_hwrx_queue);
		size = (size > len) ? len : size;

		for(size_t i = 0; i < size; i++) {
			queue_enqueue(esp_hwrx_queue, &esp_hardware_buffer[i]);
		}

		is_recv_message = true;

		esp_uart_clear_irq_flag();
		esp_uart_dma_disable_irq();
		esp_uart_dma_receive_cfg((uint8_t*)esp_hardware_buffer, sizeof(esp_hardware_buffer));
	}
}
#endif
