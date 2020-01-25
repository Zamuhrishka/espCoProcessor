/********************************************************************************
*
* @file    		esp_wifi.h
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 Wi-Fi AT Commands.
*
********************************************************************************/
#pragma once
//_____ I N C L U D E S _______________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include "esp_drv.h"
#include "convert.h"
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//! @brief  List of status during joining to wifi AP
//! @{
typedef enum
{
	AP_DISCONECT,																///< SoftAp disconnected
	AP_CONNECT,																	///< SoftAp connected
	AP_BUSY,																	///< SoftAp busy
	AP_GOT_IP,																	///< SoftAp connected and got ip address
	AP_PASS,																	///< SoftAp ok
	AP_ERROR,																	///< SoftAp error
}	ESP_JoinApStatus_t;
//! @}

//! @brief  List of available wifi modes
//! @{
typedef enum
{
	WIFI_CLIENT           		=       '1',									///< Wifi Station mode
	WIFI_AP               		=       '2',									///< Wifi SoftAP mode
	WIFI_DUAL             		=       '3'										///< Wifi SoftAP+Station mode
}	ESP_WifiMode_t;
//! @}

//! @brief  List of available encription types
//! @{
typedef enum
{
	OPEN						=		'0',
	WEP							=		'1',
	WPA							=		'2',
	WPA2						=		'3',
	MIXED 						=		'4'
}	ESP_Encription_t;
//! @}

//! @brief  List of dhcp modes
//! @{
typedef enum
{
	DHCP_OFF          			=       '0',									///< DHCP is disabled
	DHCP_ON         			=       '1',									///< DHCP is enabled
}	ESP_DhcpModes_t;
//! @}

