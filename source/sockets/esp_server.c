// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_server.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include "esp_server.h"

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

#if defined(ESP_SERVER_DEBUG)
#include "debug.h"
#endif
//_____ C O N F I G S  _________________________________________________________
//ESP_DRV_BUFFER_SIZE

#if defined(ESP_SERVER_DEBUG)
	#define ESP_DEBUG_PARAM(fmt, ...)       debug_info(fmt, __VA_ARGS__)
	#define ESP_DEBUG(fmt)                  debug_info(fmt)
	#define ESP_ERROR_PARAM(fmt, ...)       debug_error(fmt, __VA_ARGS__)
	#define ESP_ERROR(fmt)                  debug_error(fmt)
#else
	#define ESP_DEBUG_PARAM(fmt, ...)
	#define ESP_DEBUG(fmt, ...)
	#define ESP_ERROR_PARAM(fmt, ...)
	#define ESP_ERROR(fmt)
#endif
//_____ D E F I N I T I O N ____________________________________________________
struct ticket_t
{
	esp_conn_id_t id;
	size_t size;
};
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
static esp_server_receive_fn_t esp_receive_data_cb = NULL;
static uint16_t server_port = 333;
static struct ticket_t current_ticket;
static queue_t* tickets;
static enum _state
{
	GET_NEXT_TICKET = 0,
	HANDLE_TICKET,
	WAITTING,
	POST_HANDLE_TICKET
}	state = GET_NEXT_TICKET;

static uint8_t transmit_buffer[ESP_TCP_DATA_LENGTH] = {0};
static esp_conn_id_t current_tx_socket_id = ESP_ID0;
static volatile bool transmit_allow = true;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   ___________________
static bool receive_prepare_cb(esp_conn_id_t id, size_t size)
{
	struct ticket_t ticket;

	if(id > ESP_ID_NONE || id < ESP_ID0)
	{
		ESP_ERROR_PARAM("[ESP_SERVER]: invalid ID - %d!", CONVER_TO_NUMBER(id));
		return false;
	}

	ESP_DEBUG_PARAM("[ESP_SERVER]: Rx data ready len = %d, id = %d\r\n", size, CONVER_TO_NUMBER(id));

	ticket.id = id;
	ticket.size = size;

	if(!queue_is_full(tickets))
	{
		if(!queue_enqueue(tickets, &ticket))
		{
			//TODO: Error handler
			ESP_ERROR("[ESP_SERVER]: Tickets queue if full!");
		}
	}

	return true;
}

static bool receive_data_cb(const char data[], size_t size)
{
	esocket_t* socket = NULL;

	assert(NULL != data);

	ESP_DEBUG_PARAM("[ESP_SERVER]: Rx data received len = %d, id = %d\r\n", size, CONVER_TO_NUMBER(current_ticket.id));

	socket = esp_socket_find_by_id(current_ticket.id);
	if(NULL == socket)
	{
		ESP_ERROR("[ESP_SERVER]: Socket searching error!");
		state = GET_NEXT_TICKET;
		return false;
	}

#if 0
	if(size > esp_socket_rxbuf_free_space(socket))
	{
		state = GET_NEXT;
		return false;
	}
#endif

	state = POST_HANDLE_TICKET;

	for(size_t i = 0; i < size; i++)
	{
		if(!esp_socket_rx_data_enqueue(socket, (uint8_t*)&data[i]))
		{
			ESP_ERROR("[ESP_SERVER]: Enqueue data error!");
			return false;
		}

		current_ticket.size--;
	}

	return true;
}

static bool connect_cb(esp_conn_id_t id)
{
	esocket_t* socket = NULL;

	ESP_DEBUG("[ESP_SERVER]: Connect to ...");

	socket = esp_socket_find_by_id(id);
	if(NULL != socket)
	{
		ESP_DEBUG_PARAM("%d\r\n", CONVER_TO_NUMBER(id));
		esp_socket_set_status(socket, ESP_CONNECTED);
	}

	return true;
}

static bool disconnect_cb(esp_conn_id_t id)
{
	esocket_t* socket = NULL;

	ESP_DEBUG("[ESP_SERVER]: Disconnect to ...");

	socket = esp_socket_find_by_id(id);
	if(NULL != socket)
	{
		ESP_DEBUG_PARAM("%d\r\n", CONVER_TO_NUMBER(id));
		esp_socket_set_status(socket, ESP_DISCONNECTED);
	}

	return true;
}

static void transmit_pass_cb(void)
{
	//TODO: Allow transmit flag
	ESP_DEBUG("[ESP_SERVER]: Transmit done.\r\n");
	transmit_allow = true;
}



//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This function register callback function for server receive.
*
* Public function defined in esp_server.h
*/
void esp_register_server_receive_cb(const esp_server_receive_fn_t cb)
{
	if(NULL != cb)
	{
		esp_receive_data_cb = cb;
	}
}

