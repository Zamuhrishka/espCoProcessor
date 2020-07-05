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
#include "esp_sockets.h"
//_____ C O N F I G S __________________________________________________________
//_____ M A C R O S ____________________________________________________________
//_____ D E F I N I T I O N ____________________________________________________
//_____ V A R I A B L E   D E C L A R A T I O N S _______________________________
//_____ F U N C T I O N   D E C L A R A T I O N S _______________________________
/**
* @brief 	This initializations function of client sub-system.
*
* @param	none.
*
* @return 	true/false.
*/
bool esp_client_init(void);

/**
* @brief 	This function open new connection.
*
* @param	ip - remote ip address.
* @param	port - remote port.
*
* @return 	socket or NULL.
*/
esocket_t* esp_connection_open(ip4addr_t ip, uint16_t port);

/**
* @brief 	This function close connectiont.
*
* @param	socket - pointer to socket.
*
* @return 	true/false.
*/
bool esp_connection_close(esocket_t* socket);

/**
* @brief 	This function transmit data throw socket.
*
* @param	socket - pointer to socket.
* @param	data - buffer for transmit.
* @param	size - size of buffer for transmit.
*
* @return 	true/false.
*/
bool esp_client_transmit(const esocket_t* socket, const uint8_t data[], size_t size);

/**
* @brief 	This function receive data from socket.
*
* @param	socket - pointer to socket.
* @param	data - buffer for receive.
* @param	size - size of buffer for receive.
*
* @return 	number of received bytes or error code (negative value).
*/
int32_t esp_client_receive(esocket_t* socket, uint8_t data[], size_t size);