//! @brief  Params to configurate wifi AP
//! @{
typedef struct
{
	ESP_Encription_t enc;														///< Encryption method
	uint8_t channel;															///< Channel ID
	uint8_t max_con;															///< Maximum number of Stations to which ESP8266 SoftAP can be connected
	uint8_t hidden;																///< Hide or not SoftAp
}	ESP_ApParam_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
/**
* @brief 	This function setup wifi work mode.
* 			This setting will be stored in the flash system parameter area.
* 			It will not be erased even when the power is off and restarted.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_SetupWifiModeCur or ESP_SetupWifiModeDef instead.
*
* @param[in] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiMode(ESP_WifiMode_t mode, uint32_t timeout);

/**
* @brief 	This function request wifi work mode.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestWifiModeCur or ESP_RequestWifiModeDef instead.
*
* @param[out] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiMode(ESP_WifiMode_t* mode, uint32_t timeout);

/**
* @brief 	This function setup wifi work mode.
* 			This setting will be stored in the flash system parameter area.
* 			It will not be erased even when the power is off and restarted.
*
* @note		This configuration will not store in Flash.
*
* @param[in] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiModeCur(ESP_WifiMode_t mode, uint32_t timeout);

/**
* @brief 	This function request wifi work mode.
*
* @param[out] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiModeCur(ESP_WifiMode_t* mode, uint32_t timeout);

/**
* @brief 	This function setup wifi work mode.
* 			This setting will be stored in the flash system parameter area.
* 			It will not be erased even when the power is off and restarted.
*
* @note		This configuration will store in Flash system parameter area.
*
* @param[in] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiModeDef(ESP_WifiMode_t mode, uint32_t timeout);

/**
* @brief 	This function request wifi work mode.
*
* @param[out] mode type of wifi work mode.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiModeDef(ESP_WifiMode_t* mode, uint32_t timeout);

/**
* @brief 	This function join to the selected WiFi AP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_JoinToWifiApCur or ESP_JoinToWifiDef instead.
*
* @note		This configuration will store in Flash system parameter area.
*
* @param[in] ssid SSID of wi-fi AP.
* @param[in] pass password.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning   ssid and  pass must be a string - it must ending with '/0' symbol!
* @warning	 ssid must be less or equal 32 bytes and @param pass must be less or equal 64 bytes.
*
* <b>Example:</b>
* \code
* 	ESP_JoinToWifiAp("HUAWEI-DUge\0", "485754439FED179D\0", 2000);
* \endcode
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
ESPStatus_t ESP_JoinToWifiAp(const char ssid[], const char pass[], uint32_t timeout);

/**
* @brief 	This function join to the selected WiFi AP.
*
* @note		This configuration will not store in Flash.
*
* @param[in] ssid SSID of wi-fi AP.
* @param[in] pass password.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning   ssid and pass must be a string - it must ending with '/0' symbol!
* @warning	 ssid must be less or equal 32 bytes and @param pass must be less or equal 64 bytes.
*
* <b>Example:</b>
* \code
* ESP_JoinToWifiApCur("HUAWEI-DUge\0", "485754439FED179D\0", 2000);
* \endcode
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
ESPStatus_t ESP_JoinToWifiApCur(const char ssid[], const char pass[], uint32_t timeout);

/**
* @brief 	This function join to the selected WiFi AP.
*
* @note		This configuration will store in Flash.
*
* @param[in] ssid SSID of wi-fi AP.
* @param[in] pass password.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning  ssid and pass must be a string - it must ending with '/0' symbol!
* @warning  ssid must be less or equal 32 bytes and @param pass must be less or equal 64 bytes.
*
* <b>Example:</b>
* \code
* ESP_JoinToWifiApDef("HUAWEI-DUge\0", "485754439FED179D\0", 2000);
* \endcode
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
ESPStatus_t ESP_JoinToWifiApDef(const char ssid[], const char pass[], uint32_t timeout);

/**
* @brief 	This function request SSID of AP to which we are now connected.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestNameConnectedApCur or ESP_RequestNameConnectedApDef instead.
*
* @note		This configuration will store in Flash.
*
* @param[out] ssid SSID of wi-fi AP or do not change this string if we do not
* 			connected to any SSID.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning	param ssid must be less or equal 32 bytes.
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
ESPStatus_t ESP_RequestNameConnectedAp(char ssid[], uint32_t timeout);

/**
* @brief 	This function request SSID of AP to which we are now connected.
*
* @note		This configuration will not store in Flash.
*
* @param[out] ssid SSID of wi-fi AP or do not change this string if we do not
* 			connected to any SSID.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning	param ssid must be less or equal 32 bytes.
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
ESPStatus_t ESP_RequestNameConnectedApCur(char ssid[], uint32_t timeout);

/**
* @brief 	This function request SSID of AP to which we are now connected.
*
* @note		This configuration will store in Flash.
*
* @param[out] ssid SSID of wi-fi AP or do not change this string if we do not
* 			connected to any SSID.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning	param ssid must be less or equal 32 bytes.
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
ESPStatus_t ESP_RequestNameConnectedApDef(char ssid[], uint32_t timeout);

/**
* @brief 	This function unjoin from the current WiFi AP.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_UnJoinFromWifiAp(uint32_t timeout);

/**
* @brief 	This function create software WiFi AP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_CreateWifiApCur or ESP_CreateWifiApDef instead.
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don’t support WEP.
* @note 	This configuration will store in Flash system parameter area.
*
* @param[in] ssid SSID of created AP.
* @param[in] pass password of created AP.
* @param[in] channel channel of created AP.
* @param[in] enc security type of created AP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning  ssid and pass must be a string - it must ending with '/0' symbol!
*
* <b>Example:</b>
* \code
* ESP_CreateWifiAp("Mz4\0", "habrahabr\0", '5', '4', 2000);
* \endcode
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
ESPStatus_t ESP_CreateWifiAp(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout);

/**
* @brief 	This function create software WiFi AP.
*
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don’t support WEP.
* @note 	This configuration will not store in Flash.
*
* @param[in] ssid SSID of created AP.
* @param[in] pass password of created AP.
* @param[in] channel channel of created AP.
* @param[in] enc security type of created AP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning  @param ssid and @param pass must be a string - it must ending with '/0' symbol!
*
* <b>Example:</b>
* \code
* ESP_CreateWifiAp("Mz4\0", "habrahabr\0", '5', '4', 2000);
* \endcode
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
ESPStatus_t ESP_CreateWifiApCur(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout);

/**
* @brief 	This function create software WiFi AP.
*
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don’t support WEP.
* @note 	This configuration will store in Flash system parameter area.
*
* @param[in] ssid SSID of created AP.
* @param[in] pass password of created AP.
* @param[in] channel channel of created AP.
* @param[in] enc security type of created AP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning  @param ssid and @param pass must be a string - it must ending with '/0' symbol!
*
* <b>Example:</b>
* \code
* ESP_CreateWifiAp("Mz4\0", "habrahabr\0", '5', '4', 2000);
* \endcode
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
ESPStatus_t ESP_CreateWifiApDef(const char* ssid[], const char* pass[], char channel, char enc, uint32_t timeout);

/**
* @brief 	This function request he configuration parameters of the SoftAP.
*
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don’t support WEP.
* @note 	This configuration will not store in Flash system parameter area.
*
* @param[in] ssid SSID of created AP.
* @param[in] pass password of created AP.
* @param[in] param params of SoftAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @warning  param ssid and param pass must be a string - it must ending with '/0' symbol!
*
* <b>Example:</b>
* \code
* ESP_CreateWifiAp("Mz4\0", "habrahabr\0", &param, 2000);
* \endcode
*
* @return 	true/false.
*/
bool ESP_RequestSoftApParamCur(char ssid[], char pass[], ESP_ApParam_t* param, uint32_t timeout);

