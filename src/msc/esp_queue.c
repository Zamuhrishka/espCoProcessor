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
}	ESP_RxPacket_t;
//! @}

//! @brief Static queue structure
//! @{
typedef struct
{
	ESP_RxPacket_t data[ESP_QUEUE_SIZE];														//!array of data
    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	ESP_RxQueue_t;
//! @}


//! @brief Static queue structure
//! @{
typedef struct
{
	size_t size;
	char id;
	char buffer[ESP_PAYLOAD_LENGTH];
}	ESP_TxPacket_t;
//! @}

//! @brief Static queue structure
//! @{
typedef struct
{
	ESP_TxPacket_t data[ESP_QUEUE_SIZE];														//!array of data
    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	ESP_TxQueue_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________________________
//!Receive Queue
static ESP_RxQueue_t RxBuffer = {0};

//!Transmit Queue
static ESP_TxQueue_t TxBuffer = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
/**
* This function add data into receive queue.
*
* Public function defined in esp_queue.h
*/
bool ESP_RxQueueEnqueue(const char buffer[], size_t size)
{
	if(RxBuffer.size == ESP_QUEUE_SIZE) {
		return false;
	}

	RxBuffer.data[RxBuffer.write].size = size;
	if(memcpy(&RxBuffer.data[RxBuffer.write].buffer, buffer, size) == NULL) {
		return false;
	}

	RxBuffer.size++;
	RxBuffer.write = (RxBuffer.write == ESP_QUEUE_SIZE - 1ul) ? 0ul: (RxBuffer.write + 1ul);

	return true;
}

/**
* This function get data from receive queue.
*
* Public function defined in esp_queue.h
*/
bool ESP_RxQueueDenqueue(char **buffer, size_t *size)
{
	if(RxBuffer.size == 0) {
		return false;
	}

	*size = RxBuffer.data[RxBuffer.read].size;
	*buffer = RxBuffer.data[RxBuffer.read].buffer;

	RxBuffer.size--;
	RxBuffer.read = (RxBuffer.read == ESP_QUEUE_SIZE - 1ul) ? 0ul : (RxBuffer.read + 1ul);

	return true;
}

/**
* This function add data into transmit queue.
*
* Public function defined in esp_queue.h
*/
bool ESP_TxQueueEnqueue(char id, const char buffer[], size_t size)
{
	if(TxBuffer.size == ESP_QUEUE_SIZE) {
		return false;
	}

	TxBuffer.data[TxBuffer.write].size = size;
	TxBuffer.data[TxBuffer.write].id = id;
	if(memcpy(&TxBuffer.data[TxBuffer.write].buffer, buffer, size) == NULL) {
		return false;
	}

	TxBuffer.size++;
	TxBuffer.write = (TxBuffer.write == ESP_QUEUE_SIZE - 1ul) ? 0ul: (TxBuffer.write + 1ul);

	return true;
}

/**
* This function get data from transmit queue.
*
* Public function defined in esp_queue.h
*/
bool ESP_TxQueueDenqueue(char *id, char **buffer, size_t *size)
{
	if(TxBuffer.size == 0) {
		return false;
	}

	*size = TxBuffer.data[TxBuffer.read].size;
	*id = TxBuffer.data[TxBuffer.read].id;
	*buffer = TxBuffer.data[TxBuffer.read].buffer;

	TxBuffer.size--;
	TxBuffer.read = (TxBuffer.read == ESP_QUEUE_SIZE - 1ul) ? 0ul : (TxBuffer.read + 1ul);

	return true;
}
