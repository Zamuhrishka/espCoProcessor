// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include "esp_sockets.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "stack/esp_tcpip.h"
#include "queue.h"
#include "esp_port.h"
//_____ C O N F I G S  _________________________________________________________
//_____ D E F I N I T I O N ____________________________________________________
//!@brief esp socket structure
//! @{
struct esp_socket_t
{
	queue_t*			rx_buffer;												///< Receive data buffer
	queue_t*			tx_buffer;												///< Transmit data buffer
	esp_conn_state_t 	status;													///< Connection status
	esp_conn_id_t 		lid;													///< Connection ID
	esp_conn_type_t 	type;													///< Connection type
	esp_conn_tetype_t 	mode;													///< Mode (client/server)
	ip4addr_t 			ip;														///< Remote ip
	uint16_t 			remotePort;												///< Remote port
	uint16_t 			localPort;												///< Local port
	bool 				available;												///< Available flag
	size_t 				nsize;
};
//! @}
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
static esocket_t sockets_pool[ESP_MAX_CONNECTIONS] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   ___________________
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This initializations function of sockets pool.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_init(void)
{
	for(size_t i = 0; i < sizeof(sockets_pool)/sizeof(sockets_pool[0]); i++)
	{
		sockets_pool[i].rx_buffer = queue_create(ESP_TCP_DATA_LENGTH, sizeof(char));
		if(NULL == sockets_pool[0].rx_buffer) {
			return false;
		}

		sockets_pool[i].tx_buffer = queue_create(ESP_TCP_DATA_LENGTH, sizeof(char));
		if(NULL == sockets_pool[0].tx_buffer) {
			return false;
		}

		sockets_pool[i].available = true;
		sockets_pool[i].ip = 0;
		sockets_pool[i].localPort = 0;
		sockets_pool[i].mode = ESP_CLIENT;
		sockets_pool[i].remotePort = 0;
		sockets_pool[i].status = ESP_NOT_CONNECTED;
		sockets_pool[i].type = ESP_TCP;
		sockets_pool[i].lid = (esp_conn_id_t)CONVER_TO_CHAR(i);
	}

	return true;
}

/* This function configure socket as default.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_default(esocket_t* socket)
{
	socket->available = true;
	socket->ip = 0;
	socket->localPort = 0;
	socket->remotePort = 0;
	socket->status = ESP_NOT_CONNECTED;
	queue_flush(socket->rx_buffer);
}

/* This initializations function find socket by connection id.
*
* Public function defined in esp_sockets.h
*/
esocket_t* esp_socket_find_by_id(esp_conn_id_t id)
{
	uint8_t _id = (uint8_t)CONVER_TO_NUMBER(id);

	if(_id > ESP_MAX_CONNECTIONS) {
		return NULL;
	}

	return &sockets_pool[_id];
}

/* This function return connection id.
*
* Public function defined in esp_sockets.h
*/
esp_conn_id_t esp_socket_get_connection_id(const esocket_t* socket)
{
	return socket->lid;
}

/* This function set connection id.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_connection_id(esocket_t* socket, esp_conn_id_t id)
{
	socket->lid = id;
}

/* This function return connection status.
*
* Public function defined in esp_sockets.h
*/
esp_conn_state_t esp_socket_get_status(const esocket_t* socket)
{
	return socket->status;
}

/* This function set connection status.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_status(esocket_t* socket, esp_conn_state_t status)
{
	socket->status = status;
}

/* This function return connection type.
*
* Public function defined in esp_sockets.h
*/
esp_conn_type_t esp_socket_get_type(const esocket_t* socket)
{
	return socket->type;
}

/* This function set connection type.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_type(esocket_t* socket, esp_conn_type_t type)
{
	socket->type = type;
}

/* This function return connection mode.
*
* Public function defined in esp_sockets.h
*/
esp_conn_tetype_t esp_socket_get_mode(const esocket_t* socket)
{
	return socket->mode;
}

/* This function set connection mode.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_mode(esocket_t* socket, esp_conn_tetype_t mode)
{
	socket->mode = mode;
}

/* This function return connection ip.
*
* Public function defined in esp_sockets.h
*/
ip4addr_t esp_socket_get_remote_ip(const esocket_t* socket)
{
	return socket->ip;
}

/* This function set connection ip.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_remote_ip(esocket_t* socket, ip4addr_t ip)
{
	socket->ip = ip;
}

/* This function return connection remote port.
*
* Public function defined in esp_sockets.h
*/
uint16_t esp_socket_get_remote_port(const esocket_t* socket)
{
	return socket->remotePort;
}

/* This function set connection remote port.
*
* Public function defined in esp_sockets.h
*/
void esp_socket_set_remote_port(esocket_t* socket, uint16_t port)
{
	socket->remotePort = port;
}




size_t esp_socket_get_remote_size(const esocket_t* socket)
{
	return socket->nsize;
}

void esp_socket_set_remote_size(esocket_t* socket, size_t size)
{
	socket->nsize = size;
}



/* This function return count of received data.
*
* Public function defined in esp_sockets.h
*/
size_t esp_socket_get_rx_data_num(const esocket_t* socket)
{
	return queue_size(socket->rx_buffer);
}

/* This function return free space in socket rx buffer.
*
* Public function defined in esp_sockets.h
*/
size_t esp_socket_rxbuf_free_space(const esocket_t* socket)
{
	return queue_free_space(socket->rx_buffer);
}

/* This function add new data to receive buffer.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_rx_data_enqueue(esocket_t* socket, const uint8_t* data)
{
	return queue_enqueue(socket->rx_buffer, data);
}

/* This function return data from receive buffer.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_rx_data_denqueue(esocket_t* socket, uint8_t* data)
{
	return queue_denqueue(socket->rx_buffer, data);
}



/* This function return count of received data.
*
* Public function defined in esp_sockets.h
*/
size_t esp_socket_get_tx_data_num(const esocket_t* socket)
{
	return queue_size(socket->tx_buffer);
}


/* This function return free space in socket tx buffer.
*
* Public function defined in esp_sockets.h
*/
size_t esp_socket_txbuf_free_space(const esocket_t* socket)
{
	return queue_free_space(socket->tx_buffer);
}


/* This function add new data to transmit buffer.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_txbuf_enqueue(esocket_t* socket, const uint8_t* data)
{
	return queue_enqueue(socket->tx_buffer, data);
}

/* This function return data from receive buffer.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_tx_data_denqueue(esocket_t* socket, uint8_t* data)
{
	return queue_denqueue(socket->tx_buffer, data);
}
