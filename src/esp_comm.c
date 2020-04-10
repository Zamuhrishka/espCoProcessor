// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include "esp_comm.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "stack/esp_tcpip.h"
#include "esp_port.h"
#include "debug.h"
//_____ C O N F I G S  ________________________________________________________
// If unit testing is enabled override assert with mock_assert().
#if defined(UNIT_TESTING)
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif // UNIT_TESTING
//_____ D E F I N I T I O N ____________________________________________________
#define ESOCKET_POOL_MAX						5
#define ESP_TCP_DATA_LENGTH						50

struct esp_socket_t
{
	esp_conn_state_t 	status;
	esp_conn_id_t 		lid;
	esp_conn_type_t 	type;
	esp_conn_tetype_t 	tetype;
	ip4addr_t 			ip;
	uint16_t 			remotePort;
	uint16_t 			localPort;
	bool 				available;
};

//! @brief Static queue structure
//! @{
typedef struct
{
	uint8_t 			buffer[ESP_TCP_DATA_LENGTH];
    size_t 				size;													//!count of store data
    size_t 				write;													//!pointer to the write position
    size_t 				read;													//!pointer to the read position
} 	esp_tcp_queue_t;
//! @}
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
static esocket_t sockets_pool[ESOCKET_POOL_MAX] = {0};

static esp_conn_mode_t conn_mode = ESP_MULTIPLE_CONNECT;
static esp_tx_mode_t transmit_mode = ESP_NORMAL_MODE;

//!Transmit Queue
static esp_tcp_queue_t queue[ESOCKET_POOL_MAX] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   ___________________
/**
* This function used to check the queue for empty.
*
* Public function defined in queue.h
*/
bool esp_tcp_buffer_is_empty(const esp_tcp_queue_t *queue)
{
	assert(NULL != queue);

	if(queue->size == 0) {
		return true;
	}

	return false;
}

/**
* This function used to check the queue for full.
*
* Public function defined in queue.h
*/
bool esp_tcp_buffer_is_full(const esp_tcp_queue_t *queue)
{
	assert(NULL != queue);

	if(queue->size == ESP_TCP_DATA_LENGTH) {
		return true;
	}

	return false;
}

/**
* This function return size of queue.
*
* Public function defined in queue.h
*/
size_t esp_tcp_buffer_size(const esp_tcp_queue_t *queue)
{
	assert(NULL != queue);
	return queue->size;
}

/**
* This function return free size of queue.
*
* Public function defined in queue.h
*/
size_t esp_tcp_buffer_free_space(const esp_tcp_queue_t *queue)
{
	assert(NULL != queue);
	return ESP_TCP_DATA_LENGTH - queue->size;
}

/**
* This function add data into queue.
*
* Public function defined in queue.h
*/
bool esp_tcp_buffer_enqueue(esp_tcp_queue_t *queue, uint8_t data)
{
	assert(NULL != queue);

	if(esp_tcp_buffer_is_full(queue)) {
		return false;
	}

	queue->buffer[queue->write] = data;
	queue->write = (queue->write == ESP_TCP_DATA_LENGTH - 1ul) ? 0ul: (queue->write + 1ul);
	queue->size++;

	return true;
}

/**
* This function used to get data from queue.
*
* Public function defined in queue.h
*/
bool esp_tcp_buffer_denqueue(esp_tcp_queue_t *queue, uint8_t *data)
{
	assert(NULL != queue);
	assert(NULL != data);

	if(esp_tcp_buffer_is_empty(queue)) {
		return false;
	}

	*data = queue->buffer[queue->read];
	queue->read = (queue->read == ESP_TCP_DATA_LENGTH - 1ul) ? 0ul : (queue->read + 1ul);
	queue->size--;

	return true;
}

static bool receice_handlle(esp_conn_id_t id, const char data[], size_t size)
{
	if(data == NULL || size == 0)
	{
		debug_error("[TCP]: Received from id: %c...FAULT\r\n", id);
		return false;
	}

	debug_info("[TCP]: Received from id: %c\r\n", id);
	debug_info("\r\n");
	debug_dump((const void*)data, (unsigned long)&data[0], size, 1);
	debug_info("\r\n");
	debug_info("\r\n");

	for(size_t i = 0; i < size; i++)
	{
		esp_tcp_buffer_enqueue(&queue[(uint8_t)CONVER_TO_NUMBER(id)], data[i]);
	}

	return true;
}

static bool disconnect_handlle(esp_conn_id_t id)
{
	debug_info("[TCP]: Connection %c close\r\n", id);
	return true;
}

