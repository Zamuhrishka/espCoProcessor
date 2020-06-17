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
#include <esp_drv.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "convert.h"
//_____ C O N F I G S  ________________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ D E F I N I T I O N ___________________________________________________
//! @brief  List of status during joining to wifi AP
//! @{
typedef enum
{
	ESP_AP_DISCONECT,															///< SoftAp disconnected
	ESP_AP_CONNECT,																///< SoftAp connected
	ESP_AP_BUSY,																///< SoftAp busy
	ESP_AP_GOT_IP,																///< SoftAp connected and got ip address
	ESP_AP_PASS,																///< SoftAp ok
	ESP_AP_ERROR,																///< SoftAp error
}	esp_softap_status_t;
//! @}

//! @brief  List of available wifi modes
//! @{
typedef enum
{
	ESP_WIFI_STATION          	=       '1',									///< Wifi Station mode
	ESP_WIFI_SOFTAP             =       '2',									///< Wifi SoftAP mode
	ESP_WIFI_DUAL             	=       '3'										///< Wifi SoftAP+Station mode
}	esp_wifi_mode_t;
//! @}

//! @brief  List of available encription types
//! @{
typedef enum
{
	ESP_OPEN					=		'0',
	ESP_WEP						=		'1',
	ESP_WPA						=		'2',
	ESP_WPA2					=		'3',
	ESP_MIXED 					=		'4'
}	esp_encription_t;
//! @}

//! @brief  List of dhcp modes
//! @{
typedef enum
{
	ESP_DHCP_OFF          		=       '0',									///< DHCP is disabled
	ESP_DHCP_ON         		=       '1',									///< DHCP is enabled
}	esp_dhcp_mode_t;
//! @}

//! @brief  Params to configurate wifi AP
//! @{
typedef struct
{
	esp_encription_t enc;														///< Encryption method
	uint8_t channel;															///< Channel ID
	uint8_t max_con;															///< Maximum number of Stations to which ESP8266 SoftAP can be connected
	uint8_t hidden;																///< Hide or not SoftAp
}	esp_ap_config_t;
//! @}
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_mode_setup(esp_wifi_mode_t mode, uint32_t timeout);

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
bool esp_wifi_mode_request(esp_wifi_mode_t* mode, uint32_t timeout);
#else

