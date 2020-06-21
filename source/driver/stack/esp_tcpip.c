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

#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "esp_utils.h"
#include "slre.h"
#include "queue.h"
#include "convert.h"
//_____ C O N F I G S  ________________________________________________________________________
// If unit testing is enabled override assert with mock_assert().
#if defined(UNIT_TESTING)
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif // UNIT_TESTING

#define TIMEZONE_LOW_LIMIT							(-11)
#define TIMEZONE_HIGH_LIMIT							(13)
//_____ D E F I N I T I O N ___________________________________________________
//! Max size of queue buffer
#define ESP_QUEUE_SIZE                 				(10U)

//! Max size of each of element of queue
#define ESP_PAYLOAD_LENGTH							(200U)

//!Maximum connections for TCP server
#define ESP_SERVER_MAX_CONN							(5)

//!Maximum length of domain name
#define ESP_DOMAIN_NAME_LENGTH						(64)

//! @brief Static queue structure
//! @{
typedef struct
{
//	esp_tpacket_t data[ESP_QUEUE_SIZE];															//!array of data

	struct
	{
		size_t size;
		char id;
		char buffer[ESP_PAYLOAD_LENGTH];
	} data[ESP_QUEUE_SIZE];


    size_t size;																				//!count of store data
    size_t write;																				//!pointer to the write position
    size_t read;																				//!pointer to the read position
} 	esp_tqueue_t;
//! @}

typedef enum
{
	ESP_NO_EVT				=	0x00,
	ESP_START_TRANSMIT_EVT	=	0x01,
	EST_TRANSMIT_PASS_EVT	=	0x02,
} 	esp_tcpip_event_t;

//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!Transmit Queue
static esp_tqueue_t tqueue = {0};

static esp_tcpip_event_t tcpip_event = ESP_NO_EVT;

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

//!Pointer of callback function for transmit available handle
#if 0
static esp_message_garbage_fn_t esp_msg_garbage_cb = NULL;
#endif
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
static void esp_tcpip_set_event(esp_tcpip_event_t evt)
{
	tcpip_event = evt;
}

static void esp_tcpip_clr_event(esp_tcpip_event_t evt)
{
	tcpip_event = 0;
}

static bool esp_tcpip_tst_event(esp_tcpip_event_t evt)
{
	return tcpip_event;
}

/**
* This function add data into transmit queue.
*
* Public function defined in esp_queue.h
*/
static bool esp_tbuffer_enqueue(char id, const uint8_t buffer[], size_t size)
{
	if(tqueue.size == ESP_QUEUE_SIZE) {
		return false;
	}

	tqueue.data[tqueue.write].size = size;
	tqueue.data[tqueue.write].id = id;
	if(memcpy(&tqueue.data[tqueue.write].buffer, buffer, size) == NULL) {
		return false;
	}

	tqueue.size++;
	tqueue.write = (tqueue.write == ESP_QUEUE_SIZE - 1ul) ? 0ul: (tqueue.write + 1ul);

	return true;
}

/**
* This function get data from transmit queue.
*
* Public function defined in esp_queue.h
*/
static bool esp_tbuffer_denqueue(char *id, char **buffer, size_t *size)
{
	if(tqueue.size == 0) {
		return false;
	}

	*size = tqueue.data[tqueue.read].size;
	*id = tqueue.data[tqueue.read].id;
	*buffer = tqueue.data[tqueue.read].buffer;

	tqueue.size--;
	tqueue.read = (tqueue.read == ESP_QUEUE_SIZE - 1ul) ? 0ul : (tqueue.read + 1ul);

	return true;
}


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
		WAIT_TRANSMIT_ALLOW,
		TRANSMIT,
		WAIT_TRANSMIT_ACK
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

				state = WAIT_TRANSMIT_ALLOW;
				esp_tcpip_clr_event(ESP_START_TRANSMIT_EVT);
			}
			break;
		case WAIT_TRANSMIT_ALLOW:
			if(esp_tcpip_tst_event(ESP_START_TRANSMIT_EVT))
			{
				state = TRANSMIT;
				esp_tcpip_clr_event(ESP_START_TRANSMIT_EVT);
			}
			break;
		case TRANSMIT:
