// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/**********************************************************************************************
* @file    		esp_queue.c
* @author  		Kovalchuk Alexander aka mz4
* @email 		roux@yandex.ru
* @brief		Queue with fixed size.
*
* @warning 		This data structure is not thread-safe!
***********************************************************************************************/
//_____ I N C L U D E S _______________________________________________________________________
#include "esp_queue.h"

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
//_____ D E F I N I T I O N ___________________________________________________________________
//! @brief Static queue structure
//! @{
typedef struct
{
	size_t size;
	char buffer[ESP_PAYLOAD_LENGTH];
}	esp_rpacket_t;
//! @}

//! @brief Static queue structure
//! @{
typedef struct
{
	esp_rpacket_t data[ESP_QUEUE_SIZE];														//!array of data
    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	esp_rqueue_t;
//! @}


//! @brief Static queue structure
//! @{
typedef struct
{
	size_t size;
	char id;
	char buffer[ESP_PAYLOAD_LENGTH];
}	esp_tpacket_t;
//! @}

//! @brief Static queue structure
//! @{
typedef struct
{
	esp_tpacket_t data[ESP_QUEUE_SIZE];														//!array of data
    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	esp_tqueue_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________________________
//!Receive Queue
static esp_rqueue_t rqueue = {0};

//!Transmit Queue
static esp_tqueue_t tqueue = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
/**
* This function add data into receive queue.
*
* Public function defined in esp_queue.h
*/
bool esp_rbuffer_enqueue(const char buffer[], size_t size)
{
	if(rqueue.size == ESP_QUEUE_SIZE) {
		return false;
	}

	rqueue.data[rqueue.write].size = size;
	if(memcpy(&rqueue.data[rqueue.write].buffer, buffer, size) == NULL) {
		return false;
	}

	rqueue.size++;
	rqueue.write = (rqueue.write == ESP_QUEUE_SIZE - 1ul) ? 0ul: (rqueue.write + 1ul);

	return true;
}

/**
* This function get data from receive queue.
*
* Public function defined in esp_queue.h
*/
bool esp_rbuffer_denqueue(char **buffer, size_t *size)
{
	if(rqueue.size == 0) {
		return false;
	}

	*size = rqueue.data[rqueue.read].size;
	*buffer = rqueue.data[rqueue.read].buffer;

	rqueue.size--;
	rqueue.read = (rqueue.read == ESP_QUEUE_SIZE - 1ul) ? 0ul : (rqueue.read + 1ul);

	return true;
}

/**
* This function add data into transmit queue.
*
* Public function defined in esp_queue.h
*/
bool esp_tbuffer_enqueue(char id, const char buffer[], size_t size)
{
	if(tqueue.size == ESP_QUEUE_SIZE) {
		return false;
	}

	tqueue.data[tqueue.write].size = size;
	tqueue.data[tqueue.write].id = id;
	if(memcpy(&tqueue.data[tqueue.write].buffer, buffer, size) == NULL) {
		return false;
	}

	tqueue.size++;
	tqueue.write = (tqueue.write == ESP_QUEUE_SIZE - 1ul) ? 0ul: (tqueue.write + 1ul);

	return true;
}

/**
* This function get data from transmit queue.
*
* Public function defined in esp_queue.h
*/
bool esp_tbuffer_denqueue(char *id, char **buffer, size_t *size)
{
	if(tqueue.size == 0) {
		return false;
	}

	*size = tqueue.data[tqueue.read].size;
	*id = tqueue.data[tqueue.read].id;
	*buffer = tqueue.data[tqueue.read].buffer;

	tqueue.size--;
	tqueue.read = (tqueue.read == ESP_QUEUE_SIZE - 1ul) ? 0ul : (tqueue.read + 1ul);

	return true;
}
