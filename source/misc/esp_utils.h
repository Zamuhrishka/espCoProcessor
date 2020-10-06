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
//_____ M A C R O S ___________________________________________________________
//! Convert num into char
#define CONVER_TO_CHAR(_num_)                          ((char)((_num_) + 0x30))

//! Convert char into num
#define CONVER_TO_NUMBER(_char_)                       ((uint8_t)((_char_) - 0x30))
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
//_____ V A R I A B L E   D E C L A R A T I O N S _____________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E C L A R A T I O N S _____________________________
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
