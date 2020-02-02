// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_utils.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		This file contains the some support function.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "esp_utils.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "esp_queue.h"
#include "esp_port.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!@brief List of support AT commands
//! @{
static char* AT_CMD[] =
{
	//BASIC
	"AT\0",																		//!Test AT startup
	"+RST\0",																	//!Reset module
	"+GMR\0",																	//!Request Firmware Version
	"+GSLP=\0",																	//!Enter deep-sleep mode
	"E\0",																		//!AT commands echo or not
	"+RESTORE\0",																//!Factory Reset
	"+UART=\0",																	//!UART configuration
	"+UART_CUR=\0",																//!UART current configuration
	"+UART_DEF=\0",																//!UART default configuration, save to flash
	"+SLEEP:\0",																//!Sleep mode
	"+WAKEUPGPIO=\0",															//!Configures a GPIO to Wake ESP8266 up from Light-sleep Mode
	"+RFPOWER=\0",																//!Sets the Maximum Value of RF TX Power
	"+SYSMSG_CUR=\0",															//!Set Current System Messages
	"+SYSMSG_DEF=\0",															//!Set Default System Messages

	//WI-FI
	"+CWMODE=\0",																//!WIFI mode Station/SoftAP/Station+SoftAP. This API is deprecated.
	"+CWMODE?\0",																//!Request the WiFi work mode.
	"+CWMODE_CUR=\0",															//!WiFi Station/SoftAP/Station+SoftAP save to Flash.
	"+CWMODE_CUR?\0",															//!Request the WiFi work mode.
	"+CWMODE_DEF=\0",															//!WiFi default Station/SoftAP/Station+SoftAP.Save to Flash.
	"+CWMODE_DEF?\0",															//!Request the WiFi work mode.
	"+CWJAP=\0",																//!Connect to AP. This API is deprecated.
	"+CWJAP?\0",																//!Request the SSID.
	"+CWJAP_CUR=\0",															//!Connect to AP, won’t save to Flash.
	"+CWJAP_CUR?\0",															//!Request the SSID.
	"+CWJAP_DEF=\0",															//!Connect to AP, save to Flash.
	"+CWJAP_DEF?\0",															//!Request the SSID which we connected.
	"+CWLAP\0",																	//!Lists available APs.
	"+CWQAP\0",																	//!Disconnect from AP.
	"+CWSAP=\0",																//!Set configuration of ESP8266 softAP. This API is deprecated.
	"+CWSAP_CUR=\0",															//!Set configuration of ESP8266 softAP.Won’t save to Flash.
	"+CWSAP_DEF=\0",															//!Set configuration of ESP8266 softAP.Save to Flash.
	"+CWSAP_CUR?\0",
	"+CWLIF\0",																	//!Get station’s IP which is connected to ESP8266 softAP.
	"+CWDHCP=\0",																//!Enable/Disable DHCP. This API is deprecated.
	"+CWDHCP_CUR=\0",															//!Enable/Disable DHCP, won’t save to Flash.
	"+CWDHCP_DEF=\0",															//!Enable/Disable DHCP, save to Flash.
	"+CWDHCPS_CUR=\0",
	"+CWAUTOCONN=",																//!Connect to AP automatically when power on.
	"+CIPSTAMAC=\0",															//!Set mac address of ESP8266 station. This API is deprecated.
	"+CIPSTAMAC?\0",															//!Request the MAC address of station.
	"+CIPSTAMAC_CUR=\0",														//!Set mac address of ESP8266 station. Won’t save to Flash.
	"+CIPSTAMAC_CUR?\0",														//!Request the MAC address of station.
	"+CIPSTAMAC_DEF=\0",														//!Set mac address of ESP8266 station.Save to Flash.
	"+CIPSTAMAC_DEF?\0",														//!Request the MAC address of station.
	"+CIPAPMAC=\0",																//!Set mac address of ESP8266 softAP. This API is deprecated.
	"+CIPAPMAC?\0",																//!Request the MAC address of softAP.
	"+CIPAPMAC_CUR=\0",															//!Set mac address of ESP8266 softAP.Won’t save to Flash.
	"+CIPAPMAC_CUR?\0",															//!Request the MAC address of softAP.
	"+CIPAPMAC_DEF=\0",															//!Set mac address of ESP8266 softAP.Save to Flash.
	"+CIPAPMAC_DEF?\0",															//!Request the MAC address of softAP.
	"+CIPSTA=\0",																//!Set IP address of ESP8266 station. This API is deprecated.
	"+CIPSTA?\0",																//!Request the MAC address of station.
	"+CIPSTA_CUR=\0",															//!Set IP address of ESP8266 station.Won’t save to Flash.
	"+CIPSTA_CUR?\0",															//!Request the MAC address of station.
	"+CIPSTA_DEF=\0",															//!Set IP address of ESP8266 station.Save to Flash.
	"+CIPSTA_DEF?\0",															//!Request the MAC address of station.
	"+CIPAP=\0",																//!Set IP address of ESP8266 softAP. This API is deprecated.
	"+CIPAP?\0",																//!Request the MAC address of softAP.
	"+CIPAP_CUR=\0",															//!Set IP address of ESP8266 softAP.Won’t save to Flash.
	"+CIPAP_CUR?\0",															//!Request the MAC address of softAP.
	"+CIPAP_DEF=\0",															//!Set IP address of ESP8266 softAP.Save to Flash.
	"+CIPAP_DEF?\0",															//!Request the MAC address of softAP.

	//TCP-IP
	"+CIPSTATUS\0",																//!Get connection status.
	"+CIPSTART=\0",																//!Establish TCP connection or register UDP port.
	"+CIPSEND=\0",																//!Send data.
	"+CIPSEND\0",																//!Transparent Transmission.
	"+CIPSENDEX=\0",															//!Send data, if <length> or “\0” is met, data will be sent.
	"+CIPSENDBUF=\0",															//!Write data into TCP-send-buffer.
	"+CIPBUFRESET\0",															//!Reset segment ID count in single mode.
	"+CIPBUFRESET=\0",															//!Reset segment ID count in multi mode.
	"+CIPBUFSTATUS\0",															//!Check status of TCP-send-buffer in single mode.
	"+CIPBUFSTATUS=\0",															//!Check status of TCP-send-buffer in multi mode.
	"+CIPCHECKSEG=\0",															//!Check if a specific segment is sent or not.
	"+CIPCLOSE\0",																//!Close Single TCP/UDP connection.
	"+CIPCLOSE=\0",																//!Close Multi TCP/UDP connection.
	"+CIFSR\0",																	//!Get local IP address.
	"+CIPMUX=\0",																//!Set multiple connections mode.
	"+CIPMUX?\0",																//!Request multiple connections mode.
	"+CIPSERVER=\0",															//!Configure as server.
	"+CIPMODE=\0",																//!Set transmission mode.
	"+CIPMODE?\0",																//!Request transmission mode.
	"+SAVETRANSLINK=\0",														//!Save transparent transmission link to Flash.
	"+CIPSTO=\0",																//!Set timeout when ESP8266 runs as TCP server.
	"+CIPUPDATE:\0",															//!Upgrade firmware through network.
	"+PING=\0",																	//!Function PING.
	"+CIPDOMAIN=\0",															//!DNS Function.
	"+CIPSERVERMAXCONN=\0",														//!Set the Maximum Connections Allowed by Server
	"+CIPSNTPCFG=\0",															//!Sets the Configuration of SNTP
	"/0",
};
//! @}

