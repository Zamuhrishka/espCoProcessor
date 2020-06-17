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
#include <esp_drv.h>
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
}	esp_at_version_t;
//! @}

//!@brief ESP8266 SDK version struct
//! @{
typedef struct
{
	uint8_t major;
	uint8_t minor;
	uint8_t patch;
}	esp_sdk_version_t;
//! @}

//!@brief ESP8266 AT commands ID`s list
//! @{
enum esp_at_cmd_list_t
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
	SNTPCFG_SETUP,																///< Sets the configuration of SNTP
	SNTPCFG_REQUEST,															///< Request of the configuration of SNTP
	SNTPTIME,																	///< Checks the SNTP Time
	AT_UNDEF,
};
//! @}

//!@brief List of answer patterns ID
//! @{
typedef enum
{
	PATTERN_OK 	= 	0,
	PATTERN_ERROR,
	PATTERN_WIFI_CONNECT,
	PATTERN_WIFI_DISCONNECT,
	PATTERN_WIFI_ALREADY_CONNECT,
	PATTERN_WIFI_GOT_IP,
	PATTERN_NO_AP,
	PATTERN_BUSY_P,
	PATTERN_BUSY_S,
	PATTERN_TX_READY,
	PATTERN_TRANSPARENT_READY,
	PATTERN_RECEIVE,
	PATTERN_ALREADY_CONNECT,
	PATTERN_CLOSE,
	PATTERN_CONNECT,
	PATTERN_SEND_ACK,
	PATTERN_TRANSPARENT_DISABLE,
	PATTERN_UNDEF,
} 	esp_pattern_list_t;
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
bool esp_get_at_payload(uint8_t cmd, char* src, uint8_t *payload);

/**
* @brief 	This function send AT command to module.
*
* @param[in] cmd supported AT command.
* @param[in] data data if needed.
* @param[in] size size of data.
*
* @return 	true/false.
*/
bool esp_cmd_transmit(uint8_t cmd, const char data[], size_t size);

/**
* @brief 	This function send raw data to chip.
*
* @warning  This is blocking function!
*
* @param[in] data data if needed.
* @param[in] size size of data.
* @param[in] timeout timeout in msec for waiting answer from chip.
* @return 	true/false.
*/
bool esp_data_transmit(const char data[], size_t size, uint32_t timeout);

/**
* @brief 	This function receive data from chip.
*
* @note  This is not blocking function!
*
* @param[in] msg buffer for saved received answer.
* @param[in] len size of received answer.
* @return 	true/false.
*/
//bool esp_data_receive(char *msg, size_t len, uint32_t timeout);
int32_t esp_data_receive(char *msg, size_t len, uint32_t timeout);

/**
* @brief 	This function check available selected pattern
* 			in message.
*
* @param[in] msg AT message.
* @param[in] pattern pattern.
*
* @return 	true/false.
*/
bool esp_pattern_check(const char msg[], esp_pattern_list_t pattern);

/**
* @brief 	This function return pointer to answer buffer.
*
* @param	none.
*
* @return 	pointer to answer buffer.
*/
char* esp_alloc_answer_buffer(void);

/**
* @brief 	This function return pointer to param buffer.
*
* @param	none.
*
* @return 	pointer to param buffer.
*/
char* esp_alloc_param_buffer(void);

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
int8_t esp_at_version_compare(const esp_at_version_t *at1, const esp_at_version_t *at2);

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
int8_t esp_sdk_version_compare(const esp_sdk_version_t *sdk1, const esp_sdk_version_t *sdk2);

/**
* @brief 	This function convert AT version to string.
*
* @param[in] at AT commands set version.
* @param[out] str AT commands set version string.
*
* @note str param must ending with '/0'.
* 		Size of str must be at least 16 bytes.
*
* @return true/false
*/
bool esp_at_version_to_string(const esp_at_version_t *at, char *str);

/**
* @brief 	This function convert SDK versions to string.
*
* @param[in] sdk SDK version.
* @param[out] str SDK version string.
*
* @note str param must ending with '/0'.
* 		Size of str must be at least 16 bytes.
*
* @return true/false
*/
bool esp_sdk_version_to_string(const esp_sdk_version_t *sdk, char *str);
