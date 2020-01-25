// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_tcpip.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 TCP/IP-Related AT Commands.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "stack/esp_tcpip.h"

#include <string.h>
#include <string.h>
#include <stdio.h>
#include "slre.h"
#include "esp_utils.h"
#include "esp_queue.h"
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "toolbox/convert.h"
#include "esp_port.h"
//_____ D E F I N I T I O N ___________________________________________________
//!Maximum connections for TCP server
#define ESP_SERVER_MAX_CONN						5

//!Maximum length of domain name
#define ESP_DOMAIN_NAME_LENGTH					64
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!Allow transmit flag
static volatile bool tx_ready = false;

//!Transmit mode
static ESP_TransferMode_t transfer = NORMAL_MODE;

//!Current connection ID
static ESP_ConnectionId_t id = ID_NONE;

//!Size of buffer
static size_t size = 0;

static char* buffer = NULL;

//!Pointer of callback function for receive data handle.
static Esp_TcpReceiveFp_t ESP_ReceiveDataCB = NULL;

//!Pointer of callback function for connection open.
static Esp_TcpConnectFp_t ESP_CloseConnectCB = NULL;

//!Pointer of callback function for connection close.
static Esp_TcpConnectFp_t ESP_OpenConnectCB = NULL;

//!Pointer of callback function for transmit available handle
static Esp_TcpTransmitPassFp_t ESP_TransmitPassCB = NULL;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
/**
* @brief 	This function check the answer from chip for busy flag.
*
* @param[in] msg - answer from chip.
*
* @return 	true/false.
*/
static bool ESP_IsBusy(const char msg[])
{
	if(ESP_PatternCheck(msg, pattern_BUSY_P)) {
		return true;
	}

	if(ESP_PatternCheck(msg, pattern_BUSY_S)) {
		return true;
	}

	return false;
}

/**
* @brief 	This function transmit data in Normal mode.
*
* @param	none.
*
* @return 	true/false.
*/
static bool ESP_NormalTransmit(void)
{
	static enum _state
	{
		PREPARATION = 0,
		TRANSMIT,
	}	state = PREPARATION;

	char* param = ESP_AllocAnswerBuffer();
	char* ptr = param;
	char number[12] = {0};
	size_t len = 0;

	if(param == NULL) {
		return false;
	}

	switch(state)
	{
		case PREPARATION:
			if(ESP_TxQueueDenqueue((char*)&id, &buffer, &size))
			{
				if(id != ID_NONE)
				{
					*ptr = id;
					ptr++;
					if(strcat(ptr, (char*)",\0") == NULL) {
						return false;
					}
					ptr++;
				}

				Conver_DigToStringUint16(number, size);
				if(strcat(ptr++, (char*)number) == NULL) {
					return false;
				}

				len = strlen((char*)param);
				if(ESP_SendAtCmd(CIPSEND, param, len) == false) {
					return false;
				}

				state = TRANSMIT;
				tx_ready = false;
			}
			break;
		case TRANSMIT:
			if(tx_ready)
			{
				if(ESP_SendAtRawData(buffer, size) == false) {
				   return false;
				}
				state = PREPARATION;
			}
			break;
		default:
			state = PREPARATION;
	}
	return true;
}

/**
* @brief 	This function transmit data in Transparent mode.
*
* @param	none.
*
* @return 	true/false.
*/
static bool ESP_TransparentTransmit(void)
{
	static enum _state
	{
		PREPARATION = 0,
		PREPARATION1,
		TRANSMIT,
		STOP
	}	state = PREPARATION;

	switch(state)
	{
		case PREPARATION:
			if(ESP_SendAtCmd(CIPTRANSP, NULL, 0) == false) {
				return false;
			}
			state = PREPARATION1;
			break;
		case PREPARATION1:
			if(tx_ready)
			{
				state = TRANSMIT;
			}
			break;
		case TRANSMIT:
			if(ESP_TxQueueDenqueue((char*)&id, &buffer, &size))
			{
				if(ESP_SendAtRawData(buffer, size) == false) {
				   return false;
				}
			}
			break;
		default:
			state = PREPARATION;
	}
	return true;
}

