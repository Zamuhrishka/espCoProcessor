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
#include <esp_port.h>
#include <string.h>
#include <stdio.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "esp_utils.h"
#include "esp_queue.h"
#include "slre.h"
#include "convert.h"
//_____ D E F I N I T I O N ___________________________________________________
typedef enum
{
	MAX_TRY_COUNT 	=	6,														//Max retry count
}	ESP_WIFI_CONST;
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
const static char ESP_ENABLE = '1';
const static char ESP_DISABLE = '0';
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
/**
* @brief 	This function check the answer from chip during connection to wifi AP.
*
* @param	answer[in] - answer from chip.
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
static inline ESP_JoinApStatus_t ESP_JoinToWifiApAnswerHandle(const char answer[])
{
	ESP_JoinApStatus_t res = AP_ERROR;

	if(ESP_PatternCheck(answer, pattern_BUSY_P)) {
		return AP_BUSY;
	}

	if(ESP_PatternCheck(answer, pattern_BUSY_S)) {
		return AP_BUSY;
	}

	if(ESP_PatternCheck(answer, pattern_WIFI_DISCONNECT)) {
		res = AP_DISCONECT;
	}

	if(ESP_PatternCheck(answer, pattern_WIFI_CONNECT)) {
		res =  AP_CONNECT;
	}

	if(ESP_PatternCheck(answer, pattern_WIFI_ALREADY_CONNECT)) {
		res =  AP_CONNECT;
	}

	if(ESP_PatternCheck(answer, pattern_WIFI_GOT_IP)) {
		res =  AP_GOT_IP;
	}

	if(ESP_PatternCheck(answer, pattern_OK)) {
		res =  AP_PASS;
	}

	return res;
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function setup wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiMode(ESP_WifiMode_t mode, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(MODE, (char*)&mode, 1U) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiMode(ESP_WifiMode_t *mode, uint32_t timeout)
{
    struct slre_cap caps[1];
    char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQMODE, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	   return false;
	}

	if (slre_match((const char *)"\\S+:(\\d)\\r\\n\\r\\nOK\\r\\n", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (ESP_WifiMode_t)*caps[0].ptr;

    return true;
}

/**
* This function setup wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiModeCur(ESP_WifiMode_t mode, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(MODE_CUR, (char*)&mode, 1ul) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	   return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiModeCur(ESP_WifiMode_t *mode, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQMODE_CUR, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	   return false;
	}

	if (slre_match((const char *)"\\S+:(\\d)\\r\\n\\r\\nOK\\r\\n", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (ESP_WifiMode_t)*caps[0].ptr;

	return true;
}

/**
* This function setup wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiModeDef(ESP_WifiMode_t mode, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(MODE_DEF, (char*)&mode, 1ul) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	   return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request wifi work mode.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiModeDef(ESP_WifiMode_t *mode, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQMODE_DEF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	   return false;
	}

	if (slre_match((const char *)"\\S+:(\\d)\\r\\n\\r\\nOK\\r\\n", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	*mode = (ESP_WifiMode_t)*caps[0].ptr;

	return true;
}

/**
* This function join to the selected WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_JoinToWifiAp(const char ssid[], const char pass[], uint32_t timeout)
{
	size_t len = 0;
	uint8_t tryCount = MAX_TRY_COUNT;
	ESP_JoinApStatus_t status;
	ESPStatus_t result = ESP_INNER_ERR;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(JOIN, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	do
	{
		if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
			return ESP_RECEIVE_ERR;
		}

		status = ESP_JoinToWifiApAnswerHandle(answer);
		switch(status)
		{
			case AP_DISCONECT:
				result = ESP_CONNECTION_ERR;
				break;
			case AP_ERROR:
			case AP_BUSY:
				result = ESP_PASS;
				break;
			case AP_CONNECT:
			case AP_GOT_IP:
				result = ESP_PASS;
				break;
			case AP_PASS:
				return ESP_PASS;
				break;
		}
		tryCount--;
	}
	while(tryCount > 0 && result != ESP_PASS);

	return result;
}

/**
* This function join to the selected WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_JoinToWifiApCur(const char ssid[], const char pass[], uint32_t timeout)
{
	size_t len = 0;
	uint8_t tryCount = MAX_TRY_COUNT;
	ESP_JoinApStatus_t status;
	ESPStatus_t result = ESP_INNER_ERR;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(JOIN_CUR, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	do
	{
		if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
			return ESP_RECEIVE_ERR;
		}

		status = ESP_JoinToWifiApAnswerHandle(answer);
		switch(status)
		{
			case AP_DISCONECT:
				result = ESP_CONNECTION_ERR;
				break;
			case AP_ERROR:
			case AP_BUSY:
				result = ESP_INNER_ERR;
				break;
			case AP_CONNECT:
			case AP_GOT_IP:
				result = ESP_PASS;
				break;
			case AP_PASS:
				return ESP_PASS;
				break;
		}
		tryCount--;
	}
	while(tryCount > 0 && result != ESP_PASS);

	return result;
}

/**
* This function join to the selected WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_JoinToWifiApDef(const char ssid[], const char pass[], uint32_t timeout)
{
	size_t len = 0;
	uint8_t tryCount = MAX_TRY_COUNT;
	ESP_JoinApStatus_t status;
	ESPStatus_t result = ESP_INNER_ERR;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(JOIN_DEF, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	do
	{
		if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
			return ESP_RECEIVE_ERR;
		}

		status = ESP_JoinToWifiApAnswerHandle(answer);
		switch(status)
		{
			case AP_DISCONECT:
				result = ESP_CONNECTION_ERR;
				break;
			case AP_ERROR:
			case AP_BUSY:
				result = ESP_PASS;
				break;
			case AP_CONNECT:
			case AP_GOT_IP:
				result = ESP_PASS;
				break;
			case AP_PASS:
				return ESP_PASS;
				break;
		}
		tryCount--;
	}
	while(tryCount > 0 && result != ESP_PASS);

	return result;
}

/**
* This function request SSID of AP to which we are now connected.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_RequestNameConnectedAp(char ssid[], uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(REQSSID, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(slre_match((const char *)"\\S+:\"(\\S+)\",\\S+,\\d+,\\S+", answer, strlen(answer), caps, 1, 0) <= 0)
	{
		if(ESP_PatternCheck(answer, pattern_NO_AP))
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

/**
* This function request SSID of AP to which we are now connected.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_RequestNameConnectedApCur(char ssid[], uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(REQSSID_CUR, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(slre_match((const char *)"\\S+:\"(\\S+)\",\\S+,\\d+,\\S+", answer, strlen(answer), caps, 1, 0) <= 0)
	{
		if(ESP_PatternCheck(answer, pattern_NO_AP))
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

/**
* This function request SSID of AP to which we are now connected.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_RequestNameConnectedApDef(char ssid[], uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return ESP_INNER_ERR;
	}

	if(ESP_SendAtCmd(REQSSID_DEF, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if(slre_match((const char *)"\\S+:\"(\\S+)\",\\S+,\\d+,\\S+", answer, strlen(answer), caps, 1, 0) <= 0)
	{
		if(ESP_PatternCheck(answer, pattern_NO_AP))
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

/**
* This function unjoin from the current WiFi AP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_UnJoinFromWifiAp(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(UNJOIN, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function create software WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_CreateWifiAp(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout)
{
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);
	param[len++] = ',';
	param[len++] = channel;
	param[len++] = ',';
	param[len++] = enc;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(AP, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	return (ESP_PatternCheck(answer, pattern_OK)) ? ESP_PASS : ESP_INNER_ERR;
}

/**
* This function create software WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_CreateWifiApCur(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout)
{
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);
	param[len++] = ',';
	param[len++] = channel;
	param[len++] = ',';
	param[len++] = enc;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(AP_CUR, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	return (ESP_PatternCheck(answer, pattern_OK)) ? ESP_PASS : ESP_INNER_ERR;
}

/**
* This function create software WiFi AP.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_CreateWifiApDef(const char* ssid[], const char* pass[], char channel, char enc, uint32_t timeout)
{
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	if((ssid == NULL) || (pass == NULL)) {
		return ESP_PARAM_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)ssid) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)pass) == NULL) {
		return ESP_INNER_ERR;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return ESP_INNER_ERR;
	}

	len = strlen((char*)param);
	param[len++] = ',';
	param[len++] = channel;
	param[len++] = ',';
	param[len++] = enc;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(AP_DEF, param, len) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	return (ESP_PatternCheck(answer, pattern_OK)) ? ESP_PASS : ESP_INNER_ERR;
}

/**
* This function request he configuration parameters of the SoftAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestSoftApParamCur(char ssid[], char pass[], ESP_ApParam_t *param, uint32_t timeout)
{
	struct slre_cap caps[6];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(AP_REQ, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+:\"(\\S+)\",\"(\\S+)\",(\\d+),(\\d+),(\\d+),(\\d+)\r\n\r\nOK\r\n", answer, strlen(answer), caps, 6, 0) <= 0) {
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
	memcpy((void*)&param->channel, (void*)caps[2].ptr, caps[2].len);
	memcpy((void*)&param->enc, (void*)caps[3].ptr, caps[3].len);
	memcpy((void*)&param->hidden, (void*)caps[4].ptr, caps[4].len);
	memcpy((void*)&param->max_con, (void*)caps[5].ptr, caps[5].len);

	return true;
}

/**
* This function get stationís IP which is connected to ESP8266 softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_GetIpOfConnectedStation(Ipv4Addr_t *ipv4, MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[2];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(CWLIF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S*(\\S+),(\\S+)\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 2, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	return true;
}

/**
* This function enable/disable of DHCP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_ConfigDhcpMode(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout)
{
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	param[0] = mode;
	param[1] = ',';
	param[2] = dhcp;

	len = strlen(param);

	if(ESP_SendAtCmd(CWDHCP, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function enable/disable of DHCP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_ConfigDhcpModeCur(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout)
{
	size_t len = 0;

	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	param[0] = mode;
	param[1] = ',';
	param[2] = dhcp;

	len = strlen(param);

	if(ESP_SendAtCmd(CWDHCP_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function enable/disable of DHCP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_ConfigDhcpModeDef(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout)
{
	size_t len = 0;

	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	param[0] = mode;
	param[1] = ',';
	param[2] = dhcp;

	len = strlen(param);

	if(ESP_SendAtCmd(CWDHCP_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function enable connect to AP automatically when power on.
*
* Public function defined in esp_wifi.h
*/
bool ESP_AutoconnectEnable(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(CWAUTOCONN, &ESP_ENABLE, 1) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function disable connect to AP automatically when power on.
*
* Public function defined in esp_wifi.h
*/
bool ESP_AutoconnectDisable(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(CWAUTOCONN, &ESP_DISABLE, 1) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function setup MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupStationMacAddr(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTAMAC, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		 return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function setup MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool SetupStationMacAddrCur(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTAMAC_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function setup MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool SetupStationMacAddrDef(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTAMAC_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestStationMacAddr(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function request MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestStationMacAddrCur(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC_CUR, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function request MAC address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestStationMacAddrDef(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC_DEF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function setup MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupSoftApMacAddr(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAPMAC, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function setup MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupSoftApMacAddrCur(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAPMAC_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function setup MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupSoftApMacAddrDef(MacAddr_t mac, uint32_t timeout)
{
	char _mac[30] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_MacAddrToString(&mac, _mac)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)_mac) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAPMAC_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestSoftApMacAddr(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function request MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestSoftApMacAddrCur(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC_CUR, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function request MAC address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestSoftApMacAddrDef(MacAddr_t *mac, uint32_t timeout)
{
	struct slre_cap caps[1];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTAMAC_DEF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 1, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0)) {
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToMacAddr(caps[0].ptr, mac)) {
		return false;
	}

	return true;
}

/**
* This function setup IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiStationIpAddr(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTA, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiStationIpAddr(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTA, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}

/**
* This function setup IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiStationIpAddrCur(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTA_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of station.
*
* Public function defined in esp_wifi.h
*/
ESPStatus_t ESP_RequestWifiStationIpAddrCur(Ipv4Addr_t *ipv4, Ipv4Addr_t *gw, Ipv4Addr_t *mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTA_CUR, NULL, 0) == false) {
		return ESP_TRANSMIT_ERR;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return ESP_RECEIVE_ERR;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return ESP_PATTERN_ERR;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return ESP_INNER_ERR;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return ESP_INNER_ERR;
	}

	return ESP_PASS;
}

/**
* This function setup IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiStationIpAddrDef(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPSTA_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of station.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiStationIpAddrDef(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQSTA_DEF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}

/**
* This function setup IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiApIpAddr(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAP, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiApIpAddr(Ipv4Addr_t *ipv4, Ipv4Addr_t *gw, Ipv4Addr_t *mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQCIPAP, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}

/**
* This function setup IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiApIpAddrCur(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAP_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiApIpAddrCur(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQCIPAP_CUR, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}

/**
* This function setup IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_SetupWifiApIpAddrDef(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout)
{
	char ip[20] = {0};
	size_t len = 0;
	char* answer = ESP_AllocAnswerBuffer();
	char* param = ESP_AllocParamBuffer();

	if(answer == NULL || param == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(ipv4, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(gw, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\",\"\0") == NULL) {
		return false;
	}

	if(!Convert_Ipv4addrToString(mask, ip)) {
		return false;
	}

	if(strcat ((char*)param, (char*)ip) == NULL) {
		return false;
	}

	if(strcat ((char*)param, (char*)"\"\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);

	if(ESP_SendAtCmd(CIPAP_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function request IP address of softAP.
*
* Public function defined in esp_wifi.h
*/
bool ESP_RequestWifiApIpAddrDef(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout)
{
	struct slre_cap caps[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(REQCIPAP_DEF, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		  return false;
	}

	if (slre_match((const char *)"\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\\S+\"(\\S+)\"\r\n\r\nOK\r\n\\S*", answer, strlen(answer), caps, 3, 0) <= 0) {
	   return false;
	}

	if((caps[0].ptr == NULL || caps[0].len == 0) ||
	   (caps[1].ptr == NULL || caps[1].len == 0) ||
	   (caps[2].ptr == NULL || caps[2].len == 0))
	{
		return false;
	}

	memset((void *)(caps[0].ptr + caps[0].len),0,1);
	if(!Convert_StringToIpv4addr(ipv4, caps[0].ptr)) {
		return false;
	}

	memset((void *)(caps[1].ptr + caps[1].len),0,1);
	if(!Convert_StringToIpv4addr(gw, caps[1].ptr)) {
		return false;
	}

	memset((void *)(caps[2].ptr + caps[2].len),0,1);
	if(!Convert_StringToIpv4addr(mask, caps[2].ptr)) {
		return false;
	}

	return true;
}