/**
* This initialization function of socket sub-system.
*
* Public function defined in esp_server.h
*/
bool esp_server_init(uint16_t port, uint8_t maxconn, uint16_t timeout)
{
	esp_socket_init();

	tickets = queue_create(ESP_TCP_DATA_LENGTH, sizeof(struct ticket_t));
	if(NULL == tickets) {
		return false;
	}

	if(esp_multiple_connection_enable(500u) != ESP_PASS) {
		return false;
	}

#if 0
	if(esp_tcp_server_maxconn_setup(maxconn, 5000) != ESP_PASS) {
		return false;
	}

	if(esp_tcp_server_timeout_setup(timeout, 5000) != ESP_PASS) {
		return false;
	}
#endif

	if(esp_tcp_receive_mode_setup(ESP_PASSIVE_RX_MODE, 5000) != ESP_PASS) {
		return false;
	}

	if(esp_tcp_server_create(port, 5000) != ESP_PASS) {
		return false;
	}

	esp_register_received_data_num_cb(&receive_prepare_cb);
	esp_register_passive_receive_cb(&receive_data_cb);
	esp_register_close_conn_cb(&disconnect_cb);
	esp_register_open_conn_cb(&connect_cb);
	esp_register_transmit_cb(&transmit_pass_cb);

	server_port = port;

	return true;
}

/**
* This deinitialization function of socket sub-system.
*
* Public function defined in esp_server.h
*/
bool esp_server_deinit(void)
{
	if(esp_tcp_server_delete(server_port, 1000) != ESP_PASS) {
		return false;
	}

	return true;
}

/**
* This function is tcp/ip server.
*
* Public function defined in esp_server.h
*/
void esp_server_receive_handle(void)
{
	esocket_t* socket = NULL;
	switch(state)
	{
		case GET_NEXT_TICKET:
			if(!queue_is_empty(tickets))
			{
				if(queue_denqueue(tickets, (struct ticket_t*) &current_ticket))
				{
					ESP_DEBUG_PARAM("[ESP_SERVER]: New ticket %d, %d\r\n",
							CONVER_TO_NUMBER(current_ticket.id), current_ticket.size);
					state = HANDLE_TICKET;
				}
			}
			break;
		case HANDLE_TICKET:
			socket = esp_socket_find_by_id(current_ticket.id);
			if(NULL != socket)
			{
				size_t free_size = esp_socket_rxbuf_free_space(socket);
				if(0 != free_size)
				{
					size_t req_size = (free_size >= current_ticket.size) ? current_ticket.size : free_size;

					ESP_DEBUG_PARAM("[ESP_SERVER]: Send request %d, bytes\r\n", req_size);

					esp_tcp_receive_passive_data(current_ticket.id, req_size, NULL, 0);
					state = WAITTING;
				}
			}
			break;
		case WAITTING:
			break;
		case POST_HANDLE_TICKET:
			if(0 == current_ticket.size)
			{
				socket = esp_socket_find_by_id(current_ticket.id);
				if(esp_receive_data_cb != NULL) {
					esp_receive_data_cb(socket);
				}

				ESP_DEBUG("[ESP_SERVER]: POST_HANDLE_TICKET -> GET_NEXT_TICKET\r\n");
				state = GET_NEXT_TICKET;
			}
			else
			{
				ESP_DEBUG("[ESP_SERVER]: POST_HANDLE_TICKET -> HANDLE_TICKET\r\n");
				state = HANDLE_TICKET;
			}
			break;
		default:
			ESP_ERROR("[ESP_SERVER]: Unexpected state!");
			state = GET_NEXT_TICKET;
	}
}

/**
* This function transmit data to client.
*
* Public function defined in esp_server.h
*/
bool esp_server_transmit(esocket_t* socket, const uint8_t data[], size_t size)
{
	assert(NULL != socket);
	assert(NULL != data);

	if(esp_socket_get_status(socket) != ESP_CONNECTED) {
		return false;
	}

	size_t _free_size = esp_socket_txbuf_free_space(socket);
	size_t _size = _free_size < size ? _free_size : size;
	for(size_t i = 0; i < _size; i++)
	{
		if(!esp_socket_txbuf_enqueue(socket, &data[i])) {
			return i;
		}
	}

	return true;
}

int32_t esp_server_transmit_handle(void)
{
	esocket_t* socket = NULL;

	if(transmit_allow)
	{
		if(current_tx_socket_id > ESP_ID4)
		{
			current_tx_socket_id = ESP_ID0;
		}

		socket = esp_socket_find_by_id(current_tx_socket_id);
		if(NULL != socket)
		{
			if(esp_socket_get_status(socket) == ESP_CONNECTED)
			{
				size_t size = esp_socket_get_tx_data_num(socket);
				if(size != 0)
				{
					for(size_t i = 0; i < size; i++)
					{
						uint8_t data = 0;
						if(esp_socket_tx_data_denqueue(socket, &data))
						{
							transmit_buffer[i] = data;
						}
					}

					if(esp_tcp_send(current_tx_socket_id, transmit_buffer, size))
					{
						transmit_allow = false;
					}

					return size;
				}
			}
		}

		current_tx_socket_id++;
	}

	return 0;

}