/**
* @brief 	This function receive data in Normal mode.
*
* @param[in] msg - received message.
* @param[in] len - length of received message.
*
* @return 	none.
*/
static void ESP_NormalReceive(char msg[], size_t len)
{
	struct slre_cap caps[1];
	struct slre_cap caps_s[2];
	struct slre_cap caps_m[3];
	ESP_ConnectionId_t id = ID_NONE;

	if(ESP_PatternCheck(msg, pattern_CONNECT))
	{
		if (slre_match((const char*)"(\\d+),CONNECT\\S*", msg, len, caps, 1, 0) > 0) {
			id = *caps[0].ptr;
		}

		if(ESP_OpenConnectCB != NULL) {
			ESP_OpenConnectCB(id);
		}
	}

	if(ESP_PatternCheck(msg, pattern_CLOSE))
	{
		if (slre_match((const char*)"(\\d+),CLOSED\\S*", msg, len, caps, 1, 0) > 0) {
			id = *caps[0].ptr;
		}

		if(ESP_CloseConnectCB != NULL) {
			ESP_CloseConnectCB(id);
		}
	}

	if(ESP_PatternCheck(msg, pattern_TX_READY))
	{
		tx_ready = true;
	}

	if(ESP_PatternCheck(msg, pattern_SEND_ACK))
	{
		if(ESP_TransmitPassCB != NULL) {
			ESP_TransmitPassCB();
		}
	}

	if(ESP_PatternCheck(msg, pattern_RECEIVE))
	{
		if (slre_match((const char*)"\\S*IPD,(\\d+):(\\S+)", msg, strlen(msg), caps_s, 2, 0) > 0)
		{
			if(ESP_ReceiveDataCB != NULL) {
				ESP_ReceiveDataCB(ID_NONE, caps_s[1].ptr,  CONVER_TO_NUMBER(*caps_s[0].ptr));
			}
		}
		else if (slre_match((const char*)"\\S*IPD,(\\d),(\\d+):(\\S+)", msg, strlen(msg), caps_m, 3, 0) > 0)
		{
			if(ESP_ReceiveDataCB != NULL) {
				ESP_ReceiveDataCB(*caps_m[0].ptr, caps_m[2].ptr, CONVER_TO_NUMBER(*caps_m[1].ptr));
			}
		}
	}
}

