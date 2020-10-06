/********************************************************************************
*
* @file    		esp_utils.h
* @author  		Kovalchuk Alexander (roux@yandex.ru)
* @brief		This file contains the some support function.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "esp_port.h"
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//!@brief List of available status codes
//! 	  which return from functions
//! @{
typedef enum
{
	ESP_HW_PASS			=		(0),
	ESP_HW_TIMEOUT_ERR	=		(-1),
	ESP_HW_SIZE_ERR		=		(-2),
} 	esp_hw_status_t;


//!Prototype of callback function for transmit debug data.
typedef void (*esp_transmit_fn_t)(const char data[], size_t size);
typedef void (*esp_receive_fn_t)(char *msg, size_t* size, uint32_t timeout);
typedef bool (*esp_hardware_init_fn_t)(void);
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* \brief 	This function register memory allocation function.
*
* 			The registration function will be use for allocate memory
* 			for queue.
*
* 			@warning You must register allocation function before call any
* 			function this library.
*
* 			@example queue_reg_mem_alloc_cb(&malloc);
*
* \param[in] custom_malloc: pointer to the memory allocation function.
*
* \return 	none.
*/
void esp_reg_hardware_transmit_cb(esp_transmit_fn_t cb);

/**
* \brief 	This function register memory free function.
*
* 			The registration function will be use for free memory
* 			of queue.
*
* 			@warning You must register free function before call any
* 			function this library.
*
* 			@example queue_reg_mem_free_cb(&free);
*
* \param[in] custom_free: pointer to the memory free function.
*
* \return 	none.
*/
void esp_reg_hardware_receive_cb(esp_receive_fn_t cb);

void esp_reg_hardware_init_cb(esp_hardware_init_fn_t cb);

/**
* @brief 	Low level function for transmit data throw UART.
*
* @param[in] data: array of data.
* @param[in] size: size of data.
*
* @return 	none.
*/
bool esp_msg_transmit(const char data[], size_t size);

/**
* @brief 	Low level function for receive data throw UART in block mode.
*
* @param[in] data: array of data.
* @param[in] maxsize: maximum size of data.
* @param[in] timeout: timeout in msec. for waiting data.
*
* @return 	none.
*/
int8_t esp_msg_receive(char msg[], size_t *size, uint32_t timeout);