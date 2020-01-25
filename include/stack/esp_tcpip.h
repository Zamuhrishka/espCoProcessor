/********************************************************************************
*
* @file    		esp_tcpip.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 TCP/IP-Related AT Commands.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include "convert.h"
#include "esp_drv.h"
#include "esp_utils.h"
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//! @brief  Connection status
//! @{
typedef enum
{
	ESP_GOTIP        		=       '2',										///< The ESP8266 Station is connected to an AP and its IP is obtained
	ESP_CONNECTED        	=       '3',										///< The ESP8266 Station has created a TCP or UDP transmission
	ESP_DISCONNECTED		=		'4',										///< The TCP or UDP transmission of ESP8266 Station is disconnected
	ESP_NOT_CONNECTED		=		'5',										///< The ESP8266 Station does NOT connect to an AP
}	ESP_ConnStat_t;
//! @}

//! @brief  Connection types
//! @{
typedef enum
{
	ESP_TCP        			=       1,											///< TCP connection type
	ESP_UDP        			=       2,											///< UDP connection type
}	ESP_ConnType_t;
//! @}

//! @brief  Connection wifi types
//! @{
typedef enum
{
	ESP_CLIENT        		=       1,											///< Create the TCP client
	ESP_SERVER       		=       2,											///< Create the TCP server
}	ESP_ConnTeType_t;
//! @}

//! @brief  Transmit modes
//! @{
typedef enum
{
	NORMAL_MODE        		=       '0',										///< Normal transmit mode
	TRANSPARENT_MODE        =       '1',										///< Transparent transmit mode
}	ESP_TransferMode_t;
//! @}

//! @brief List of connections modes
//! @{
typedef enum
{
	SINGLE_CONNECT        	=       '0',										///< Single connection mode
	MULTIPLE_CONNECT        =       '1',										///< Multiple connection mode
}	ESP_ConnectionMode_t;
//! @}

//! @brief List of UDP connection work modes
//! @{
typedef enum
{
	UDP_MODE0        		=       '0',
	UDP_MODE1        		=       '1',
	UDP_MODE2        		=       '2',
}	ESP_UdpMode_t;
//! @}

//! @brief  List of valid connection IDs
//! @{
typedef enum
{
	ID0           			=       '0',
	ID1         			=       '1',
	ID2         			=       '2',
	ID3         			=       '3',
	ID4         			=       '4',
	ID_ALL         			=       '5',
	ID_NONE					=		'6'
}	ESP_ConnectionId_t;
//! @}

//! @brief  structure of TCP/IP connection status
//! @{
typedef struct
{
	ESP_ConnStat_t stat;														///< Status of the ESP8266 Station interface.
	ESP_ConnTeType_t tetype;													///< Server or Client
	ESP_ConnType_t type;														///< String parameter, "TCP" or "UDP"
	ESP_ConnectionId_t id;														///< ID of the connection (0~4), used for multiple connections
	Ipv4Addr_t remoteIp;														///< The remote IP address
	uint16_t remotePort;														///< The remote port number
	uint16_t localPort;															///< ESP8266 local port number
}	ESP_ConnStatus_t;
//! @}

//! @brief  structure of TCP connection param
//! @{
typedef struct
{
	ESP_ConnectionId_t id;														///< ID of the connection (0~4), used for multiple connections
	Ipv4Addr_t remoteIp;														///< The remote IP address
	uint16_t remotePort;														///< The remote port number
	uint16_t localPort;															///< ESP8266 local port number
	uint32_t keepAlive;															///< Detection time interval when TCP is kept alive
}	TCP_ConnParam_t;
//! @}

//! @brief  structure of UDP connection param
//! @{
typedef struct
{
	ESP_ConnectionId_t id;														///< ID of the connection (0~4), used for multiple connections
	uint16_t localPort;															///< ESP8266 local port number
	Ipv4Addr_t remoteIp;														///< The remote IP address
	uint16_t remotePort;														///< The remote port number
	ESP_UdpMode_t mode;
}	UDP_ConnParam_t;
//! @}

//!Prototype of callback function for receive data handle.
typedef bool (*Esp_TcpReceiveFp_t)(ESP_ConnectionId_t id, const char data[], size_t size);

//!Prototype of callback function for connection open/close.
typedef bool (*Esp_TcpConnectFp_t)(ESP_ConnectionId_t id);

//!Prototype of callback function for transmit available handle
typedef void (*Esp_TcpTransmitPassFp_t)(void);
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* @brief 	This function register TCP data receive callback.
*
* @param[in] cb	callback function.
*
* @return 	none.
*/
void ESP_RegisterReceiceCB(const Esp_TcpReceiveFp_t cb);

/**
* @brief 	This function register TCP connection close callback.
*
* @param[in] cb	callback function.
*
* @return 	none.
*/
void ESP_RegisterCloseConnectCB(const Esp_TcpConnectFp_t cb);

/**
* @brief 	This function register TCP connection close callback.
*
* @param[in] cb	callback function.
*
* @return 	none.
*/
void ESP_RegisterOpenConnectCB(const Esp_TcpConnectFp_t cb);

/**
* @brief 	This function register transmit pass callback.
*
* @param[in] cb	callback function.
*
* @return 	none.
*/
void ESP_RegisterTransmitPassCB(const Esp_TcpTransmitPassFp_t cb);

/**
* @brief 	This function read the connection status.
*
* @param	status[out]	connection status.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* </ul>
*/
ESPStatus_t ESP_RequestConnectionStatus(ESP_ConnStatus_t *status, uint32_t timeout);