/**
* @brief 	This function receive data in Transparent mode.
*
* @param[in] msg - received message.
* @param[in] len - length of received message.
*
* @return 	none.
*/
static void ESP_TransparentReceive(char msg[], size_t len)
{
	if(ESP_PatternCheck(msg, pattern_TRANSPARENT_READY))
	{
		tx_ready = true;
	}
	else
	{
		ESP_ReceiveDataCB(ID_NONE, msg, len);
	}
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function register TCP data receive callback.
*
* Public function defined in esp_tcpip.h
*/
void ESP_RegisterReceiceCB(const Esp_TcpReceiveFp_t cb)
{
	if(cb != NULL) {
		ESP_ReceiveDataCB = cb;
	}
}

/**
* This function register TCP connection close callback.
*
* Public function defined in esp_tcpip.h
*/
void ESP_RegisterCloseConnectCB(const Esp_TcpConnectFp_t cb)
{
	if(cb != NULL) {
		ESP_CloseConnectCB = cb;
	}
}

/**
* This function register TCP connection close callback.
*
* Public function defined in esp_tcpip.h
*/
void ESP_RegisterOpenConnectCB(const Esp_TcpConnectFp_t cb)
{
	if(cb != NULL) {
		ESP_OpenConnectCB = cb;
	}
}

/**
* This function register transmit pass callback.
*
* Public function defined in esp_tcpip.h
*/
void ESP_RegisterTransmitPassCB(const Esp_TcpTransmitPassFp_t cb)
{
	if(cb != NULL) {
		ESP_TransmitPassCB = cb;
	}
}

/**
* This function read the connection status.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_RequestConnectionStatus(ESP_ConnStatus_t *status, uint32_t timeout)
{
	struct slre_cap caps[6];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd((uint8_t)CIPSTATUS, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	if(!ESP_PatternCheck(answer, pattern_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match((const char*)"STATUS:(\\d)\r\n\\S+CIPSTATUS:(\\d),\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+),\\S+", answer, strlen(answer), caps, 6, 0) <= 0) {
	   return ESP_PATTERN_ERR;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0) ||
	   (caps[3].ptr == NULL || caps[3].len == 0) ||
	   (caps[4].ptr == NULL || caps[4].len == 0) ||
	   (caps[5].ptr == NULL || caps[5].len == 0))
	{
		return ESP_INNER_ERR;
	}

	(*status).stat = *caps[0].ptr;
	(*status).id = *caps[1].ptr;
	(*status).type = (strcmp (caps[2].ptr, (const char*) "TCP\0")) ? ESP_TCP : ESP_UDP;

	memset((void *)(caps[3].ptr + caps[3].len),0,1);
	if(!Convert_StringToIpv4addr(&status->remoteIp, caps[3].ptr)) {
		return ESP_INNER_ERR;
	}

	(*status).remotePort = Conver_StringToUint16(caps[4].ptr);
	(*status).localPort = Conver_StringToUint16(caps[5].ptr);

	return ESP_PASS;
}

/**
* This function establish TCP connection.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_EstablishConnectTCP(const TCP_ConnParam_t *connParam, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if(connParam == NULL || (connParam->id != ID0 && connParam->id != ID1 &&
							connParam->id != ID2 && connParam->id != ID3 &&
							connParam->id != ID4 && connParam->id != ID_ALL &&
							connParam->id != ID_NONE))
	{
		return ESP_PARAM_ERR;
	}

	if(connParam->id != ID_NONE)
	{
		param[0] = connParam->id;
		param[1] = ',';
	}

	if(strcat((param + strlen((char*)param)), (char*)"\"TCP\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(!Convert_Ipv4addrToString(connParam->remoteIp, (param + strlen((char*)param)))) {
		return ESP_INNER_ERR;
	}

	if(strcat((param + strlen((char*)param)), (char*)"\",\0") == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint16((param + strlen((char*)param)), connParam->remotePort);

	if(connParam->keepAlive != 0)
	{
		if(strcat ((param + strlen((char*)param)), (char*)",\0") == NULL) {
			return false;
		}

		Conver_DigToStringUint16((param + strlen((char*)param)), connParam->keepAlive);
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTART, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 5000ul) < 0) {
	  return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	if((!ESP_PatternCheck(answer, pattern_OK)) && (!ESP_PatternCheck(answer, pattern_ALREADY_CONNECT))) {
	   return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function establish UDP connection.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_EstablishConnectUDP(const UDP_ConnParam_t *connParam, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if(connParam == NULL || (connParam->id != ID0 && connParam->id != ID1 &&
							connParam->id != ID2 && connParam->id != ID3 &&
							connParam->id != ID4 && connParam->id != ID_ALL &&
							connParam->id != ID_NONE))
	{
		return ESP_PARAM_ERR;
	}

	if(connParam->id != ID_NONE)
	{
		param[0] = connParam->id;
		param[1] = ',';
	}

	if(strcat((param + strlen((char*)param)), (char*)"\"UDP\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(!Convert_Ipv4addrToString(connParam->remoteIp, (param + strlen((char*)param)))) {
		return ESP_INNER_ERR;
	}

	if(strcat((param + strlen((char*)param)), (char*)"\",\0") == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint16((param + strlen((char*)param)), connParam->remotePort);

	if(connParam->localPort != 0)
	{
		if(strcat ((param + strlen((char*)param)), (char*)",\0") == NULL) {
			return false;
		}

		Conver_DigToStringUint16((param + strlen((char*)param)), connParam->localPort);
	}

	if(connParam->mode != 0)
	{
		if(strcat ((param + strlen((char*)param)), (char*)",\0") == NULL) {
			return ESP_INNER_ERR;
		}

		if(strcat((param + strlen((char*)param)), (char*)&connParam->mode) == NULL) {
			return ESP_INNER_ERR;
		}
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTART, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 5000ul) < 0) {
	  return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	if((!ESP_PatternCheck(answer, pattern_OK)) && (!ESP_PatternCheck(answer, pattern_ALREADY_CONNECT))) {
	   return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function close connection by ID.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_CloseMultiConnection(ESP_ConnectionId_t id, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(CIPCLOSE_M, (char*)&id, 1) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 20000ul) < 0) {
	  return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function close close current connection.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_CloseSingleConnection(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(CIPCLOSE_S, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 20000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function enable multiple connections mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_MultipleConnectionEnable(uint32_t timeout)
{
	ESP_ConnectionMode_t mode = MULTIPLE_CONNECT;
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function enable single connections mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_SingleConnectionEnable(uint32_t timeout)
{
	ESP_ConnectionMode_t mode = SINGLE_CONNECT;
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	if(!ESP_PatternCheck(answer, pattern_OK))
	{
		if(strstr(answer, "CIPSERVER must be 0\r\n\0") != NULL) {
			return ESP_SERVER_ON;
		}
	}

	return ESP_PASS;
}

/**
* This function request multiplexer mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_RequestConfigMux(ESP_ConnectionMode_t *mode, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(mode == NULL) {
		return ESP_PARAM_ERR;
	}

	if(ESP_SendAtCmd(REQCIPMUX, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	if(!ESP_PatternCheck(answer, pattern_OK))	{
		return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMUX:(\\d+)\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return ESP_INNER_ERR;
	}

	*mode = (ESP_ConnectionMode_t)(*caps[0].ptr);

	return ESP_PASS;
}

/**
* This function setup transfer mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_SetupTransferMode(ESP_TransferMode_t mode, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(CIPMODE, (char*)&mode, (size_t)1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	transfer = mode;

	return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function request transfer mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_RequestTransferMode(ESP_TransferMode_t *mode, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(REQCIPMODE, NULL, 0ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_PASS;
	}

	if(!ESP_PatternCheck(answer, pattern_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMODE:(\\d+)\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (ESP_TransferMode_t)(*caps[0].ptr);

	return ESP_PASS;
}

/**
* This function disable transparent transmit mode.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_TransparentModeDisable(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtRawData("+++", strlen("+++")) == false) {
	   return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 10000ul) < 0) {
	   return ESP_RECEIVE_ERR;
	}

	if(ESP_IsBusy(answer)) {
		return ESP_BUSY;
	}

	return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function start TCP server.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_StartServerTCP(uint16_t port, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	param[0] = '1';
	if(port != 0)
	{
		param[1] = ',';
		Conver_DigToStringUint16((char*)&param[2], port);
	}

	len = strlen((char*)param);
	if(ESP_SendAtCmd(CIPSERVER, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	while(true)
	{
		if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
		   return ESP_RECEIVE_ERR;
		}

		if(ESP_IsBusy(answer)) {
			return ESP_BUSY;
		}

		return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
	}

	return ESP_INNER_ERR;
}

/**
* This function stop TCP server.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_CloseServerTCP(uint16_t port, uint32_t timeout)
{
	char* strPort = NULL;
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	param[0] = '0';
	if(port != 0)
	{
		Conver_DigToStringUint32(strPort, port);

		if(strcat ((char*)param, (char*)",\0") == NULL) {
			return ESP_INNER_ERR;
		}

		if(strcat ((char*)param, strPort) == NULL) {
			return ESP_INNER_ERR;
		}
	}

	len = strlen((char*)param);
	if(ESP_SendAtCmd(CIPSERVER, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	while(true)
	{
		if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
			return ESP_RECEIVE_ERR;
		}

		if(ESP_IsBusy(answer)) {
			return ESP_BUSY;
		}

		return (!ESP_PatternCheck(answer, pattern_OK)) ? ESP_INNER_ERR : ESP_PASS;
	}

	return ESP_INNER_ERR;
}

/**
* This function sets the TCP Server Timeout.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_SetupServerTimeout(uint16_t servTimeout, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint16(param, servTimeout);

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTO, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	return (ESP_PatternCheck(answer, pattern_ERROR)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function set the Maximum Connections Allowed by Server.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_SetupServerMaxConn(uint8_t max_conn, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if(max_conn > ESP_SERVER_MAX_CONN) {
		return ESP_PARAM_ERR;
	}

	Conver_DigToStringUint8(param, max_conn);

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSERVERMAXCONN, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(!ESP_PatternCheck(answer, pattern_OK)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function setup DNS name.
*
* Public function defined in esp_tcpip.h
*/
ESPStatus_t ESP_SetupDomainName(const char domain[], uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if(domain == NULL || strlen(domain) > ESP_DOMAIN_NAME_LENGTH) {
		return ESP_PARAM_ERR;
	}

	param[0] = '"';
	strcpy(&param[1], domain);
	len = strlen((char*)param);
	param[len] = '"';
	len++;

	if(ESP_SendAtCmd(CIPDOMAIN, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(!ESP_PatternCheck(answer, pattern_OK)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function ping packets.
*
* Public function defined in esp_tcpip.h
*/
uint32_t ESP_Ping(Ipv4Addr_t ip, uint32_t timeout)
{
	struct slre_cap caps[1];
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	param[0] = '"';
	Convert_Ipv4addrToString(ip, &param[1]);
	len = strlen((char*)param);
	param[len] = '"';
	len++;

	if(ESP_SendAtCmd(PING, param, len) == false) {
		return 0;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, 6000ul) < 0) {
		return 0;
	}

	if(ESP_PatternCheck(answer, pattern_ERROR)) {
		return 0;
	}

	if (slre_match((const char*)"\\+(\\d+)\r\n", answer, strlen(answer), caps, 1, 0) <= 0) {
		return 0;
	}

	return Conver_StringToUint32(caps[0].ptr);
}

/**
* This function add data into transmit buffer.
*
* Public function defined in esp_tcpip.h
*/
bool ESP_TransmitData(ESP_ConnectionId_t id, const char data[], size_t size)
{
    if((data == NULL) || ((size == 0))) {
    	return false;
    }

    return ESP_TxQueueEnqueue((char)id, data, size);
}

/**
* This function handle of transmit buffer.
*
* Public function defined in esp_tcpip.h
*/
bool ESP_TransmitDataHandle(void)
{
	bool res = false;

	if(transfer == TRANSPARENT_MODE)
	{
		res = ESP_TransparentTransmit();
	}
	else
	{
		res = ESP_NormalTransmit();
	}

	return res;
}

/**
* This function handle TCP connections.
*
* Public function defined in esp_tcpip.h
*/
void ESP_TcpMsgHandleCallBack(char* msg, size_t len)
{
	if(transfer == TRANSPARENT_MODE)
	{
		ESP_TransparentReceive(msg, len);
	}
	else
	{
		ESP_NormalReceive(msg, len);
	}
}

