/*******************************************************************************
* @file    		esp_drv.h
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
#pragma once
//_____ I N C L U D E S ________________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "stack/esp_tcpip.h"
#include "queue.h"
//_____ C O N F I G S __________________________________________________________
//! Max size of receive data buffer in each socket
#define ESP_TCP_DATA_LENGTH						50
//_____ M A C R O S ____________________________________________________________
//_____ D E F I N I T I O N ____________________________________________________
struct esocket_t;
typedef struct esp_socket_t esocket_t;
//_____ V A R I A B L E   D E C L A R A T I O N S _______________________________
//_____ F U N C T I O N   D E C L A R A T I O N S _______________________________
/**
* @brief 	This initializations function of sockets pool.
*
* @param	mode - mode of socket work.
*
* @return 	true/false.
*/
bool esp_socket_init(void);

/**
* @brief 	This function configure socket as default.
*
* @param	socket - pointer to socket.
*
* @return 	none.
*/
void esp_socket_default(esocket_t* socket);

/**
* @brief 	This initializations function find socket by connection id.
*
* @param	id - connection id.
*
* @return 	pointer to socket.
*/
esocket_t* esp_socket_find_by_id(esp_conn_id_t id);

/**
* @brief 	This function return connection id.
*
* @param	socket - pointer to socket.
*
* @return 	connection id.
*/
esp_conn_id_t esp_socket_get_connection_id(const esocket_t* socket);

/**
* @brief 	This function set connection id.
*
* @param	socket - pointer to socket.
* @param	id - connection id.
*
* @return 	none.
*/
void esp_socket_set_connection_id(esocket_t* socket, esp_conn_id_t id);

/**
* @brief 	This function return connection status.
*
* @param	socket - pointer to socket.
*
* @return 	connection status.
*/
esp_conn_state_t esp_socket_get_status(const esocket_t* socket);

/**
* @brief 	This function set connection status.
*
* @param	socket - pointer to socket.
* @param	status - connection status.
*
* @return 	none.
*/
void esp_socket_set_status(esocket_t* socket, esp_conn_state_t status);

/**
* @brief 	This function return connection type.
*
* @param	socket - pointer to socket.
*
* @return 	connection type.
*/
esp_conn_type_t esp_socket_get_type(const esocket_t* socket);

/**
* @brief 	This function set connection type.
*
* @param	socket - pointer to socket.
* @param	type - connection type.
*
* @return 	none.
*/
void esp_socket_set_type(esocket_t* socket, esp_conn_type_t type);

/**
* @brief 	This function return connection mode.
*
* @param	socket - pointer to socket.
*
* @return 	connection mode.
*/
esp_conn_tetype_t esp_socket_get_mode(const esocket_t* socket);

/**
* @brief 	This function set connection mode.
*
* @param	socket - pointer to socket.
* @param	mode - connection mode.
*
* @return 	none.
*/
void esp_socket_set_mode(esocket_t* socket, esp_conn_tetype_t mode);

/**
* @brief 	This function return remote ip.
*
* @param	socket - pointer to socket.
*
* @return 	remote ip.
*/
ip4addr_t esp_socket_get_remote_ip(const esocket_t* socket);

/**
* @brief 	This function set remote ip.
*
* @param	socket - pointer to socket.
* @param	ip - remote ip.
*
* @return 	none.
*/
void esp_socket_set_remote_ip(esocket_t* socket, ip4addr_t ip);

/**
* @brief 	This function return remote port.
*
* @param	socket - pointer to socket.
*
* @return 	remote port.
*/
uint16_t esp_socket_get_remote_port(const esocket_t* socket);

/**
* @brief 	This function set remote port.
*
* @param	socket - pointer to socket.
* @param	ip - remote port.
*
* @return 	none.
*/
void esp_socket_set_remote_port(esocket_t* socket, uint16_t port);

/**
* @brief 	This function return count of received data.
*
* @param	socket - pointer to socket.
*
* @return 	count of received data.
*/
size_t esp_socket_get_rx_data_num(const esocket_t* socket);

/**
* @brief 	This function return free space in socket rx buffer.
*
* @param	socket - pointer to socket.
*
* @return 	free space in socket rx buffer.
*/
size_t esp_socket_rxbuf_free_space(const esocket_t* socket);

/**
* @brief 	This function add new data to receive buffer.
*
* @param	socket - pointer to socket.
* @param	data - data for saving.
*
* @return 	true/false.
*/
bool esp_socket_rx_data_enqueue(esocket_t* socket, const uint8_t* data);

/**
* @brief 	This function return data from receive buffer.
*
* @param	socket - pointer to socket.
* @param	data - received data.
*
* @return 	true/false.
*/
bool esp_socket_rx_data_denqueue(esocket_t* socket, uint8_t* data);

/**
* @brief 	This function return free space in socket tx buffer.
*
* @param	socket - pointer to socket.
*
* @return 	free space in socket tx buffer.
*/
size_t esp_socket_txbuf_free_space(const esocket_t* socket);

/**
* @brief 	This function add new data to transmit buffer.
*
* @param	socket - pointer to socket.
* @param	data - data for saving.
*
* @return 	true/false.
*/
bool esp_socket_txbuf_enqueue(esocket_t* socket, const uint8_t* data);
