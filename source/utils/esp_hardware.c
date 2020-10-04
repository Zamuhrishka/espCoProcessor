// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_utils.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		This file contains the some support function.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "esp_mm.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "esp_port.h"
#include "convert.h"
#if defined(ESP_PORT_DEBUG)
#include "debug.h"
#endif
//_____ C O N F I G ___________________________________________________________
#if defined(ESP_MM_DEBUG)
	#define ESP_MM_DEBUG_PARAM(fmt, ...)	       	debug_info(fmt, __VA_ARGS__)
	#define ESP_MM_DEBUG(fmt)	                  	debug_info(fmt)
	#define ESP_MM_ERROR_PARAM(fmt, ...)	       	debug_error(fmt, __VA_ARGS__)
	#define ESP_MM_ERROR(fmt)	                  	debug_error(fmt)
#else
	#define ESP_MM_DEBUG_PARAM(fmt, ...)
	#define ESP_MM_DEBUG(fmt, ...)
	#define ESP_MM_ERROR_PARAM(fmt, ...)
	#define ESP_MM_ERROR(fmt)
#endif
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!Pointer of callback function for for transmit debug data.
static esp_transmit_fn_t esp_msg_transmit = NULL;
//!Pointer of callback function for for transmit debug data.
static esp_receive_fn_t esp_msg_receive = NULL;
//!Pointer of callback function for for transmit debug data.
static esp_hardware_init_fn_t esp_hw_init = NULL;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
void esp_reg_transmit_cb(esp_transmit_fn_t cb)
{
	assert(cb != NULL);

	esp_msg_transmit = cb;
}

void esp_reg_receive_cb(esp_receive_fn_t cb)
{
	assert(cb != NULL);

	esp_msg_receive = cb;
}

void esp_reg_harsware_init_cb(esp_hardware_init_fn_t cb)
{
	assert(cb != NULL);

	esp_hw_init = cb;
}

bool esp_is_hw_cb_valid(void)
{
	return (esp_msg_transmit != NULL &&
			esp_msg_receive != NULL &&
			esp_hw_init != NULL);
}