//!@brief List of support message patterns
//! @{
char* ESP_Patterns[] =
{
	"OK\r\n\0",
	"\r\nERROR\r\n\0",
	"WIFI CONNECTED\r\n\0",
	"WIFI DISCONNECT\r\n\0",
	"ALREADY CONNECT\r\n\0",
	"WIFI GOT IP\r\n\0",
	"No Ap\r\n\0",
	"\r\nbusy p...\r\n\0",
	"\r\nbusy s...\r\n\0",
	"\r\nOK\r\n> \0",
	"\r\nOK\r\n\r\n>\0",
	"+IPD,\0",
	"ALREAY CONNECT\0",
	"CLOSED\r\n\0",
	"CONNECT\r\n\0",
	"SEND OK\0",
	"+++",
};
//! @}

//!Buffer for store AT commands ready for transmit
static char transmitAT[200] = {0};

//!Buffer for receive answer from esp8266
static char answer[ESP_ANSWER_BUFF_SIZE] = {0};

//!Buffer for store params for AT commands
static char param[ESP_PARAM_BUFF_SIZE] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function highlights the payload of AT answer.
*
* Public function defined in esp_utils.h
*/
bool esp_get_at_payload(uint8_t cmd, char* src, uint8_t* payload)
{
	char cmdStr[24] = {0};
	char* ptr = AT_CMD[cmd];
	char* sposP = NULL;
	char* eposP = NULL;
	char spos = 0;
	char epos = 0;
	uint8_t i = 2;

	cmdStr[0] = 'A';
	cmdStr[1] = 'T';

	while((*ptr) != '\0') {
		cmdStr[i++] = *ptr++;
	}

	eposP = strstr(src, ESP_Patterns[PATTERN_OK]);
	if(eposP == 0) {
		return false;
	}
	epos = eposP - src;

	sposP = strstr(src, cmdStr);
	if(sposP != 0)
	{
		spos = ((sposP - src) + (i + 3ul));
	}
	else
	{
		sposP = src;
		spos = 2ul;
	}

	memcpy (payload, (sposP + spos), (epos - spos - 2ul));
	return true;
}

