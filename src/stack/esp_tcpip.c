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
#include "esp_tcpip.h"

#include <string.h>
#include <assert.h>
#include "esp_utils.h"
#include "slre.h"
#include "toolbox/convert.h"
//_____ C O N F I G S  ________________________________________________________________________
// If unit testing is enabled override assert with mock_assert().
#if defined(UNIT_TESTING)
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif // UNIT_TESTING
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
static esp_tx_mode_t transfer = ESP_NORMAL_MODE;

//!Current connection ID
static esp_conn_id_t id = ESP_ID_NONE;

//!Size of buffer
static size_t size = 0;

static char* buffer = NULL;

//!Pointer of callback function for receive data handle.
static esp_tcpip_receive_fn_t esp_tcpip_receive_cb = NULL;

//!Pointer of callback function for connection open.
static esp_tcpip_connect_fn_t esp_tcpip_close_connect = NULL;

//!Pointer of callback function for connection close.
static esp_tcpip_connect_fn_t esp_tcpip_open_connect = NULL;

//!Pointer of callback function for transmit available handle
static esp_tcpip_transmit_fn_t esp_tcpip_transmit_cb = NULL;
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
/**
* @brief 	This function check the pAnswer from chip for busy flag.
*
* @pParam[in] msg - pAnswer from chip.
*
* @return 	true/false.
*/
static bool esp_is_busy(const char msg[])
{
	if(esp_pattern_check(msg, PATTERN_BUSY_P)) {
		return true;
	}

	if(esp_pattern_check(msg, PATTERN_BUSY_S)) {
		return true;
	}

	return false;
}

/**
* @brief 	This function transmit data in Normal mode.
*
* @pParam	none.
*
* @return 	true/false.
*/
static bool esp_normal_transmit(void)
{
	static enum _state
	{
		PREPARATION = 0,
		TRANSMIT,
	}	state = PREPARATION;

	char* pParam = esp_alloc_answer_buffer();
	if(pParam == NULL) {
		return false;
	}

	char* ptr = pParam;
	char number[12] = {0};
	size_t len = 0;

	switch(state)
	{
		case PREPARATION:
			if(esp_tbuffer_denqueue((char*)&id, &buffer, &size))
			{
				if(id != ESP_ID_NONE)
				{
					*ptr = id;
					ptr++;
					if(strcat(ptr, (char*)",\0") == NULL) {
						return false;
					}
					ptr++;
				}

				convert_uint16_to_string(number, size);
				if(strcat(ptr++, (char*)number) == NULL) {
					return false;
				}

				len = strlen((char*)pParam);
				if(esp_cmd_transmit(CIPSEND, pParam, len) == false) {
					return false;
				}

				state = TRANSMIT;
				tx_ready = false;
			}
			break;
		case TRANSMIT:
			if(tx_ready)
			{
				if(esp_data_send(buffer, size) == false) {
				   return false;
				}
				state = PREPARATION;
			}
			break;
		default:
			state = PREPARATION;
			assert(false);
			break;
	}
	return true;
}

/**
* @brief 	This function transmit data in Transparent mode.
*
* @pParam	none.
*
* @return 	true/false.
*/
static bool esp_transparent_transmit(void)
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
			if(esp_cmd_transmit(CIPTRANSP, NULL, 0) == false) {
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
			if(esp_tbuffer_denqueue((char*)&id, &buffer, &size))
			{
				if(esp_data_send(buffer, size) == false) {
				   return false;
				}
			}
			break;
		default:
			state = PREPARATION;
			assert(false);
			break;
	}
	return true;
}

