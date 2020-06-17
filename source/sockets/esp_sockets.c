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
	esp_conn_state_t 	status;													///< Connection status
	esp_conn_id_t 		lid;													///< Connection ID
	esp_conn_type_t 	type;													///< Connection type
	esp_conn_tetype_t 	mode;													///< Mode (client/server)
	ip4addr_t 			ip;														///< Remote ip
	uint16_t 			remotePort;												///< Remote port
	uint16_t 			localPort;												///< Local port
	bool 				available;												///< Available flag
};
//! @}
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
static esocket_t sockets_pool[ESOCKET_POOL_MAX] = {0};
static esp_conn_mode_t conn_mode = ESP_SINGLE_CONNECT;
static esp_tx_mode_t transmit_mode = ESP_NORMAL_MODE;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   ___________________
static esocket_t* esp_socket_alloc(void)
{
	for(size_t i = 0; i < sizeof(sockets_pool); i++)
	{
		if(sockets_pool[i].available)
		{
			sockets_pool[i].available = false;
			return &sockets_pool[i];
		}
	}

	return NULL;
}

static void esp_socket_free(esocket_t* socket)
{
	socket->available = true;
	socket->ip = 0;
	socket->localPort = 0;
	socket->remotePort = 0;
	socket->status = ESP_NOT_CONNECTED;
	queue_flush(socket->rx_buffer);
}

static bool receice_handle(esp_conn_id_t id, const char data[], size_t size)
{
	queue_t* pQueue = NULL;

	assert(NULL != data);

	pQueue = sockets_pool[(uint8_t)CONVER_TO_NUMBER(id)].rx_buffer;
	if(NULL == pQueue) {
		return false;
	}

	if(size > queue_free_space(pQueue)) {
		return false;
	}

	for(size_t i = 0; i < size; i++)
	{
		queue_enqueue(pQueue, &data[i]);
	}

	return true;
}

static bool connect_handle(esp_conn_id_t id)
{
	esocket_t* socket = NULL;

	socket = &sockets_pool[(uint8_t)CONVER_TO_NUMBER(id)];
	if(NULL == socket) {
		return false;
	}

	socket->status = ESP_CONNECTED;

	return true;
}

static bool disconnect_handle(esp_conn_id_t id)
{
	esocket_t* socket = NULL;

	socket = &sockets_pool[(uint8_t)CONVER_TO_NUMBER(id)];
	if(NULL == socket) {
		return false;
	}

	socket->status = ESP_DISCONNECTED;

	return true;
}

static void transmit_pass_handle(void)
{

}
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This initializations function of socket sub-system.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_init(esp_socket_type_t mode)
{
	if(mode == ESP_SOCKET_CLIENT)
	{
		conn_mode = ESP_SINGLE_CONNECT;

		sockets_pool[0].available = true;
		sockets_pool[0].lid = ESP_ID_NONE;
		sockets_pool[0].rx_buffer = queue_create(ESP_TCP_DATA_LENGTH, sizeof(char));
		if(NULL == sockets_pool[0].rx_buffer) {
			return false;
		}

		if(esp_single_connection_enable(5000u) != ESP_PASS) {
			return false;
		}
	}
	else if(mode == ESP_SOCKET_SERVER)
	{
		conn_mode = ESP_MULTIPLE_CONNECT;

		for(size_t i = 0; i < sizeof(sockets_pool)/sizeof(sockets_pool[0]); i++)
		{
			sockets_pool[i].available = true;
			sockets_pool[i].lid = (esp_conn_id_t)CONVER_TO_CHAR(i);
			sockets_pool[i].rx_buffer = queue_create(ESP_TCP_DATA_LENGTH, sizeof(char));
			if(NULL == sockets_pool[0].rx_buffer) {
				return false;
			}
		}

		if(esp_multiple_connection_enable(5000u) != ESP_PASS) {
			return false;
		}
	}

	transmit_mode = ESP_NORMAL_MODE;

	esp_register_receive_cb(&receice_handle);
	esp_register_close_conn_cb(&disconnect_handle);
	esp_register_open_conn_cb(&connect_handle);
	esp_register_transmit_cb(&transmit_pass_handle);

	return true;

}

/**
* This function create and open new socket.
*
* Public function defined in esp_sockets.h
*/
esocket_t* esp_socket_open(ip4addr_t ip, uint16_t port)
{
	esocket_t* _socket = NULL;
	esp_tcp_cfg_t connParam;
	esp_conn_status_t status;
	esp_status_t res = ESP_INNER_ERR;
	size_t count = 3;

	_socket = esp_socket_alloc();
	if(_socket == NULL) {
		return NULL;
	}

	connParam.remoteIp = ip;
	connParam.remotePort = port;
	connParam.id = _socket->lid;
	connParam.keepAlive = 0;
	queue_flush(_socket->rx_buffer);

	if(esp_tcp_connect(&connParam, 5000u) != ESP_PASS)
	{
		esp_socket_free(_socket);
		return NULL;
	}

	while(res != ESP_PASS && count != 0)
	{
	  res = esp_conn_status_request(&status, 5000u);
	  count--;
	}

	if(res != ESP_PASS || count == 0)
	{
		esp_socket_free(_socket);
		return NULL;
	}

	if(conn_mode == ESP_SINGLE_CONNECT)
	{
		transmit_mode = ESP_TRANSPARENT_MODE;
		_socket->lid = ESP_ID0;

		if(esp_transmit_mode_setup(ESP_TRANSPARENT_MODE, 5000u) != ESP_PASS) {
			return false;
		}
	}

	_socket->ip = ip;
	_socket->remotePort = port;
	_socket->mode = ESP_CLIENT;
	_socket->type = ESP_TCP;
	_socket->localPort = status.localPort;
	_socket->status = status.stat;

	return _socket;
}

/**
* This function close and delete socket.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_close(esocket_t* socket)
{
	assert(NULL != socket);

	if(ESP_NORMAL_MODE == transmit_mode)
	{
		if(esp_close_connection_m(socket->lid, 500) != ESP_PASS) {
			return false;
		}
	}
	else
	{
		if(esp_transparent_mode_disable(500) != ESP_PASS) {
			return false;
		}

		if(esp_close_connection(500) != ESP_PASS) {
			return false;
		}
	}

	esp_socket_free(socket);

	return true;
}

/**
* This function transmit data throw socket.
*
* Public function defined in esp_sockets.h
*/
bool esp_socket_transmit(const esocket_t* socket, const uint8_t data[], size_t size)
{
	assert(NULL != socket);
	assert(NULL != data);

	if(socket->status != ESP_CONNECTED) {
		return false;
	}

	return esp_tcp_send(socket->lid, data, size);
}

/**
* This function receive data from socket.
*
* Public function defined in esp_sockets.h
*/
int32_t esp_socket_receive(const esocket_t* socket, uint8_t data[], size_t size)
{
	int32_t _size = 0;

	assert(NULL != socket);
	assert(NULL != data);

	if(socket->status != ESP_CONNECTED) {
		return false;
	}

	_size = queue_size(socket->rx_buffer);

	if(_size > 0)
	{
		_size = _size < size ? _size : size;
		for(size_t i = 0; i < _size; i++)
		{
			if(!queue_denqueue(socket->rx_buffer, &data[i])) {
				return i;
			}
		}
	}

	return _size;
}
