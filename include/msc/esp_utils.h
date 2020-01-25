/********************************************************************************
*
* @file    		esp_utils.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		This file contains the some support function.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
//_____ C O N F I G S  ________________________________________________________
//! Max size of buffer for answer store
#define ESP_ANSWER_BUFF_SIZE							200

//! Max size of buffer for param for AT command store
#define ESP_PARAM_BUFF_SIZE								200
//_____ M A C R O S ___________________________________________________________
//! Convert num into char
#define CONVER_TO_CHAR(_num_)                          (((_num_) + 0x30))

//! Convert char into num
#define CONVER_TO_NUMBER(_char_)                       (((_char_) - 0x30))
//_____ D E F I N I T I O N ___________________________________________________
//!@brief ESP8266 AT command version struct
//! @{
typedef struct
{
	uint8_t major;
	uint8_t minor;
	uint8_t patch[2];
}	ESP_AtVersion;
//! @}

//!@brief ESP8266 SDK version struct
//! @{
typedef struct
{
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
}	ESP_SdkVersion;
//! @}

//!@brief ESP8266 AT commands ID`s list
//! @{
enum ESP_AtCommandsList_t
{
	//BASIC
	AT = 0,																		///< Test AT startup
	RST,																		///< Restart module
	GMR,																		///< Request Firmware Version
	GSLP,																		///< Enter deep-sleep mode
	ATE,																		///< AT commands echo or not
	RESTORE,																	///< Factory Reset
	UART,																		///< UART configuration,
	UART_CUR,																	///< UART current configuration
	UART_DEF,																	///< UART default configuration, save to flash
	SLEEP,																		///< Sleep mode
	WAKEUPGPIO,																	///< Configures a GPIO to Wake ESP8266 up from Light-sleep Mode
	RFPOWER,																	///< Sets the Maximum Value of RF TX Power
	SYSMSG_CUR,																	///< Set Current System Messages
	SYSMSG_DEF,																	///< Set Default System Messages

   //WI-FI
	MODE,																		///< WIFI Station/SoftAP/Station+SoftAP. This API is deprecated.
	REQMODE,																	///< Request the WiFi work mode.
	MODE_CUR,																	///< WiFi Station/SoftAP/Station+SoftAP do not save to Flash.
	REQMODE_CUR,																///< Request the WiFi work mode.
	MODE_DEF,																	///< WiFi default Station/SoftAP/Station+SoftAP.Save to Flash.
	REQMODE_DEF,																///< Request the WiFi work mode.
	JOIN,																		///< Connect to AP. This API is deprecated.
	REQSSID,																	///< Request the SSID which we connected.
	JOIN_CUR,																	///< Connect to AP, won’t save to Flash.
	REQSSID_CUR,																///< Request the SSID.
	JOIN_DEF,																	///< Connect to AP, save to Flash.
	REQSSID_DEF,																///< Request the SSID.
	APLIST,																		///< Lists available APs.
	UNJOIN,																		///< Disconnect from AP.
	AP,																			///< Set configuration of ESP8266 softAP. This API is deprecated.
	AP_CUR,																		///< Set configuration of ESP8266 softAP.Won’t save to Flash.
	AP_DEF,																		///< Set configuration of ESP8266 softAP.Save to Flash.
	AP_REQ,
	CWLIF,																		///< Get station’s IP which is connected to ESP8266 softAP.
	CWDHCP,																		///< Enable/Disable DHCP. This API is deprecated.
	CWDHCP_CUR,																	///< Enable/Disable DHCP, won’t save to Flash.
	CWDHCP_DEF,																	///< Enable/Disable DHCP, save to Flash.
	CWDHCPS_CUR,
	CWAUTOCONN,																	///< Connect to AP automatically when power on.
	CIPSTAMAC,																	///< Set mac address of ESP8266 station. This API is deprecated.
	REQSTAMAC,																	///< Request the MAC address of station.
	CIPSTAMAC_CUR,																///< Set mac address of ESP8266 station. Won’t save to Flash.
	REQSTAMAC_CUR,																///< Request the MAC address of station.
	CIPSTAMAC_DEF,																///< Set mac address of ESP8266 station.Save to Flash.
	REQSTAMAC_DEF,																///< Request the MAC address of station.
	CIPAPMAC,																	///< Set mac address of ESP8266 softAP. This API is deprecated.
	REQAPMAC,																	///< Request the MAC address of softAP.
	CIPAPMAC_CUR,																///< Set mac address of ESP8266 softAP.Won’t save to Flash.
	REQAPMAC_CUR,																///< Request the MAC address of softAP.
	CIPAPMAC_DEF,																///< Set mac address of ESP8266 softAP.Save to Flash.
	REQAPMAC_DEF,																///< Request the MAC address of softAP.
	CIPSTA,																		///< Set IP address of ESP8266 station. This API is deprecated.
	REQSTA,																		///< Request the MAC address of station.
	CIPSTA_CUR,																	///< Set IP address of ESP8266 station.Won’t save to Flash.
	REQSTA_CUR,																	///< Request the MAC address of station.
	CIPSTA_DEF,																	///< Set IP address of ESP8266 station.Save to Flash.
	REQSTA_DEF,																	///< Request the MAC address of station.
	CIPAP,																		///< Set IP address of ESP8266 softAP. This API is deprecated.
	REQCIPAP,																	///< Request the MAC address of softAP.
	CIPAP_CUR,																	///< Set IP address of ESP8266 softAP.Won’t save to Flash.
	REQCIPAP_CUR,																///< Request the MAC address of softAP.
	CIPAP_DEF,																	///< Set IP address of ESP8266 softAP.Save to Flash.
	REQCIPAP_DEF,																///< Request the MAC address of softAP.

	//TCP-IP
	CIPSTATUS,																	///< Get connection status.
	CIPSTART,																	///< Establish TCP connection or register UDP port.
	CIPSEND,																	///< Send data.
	CIPTRANSP,																	///< Transparent Transmission.
	CIPSENDEX,																	///< Send data, if <length> or “\0” is met, data will be sent.
	CIPSENDBUF,																	///< Write data into TCP-send-buffer.
	CIPBUFRESET_S,																///< Reset segment ID count in single mode.
	CIPBUFRESET_M,																///< Reset segment ID count in multi mode.
	CIPBUFSTATUS_S,																///< Check status of TCP-send-buffer in single mode.
	CIPBUFSTATUS_M,																///< Check status of TCP-send-buffer in multi mode.
	CIPCHECKSEG,																///< Check if a specific segment is sent or not.
	CIPCLOSE_S,																	///< Close Single TCP/UDP connection.
	CIPCLOSE_M,																	///< Close Multi TCP/UDP connection.
	CIFSR,																		///< Get local IP address.
	CIPMUX,																		///< Set multiple connections mode.
	REQCIPMUX,																	///< Request multiple connections mode.
	CIPSERVER,																	///< Configure as server.
	CIPMODE,																	///< Set transmission mode.
	REQCIPMODE,																	///< Request transmission mode.
	SAVETRANSLINK,																///< Save transparent transmission link to Flash.
	CIPSTO,																		///< Set timeout when ESP8266 runs as TCP server.
	CIUPDATE,																	///< Upgrade firmware through network.
	PING,																		///< Function PING.
	CIPDOMAIN,																	///< DNS Function.
	CIPSERVERMAXCONN,															///< Set the Maximum Connections Allowed by Server
	CIPSNTPCFG,																	///< Sets the Configuration of SNTP
	AT_UNDEF,
};
//! @}

//!@brief List of answer patterns ID
//! @{
typedef enum
{
	pattern_OK 	= 	0,
	pattern_ERROR,
	pattern_WIFI_CONNECT,
	pattern_WIFI_DISCONNECT,
	pattern_WIFI_ALREADY_CONNECT,
	pattern_WIFI_GOT_IP,
	pattern_NO_AP,
	pattern_BUSY_P,
	pattern_BUSY_S,
	pattern_TX_READY,
	pattern_TRANSPARENT_READY,
	pattern_RECEIVE,
	pattern_ALREADY_CONNECT,
	pattern_CLOSE,
	pattern_CONNECT,
	pattern_SEND_ACK,
	pattern_TRANSPARENT_DISABLE,
	pattern_UNDEF,
} 	ESP_PatternsList_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* @brief 	This function highlights the payload of AT answer.
*
* @param[in] cmd supported AT command.
* @param[in] src string which store the AT message.
* @param[out] payload payload.
*
* @return 	true/false.
*/
bool ESP_GetAtPayload(uint8_t cmd, char* src, uint8_t *payload);