/**
* @brief 	This function receive data in Normal mode.
*
* @pParam[in] msg - received message.
* @pParam[in] len - length of received message.
*
* @return 	none.
*/
static void esp_normal_receive(char msg[], size_t len)
{
	struct slre_cap caps[1];
	struct slre_cap caps_s[2];
	struct slre_cap caps_m[3];
	esp_conn_id_t id = ESP_ID_NONE;

	if(esp_pattern_check(msg, PATTERN_CONNECT))
	{
		if (slre_match((const char*)"(\\d+),CONNECT\\S*", msg, len, caps, 1, 0) > 0) {
			id = *caps[0].ptr;
		}

		if(esp_tcpip_open_connect != NULL) {
			esp_tcpip_open_connect(id);
		}
	}

	if(esp_pattern_check(msg, PATTERN_CLOSE))
	{
		if (slre_match((const char*)"(\\d+),CLOSED\\S*", msg, len, caps, 1, 0) > 0) {
			id = *caps[0].ptr;
		}

		if(esp_tcpip_close_connect != NULL) {
			esp_tcpip_close_connect(id);
		}
	}

	if(esp_pattern_check(msg, PATTERN_TX_READY))
	{
		tx_ready = true;
	}

	if(esp_pattern_check(msg, PATTERN_SEND_ACK))
	{
		if(esp_tcpip_transmit_cb != NULL) {
			esp_tcpip_transmit_cb();
		}
	}

	if(esp_pattern_check(msg, PATTERN_RECEIVE))
	{
		if (slre_match((const char*)"\\S*IPD,(\\d+):(\\S+)", msg, strlen(msg), caps_s, 2, 0) > 0)
		{
			if(esp_tcpip_receive_cb != NULL) {
				esp_tcpip_receive_cb(ESP_ID_NONE, caps_s[1].ptr,  CONVER_TO_NUMBER(*caps_s[0].ptr));
			}
		}
		else if (slre_match((const char*)"\\S*IPD,(\\d),(\\d+):(\\S+)", msg, strlen(msg), caps_m, 3, 0) > 0)
		{
			if(esp_tcpip_receive_cb != NULL) {
				esp_tcpip_receive_cb(*caps_m[0].ptr, caps_m[2].ptr, CONVER_TO_NUMBER(*caps_m[1].ptr));
			}
		}
	}
}

/**
* @brief 	This function receive data in Transparent mode.
*
* @pParam[in] msg - received message.
* @pParam[in] len - length of received message.
*
* @return 	none.
*/
static void esp_transparent_receive(char msg[], size_t len)
{
	if(esp_pattern_check(msg, PATTERN_TRANSPARENT_READY))
	{
		tx_ready = true;
	}
	else
	{
		esp_tcpip_receive_cb(ESP_ID_NONE, msg, len);
	}
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function register TCP data receive callback.
*
* Public function defined in esp_tcpip.h
*/
void esp_register_receive_cb(const esp_tcpip_receive_fn_t cb)
{
	if(NULL != cb)
	{
		esp_tcpip_receive_cb = cb;
	}
}

/**
* This function register TCP connection close callback.
*
* Public function defined in esp_tcpip.h
*/
void esp_register_close_conn_cb(const esp_tcpip_connect_fn_t cb)
{
	if(NULL != cb)
	{
		esp_tcpip_close_connect = cb;
	}
}

/**
* This function register TCP connection close callback.
*
* Public function defined in esp_tcpip.h
*/
void esp_register_open_conn_cb(const esp_tcpip_connect_fn_t cb)
{
	if(NULL != cb)
	{
		esp_tcpip_open_connect = cb;
	}
}

/**
* This function register transmit pass callback.
*
* Public function defined in esp_tcpip.h
*/
void esp_register_transmit_cb(const esp_tcpip_transmit_fn_t cb)
{
	if(NULL != cb)
	{
		esp_tcpip_transmit_cb = cb;
	}
}

/**
* This function read the connection status.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_conn_status_request(esp_conn_status_t *status, uint32_t timeout)
{
	size_t len = 0;
	struct slre_cap caps[6] = {0};
	char* pAnswer = NULL;

	assert(NULL != status);

	pAnswer = esp_alloc_answer_buffer();
	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit((uint8_t)CIPSTATUS, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match((const char*)"STATUS:(\\d)\r\n\\S+CIPSTATUS:(\\d),\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+),\\S+", pAnswer, /*strlen(pAnswer)*/len, caps, 6, 0) <= 0) {
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

	memset((void *)(caps[3].ptr + caps[3].len), 0, 1);
	if(!convert_string_to_ip4addr(&status->remoteIp, caps[3].ptr)) {
		return ESP_INNER_ERR;
	}

	(*status).remotePort = convert_string_to_uint16(caps[4].ptr);
	(*status).localPort = convert_string_to_uint16(caps[5].ptr);

	return ESP_PASS;
}

