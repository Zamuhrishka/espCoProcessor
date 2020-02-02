/**********************************************************************************************
* @file    		esp_queue.h
* @author  		Kovalchuk Alexander aka mz4
* @email 		roux@yandex.ru
* @brief		Queue with fixed size.
*
* @warning 		This data structure is not thread-safe!
***********************************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________________________
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
//_____ Ñ O N F I G S__________________________________________________________________________
//! Max size of queue buffer
#define ESP_QUEUE_SIZE                 				(10U)

//! Max size of each of element of queue
#define ESP_PAYLOAD_LENGTH							(200U)
//_____ M A C R O S ___________________________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________________________
/**
* @brief 	Add data into queue.
*
* @param[in] id - id of tcp/udp connection.
* @param[in] buffer - buffer for saving in to queue.
* @param[in] size - size of saving buffer.
*
* @return 	true/false.
*/
bool esp_rbuffer_enqueue(const char buffer[], size_t size);

/**
* @brief 	Add data into queue.
*
* @param[in] id - id of tcp/udp connection.
* @param[out] buffer - buffer for saving in to queue.
* @param[out] size - size of saving buffer.
*
* @return 	true/false.
*/
bool esp_rbuffer_denqueue(char **buffer, size_t *size);

/**
* @brief 	Get data from queue.
*
* @param[in] id - id of tcp/udp connection.
* @param[in] buffer - buffer for saving in to queue.
* @param[in] size - size of saving buffer.
*
* @return 	true/false.
*/
bool esp_tbuffer_enqueue(char id, const char buffer[], size_t size);

/**
* @brief 	Get data from queue.
*
* @param[out] id - id of tcp/udp connection.
* @param[out] buffer - buffer for saving in to queue.
* @param[out] size - size of saving buffer.
*
* @return 	true/false.
*/
bool esp_tbuffer_denqueue(char *id, char **buffer, size_t *size);