//			if(esp_data_send(buffer, size) == false)
			if(esp_data_transmit(buffer, size, 0) == false)
			{
				state = PREPARATION;
				return false;
			}

			state = WAIT_TRANSMIT_ACK;
			esp_tcpip_clr_event(EST_TRANSMIT_PASS_EVT);
			break;
		case WAIT_TRANSMIT_ACK:
			if(esp_tcpip_tst_event(EST_TRANSMIT_PASS_EVT))
			{
				esp_tcpip_clr_event(EST_TRANSMIT_PASS_EVT);
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
		WAIT_TRANSMIT_ALLOW,
		TRANSMIT,
		STOP
	}	state = PREPARATION;

	switch(state)
	{
		case PREPARATION:
			if(esp_cmd_transmit(CIPTRANSP, NULL, 0) == false) {
				return false;
			}
			state = WAIT_TRANSMIT_ALLOW;
			break;
		case WAIT_TRANSMIT_ALLOW:
			if(esp_tcpip_tst_event(ESP_START_TRANSMIT_EVT))
			{
				state = TRANSMIT;
				esp_tcpip_clr_event(ESP_START_TRANSMIT_EVT);
			}
			break;
		case TRANSMIT:
			if(esp_tbuffer_denqueue((char*)&id, &buffer, &size))
			{
//				if(esp_data_send(buffer, size) == false) {
				if(esp_data_transmit(buffer, size, 0) == false) {
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
		esp_tcpip_set_event(ESP_START_TRANSMIT_EVT);
	}

	if(esp_pattern_check(msg, PATTERN_SEND_ACK))
	{
		esp_tcpip_set_event(EST_TRANSMIT_PASS_EVT);
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
		esp_tcpip_set_event(ESP_START_TRANSMIT_EVT);
	}
	else
	{
		if(esp_tcpip_receive_cb != NULL) {
			esp_tcpip_receive_cb(/*ESP_ID_NONE*/ESP_ID0, msg, len);
		}
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match((const char*)"STATUS:(\\d)\r\n\\S+CIPSTATUS:(\\d),\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+)\\S+", pAnswer, /*strlen(pAnswer)*/len, caps, 6, 0) <= 0) {
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

#if 0
	(*status).tetype = *caps[6].ptr;
#endif

	return ESP_PASS;
}

/**
* This function establish TCP connection.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_connect(const esp_tcp_cfg_t *cfg, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPCLOSE_M, (char*)&id, 1) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPCLOSE_S, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	esp_conn_mode_t mode = ESP_MULTIPLE_CONNECT;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	esp_conn_mode_t mode = ESP_SINGLE_CONNECT;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMUX, (char*)&mode, 1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK))	{
		return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMUX:(\\d+)\\S*", pAnswer, len, caps, 1, 0) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(CIPMODE, (char*)&mode, (size_t)1ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_BUSY;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
		return ESP_INNER_ERR;
	}

	transfer = mode;

	return ESP_PASS;
}

/**
* This function request transfer mode.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_transmit_mode_request(esp_tx_mode_t *mode, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_PASS;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match("\\S*CIPMODE:(\\d+)\\S*", pAnswer, len, caps, 1, 0) <= 0) {
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
	if(esp_data_transmit("+++", strlen("+++"), 0) == false) {
//	if(esp_data_transmit(buffer, size, 0) == false) {
	   return ESP_TRANSMIT_ERR;
	}

	return ESP_PASS;
}

/**
* This function start TCP server.
*
* Public function defined in esp_tcpip.h
*/
esp_status_t esp_tcp_server_create(uint16_t port, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
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
		if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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
		if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;

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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;

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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
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

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_pattern_check(pAnswer, PATTERN_ERROR)) {
		return 0;
	}

	if (slre_match((const char*)"\\+(\\d+)\r\n", pAnswer, len, caps, 1, 0) <= 0) {
		return 0;
	}

	return convert_string_to_uint32(caps[0].ptr);
}


esp_status_t esp_sntp_timezone_setup(int8_t timezone, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pParam = NULL;
	char* pAnswer = NULL;

	assert(timezone > TIMEZONE_LOW_LIMIT
			&& timezone < TIMEZONE_HIGH_LIMIT);

	pParam = esp_alloc_param_buffer();
	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	pParam[0] = '1';
	pParam[1] = ',';

	if(timezone < 0)
	{
		pParam[2] = '-';
		convert_uint8_to_string(&pParam[3], abs(timezone));
	}
	else
	{
		convert_uint8_to_string(&pParam[2], abs(timezone));
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(SNTPCFG_SETUP, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	while(true)
	{
		if(esp_data_receive(pAnswer, len, timeout) <= 0) {
			return ESP_RECEIVE_ERR;
		}

		if(esp_is_busy(pAnswer)) {
			return ESP_BUSY;
		}

		return (!esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_INNER_ERR : ESP_PASS;
	}

	return ESP_INNER_ERR;
}



esp_status_t esp_get_time(esp_datetime_t* dt, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[7] = {0};
	char* pAnswer = NULL;

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(SNTPTIME, NULL, 0ul) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if(esp_is_busy(pAnswer)) {
		return ESP_PASS;
	}

	if(!esp_pattern_check(pAnswer, PATTERN_OK)) {
	   return ESP_INNER_ERR;
	}

	if (slre_match("\\+CIPSNTPTIME:(\\S+)\\s(\\S+)\\s(\\d+)\\s(\\d+):(\\d+):(\\d+)\\s(\\d+)\\S*", pAnswer, strlen(pAnswer)/*len*/, caps, 7, 0) <= 0) {
	   return false;
	}

	dt->year = convert_string_to_uint16(caps[6].ptr);
	dt->month = convert_month_to_num(caps[1].ptr);
	dt->day = convert_string_to_uint8(caps[2].ptr);
	dt->day_of_week = convert_day_of_week_to_num(caps[0].ptr);
	dt->hour = convert_string_to_uint8(caps[3].ptr);
	dt->min = convert_string_to_uint8(caps[4].ptr);
	dt->sec = convert_string_to_uint8(caps[5].ptr);

	return ESP_PASS;
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

void esp_tcp_receive_handle(void)
{
//	size_t size = 0;
//	char* buffer = NULL;
//
//	if(esp_data_receive(&buffer, &size, 0))
//	{
//		if(transfer == ESP_TRANSPARENT_MODE)
//		{
//			esp_transparent_receive(buffer, size);
//		}
//		else
//		{
//			esp_normal_receive(buffer, size);
//		}
//
//#if 0
//		if(esp_msg_garbage_cb != NULL)
//		{
//			esp_msg_garbage_cb(buffer, size);
//		}
//#endif
//	}
}

bool esp_tcpip_receive_handle(char *msg, size_t size)
{
	if(transfer == ESP_TRANSPARENT_MODE)
	{
		esp_transparent_receive(msg, size);
	}
	else
	{
		esp_normal_receive(msg, size);
	}

	return true;
}