/**
* This function establish TCP connection.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_connect(const esp_tcp_cfg_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* pParam = NULL;
	char* pAnswer = NULL;

	assert(NULL != cfg);

	pParam = esp_alloc_param_buffer();
	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(cfg->id != ESP_ID_NONE)
	{
		pParam[0] = cfg->id;
		pParam[1] = ',';
	}

	if(strcat((pParam + strlen((char*)pParam)), (char*)"\"TCP\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(!convert_ip4addr_to_string(cfg->remoteIp, (pParam + strlen((char*)pParam)))) {
		return ESP_INNER_ERR;
	}

	if(strcat((pParam + strlen((char*)pParam)), (char*)"\",\0") == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint16_to_string((pParam + strlen((char*)pParam)), cfg->remotePort);

	if(cfg->keepAlive != 0)
	{
		if(strcat ((pParam + strlen((char*)pParam)), (char*)",\0") == NULL) {
			return ESP_INNER_ERR;
		}

		convert_uint16_to_string((pParam + strlen((char*)pParam)), cfg->keepAlive);
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSTART, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if((!esp_pattern_check(pAnswer, PATTERN_OK)) && (!esp_pattern_check(pAnswer, PATTERN_ALREADY_CONNECT))) {
	   return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function establish UDP connection.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_udp_transmission(const esp_udp_cfg_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* pParam = NULL;
	char* pAnswer = NULL;

	assert(NULL != cfg);

	pParam = esp_alloc_param_buffer();
	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(cfg->id != ESP_ID_NONE)
	{
		pParam[0] = cfg->id;
		pParam[1] = ',';
	}

	if(strcat((pParam + strlen((char*)pParam)), (char*)"\"UDP\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(!convert_ip4addr_to_string(cfg->remoteIp, (pParam + strlen((char*)pParam)))) {
		return ESP_INNER_ERR;
	}

	if(strcat((pParam + strlen((char*)pParam)), (char*)"\",\0") == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint16_to_string((pParam + strlen((char*)pParam)), cfg->remotePort);

	if(cfg->localPort != 0)
	{
		if(strcat ((pParam + strlen((char*)pParam)), (char*)",\0") == NULL) {
			return false;
		}

		convert_uint16_to_string((pParam + strlen((char*)pParam)), cfg->localPort);
	}

	if(cfg->mode != 0)
	{
		if(strcat ((pParam + strlen((char*)pParam)), (char*)",\0") == NULL) {
			return ESP_INNER_ERR;
		}

		if(strcat((pParam + strlen((char*)pParam)), (char*)&cfg->mode) == NULL) {
			return ESP_INNER_ERR;
		}
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSTART, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if((!esp_pattern_check(pAnswer, PATTERN_OK)) && (!esp_pattern_check(pAnswer, PATTERN_ALREADY_CONNECT))) {
	   return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function close connection by ID.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_close_connection_m(esp_conn_id_t id, uint32_t timeout)
{
	size_t len = 0;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPCLOSE_M, (char*)&id, 1) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function close close current connection.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_close_connection(uint32_t timeout)
{
	size_t len = 0;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPCLOSE_S, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function enable multiple connections mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_multiple_connection_enable(uint32_t timeout)
{
	size_t len = 0;
	esp_conn_mode_t mode = ESP_MULTIPLE_CONNECT;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function enable single connections mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_single_connection_enable(uint32_t timeout)
{
	size_t len = 0;
	esp_conn_mode_t mode = ESP_SINGLE_CONNECT;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK))
	{
		if(strstr(pAnswer, "CIPSERVER must be 0\r\n\0") != NULL) {
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
esp_status_t esp_mux_cfg_request(esp_conn_mode_t *mode, uint32_t timeout)
{
	size_t len = 0;
	struct slre_cap caps[1] = {0};
	char* pAnswer = NULL;

	assert(NULL != mode);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(REQCIPMUX, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK))	{
		return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMUX:(\\d+)\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return ESP_INNER_ERR;
	}

	*mode = (esp_conn_mode_t)(*caps[0].ptr);

	return ESP_PASS;
}

/**
* This function setup transfer mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_transmit_mode_setup(esp_tx_mode_t mode, uint32_t timeout)
{
	size_t len = 0;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMODE, (char*)&mode, (size_t)1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	transfer = mode;

	return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function request transfer mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_transmit_mode_request(esp_tx_mode_t *mode, uint32_t timeout)
{
	size_t len = 0;
	struct slre_cap caps[1] = {0};
	char* pAnswer = NULL;

	assert(NULL != mode);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(REQCIPMODE, NULL, 0ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_PASS;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMODE:(\\d+)\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (esp_tx_mode_t)(*caps[0].ptr);

	return ESP_PASS;
}

/**
* This function disable transparent transmit mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_transparent_mode_disable(uint32_t timeout)
{
	size_t len = 0;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_data_send("+++", strlen("+++")) == false) {
	   return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function start TCP server.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_server_create(uint16_t port, uint32_t timeout)
{
	size_t len = 0;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	pParam[0] = '1';
	if(port != 0)
	{
		pParam[1] = ',';
		convert_uint16_to_string((char*)&pParam[2], port);
	}

	len = strlen((char*)pParam);
	if(esp_cmd_transmit(CIPSERVER, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	while(true)
	{
		if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
			return ESP_RECEIVE_ERR;
		}

		if(esp_is_busy(pAnswer)) {
			return ESP_BUSY;
		}

		return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
	}

	return ESP_INNER_ERR;
}

/**
* This function stop TCP server.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_server_delete(uint16_t port, uint32_t timeout)
{
	char* strPort = NULL;
	size_t len = 0;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	pParam[0] = '0';
	if(port != 0)
	{
		convert_uint32_to_string(strPort, port);

		if(strcat ((char*)pParam, (char*)",\0") == NULL) {
			return ESP_INNER_ERR;
		}

		if(strcat ((char*)pParam, strPort) == NULL) {
			return ESP_INNER_ERR;
		}
	}

	len = strlen((char*)pParam);
	if(esp_cmd_transmit(CIPSERVER, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	while(true)
	{
		if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
			return ESP_RECEIVE_ERR;
		}

		if(esp_is_busy(pAnswer)) {
			return ESP_BUSY;
		}

		return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
	}

	return ESP_INNER_ERR;
}

/**
* This function sets the TCP Server Timeout.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_server_timeout_setup(uint16_t stimeout, uint32_t timeout)
{
	size_t len = 0;

	assert(7200 > stimeout);

	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	convert_uint16_to_string(pParam, stimeout);

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSTO, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	return (esp_pattern_check(pAnswer, PATTERN_ERROR)) ? ESP_INNER_ERR : ESP_PASS;
}

/**
* This function set the Maximum Connections Allowed by Server.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_server_maxconn_setup(uint8_t conn, uint32_t timeout)
{
	size_t len = 0;

	assert(5 > conn);

	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(conn > ESP_SERVER_MAX_CONN) {
		return ESP_PARAM_ERR;
	}

	convert_uint8_to_string(pParam, conn);

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSERVERMAXCONN, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function setup DNS name.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_domain_name_setup(const char domain[], uint32_t timeout)
{
	size_t len = 0;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(domain == NULL || strlen(domain) > ESP_DOMAIN_NAME_LENGTH) {
		return ESP_PARAM_ERR;
	}

	pParam[0] = '"';
	strcpy(&pParam[1], domain);
	len = strlen((char*)pParam);
	pParam[len] = '"';
	len++;

	if(esp_cmd_transmit(CIPDOMAIN, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function ping packets.
*
* Public function defined in esp_tcpip.h
*/
uint32_t esp_ping(ip4addr_t ip, uint32_t timeout)
{
	struct slre_cap caps[1];
	size_t len = 0;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	pParam[0] = '"';
	convert_ip4addr_to_string(ip, &pParam[1]);
	len = strlen((char*)pParam);
	pParam[len] = '"';
	len++;

	if(esp_cmd_transmit(PING, pParam, len) == false) {
		return 0;
	}

	if(esp_response_receive(&pAnswer, &len, timeout) != ESP_PASS) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_pattern_check(pAnswer, PATTERN_ERROR)) {
		return 0;
	}

	if (slre_match((const char*)"\\+(\\d+)\r\n", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
		return 0;
	}

	return convert_string_to_uint32(caps[0].ptr);
}

/**
* This function add data into transmit buffer.
*
* Public function defined in esp_tcpip.h
*/
bool esp_tcp_send(esp_conn_id_t id, const uint8_t data[], size_t size)
{
	assert(NULL != data);
	assert(size);

    return esp_tbuffer_enqueue((char)id, data, size);
}

/**
* This function handle of transmit buffer.
*
* Public function defined in esp_tcpip.h
*/
bool esp_tcp_transmit_handle(void)
{
	bool res = false;

	if(transfer == ESP_TRANSPARENT_MODE)
	{
		res = esp_transparent_transmit();
	}
	else
	{
		res = esp_normal_transmit();
	}

	return res;
}

/**
* This function handle TCP connections.
*
* Public function defined in esp_tcpip.h
*/
void esp_tcp_receive_handle(char* msg, size_t len)
{
	assert(msg);

	if(transfer == ESP_TRANSPARENT_MODE)
	{
		esp_transparent_receive(msg, len);
	}
	else
	{
		esp_normal_receive(msg, len);
	}
}

