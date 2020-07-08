// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_wifi.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 Wi-Fi AT Commands.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "esp_wifi.h"

#include <string.h>
#include <assert.h>
#include "esp_utils.h"
#include "slre.h"
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
//_____ D E F I N I T I O N ___________________________________________________
typedef enum
{
	MAX_TRY_COUNT 	=	6,														//Max retry count
}	ESP_WIFI_CONST;
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
/**
* @brief 	This function check the answer from chip during connection to wifi AP.
*
* @pParam	answer[in] - answer from chip.
*
* @return
* <ul>
* <li> <b>AP_CONNECT</b> - Connected to AP.
* <li> <b>AP_DISCONECT</b> - Disconnected from AP.
* <li> <b>ESP_BUSY</b> - ESP8266 busy for now and do not able receive any commands.
* <li> <b>AP_GOT_IP</b> - Got ip address from AP.
* <li> <b>AP_PASS</b> - There are no error.
* <li> <b>AP_ERROR</b> - There are some error.
* </ul>
*/
static inline esp_softap_status_t esp_join_ap_answer_handler(const char answer[])
{
	esp_softap_status_t res = ESP_AP_ERROR;

	if(esp_pattern_check(answer, PATTERN_BUSY_P)) {
		return ESP_AP_BUSY;
	}

	if(esp_pattern_check(answer, PATTERN_BUSY_S)) {
		return ESP_AP_BUSY;
	}

	if(esp_pattern_check(answer, PATTERN_WIFI_DISCONNECT)) {
		res = ESP_AP_DISCONECT;
	}

	if(esp_pattern_check(answer, PATTERN_WIFI_CONNECT)) {
		res =  ESP_AP_CONNECT;
	}

	if(esp_pattern_check(answer, PATTERN_WIFI_ALREADY_CONNECT)) {
		res =  ESP_AP_CONNECT;
	}

	if(esp_pattern_check(answer, PATTERN_WIFI_GOT_IP)) {
		res =  ESP_AP_GOT_IP;
	}

	if(esp_pattern_check(answer, PATTERN_OK)) {
		res =  ESP_AP_PASS;
	}

	return res;
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function setup wifi work mode.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_mode_setup(ESP_WifiMode_t mode, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(MODE, (char*)&mode, 1U) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request wifi work mode.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_mode_request(ESP_WifiMode_t *mode, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
    struct slre_cap caps[1] = {0};
    char* pAnswer = NULL;

    assert(NULL != mode);

    pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(REQMODE, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if(slre_match((const char *)"\\S+:(\\d)\\r\\n\\r\\nOK\\r\\n", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (ESP_WifiMode_t)*caps[0].ptr;

    return true;
}

#else
/**
* This function setup wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_mode_setup(esp_wifi_mode_t mode, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit((save ? MODE_DEF : MODE_CUR), (char*)&mode, 1ul) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_mode_request(esp_wifi_mode_t *mode, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1] = {0};
	char* pAnswer = NULL;

	assert(NULL != mode);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit((save ? REQMODE_DEF : REQMODE_CUR), NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if(slre_match((const char *)"\\S+:(\\d)\\r\\n\\r\\nOK\\r\\n", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (esp_wifi_mode_t)*caps[0].ptr;

	return true;
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function join to the selected WiFi AP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_ap_join(const char ssid[], const char pass[], uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	uint8_t tryCount = MAX_TRY_COUNT;
	esp_softap_status_t status;
	esp_status_t result = ESP_INNER_ERR;
	char* pAnswer = NULL;
	char* pParam = NULL;

	assert(NULL != ssid);
	assert(NULL != pass);

	pAnswer = esp_alloc_answer_buffer();
	pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(JOIN, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	do
	{
		if(esp_data_receive(pAnswer, len, timeout) <= 0) {
			return ESP_INNER_ERR;
		}

		status = ESP_JoinToWifiApAnswerHandle(pAnswer);
		switch(status)
		{
			case ESP_AP_DISCONECT:
				result = ESP_CONNECTION_ERR;
				break;
			case ESP_AP_ERROR:
			case ESP_AP_BUSY:
				result = ESP_PASS;
				break;
			case ESP_AP_CONNECT:
			case ESP_AP_GOT_IP:
				result = ESP_PASS;
				break;
			case ESP_AP_PASS:
				return ESP_ESP_PASS;
				break;
		}
		tryCount--;
	}
	while(tryCount > 0 && result != ESP_PASS);

	return result;
}
#else

/**
* This function join to the selected WiFi AP.
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_ap_join(const char ssid[], const char pass[], bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	uint8_t tryCount = MAX_TRY_COUNT;
	esp_softap_status_t status;
	esp_status_t result = ESP_INNER_ERR;
	char* pAnswer = NULL;
	char* pParam = NULL;

	assert(NULL != ssid);
	assert(NULL != pass);

	pAnswer = esp_alloc_answer_buffer();
	pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? JOIN_DEF : JOIN_CUR), pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	do
	{
		if(esp_data_receive(pAnswer, ESP_ANSWER_BUFF_SIZE, timeout) <= 0) {
			return ESP_INNER_ERR;
		}

		status = esp_join_ap_answer_handler(pAnswer);
		switch(status)
		{
			case ESP_AP_DISCONECT:
				result = ESP_CONNECTION_ERR;
				break;
			case ESP_AP_ERROR:
			case ESP_AP_BUSY:
				result = ESP_INNER_ERR;
				break;
			case ESP_AP_CONNECT:
			case ESP_AP_GOT_IP:
				result = ESP_PASS;
				break;
			case ESP_AP_PASS:
				return ESP_PASS;
				break;
		}
		tryCount--;
	}
	while(tryCount > 0 && result != ESP_PASS);

	return result;
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function request SSID of AP to which we are now connected.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_ap_ssid_request(char ssid[], uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != ssid);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit(REQSSID, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_INNER_ERR;
	}

	if(slre_match((const char *)"\\S+:\"(\\S+)\",\\S+,\\d+,\\S+", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0)
	{
		if(esp_pattern_check(pAnswer, PATTERN_NO_AP))
		{
			memset(ssid, '0', 32);
			return ESP_PASS;
		}
		return ESP_INNER_ERR;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return ESP_INNER_ERR;
	}

	memcpy((void*)ssid, (void*)caps[0].ptr, caps[0].len);

	return ESP_PASS;
}
#else

/**
* This function request SSID of AP to which we are now connected.
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_ap_ssid_request(char ssid[], bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != ssid);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit((save ? REQSSID_DEF : REQSSID_CUR), NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_INNER_ERR;
	}

	if(slre_match((const char *)"\\S+:\"(\\S+)\",\\S+,\\d+,\\S+", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0)
	{
		if(esp_pattern_check(pAnswer, PATTERN_NO_AP))
		{
			memset(ssid, '0', 32);
			return ESP_PASS;
		}
		return ESP_INNER_ERR;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return ESP_INNER_ERR;
	}

	memcpy((void*)ssid, (void*)caps[0].ptr, caps[0].len);

	return ESP_PASS;
}
#endif

/**
* This function unjoin from the current WiFi AP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_ap_unjoin(uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(UNJOIN, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function create software WiFi AP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_softap_cfg(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = NULL;
	char* pParam = NULL;

	assert(NULL != ssid);
	assert(NULL != pass);

	pAnswer = esp_alloc_answer_buffer();
	pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)pParam);
	pParam[len++] = ',';
	pParam[len++] = channel;
	pParam[len++] = ',';
	pParam[len++] = enc;
	pParam[len] = 0;
	len = strlen((char*)pParam);

	if(esp_cmd_transmit(AP, pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_INNER_ERR;
	}

	return (esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_PASS : ESP_INNER_ERR;
}
#else

/**
* This function create software WiFi AP.
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_softap_setup(const char ssid[], const char pass[], char channel, char enc, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = NULL;
	char* pParam = NULL;

	assert(NULL != ssid);
	assert(NULL != pass);

	pAnswer = esp_alloc_answer_buffer();
	pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)pParam);
	pParam[len++] = ',';
	pParam[len++] = channel;
	pParam[len++] = ',';
	pParam[len++] = enc;
	pParam[len] = 0;
	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? AP_DEF : AP_CUR), pParam, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_INNER_ERR;
	}

	return (esp_pattern_check(pAnswer, PATTERN_OK)) ? ESP_PASS : ESP_INNER_ERR;
}
#endif

/**
* This function request he configuration parameters of the SoftAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_request(char ssid[], char pass[], esp_ap_config_t *pParam, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[6];
	char* pAnswer = NULL;

	assert(NULL != ssid);
	assert(NULL != pass);
	assert(NULL != pParam);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(AP_REQ, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+:\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+),(\\d+),(\\d+)\r\n\r\nOK\r\n", pAnswer, /*strlen(pAnswer)*/len, caps, 6, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0) ||
	   (caps[3].ptr == NULL || caps[3].len == 0) ||
	   (caps[4].ptr == NULL || caps[4].len == 0) ||
	   (caps[5].ptr == NULL || caps[5].len == 0))
	{
		return false;
	}

	memcpy((void*)ssid, (void*)caps[0].ptr, caps[0].len);
	memcpy((void*)pass, (void*)caps[1].ptr, caps[1].len);
	memcpy((void*)&pParam->channel, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&pParam->enc, (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&pParam->hidden, (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&pParam->max_con, (void*)caps[5].ptr, caps[5].len);

	return true;
}

/**
* This function get station�s IP which is connected to ESP8266 softAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_get_ip_of_connected_station(ip4addr_t *ipv4, mac_t *mac, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[2];
	char* pAnswer = NULL;

	assert(NULL != ipv4);
	assert(NULL != mac);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(CWLIF, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S*(\\S+),(\\S+)\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 2, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_ip4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	return true;
}

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function enable/disable of DHCP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_dhcp_mode_setup(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	pParam[0] = mode;
	pParam[1] = ',';
	pParam[2] = dhcp;

	len = strlen(pParam);

	if(esp_cmd_transmit(CWDHCP, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#else

/**
* This function enable/disable of DHCP.
*
* Public function defined in esp_wifi.h
*/
bool esp_dhcp_mode_setup(esp_wifi_mode_t mode, esp_dhcp_mode_t dhcp, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;

	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	pParam[0] = mode;
	pParam[1] = ',';
	pParam[2] = dhcp;

	len = strlen(pParam);

	if(esp_cmd_transmit((save ? CWDHCP_DEF : CWDHCP_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, ESP_ANSWER_BUFF_SIZE, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#endif

/**
* This function enable connect to AP automatically when power on.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_autoconnect_enable(uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char esp_enable = '1';

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(CWAUTOCONN, &esp_enable, 1) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function disable connect to AP automatically when power on.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_autoconnect_disable(uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char esp_disable = '0';

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(CWAUTOCONN, &esp_disable, 1) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function setup MAC address of station.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_mac_setup(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_mac_addr_to_string(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSTAMAC, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#else

/**
* This function setup MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_mac_setup(mac_t mac, bool save, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_mac_addr_to_string(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? CIPSTAMAC_DEF : CIPSTAMAC_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function request MAC address of station.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_mac_request(mac_t *mac, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != mac);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(REQSTAMAC, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_mac_addr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}
#else

/**
* This function request MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_mac_request(mac_t *mac, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != mac);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit((save ? REQSTAMAC_DEF : REQSTAMAC_CUR), NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_mac_addr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function setup MAC address of softAP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_mac_setup(mac_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_mac_addr_to_string(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPAPMAC, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive_block((uint8_t*)pAnswer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#else

/**
* This function setup MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_mac_setup(mac_t mac, bool save, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_mac_addr_to_string(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? CIPAPMAC_DEF : CIPAPMAC_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function request MAC address of softAP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_mac_request(mac_t *mac, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != mac);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(REQSTAMAC, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_mac_addr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}
#else

/**
* This function request MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_mac_request(mac_t *mac, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[1];
	char* pAnswer = NULL;

	assert(NULL != mac);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit((save ? REQSTAMAC_DEF : REQSTAMAC_CUR), NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_mac_addr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function setup IP address of station.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_ip_setup(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPSTA, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request IP address of station.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_ip_request(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[3];
	char* pAnswer = NULL;

	assert(NULL != ipv4);
	assert(NULL != gw);
	assert(NULL != mask);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(REQSTA, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_ip4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!convert_string_to_ip4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!convert_string_to_ip4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}
#else

/**
* This function setup IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_station_ip_setup(ip4addr_t ipv4, ip4addr_t gw, ip4addr_t mask, bool save, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_ip4addr_to_string(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!convert_ip4addr_to_string(gw, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!convert_ip4addr_to_string(mask, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? CIPSTA_DEF : CIPSTA_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request IP address of station.
*
* Public function defined in esp_wifi.h
*/
esp_status_t esp_wifi_station_ip_request(ip4addr_t *ipv4, ip4addr_t *gw, ip4addr_t *mask, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[3];
	char* pAnswer = NULL;

	assert(NULL != ipv4);
	assert(NULL != gw);
	assert(NULL != mask);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return ESP_MEM_ALLOC_ERR;
	}

	if(esp_cmd_transmit((save ? REQSTA_DEF : REQSTA_CUR), NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return ESP_RECEIVE_ERR;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, strlen(pAnswer)/*len*/, caps, 3, 0) <= 0) {
	   return ESP_PATTERN_ERR;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_ip4addr(ipv4, caps[0].ptr)) {
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!convert_string_to_ip4addr(gw, caps[1].ptr)) {
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!convert_string_to_ip4addr(mask, caps[2].ptr)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}
#endif



#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function setup IP address of softAP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_ip_setup(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit(CIPAP, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request IP address of softAP.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_ip_request(Ipv4Addr_t *ipv4, Ipv4Addr_t *gw, Ipv4Addr_t *mask, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[3];
	char* pAnswer = NULL;

	assert(NULL != ipv4);
	assert(NULL != gw);
	assert(NULL != mask);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit(REQCIPAP, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_ip4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!convert_string_to_ip4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!convert_string_to_ip4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}

#else

/**
* This function setup IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_ip_setup(ip4addr_t ipv4, ip4addr_t gw, ip4addr_t mask, bool save, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();
	char* pParam = esp_alloc_param_buffer();

	if(NULL == pAnswer || NULL == pParam) {
		return false;
	}

	if(!convert_ip4addr_to_string(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!convert_ip4addr_to_string(gw, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!convert_ip4addr_to_string(mask, ip)) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)pParam, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? CIPAP_DEF : CIPAP_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function request IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool esp_wifi_softap_ip_request(ip4addr_t* ipv4, ip4addr_t* gw, ip4addr_t* mask, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	struct slre_cap caps[3];
	char* pAnswer = NULL;

	assert(NULL != ipv4);
	assert(NULL != gw);
	assert(NULL != mask);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer) {
		return false;
	}

	if(esp_cmd_transmit((save ? REQCIPAP_DEF : REQCIPAP_CUR), NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", pAnswer, /*strlen(pAnswer)*/len, caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!convert_string_to_ip4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!convert_string_to_ip4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!convert_string_to_ip4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}
#endif


bool esp_wifi_receive_handle(char *msg, size_t size)
{
	return false;
}
