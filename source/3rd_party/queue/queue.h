/**
* \file    		queue.h
* \author  		Kovalchuk Alexander (roux@yandex.ru)
* \brief		Queue with fixed size.
*
* \warning 		This data structure is not thread-safe!
*/

/*
 * Copyright (c) year Alexander KOVALCHUK
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without restriction,
 * including without limitation the rights to use, copy, modify, merge,
 * publish, distribute, sublicense, and/or sell copies of the Software,
 * and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 * OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 * HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * This file is part of fwContainers library.
 *
 * Author: Alexander KOVALCHUK <roux@yandex.ru>
 */

#pragma once
//_____ I N C L U D E S _______________________________________________________________________
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//_____ C O N F I G S__________________________________________________________________________
#ifdef QUEUE_STATIC_MODE
#define MAX_QUEUES_IN_POOL                              32U
#define QUEUE_SIZE_IN_BYTES        						32U
#endif
//_____ M A C R O S ___________________________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________________________
struct Queue_t;
typedef struct Queue_t queue_t;
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________________________
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
void queue_reg_mem_alloc_cb(void* (*custom_malloc)(size_t sizemem));

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
void queue_reg_mem_free_cb(void (*custom_free)(void * ptrmem));

/**
* \brief 	Create new queue.
*
* \param[in] capacity: size of queue.
* \param[in] nodeSize: size of element of queue.
*
* \return 	pointer to new queue or NULL.
*/
queue_t* queue_create(size_t capacity, size_t esize);

/**
* \brief 	Delete queue.
*
* \param[in] queue: pointer to queue.
*
* \return 	none.
*/
void queue_delete(queue_t **queue);

/**
* \brief 	This function check the queue for empty.
*
* \param[in] queue: pointer to queue.
*
* \return 	true/false.
*/
bool queue_is_empty(const queue_t *queue);

/**
* \brief 	This function check the queue for full.
*
* \param[in] queue: pointer to queue.
*
* \return 	true/false.
*/
bool queue_is_full(const queue_t *queue);

/**
* \brief 	This function return size of queue.
*
* \param[in] queue: pointer to queue.
*
* \return 	true/false.
*/
size_t queue_size(const queue_t *queue);

/**
* \brief 	This function return free size of queue.
*
* \param[in] queue: pointer to queue.
*
* \return 	true/false.
*/
size_t queue_free_space(const queue_t *queue);

/**
* \brief 	Add data into queue.
*
* \param[in] queue: pointer to queue.
* \param[in] data: pointer to data for saving in to queue.
*
* \return 	true/false.
*/
bool queue_enqueue(queue_t *queue, const void *data);

/**
* \brief 	Get data from queue.
*
* \param[in] queue: pointer to queue.
* \param[out] data: pointer to retrieve data from queue.
*
* \return 	true/false.
*/
bool queue_denqueue(queue_t *queue, void *data);

/**
* \brief 	This function used to get data from queue without deleting.
*
* \param[in] queue: pointer to queue.
* \param[out] data: pointer to retrieve data from queue.
*
* \return 	true/false.
*/
bool queue_peek(const queue_t *queue, void *data);

/**
* \brief 	This function search data in the queue.
*
* \param[in] queue: pointer to queue.
* \param[in] data: pointer to retrieve data from queue.
* \param[in] is_equal: callback function for compare data.
*
* \return 	true - if data find;
* 			false - if data not find.
*/
bool queue_find(const queue_t *queue, const void *data, bool (*is_equal)(const void*, const void*));

/**
* \brief 	This function used to reset queue.
*
* \param[in] queue: pointer to queue.
*
* \return 	true/false.
*/
void queue_flush(queue_t *queue);
