// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_at_cmd.c
* @author  		Kovalchuk Alexander (roux@yandex.ru)
* @brief		This file contains the some support function.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "esp_at_cmd.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "esp_hw_interface.h"
// #include "esp_port.h"
// #include "convert.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//!@brief List of support AT commands
//! @{
static const char* AT_CMD[] =
{
	//BASIC
	"AT",																		//!Test AT startup
	"+RST",																		//!Reset module
	"+GMR",																		//!Request Firmware Version
	"+GSLP=",																	//!Enter deep-sleep mode
	"E",																		//!AT commands echo or not
	"+RESTORE",																	//!Factory Reset
	"+UART=",																	//!UART configuration
	"+UART_CUR=",																//!UART current configuration
	"+UART_DEF=",																//!UART default configuration, save to flash
	"+SLEEP:",																	//!Sleep mode
	"+WAKEUPGPIO=",																//!Configures a GPIO to Wake ESP8266 up from Light-sleep Mode
	"+RFPOWER=",																//!Sets the Maximum Value of RF TX Power
	"+SYSMSG_CUR=",																//!Set Current System Messages
	"+SYSMSG_DEF=",																//!Set Default System Messages

	//WI-FI
	"+CWMODE=",																	//!WIFI mode Station/SoftAP/Station+SoftAP. This API is deprecated.
	"+CWMODE?",																	//!Request the WiFi work mode.
	"+CWMODE_CUR=",																//!WiFi Station/SoftAP/Station+SoftAP save to Flash.
	"+CWMODE_CUR?",																//!Request the WiFi work mode.
	"+CWMODE_DEF=",																//!WiFi default Station/SoftAP/Station+SoftAP.Save to Flash.
	"+CWMODE_DEF?",																//!Request the WiFi work mode.
	"+CWJAP=",																	//!Connect to AP. This API is deprecated.
	"+CWJAP?",																	//!Request the SSID.
	"+CWJAP_CUR=",																//!Connect to AP, won’t save to Flash.
	"+CWJAP_CUR?",																//!Request the SSID.
	"+CWJAP_DEF=",																//!Connect to AP, save to Flash.
	"+CWJAP_DEF?",																//!Request the SSID which we connected.
	"+CWLAP",																	//!Lists available APs.
	"+CWQAP",																	//!Disconnect from AP.
	"+CWSAP=",																	//!Set configuration of ESP8266 softAP. This API is deprecated.
	"+CWSAP_CUR=",																//!Set configuration of ESP8266 softAP.Won’t save to Flash.
	"+CWSAP_DEF=",																//!Set configuration of ESP8266 softAP.Save to Flash.
	"+CWSAP_CUR?",
	"+CWLIF",																	//!Get station’s IP which is connected to ESP8266 softAP.
	"+CWDHCP=",																	//!Enable/Disable DHCP. This API is deprecated.
	"+CWDHCP_CUR=",																//!Enable/Disable DHCP, won’t save to Flash.
	"+CWDHCP_DEF=",																//!Enable/Disable DHCP, save to Flash.
	"+CWDHCPS_CUR=",
	"+CWAUTOCONN=",																//!Connect to AP automatically when power on.
	"+CIPSTAMAC=",																//!Set mac address of ESP8266 station. This API is deprecated.
	"+CIPSTAMAC?",																//!Request the MAC address of station.
	"+CIPSTAMAC_CUR=",															//!Set mac address of ESP8266 station. Won’t save to Flash.
	"+CIPSTAMAC_CUR?",															//!Request the MAC address of station.
	"+CIPSTAMAC_DEF=",															//!Set mac address of ESP8266 station.Save to Flash.
	"+CIPSTAMAC_DEF?",															//!Request the MAC address of station.
	"+CIPAPMAC=",																//!Set mac address of ESP8266 softAP. This API is deprecated.
	"+CIPAPMAC?",																//!Request the MAC address of softAP.
	"+CIPAPMAC_CUR=",															//!Set mac address of ESP8266 softAP.Won’t save to Flash.
	"+CIPAPMAC_CUR?",															//!Request the MAC address of softAP.
	"+CIPAPMAC_DEF=",															//!Set mac address of ESP8266 softAP.Save to Flash.
	"+CIPAPMAC_DEF?",															//!Request the MAC address of softAP.
	"+CIPSTA=",																	//!Set IP address of ESP8266 station. This API is deprecated.
	"+CIPSTA?",																	//!Request the MAC address of station.
	"+CIPSTA_CUR=",																//!Set IP address of ESP8266 station.Won’t save to Flash.
	"+CIPSTA_CUR?",																//!Request the MAC address of station.
	"+CIPSTA_DEF=",																//!Set IP address of ESP8266 station.Save to Flash.
	"+CIPSTA_DEF?",																//!Request the MAC address of station.
	"+CIPAP=",																	//!Set IP address of ESP8266 softAP. This API is deprecated.
	"+CIPAP?",																	//!Request the MAC address of softAP.
	"+CIPAP_CUR=",																//!Set IP address of ESP8266 softAP.Won’t save to Flash.
	"+CIPAP_CUR?",																//!Request the MAC address of softAP.
	"+CIPAP_DEF=",																//!Set IP address of ESP8266 softAP.Save to Flash.
	"+CIPAP_DEF?",																//!Request the MAC address of softAP.

	//TCP-IP
	"+CIPSTATUS",																//!Get connection status.
	"+CIPSTART=",																//!Establish TCP connection or register UDP port.
	"+CIPSEND=",																//!Send data.
	"+CIPSEND",																	//!Transparent Transmission.
	"+CIPSENDEX=",																//!Send data, if <length> or “” is met, data will be sent.
	"+CIPSENDBUF=",																//!Write data into TCP-send-buffer.
	"+CIPBUFRESET",																//!Reset segment ID count in single mode.
	"+CIPBUFRESET=",															//!Reset segment ID count in multi mode.
	"+CIPBUFSTATUS",															//!Check status of TCP-send-buffer in single mode.
	"+CIPBUFSTATUS=",															//!Check status of TCP-send-buffer in multi mode.
	"+CIPCHECKSEG=",															//!Check if a specific segment is sent or not.
	"+CIPCLOSE",																//!Close Single TCP/UDP connection.
	"+CIPCLOSE=",																//!Close Multi TCP/UDP connection.
	"+CIFSR",																	//!Get local IP address.
	"+CIPMUX=",																	//!Set multiple connections mode.
	"+CIPMUX?",																	//!Request multiple connections mode.
	"+CIPSERVER=",																//!Configure as server.
	"+CIPMODE=",																//!Set transmission mode.
	"+CIPMODE?",																//!Request transmission mode.
	"+SAVETRANSLINK=",															//!Save transparent transmission link to Flash.
	"+CIPSTO=",																	//!Set timeout when ESP8266 runs as TCP server.
	"+CIPUPDATE:",																//!Upgrade firmware through network.
	"+PING=",																	//!Function PING.
	"+CIPDOMAIN=",																//!DNS Function.
	"+CIPSERVERMAXCONN=",														//!Set the Maximum Connections Allowed by Server
	"+CIPRECVMODE=",															//!Set TCP Receive Mode
	"+CIPRECVMODE?",															//!Query TCP Receive Mode
	"+CIPRECVDATA=",															//!Get TCP Data in Passive Receive Mode
	"+CIPSNTPCFG=",																//!Sets the configuration of SNTP
	"+CIPSNTPCFG?",																///< Request of the configuration of SNTP
	"+CIPSNTPTIME?",															///< Checks the SNTP Time
	"/0",
};
//! @}

