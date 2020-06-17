/*******************************************************************************
* @file    		esp_drv.h
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdbool.h>
//_____ C O N F I G S _________________________________________________________
//! Max size of receive data buffer
#define RX_BUFFER_SIZE							200U
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//!@brief List of available status codes
//! 	  which return from functions
//! @{
typedef enum
{
	ESP_PASS 			= 		0,												///< Function pass return value
	ESP_BUSY			=		1,												///< Function busy return value
	ESP_TIMEOUT 		= 		2,												///< Function timeout return value
	ESP_SERVER_ON		= 		3,												///< Connection to server error
	ESP_TRANSMIT_ERR    =       (-1),											///< An error occurred during the transfer
	ESP_RECEIVE_ERR     =       (-2),											///< An error occurred during the receive
	ESP_PARAM_ERR       =       (-3),											///< Wrong parameters error
	ESP_PATTERN_ERR     =       (-4),											///< Regular expression error
	ESP_INNER_ERR		=		(-5),											///< Some inner error
	ESP_SIZE_ERR		=		(-6),											///< Size error
	ESP_CONNECTION_ERR 	= 		(-7),											///< Connection error
	ESP_MEM_ALLOC_ERR	=		(-8),
	ESP_BUF_SIZE_ERR	=		(-9),
} 	esp_status_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S ____________________________________________________
//_____ F U N C T I O N   D E C L A R A T I O N S ____________________________________________________
/**
* @brief 	This initializations function of esp driver.
*
* @param	none.
*
* @return 	true/false.
*/
bool esp_drv_init(void);

/**
* @brief 	This function is handle of received data from esp chip.
*
* @param	none.
*
* @return 	none.
*/
void esp_drv_receive_handle(void);

/**
* @brief 	This function is handle of transmit data to esp chip.
*
* @param	none.
*
* @return 	none.
*/
void esp_drv_transmit_handle(void);

