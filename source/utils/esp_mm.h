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
#ifdef ESP_MM_STATIC_MODE
//! Max size of buffer for answer store
#define ESP_ANSWER_BUFF_SIZE                             200

#if ESP_ANSWER_BUFF_SIZE < ESP_DRV_BUFFER_SIZE
	#warning "Value of ESP_ANSWER_BUFF_SIZE is less then ESP_DRV_BUFFER_SIZE!\
			 This can be source of error. Please increase ESP_ANSWER_BUFF_SIZE."
#endif

//! Max size of buffer for param for AT command store
#define ESP_PARAM_BUFF_SIZE                              200
#endif
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
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
void esp_reg_mem_alloc(void* (*custom_malloc)(size_t sizemem));

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
void esp_reg_mem_free(void (*custom_free)(void * ptrmem));

/**
* @brief 	This function return pointer to answer buffer.
*
* @param	none.
*
* @return 	pointer to answer buffer.
*/
char* esp_alloc_answer_buffer(void);

/**
* @brief 	This function return pointer to param buffer.
*
* @param	none.
*
* @return 	pointer to param buffer.
*/
char* esp_alloc_param_buffer(void);

void esp_free_buffer(void* ptrmem);