//!@brief List of support message patterns
//! @{
char* ESP_Patterns[] =
{
	"OK\r\n",
	"\r\nERROR\r\n",
	"WIFI CONNECTED\r\n",
	"WIFI DISCONNECT\r\n",
	"ALREADY CONNECT\r\n",
	"WIFI GOT IP\r\n",
	"No Ap\r\n",
	"\r\nbusy p...\r\n",
	"\r\nbusy s...\r\n",
	"\r\nOK\r\n> ",
	"\r\nOK\r\n\r\n>",
	"+IPD,",
	"ALREAY CONNECT",
	"CLOSED\r\n",
	"CONNECT\r\n",
	"SEND OK",
	"+++",
	"+CIPRECVDATA"
};
//! @}

//!Buffer for store AT commands ready for transmit
static char transmitAT[ESP_AT_CMD_TXBUF_SIZE] = {0};

// //!Buffer for receive answer from esp8266
static char answer[ESP_ANSWER_BUFF_SIZE] = {0};

// //!Buffer for store params for AT commands
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
//	uintptr_t eposP1 = strstr(src, ESP_Patterns[PATTERN_OK]);
	if(eposP == 0) {
		return false;
	}
	epos = eposP - src;
//	ptrdiff_t epos1 = eposP - src;

	sposP = strstr(src, cmdStr);
//	uintptr_t sposP1 = strstr(src, cmdStr);
	if(sposP != 0)
	{
		spos = ((sposP - src) + (i + 3ul));
//		ptrdiff_t spos1 = ((sposP - src) + (i + 3ul));
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
bool esp_cmd_transmit(uint8_t cmd, const char data[], size_t size)
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

	return esp_msg_transmit(transmitAT, i);
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
	memset(answer, 0, ESP_ANSWER_BUFF_SIZE);
	return answer;
}

/**
* This function return pointer to param buffer.
*
* Public function defined in esp_utils.h
*/
char* esp_alloc_param_buffer(void)
{
	memset(param, 0, ESP_PARAM_BUFF_SIZE);

	return param;
}