static bool connect_handlle(esp_conn_id_t id)
{
	debug_info("[TCP]: Connection %c open\r\n", id);
	return true;
}

static void transmit_pass_handle(void)
{
	debug_info("[TCP]: Transmit Pass\r\n");
}

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
}
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
bool esp_comm_init(esp_conn_mode_t mode)
{
	conn_mode = mode;

	if(mode == ESP_SINGLE_CONNECT)
	{
		sockets_pool[0].available = true;
		sockets_pool[0].lid = ESP_ID_NONE;

		if(esp_single_connection_enable(5000u) != ESP_PASS) {
			return false;
		}
	}
	else if(mode == ESP_MULTIPLE_CONNECT)
	{
		for(size_t i = 0; i < sizeof(sockets_pool)/sizeof(sockets_pool[0]); i++)
		{
			sockets_pool[i].available = true;
			sockets_pool[i].lid = (esp_conn_id_t)CONVER_TO_CHAR(i);
		}

		if(esp_multiple_connection_enable(5000u) != ESP_PASS) {
			return false;
		}
	}

	transmit_mode = ESP_NORMAL_MODE;

	esp_register_receive_cb(&receice_handlle);
	esp_register_close_conn_cb(&disconnect_handlle);
	esp_register_open_conn_cb(&connect_handlle);
	esp_register_transmit_cb(&transmit_pass_handle);

	return true;

}

esocket_t* esp_tcp_open(ip4addr_t ip, uint16_t port)
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

		if(esp_transmit_mode_setup(ESP_TRANSPARENT_MODE, 5000u) != ESP_PASS) {
			return false;
		}
	}

	_socket->ip = ip;
	_socket->remotePort = port;
	_socket->tetype = ESP_CLIENT;
	_socket->type = ESP_TCP;
//	_socket->lid = status.id;
	_socket->localPort = status.localPort;
	_socket->status = status.stat;

	return _socket;
}

bool esp_tcp_close(esocket_t* socket)
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

esocket_t* esp_tcp_server_open(uint16_t port, uint8_t maxconn, uint16_t timeout)
{
	esocket_t* _socket = NULL;

	_socket = esp_socket_alloc();
	if(_socket == NULL) {
		return NULL;
	}

	if(conn_mode != ESP_MULTIPLE_CONNECT)
	{
		if(esp_multiple_connection_enable(500u) != ESP_PASS)
		{
			esp_socket_free(_socket);
			return NULL;
		}
	}

	if(esp_tcp_server_maxconn_setup(maxconn, 500) != ESP_PASS)
	{
		esp_socket_free(_socket);
		return NULL;
	}


	if(esp_tcp_server_timeout_setup(timeout, 500) != ESP_PASS)
	{
		esp_socket_free(_socket);
		return NULL;
	}

	if(esp_tcp_server_create(port, 1000) != ESP_PASS)
	{
		esp_socket_free(_socket);
		return NULL;
	}

	_socket->localPort = port;
	_socket->ip = 0;
	_socket->remotePort = 0;
	_socket->tetype = ESP_SERVER;
	_socket->type = ESP_TCP;
	_socket->status = ESP_NOT_CONNECTED;

	return _socket;
}

bool esp_server_close(esocket_t* socket)
{
	assert(NULL != socket);

	if(esp_tcp_server_delete(socket->localPort, 1000) != ESP_PASS) {
		return false;
	}

	esp_socket_free(socket);

	return true;
}

bool esp_tcp_transmit(const esocket_t* socket, const uint8_t data[], size_t size)
{
	assert(NULL != socket);

	return esp_tcp_send(socket->lid, data, size);
}

bool esp_tcp_is_data_ready(esocket_t* socket)
{
	assert(NULL != socket);

	return !esp_tcp_buffer_is_empty(&queue[(uint8_t)CONVER_TO_NUMBER(socket->lid)]);
}


size_t esp_tcp_get_receive_data_number(esocket_t* socket)
{
	assert(NULL != socket);

	return esp_tcp_buffer_size(&queue[(uint8_t)CONVER_TO_NUMBER(socket->lid)]);
}


bool esp_tcp_receive(esocket_t* socket, uint8_t data[], size_t size)
{
	assert(NULL != socket);

	if(data == NULL) {
		return false;
	}

	for(size_t i = 0; i < size; i++)
	{
		esp_tcp_buffer_denqueue(&queue[(uint8_t)CONVER_TO_NUMBER(socket->lid)], &data[i]);
	}

	return true;
}

