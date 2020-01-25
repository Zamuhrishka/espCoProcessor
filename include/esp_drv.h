/*******************************************************************************
* @file    		esp_drv.h
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________________________________
#include "stdbool.h"
#include "stdint.h"
#include "stdlib.h"
//_____ C O N F I G S ________________________________________________________________________________
//_____ M A C R O S __________________________________________________________________________________
//_____ D E F I N I T I O N __________________________________________________________________________
typedef enum
{
	BLOCK,
	UNBLOCK
} 	Blocking_t;

//!@brief List of available status codes
//! 	  which return from functions
//! @{
typedef enum
{
	ESP_PASS 			= 		0,																	///< Function pass return value
	ESP_BUSY			=		1,																	///< Function busy return value
	ESP_TIMEOUT 		= 		2,																	///< Function timeout return value
	ESP_SERVER_ON		= 		3,																	///< Connection to server error
	ESP_TRANSMIT_ERR    =       (-1),																///< An error occurred during the transfer
	ESP_RECEIVE_ERR     =       (-2),																///< An error occurred during the receive
	ESP_PARAM_ERR       =       (-3),																///< Wrong parameters error
	ESP_PATTERN_ERR     =       (-4),																///< Regular expression error
	ESP_INNER_ERR		=		(-5),																///< Some inner error
	ESP_SIZE_ERR		=		(-6),																///< Size error
	ESP_CONNECTION_ERR 	= 		(-7),																///< Connection error
} 	ESPStatus_t;
//! @}

//!@brief List of available events
//! @{
typedef enum
{
	ESP_READY_EVT,
	ESP_ERROR_EVT,
	ESP_WIFI_CONN_EVT,
	ESP_WIFI_GOT_IP_EVT,
	ESP_WIFI_DISCONN_EVT,
	ESP_BUSY_S_EVT,
	ESP_BUSY_P_EVT,
	ESP_STA_CONN_EVT,
	ESP_STA_DISCONN_EVT,
	ESP_STA_IP_EVT,
	ESP_UNDEFINE_EVT
} 	ESP_Events_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S ____________________________________________________
//_____ F U N C T I O N   D E C L A R A T I O N S ____________________________________________________
/**
* @brief 	This function initializate ESP8266 module.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_Init(void);

/**
* @brief 	This function handle of receive data.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_MsgHandlCallBack(void);
