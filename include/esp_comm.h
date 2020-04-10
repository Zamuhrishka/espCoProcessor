/*******************************************************************************
* @file    		esp_drv.h
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "stack/esp_tcpip.h"
//_____ C O N F I G S ________________________________________________________________________________
//_____ M A C R O S __________________________________________________________________________________
//_____ D E F I N I T I O N __________________________________________________________________________
struct esocket_t;
typedef struct esp_socket_t esocket_t;

//!Prototype of callback function for transmit available handle
typedef void (*esp_message_garbage_fn_t)(char* msg, size_t len);
//_____ V A R I A B L E   D E C L A R A T I O N S ____________________________________________________
//_____ F U N C T I O N   D E C L A R A T I O N S ____________________________________________________
bool esp_comm_init(esp_conn_mode_t mode);
esocket_t* esp_tcp_open(ip4addr_t ip, uint16_t port);
bool esp_tcp_close(esocket_t* socket);
bool esp_tcp_disconnect(esocket_t* socket);
bool esp_tcp_transmit(const esocket_t* socket, const uint8_t data[], size_t size);
bool esp_tcp_is_data_ready(esocket_t* socket);
size_t esp_tcp_get_receive_data_number(esocket_t* socket);
bool esp_tcp_receive(esocket_t* socket, uint8_t data[], size_t size);
esocket_t* esp_tcp_server_open(uint16_t port, uint8_t maxconn, uint16_t timeout);
esocket_t* esp_udp_open(ip4addr_t ip, uint16_t port);
bool esp_udp_close(esocket_t* socket);