/**
* @brief 	This function establish TCP connection.
*
* @param[out] connParam	connection configuration.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_EstablishConnectTCP(const TCP_ConnParam_t *connParam, uint32_t timeout);

/**
* @brief 	This function establish UDP connection.
*
* @param[in] connParam	connection configuration.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_EstablishConnectUDP(const UDP_ConnParam_t *param, uint32_t timeout);

/**
* @brief 	This function close connection by ID.
*
* @param[in] id	connection id.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PASS</b> - There are no error.
* </ul>
*/
ESPStatus_t ESP_CloseMultiConnection(ESP_ConnectionId_t id, uint32_t timeout);

/**
* @brief 	This function close current connection.
*
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PASS</b> - There are no error.
* </ul>
*/
ESPStatus_t ESP_CloseSingleConnection(uint32_t timeout);

/**
* @brief 	This function enable multiple connections mode.
*
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @warning 	This mode can only be set when transparent transmission disabled.
* @warning	This mode can only be changed after all connections are disconnected.
* @warning 	If TCP server is started, has to delete TCP server first， then
*			change to single connection is allowed.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PASS</b> - There are no error.
* </ul>
*/
ESPStatus_t ESP_MultipleConnectionEnable(uint32_t timeout);

/**
* @brief 	This function enable single connections mode.
*
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @warning 	This mode can only be set when transparent transmission disabled.
* @warning	This mode can only be changed after all connections are disconnected.
* @warning 	If TCP server is started, has to delete TCP server first， then
*			change to single connection is allowed.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PASS</b> - There are no error.
* </ul>
*/
ESPStatus_t ESP_SingleConnectionEnable(uint32_t timeout);

/**
* @brief 	This function request multiplexer mode.
*
* @param[out] mode	multiplexer mode.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_RequestConfigMux(ESP_ConnectionMode_t *mode, uint32_t timeout);

/**
* @brief 	This function setup transfer mode.
*
* @param[in] mode	transfer mode.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @note		This configuration would not save into Flash.
* @warning	UART-WiFi passthrough mode, only for TCP single connection.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_SetupTransferMode(ESP_TransferMode_t mode, uint32_t timeout);

/**
* @brief 	This function request transfer mode.
*
* @param[out] mode	transfer mode.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_RequestTransferMode(ESP_TransferMode_t *mode, uint32_t timeout);

/**
* @brief 	This function disable transparent transmit mode.
*
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @note 	Please wait for at least one second before
*			sending the next AT command.
*
* @warning	After this command the ESP8266 may freeze. If this occur -
* 			please use reset function.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_TransparentModeDisable(uint32_t timeout);

/**
* @brief 	This function start TCP server.
*
* @param[in] port	TCP port or 0 for default port 333.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @note		Server can only be created when AT+CIPMUX=1.
* @note 	When a client is connected to the server, it will
* 			take up one connection， be gave an id.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_StartServerTCP(uint16_t port, uint32_t timeout);

/**
* @brief 	This function stop TCP server.
*
* @param[in] port	TCP port or 0 for default port 333.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_CloseServerTCP(uint16_t port, uint32_t timeout);

/**
* @brief 	This function sets the TCP Server Timeout.
*
* @param[in] servTimeout	TCP server timeout within the range of 0 ~ 7200s.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @note		ESP8266 configured as a TCP server will disconnect from the TCP
* 			client that does not communicate with it until timeout.
* @warning 	If timeout = 0, the connection will never time out.
* 			This configuration is not recommended.
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_SetupServerTimeout(uint16_t servTimeout, uint32_t timeout);

/**
* @brief 	This function set the Maximum Connections Allowed by Server.
*
* @param[in] max_conn	the maximum number of clients allowed to
* 			connect to the TCP or SSL server, range: [1, 5].
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @warning 	To set this configuration, you should call the this command
* 			before creating a server.
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_SetupServerMaxConn(uint8_t max_conn, uint32_t timeout);

/**
* @brief 	This function setup DNS name.
*
* @param[in] domain	domain name.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @warning 	The domain name, length should be less than 64 bytes.
*
* @return
* <ul>
* <li> <b>ESP_TRANSMIT_ERR</b> - Error during transmit AT command.
* <li> <b>ESP_RECEIVE_ERR</b> - Error during receive AT command.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>ESP_INNER_ERR</b> - Some inner error.
* <li> <b>ESP_PATTERN_ERR</b> - Error of regex pattern.
* <li> <b>ESP_PASS</b> - There are no error.
* <li> <b>ESP_PARAM_ERR</b> - Wrong params.
* </ul>
*/
ESPStatus_t ESP_SetupDomainName(const char domain[], uint32_t timeout);

/**
* @brief 	This function ping packets.
*
* @param[in] ip	ip address for ping.
* @param[in] timeout	timeout in msec for waiting answer from chip.
*
* @return	the response time of ping.
*/
uint32_t ESP_Ping(Ipv4Addr_t ip, uint32_t timeout);

/**
* @brief 	This function add data into transmit buffer.
*
* @param[in] id	id of connection.
* @param[in] data	data for transmitting.
* @param[in] size	size of transmitted data.
*
* @note 	MAX 2048 bytes.
*
* @return 	true/false.
*/
bool ESP_TransmitData(ESP_ConnectionId_t id, const char data[], size_t size);

/**
* @brief 	This function handle of transmit buffer.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_TransmitDataHandle(void);

/**
* @brief 	This function handle TCP connections.
*
* @param[in] msg	received message.
* @param[in] len	length of received message.
*
* @return 	none.
*/
void ESP_TcpMsgHandleCallBack(char* msg, size_t len);
