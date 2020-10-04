/********************************************************************************
*
* @file    		esp_port.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
//_____ C O N F I G S  ________________________________________________________
#define ESP_DRV_BUFFER_SIZE							200
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//!@brief List of available status codes
//! 	  which return from functions
//! @{
typedef enum
{
	ESP_PORT_PASS			=		(0),
	ESP_PORT_TIMEOUT_ERR	=		(-1),
	ESP_PORT_SIZE_ERR		=		(-2),
} 	esp_port_status_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* @brief 	This function init hardware unit.
*
* @param	none.
*
* @return 	none.
*/
bool esp_harware_init(void);

/**
* @brief 	Low level function for enable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool esp_hardware_power_on(void);

/**
* @brief 	Low level function for disable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool esp_hardware_power_off(void);

/**
* @brief 	Low level function for transmit data throw UART.
*
* @param[in] data: array of data.
* @param[in] size: size of data.
*
* @return 	none.
*/
bool esp_hardware_transmit_block(const char data[], uint16_t size);

/**
* @brief 	Low level function for receive data throw UART in block mode.
*
* @param[in] data: array of data.
* @param[in] maxsize: maximum size of data.
* @param[in] timeout: timeout in msec. for waiting data.
*
* @return 	none.
*/
int32_t esp_hardware_receive_block(char *msg, size_t len, uint32_t timeout);

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
void esp_hardware_receive_irq(void);
