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
//!Pointer to the memory allocation function
static void* (*mem_alloc_fn)(size_t sizemem) = NULL;
//!Pointer to the memory free function
static void (*mem_free_fn) (void *ptrmem) = NULL;

#if defined(ESP_MM_STATIC_MODE)
	void* queue_static_malloc(size_t sizemem);
	void queue_static_free(void * ptrmem);

	mem_alloc_fn = queue_static_malloc;
	mem_free_fn = queue_static_free;

	//!Buffer for receive answer from esp8266
	static char answer[ESP_ANSWER_BUFF_SIZE] = {0};

	//!Buffer for store params for AT commands
	static char param[ESP_PARAM_BUFF_SIZE] = {0};
#endif
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
inline static bool is_callbacks_valid(void)
{
	return ((mem_free_fn != NULL) && (mem_alloc_fn != NULL)) ? true : false;
}
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function used to register function for alloc memory.
*
* Public function defined in queue.h
*/
void esp_reg_mem_alloc(void* (*custom_malloc)(size_t sizemem))
{
	assert(custom_malloc);
#if defined(ESP_MM_STATIC_MODE)
	mem_alloc_fn = queue_static_malloc;
#else
	mem_alloc_fn = custom_malloc;
#endif
}

/**
* This function used to register function for free memory.
*
* Public function defined in queue.h
*/
void esp_reg_mem_free(void (*custom_free)(void * ptrmem))
{
	assert(custom_free);
#if defined(ESP_MM_STATIC_MODE)
	mem_free_fn = queue_static_free;
#else
	mem_free_fn = custom_free;
#endif
}

/**
* This function return pointer to answer buffer.
*
* Public function defined in esp_utils.h
*/
char* esp_alloc_answer_buffer(void)
{
	char* pAnswer = NULL;

#if defined(ESP_MM_STATIC_MODE)
	pAnswer = answer;
#else
	if(is_callbacks_valid())
	{
		pAnswer = mem_alloc_fn(ESP_ANSWER_BUFF_SIZE);
	}

#endif
	if(NULL != pAnswer) {
		memset(pAnswer, 0, ESP_ANSWER_BUFF_SIZE);
	}

	return pAnswer;
}

/**
* This function return pointer to param buffer.
*
* Public function defined in esp_utils.h
*/
char* esp_alloc_param_buffer(void)
{
	char* pParam = NULL;

#if defined(ESP_MM_STATIC_MODE)
	pAnswer = answer;
#else
	if(is_callbacks_valid())
	{
		pAnswer = mem_alloc_fn(ESP_PARAM_BUFF_SIZE);
	}

#endif
	if(NULL != pParam) {
		memset(pParam, 0, ESP_PARAM_BUFF_SIZE);
	}

	return pParam;
}

void esp_free_buffer(void* ptrmem)
{
#if !defined(ESP_MM_STATIC_MODE)
	mem_free_fn(ptrmem);
#endif
}
