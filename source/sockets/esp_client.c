// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include "esp_client.h"

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
#include "esp_sockets.h"
//_____ C O N F I G S  _________________________________________________________
//_____ D E F I N I T I O N ____________________________________________________
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   ___________________
static bool receice_handle(esp_conn_id_t id, const char data[], size_t size)
{
	esocket_t* _socket = NULL;

	assert(NULL != data);

	_socket = esp_socket_find_by_id(id);
	if(NULL == _socket) {
		return false;
	}

	if(size > esp_socket_rxbuf_free_space(_socket)) {
		return false;
	}

	for(size_t i = 0; i < size; i++)
	{
		if(!esp_socket_rx_data_enqueue(_socket, (const uint8_t*)&data[i])) {
			return false;
		}
	}

	return true;
}

static bool connect_handle(esp_conn_id_t id)
{
//	esp_socket_set_status(_socket, ESP_CONNECTED);
	return true;
}

static bool disconnect_handle(esp_conn_id_t id)
{
	return true;
}

static void transmit_pass_handle(void)
{

}
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This initializations function of client sub-system.
*
* Public function defined in esp_client.h
*/
bool esp_client_init(void)
{
	if(!esp_socket_init()) {
		return false;
	}

	if(esp_single_connection_enable(500u) != ESP_PASS) {
		return false;
	}

	esp_register_receive_cb(&receice_handle);
	esp_register_close_conn_cb(&disconnect_handle);
	esp_register_open_conn_cb(&connect_handle);
	esp_register_transmit_cb(&transmit_pass_handle);

	return true;
}

/**
* This function open new connection.
*
* Public function defined in esp_client.h
*/
esocket_t* esp_connection_open(ip4addr_t ip, uint16_t port)
{
	esocket_t* _socket = NULL;
	esp_tcp_cfg_t connParam;
	esp_conn_status_t status;
	esp_status_t res = ESP_INNER_ERR;
	size_t count = 3;

	_socket = esp_socket_find_by_id(ESP_ID0);
	if(NULL == _socket) {
		return NULL;
	}

	connParam.remoteIp = ip;
	connParam.remotePort = port;
	connParam.id = ESP_ID_NONE;
	connParam.keepAlive = 0;

	if(esp_tcp_connect(&connParam, 500u) != ESP_PASS) {
		return NULL;
	}

	while(res != ESP_PASS && count != 0)
	{
		res = esp_conn_status_request(&status, 500u);
		count--;
	}

	if(res != ESP_PASS || count == 0) {
		return NULL;
	}

	if(esp_transmit_mode_setup(ESP_TRANSPARENT_MODE, 500u) != ESP_PASS) {
		return NULL;
	}

	esp_socket_set_remote_ip(_socket, ip);
	esp_socket_set_remote_port(_socket, port);
	esp_socket_set_type(_socket, ESP_TCP);
	esp_socket_set_mode(_socket, ESP_CLIENT);
	esp_socket_set_status(_socket, status.stat);

	return _socket;
}

/**
* This function close new connection.
*
* Public function defined in esp_client.h
*/
bool esp_connection_close(esocket_t* socket)
{
	assert(NULL != socket);

	if(esp_transparent_mode_disable(500) != ESP_PASS) {
		return false;
	}

	if(esp_close_connection(500) != ESP_PASS) {
		return false;
	}

	esp_socket_free(socket);

	return true;
}

/**
* This function transmit data throw socket.
*
* Public function defined in esp_client.h
*/
bool esp_client_transmit(const esocket_t* socket, const uint8_t data[], size_t size)
{
	assert(NULL != socket);
	assert(NULL != data);

	if(esp_socket_get_status(socket) != ESP_CONNECTED) {
		return false;
	}

	return esp_tcp_send(esp_socket_get_connection_id(socket), data, size);
}

/**
* This function receive data from socket.
*
* Public function defined in esp_client.h
*/
int32_t esp_client_receive(esocket_t* socket, uint8_t data[], size_t size)
{
	int32_t _size = 0;

	assert(NULL != socket);
	assert(NULL != data);

	if(esp_socket_get_status(socket) != ESP_CONNECTED) {
		return false;
	}

	_size = esp_socket_get_rx_data_num(socket);

	if(_size > 0)
	{
		_size = _size < size ? _size : size;
		for(size_t i = 0; i < _size; i++)
		{
			if(!esp_socket_rx_data_denqueue(socket, &data[i])) {
				return i;
			}
		}
	}

	return _size;
}