/**
* This function send AT command to module.
*
* Public function defined in esp_utils.h
*/
bool esp_at_cmd_send(uint8_t cmd, const char data[], size_t size)
{
	uint8_t i = 2;
	char* ptr = AT_CMD[cmd];

	memset((void*)transmitAT, 0, sizeof(transmitAT));

	transmitAT[0] = 'A';
	transmitAT[1] = 'T';

	while((*ptr) != '\0') {
		transmitAT[i++] = *ptr++;
	}

	if((size != 0) && (data != NULL))
	{
		for(uint8_t j = 0; j < size; j++) {
			transmitAT[i++] = data[j];
		}
	}
	transmitAT[i++] = '\r';
	transmitAT[i++] = '\n';
	esp_hardware_transmit_block(transmitAT, i);
	return true;
}

/**
* This function send raw AT command to module.
*
* Public function defined in esp_utils.h
*/
bool esp_at_raw_cmd_send(const char cmd[], const char data[], size_t size)
{
	uint8_t i = 2;

	memset((void*)transmitAT, 0, sizeof(transmitAT));

	transmitAT[0] = 'A';
	transmitAT[1] = 'T';

	while(cmd[i-2] != ' ')
	{
		transmitAT[i] = cmd[i-2];
		i++;
	}

	if((size != 0) && (data != NULL)) {
		for(uint8_t j = 0; j < size; i++) {
			transmitAT[i] = data[j];
			i++;
		}
	}
	transmitAT[i++] = 0x0D;
	esp_hardware_transmit_block(transmitAT, i);
	return true;
}

/**
* This function send raw data to module.
*
* Public function defined in esp_utils.h
*/
bool esp_data_send(const char data[], size_t size)
{
	esp_hardware_transmit_block(data, size);
	return true;
}

/**
* This function check available selected pattern
* in message.
*
* Public function defined in esp_utils.h
*/
bool esp_pattern_check(const char msg[], esp_pattern_list_t pattern)
{
	return (strstr(msg, ESP_Patterns[pattern]) != NULL) ? true : false;
}

/**
* This function return pointer to answer buffer.
*
* Public function defined in esp_utils.h
*/
char* esp_alloc_answer_buffer(void)
{
	memset(answer,0,sizeof(answer));
	return answer;
}

/**
* This function return pointer to param buffer.
*
* Public function defined in esp_utils.h
*/
char* esp_alloc_param_buffer(void)
{
	memset(param,0,sizeof(param));
	return param;
}

/**
* This function compare AT versions.
*
* Public function defined in esp_utils.h
*/
int8_t esp_at_version_compare(const esp_at_version_t *at1, const esp_at_version_t *at2)
{
	uint32_t at1Ver = 0;
	uint32_t at2Ver = 0;

	if(at1 == NULL || at2 == NULL) {
		return 0;
	}

	at1Ver = (at1->major * 1000) + (at1->minor * 100) + (at1->patch[1] * 10) + at1->patch[0];
	at2Ver = (at2->major * 1000) + (at2->minor * 100) + (at2->patch[1] * 10) + at2->patch[0];

	if(at1Ver > at2Ver)
	{
		return 1;
	}
	else if(at1Ver < at2Ver)
	{
		return -1;
	}

	return 0;
}

/**
* This function compare SDK versions.
*
* Public function defined in esp_utils.h
*/
int8_t esp_sdk_version_compare(const esp_sdk_version_t *sdk1, const esp_sdk_version_t *sdk2)
{
	uint32_t sdk1Ver = 0;
	uint32_t sdk2Ver = 0;

	if(sdk1 == NULL || sdk2 == NULL) {
		return 0;
	}

	sdk1Ver = (sdk1->major * 100) + (sdk1->minor * 10) + sdk1->patch;
	sdk2Ver = (sdk2->major * 100) + (sdk2->minor * 10) + sdk2->patch;

	if(sdk1Ver > sdk2Ver)
	{
		return 1;
	}
	else if(sdk1Ver < sdk2Ver)
	{
		return -1;
	}

	return 0;
}