/**
* @brief 	This function setup wifi work mode.
*
* @param[in] mode type of wifi work mode.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_mode_setup(esp_wifi_mode_t mode, bool save, uint32_t timeout);

/**
* @brief 	This function request wifi work mode.
*
* @param[out] mode type of wifi work mode.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_mode_request(esp_wifi_mode_t* mode, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
esp_status_t esp_wifi_ap_join(const char ssid[], const char pass[], uint32_t timeout);
#else

/**
* @brief 	This function join to the selected WiFi AP.
*
* @param[in] ssid SSID of wi-fi AP.
* @param[in] pass password.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
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
esp_status_t esp_wifi_ap_join(const char ssid[], const char pass[], bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
esp_status_t esp_wifi_ap_ssid_request(char ssid[], uint32_t timeout);
#else

/**
* @brief 	This function request SSID of AP to which we are now connected.
*
* @param[out] ssid SSID of wi-fi AP or do not change this string if we do not
* 			connected to any SSID.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
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
esp_status_t esp_wifi_ap_ssid_request(char ssid[], bool save, uint32_t timeout);
#endif

/**
* @brief 	This function unjoin from the current WiFi AP.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_ap_unjoin(uint32_t timeout);

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* @brief 	This function create software WiFi AP.
*
* @warning	This function is deprecated for feature use.
* 			Please use ESP_CreateWifiApCur or ESP_CreateWifiApDef instead.
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don�t support WEP.
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
esp_status_t esp_wifi_softap_cfg(const char ssid[], const char pass[], char channel, char enc, uint32_t timeout);
#else

/**
* @brief 	This function create software WiFi AP.
*
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don�t support WEP.
*
* @param[in] ssid SSID of created AP.
* @param[in] pass password of created AP.
* @param[in] channel channel of created AP.
* @param[in] enc security type of created AP.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
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
esp_status_t esp_wifi_softap_setup(const char ssid[], const char pass[], char channel, char enc, bool save, uint32_t timeout);
#endif

/**
* @brief 	This function request he configuration parameters of the SoftAP.
*
* @note 	This function is only available when softAP mode enable.
* @note 	ESP8266 softAP don�t support WEP.
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
bool esp_wifi_softap_request(char ssid[], char pass[], esp_ap_config_t* param, uint32_t timeout);

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
bool esp_wifi_get_ip_of_connected_station(ip4addr_t *ipv4, mac_t *mac, uint32_t timeout);

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_dhcp_mode_setup(ESP_WifiMode_t mode, ESP_DhcpModes_t dhcp, uint32_t timeout);
#else

/**
* @brief 	This function enable/disable DHCP.
*
* @note		This configuration interact with static IP related AT commands(AT
*			+CIPSTA related and AT+CIPAP related):
*			* If enable DHCP, static IP will be disabled;
*			* If enable static IP, DHCP will be disabled;
*			* This will depends on the last configuration.
*
* @param[in] mode type of wifi work mode.
* @param[in] dhcp state.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_dhcp_mode_setup(esp_wifi_mode_t mode, esp_dhcp_mode_t dhcp, bool save, uint32_t timeout);
#endif

/**
* @brief 	This function enable connect to AP automatically after power on.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_autoconnect_enable(uint32_t timeout);

/**
* @brief 	This function disable connect to AP automatically after power on.
*
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_autoconnect_disable(uint32_t timeout);

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_station_mac_setup(MacAddr_t mac, uint32_t timeout);
#else

/**
* @brief 	This function setup MAC address of station.
*
* @param[in] mac MAC address of ESP8266 station.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_station_mac_setup(mac_t mac, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_station_mac_request(MacAddr_t* mac, uint32_t timeout);
#else

/**
* @brief 	This function request MAC address of station.
*
* @param[out] mac MAC address of ESP8266 station.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_station_mac_request(mac_t* mac, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_softap_mac_setup(MacAddr_t mac, uint32_t timeout);
#else

/**
* @brief 	This function setup MAC address of softAP.
*
* @param[in] mac MAC address of ESP8266 softAP.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_softap_mac_setup(mac_t mac, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_softap_mac_request(MacAddr_t* mac, uint32_t timeout);
#else

/**
* @brief 	This function request MAC address of softAP.
*
* @param[out] mac MAC address of ESP8266 softAP.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_softap_mac_request(mac_t* mac, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_station_ip_setup(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

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
bool esp_wifi_station_ip_request(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);
#else

/**
* @brief 	This function setup IP address of station.
*
* @param[in] ipv4 IP address of ESP8266 station.
* @param[in] gw Gateway IP address of ESP8266 station.
* @param[in] mask Netmask IP address of ESP8266 station.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_station_ip_setup(ip4addr_t ipv4, ip4addr_t gw, ip4addr_t mask, bool save, uint32_t timeout);

/**
* @brief 	This function request IP address of station.
*
* @param[out] ipv4 IP address of ESP8266 station.
* @param[out] gw Gateway IP address of ESP8266 station.
* @param[out] mask Netmask IP address of ESP8266 station.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
esp_status_t esp_wifi_station_ip_request(ip4addr_t* ipv4, ip4addr_t* gw, ip4addr_t* mask, bool save, uint32_t timeout);
#endif

#ifdef ESP_DEPRECATED_API_SUPPORT
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
bool esp_wifi_softap_ip_setup(Ipv4Addr_t ipv4, Ipv4Addr_t gw, Ipv4Addr_t mask, uint32_t timeout);

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
bool esp_wifi_softap_ip_request(Ipv4Addr_t* ipv4, Ipv4Addr_t* gw, Ipv4Addr_t* mask, uint32_t timeout);
#else

/**
* @brief 	This function setup IP address of softAP.
*
* @param[in] ipv4 IP address of ESP8266 softAP.
* @param[in] gw Gateway IP address of ESP8266 softAP.
* @param[in] mask Netmask IP address of ESP8266 softAP.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_softap_ip_setup(ip4addr_t ipv4, ip4addr_t gw, ip4addr_t mask, bool save, uint32_t timeout);

/**
* @brief 	This function request IP address of softAP.
*
* @param[out] ipv4 IP address of ESP8266 softAP.
* @param[out] gw Gateway IP address of ESP8266 softAP.
* @param[out] mask Netmask IP address of ESP8266 softAP.
* @param[in] save flag save or not configuration in ESP8266 flash memory.
* @param[in] timeout timeout in msec for waiting answer from chip.
*
* @return 	true/false.
*/
bool esp_wifi_softap_ip_request(ip4addr_t* ipv4, ip4addr_t* gw, ip4addr_t* mask, bool save, uint32_t timeout);
#endif


bool esp_wifi_receive_handle(char *msg, size_t size);