/**
* @brief 	This function send AT command to module.
*
* @param[in] cmd supported AT command.
* @param[in] data data if needed.
* @param[in] size size of data.
*
* @return 	true/false.
*/
bool ESP_SendAtCmd(uint8_t cmd, const char data[], size_t size);

/**
* @brief 	This function send raw AT command to module.
*
* @param[in] cmd any of AT command.
* @param[in] data data if needed.
* @param[in] size size of data.
*
* @return 	true/false.
*/
bool ESP_SendAtRawCmd(const char cmd[], const char data[], size_t size);

/**
* @brief 	This function send raw data to module.
*
* @param[in] data data for transmit.
* @param[in] size size of data.
*
* @return 	true/false.
*/
bool ESP_SendAtRawData(const char data[], size_t size);

/**
* @brief 	This function check available selected pattern
* 			in message.
*
* @param[in] msg AT message.
* @param[in] pattern pattern.
*
* @return 	true/false.
*/
bool ESP_PatternCheck(const char msg[], ESP_PatternsList_t pattern);

/**
* @brief 	This function return pointer to answer buffer.
*
* @param	none.
*
* @return 	pointer to answer buffer.
*/
char* ESP_AllocAnswerBuffer(void);

/**
* @brief 	This function return pointer to param buffer.
*
* @param	none.
*
* @return 	pointer to param buffer.
*/
char* ESP_AllocParamBuffer(void);

/**
* @brief 	This function compare two AT commands sets versions.
*
* @param[in] at1 first AT commands set version.
* @param[in] at2 second AT commands set version.
*
* @return
* <ul>
* <li> <b>1 - if at1 > at2</b>
* <li> <b>-1 - if at1 < at2</b>
* <li> <b>0 - if at1 = at2</b>
* </ul>
*/
int8_t ESP_CompareAtVersions(const ESP_AtVersion *at1, const ESP_AtVersion *at2);

/**
* @brief 	This function compare two SDK versions.
*
* @param[in] sdk1 first SDK version.
* @param[in] sdk2 second SDK version.
*
* @return
* <ul>
* <li> <b>1 - if sdk1 > sdk2</b>
* <li> <b>-1 - if sdk1 < sdk2</b>
* <li> <b>0 - if sdk1 = sdk2</b>
* </ul>
*/
int8_t ESP_CompareSdkVersions(const ESP_SdkVersion *sdk1, const ESP_SdkVersion *sdk2);
