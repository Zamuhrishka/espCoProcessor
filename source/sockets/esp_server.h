/*******************************************************************************
* @file    		esp_server.h
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
typedef void (*esp_server_receive_fn_t)(esocket_t* socket);
//_____ V A R I A B L E   D E C L A R A T I O N S _______________________________
//_____ F U N C T I O N   D E C L A R A T I O N S _______________________________
void esp_register_server_receive_cb(const esp_server_receive_fn_t cb);

/**
* @brief 	This initializations function of socket sub-system.
*
* @param	mode - mode of socket work.
*
* @return 	true/false.
*/
bool esp_server_init(uint16_t port, uint8_t maxconn, uint16_t timeout);

bool esp_server_deinit(void);

void esp_server_receive_handle(void);

/**
* @brief 	This function transmit data throw socket.
*
* @param	socket - pointer to socket.
* @param	data - buffer for transmit.
* @param	size - size of buffer for transmit.
*
* @return 	true/false.
*/
bool esp_server_transmit(esocket_t* socket, const uint8_t data[], size_t size);

int32_t esp_server_transmit_handle(void);
