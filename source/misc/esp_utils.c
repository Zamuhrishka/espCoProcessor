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
#include <assert.h>
#include "esp_port.h"
#include "convert.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
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

/**
* This function compare SDK versions.
*
* Public function defined in esp_utils.h
*/
bool esp_at_version_to_string(const esp_at_version_t *at, char *str)
{
	size_t len = 0;

	assert(at != NULL);
	assert(str != NULL);

	convert_uint8_to_string(str, at->major);
	len = strlen(str);
	str[len++] = '.';
	convert_uint8_to_string((str + len), at->minor);
	len = strlen(str);
	str[len++] = '.';
	convert_uint8_to_string((str + len), at->patch[0]);
	len = strlen(str);
	str[len++] = '.';
	convert_uint8_to_string((str + len), at->patch[1]);

	return true;
}

/**
* This function compare SDK versions.
*
* Public function defined in esp_utils.h
*/
bool esp_sdk_version_to_string(const esp_sdk_version_t *sdk, char *str)
{
	size_t len = 0;

	assert(sdk != NULL);
	assert(str != NULL);

	convert_uint8_to_string(str, sdk->major);
	len = strlen(str);
	str[len++] = '.';
	convert_uint8_to_string((str + len), sdk->minor);
	len = strlen(str);
	str[len++] = '.';
	convert_uint8_to_string((str + len), sdk->patch);

	return true;
}