/**
* @brief 	This function get stations IP which is connected to ESP8266 softAP.
*
* @note		This function only available when ESP8266 softAP DHCP enable.
*
* @param[out] ipv4 IP address of connected station.
* @param[out] mac MAC address of connected station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_GetIpOfConnectedStation(Ipv4Addr_t *ipv4, MacAddr_t *mac, uint32_t timeout);

/**
* @brief 	This function enable/disable DHCP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_ConfigDhcpModeCur or ESP_ConfigDhcpModeDef instead.
*
* @note		This configuration will store in Flash user parameter area.
* @note		This configuration interact with static IP related AT commands(AT
*			+CIPSTA related and AT+CIPAP related):
*			* If enable DHCP, static IP will be disabled;
*			* If enable static IP, DHCP will be disabled;
*			* This will depends on the last configuration.
*
* @param[in] mode type of wifi work mode.
* @param[in] dhcp state.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_ConfigDhcpMode(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout);

/**
* @brief 	This function enable/disable DHCP.
*
* @note		This configuration will not store in Flash.
* @note		This configuration interact with static IP related AT commands(AT
*			+CIPSTA related and AT+CIPAP related):
*			* If enable DHCP, static IP will be disabled;
*			* If enable static IP, DHCP will be disabled;
*			* This will depends on the last configuration.
*
* @param[in] mode type of wifi work mode.
* @param[in] dhcp state.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_ConfigDhcpModeCur(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout);

/**
* @brief 	This function enable/disable DHCP.
*
* @note		This configuration will store in Flash user parameter area.
* @note		This configuration interact with static IP related AT commands(AT
*			+CIPSTA related and AT+CIPAP related):
*			* If enable DHCP, static IP will be disabled;
*			* If enable static IP, DHCP will be disabled;
*			* This will depends on the last configuration.
*
* @param[in] mode type of wifi work mode.
* @param[in] dhcp state.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_ConfigDhcpModeDef(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout);

/**
* @brief 	This function enable connect to AP automatically after power on.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_AutoconnectEnable(uint32_t timeout);

/**
* @brief 	This function disable connect to AP automatically after power on.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_AutoconnectDisable(uint32_t timeout);

/**
* @brief 	This function setup MAC address of station.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_SetupWifiStationIpAddrCur or
* 			ESP_SetupWifiStationIpAddrDef instead.
*
* @param[in] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupStationMacAddr(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function setup MAC address of station.
*
* @param[in] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool SetupStationMacAddrCur(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function setup MAC address of station.
*
* @param[in] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool SetupStationMacAddrDef(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of station.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestWifiStationIpAddrCur or
* 			ESP_RequestWifiStationIpAddrDef instead.
*
* @param[out] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestStationMacAddr(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of station.
*
* @param[out] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestStationMacAddrCur(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of station.
*
* @param[out] mac MAC address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestStationMacAddrDef(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function setup MAC address of softAP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_SetupWifiApIpAddrCur or
* 			ESP_SetupWifiApIpAddrDef instead.
*
* @param[in] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupSoftApMacAddr(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function setup MAC address of softAP.
*
* @param[in] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupSoftApMacAddrCur(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function setup MAC address of softAP.
*
* @param[in] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupSoftApMacAddrDef(MacAddr_t mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of softAP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestWifiApIpAddrCur or
* 			ESP_RequestWifiApIpAddrDef instead.
*
* @param[out] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestSoftApMacAddr(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of softAP.
*
* @param[out] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestSoftApMacAddrCur(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function request MAC address of softAP.
*
* @param[out] mac MAC address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestSoftApMacAddrDef(MacAddr_t* mac, uint32_t timeout);

/**
* @brief 	This function setup IP address of station.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_SetupWifiStationIpAddrCur or ESP_SetupWifiStationIpAddrDef instead.
*
* @param[in] ipv4 IP address of ESP8266 station.
* @param[in] gw Gateway IP address of ESP8266 station.
* @param[in] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiStationIpAddr(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of station.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestWifiStationIpAddrCur or ESP_RequestWifiStationIpAddrDef instead.
*
* @param[out] ipv4 IP address of ESP8266 station.
* @param[out] gw Gateway IP address of ESP8266 station.
* @param[out] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiStationIpAddr(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);

/**
* @brief 	This function setup IP address of station.
*
* @param[in] ipv4 IP address of ESP8266 station.
* @param[in] gw Gateway IP address of ESP8266 station.
* @param[in] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiStationIpAddrCur(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of station.
*
* @param[out] ipv4 IP address of ESP8266 station.
* @param[out] gw Gateway IP address of ESP8266 station.
* @param[out] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
ESPStatus_t ESP_RequestWifiStationIpAddrCur(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);

/**
* @brief 	This function setup IP address of station.
*
* @param[in] ipv4 IP address of ESP8266 station.
* @param[in] gw Gateway IP address of ESP8266 station.
* @param[in] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiStationIpAddrDef(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of station.
*
* @param[out] ipv4 IP address of ESP8266 station.
* @param[out] gw Gateway IP address of ESP8266 station.
* @param[out] mask Netmask IP address of ESP8266 station.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiStationIpAddrDef(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);

/**
* @brief 	This function setup IP address of softAP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_SetupWifiApIpAddrCur or ESP_SetupWifiApIpAddrDef instead.
*
* @param[in] ipv4 IP address of ESP8266 softAP.
* @param[in] gw Gateway IP address of ESP8266 softAP.
* @param[in] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiApIpAddr(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of softAP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_RequestWifiApIpAddrCur or ESP_RequestWifiApIpAddrDef instead.
*
* @param[out] ipv4 IP address of ESP8266 softAP.
* @param[out] gw Gateway IP address of ESP8266 softAP.
* @param[out] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiApIpAddr(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);

/**
* @brief 	This function setup IP address of softAP.
*
* @param[in] ipv4 IP address of ESP8266 softAP.
* @param[in] gw Gateway IP address of ESP8266 softAP.
* @param[in] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiApIpAddrCur(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of softAP.
*
* @param[out] ipv4 IP address of ESP8266 softAP.
* @param[out] gw Gateway IP address of ESP8266 softAP.
* @param[out] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiApIpAddrCur(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);

/**
* @brief 	This function setup IP address of softAP.
*
* @param[in] ipv4 IP address of ESP8266 softAP.
* @param[in] gw Gateway IP address of ESP8266 softAP.
* @param[in] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_SetupWifiApIpAddrDef(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

/**
* @brief 	This function request IP address of softAP.
*
* @param[out] ipv4 IP address of ESP8266 softAP.
* @param[out] gw Gateway IP address of ESP8266 softAP.
* @param[out] mask Netmask IP address of ESP8266 softAP.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool ESP_RequestWifiApIpAddrDef(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);
