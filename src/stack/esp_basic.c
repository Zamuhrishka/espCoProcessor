// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_basic.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief		API for ESP8266 Basic AT Commands.
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include "stack/esp_basic.h"

#include <string.h>
#include "slre.h"
#include "esp_drv.h"
#include "esp_utils.h"
#include "esp_port.h"
#include "esp_queue.h"
#include "convert.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
static const char ESP_ENABLE = '1';
static const char ESP_DISABLE = '0';
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function used to test the setup function.
*
* Public function defined in esp_basic.h
*/
bool ESP_Test(uint32_t timeout)
{
	char cmd[] = "AT\r\n";
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtRawData(cmd, sizeof(cmd)) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function used used to restart the module.
*
* Public function defined in esp_basic.h
*/
bool ESP_Reset(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(RST, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function return the version of AT commands and SDK that you are using.
*
* Public function defined in esp_basic.h
*/
bool ESP_GetVersion(ESP_AtVersion *at, ESP_SdkVersion *sdk, uint32_t timeout)
{
	struct slre_cap capsAt[4];
	struct slre_cap capsSDK[3];
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || at == NULL || sdk == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(GMR, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	if (slre_match((const char *)"AT version:(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\\S+", answer, strlen(answer), capsAt, 4, 0) <= 0) {
	   return false;
	}

	if (slre_match((const char *)"\\S+\r\nSDK version:(\\d+)\\.(\\d+)\\.(\\d+)\\S+", answer, strlen(answer), capsSDK, 3, 0) <= 0) {
	   return false;
	}

	at->major = Conver_StringToUint8(capsAt[0].ptr);
	at->minor = Conver_StringToUint8(capsAt[1].ptr);
	at->patch[0] = Conver_StringToUint8(capsAt[2].ptr);
	at->patch[1] = Conver_StringToUint8(capsAt[3].ptr);

	sdk->major = Conver_StringToUint8(capsSDK[0].ptr);
	sdk->minor = Conver_StringToUint8(capsSDK[1].ptr);
	sdk->patch = Conver_StringToUint8(capsSDK[2].ptr);

	return true;
}

/**
* This function is used to invoke the deep-sleep mode of the module.
*
* Public function defined in esp_basic.h
*/
bool ESP_EnterToDeepSleep(uint32_t time, uint32_t timeout)
{
	size_t param_size = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint32(param, time);
	param_size = strlen((char*)param);

	if(ESP_SendAtCmd(GSLP, param, param_size) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function enable echo with module answers.
*
* Public function defined in esp_basic.h
*/
bool ESP_EnableEcho(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(ATE, &ESP_ENABLE, 1) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return true;
}

/**
* This function disable echo with module answers.
*
* Public function defined in esp_basic.h
*/
bool ESP_DisableEcho(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(ATE, &ESP_DISABLE, 1) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return true;
}

/**
* This function is used to reset all parameters saved in flash.
*
* Public function defined in esp_basic.h
*/
bool ESP_Restore(uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

	if(ESP_SendAtCmd(RESTORE, NULL, 0) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets the UART configuration.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupUartParam(const ESP_UartParam_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL || cfg == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint32(param, cfg->baudRate);
	len = strlen((char*)param);

	if(strcat ((char*)param, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);
	param[len++] = cfg->dataBits;
	param[len++] = ',';
	param[len++] = cfg->stopBits;
	param[len++] = ',';
	param[len++] = cfg->parity;
	param[len++] = ',';
	param[len++] = cfg->flowControl;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(UART, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets the UART configuration.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupUartParamCur(const ESP_UartParam_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL || cfg == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint32(param, cfg->baudRate);
	len = strlen((char*)param);

	if(strcat ((char*)param, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);
	param[len++] = cfg->dataBits;
	param[len++] = ',';
	param[len++] = cfg->stopBits;
	param[len++] = ',';
	param[len++] = cfg->parity;
	param[len++] = ',';
	param[len++] = cfg->flowControl;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(UART_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets the UART configuration.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupUartParamDef(const ESP_UartParam_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL || cfg == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint32(param, cfg->baudRate);
	len = strlen((char*)param);

	if(strcat ((char*)param, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);
	param[len++] = cfg->dataBits;
	param[len++] = ',';
	param[len++] = cfg->stopBits;
	param[len++] = ',';
	param[len++] = cfg->parity;
	param[len++] = ',';
	param[len++] = cfg->flowControl;
	param[len] = 0;
	len = strlen((char*)param);

	if(ESP_SendAtCmd(UART_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets ESP8266 sleep mode.
*
* Public function defined in esp_basic.h
*/
bool ESP_Sleep(ESP_SleepModes_t mode, uint32_t timeout)
{
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL) {
		return false;
	}

    if(ESP_SendAtCmd(SLEEP, (char*)&mode, 1ul) == false) {
    	return false;
    }

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function configures a GPIO to wake ESP8266 up from Light-sleep mode.
*
* Public function defined in esp_basic.h
*/
bool ESP_ConfigWakeupGpio(const ESP_WakeupGpioParam_t *gpio, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL || gpio == NULL) {
		return ESP_INNER_ERR;
	}

	param[len++] = gpio->enable;
	param[len++] = ',';

	Conver_DigToStringUint8(&param[len], gpio->trigger_GPIO);

	len += strlen((char*)param);
	param[len++] = ',';

	param[len++] = gpio->trigger_level;
	param[len++] = ',';

	if(gpio->awake_GPIO != 0)
	{
		Conver_DigToStringUint8(&param[len], gpio->awake_GPIO);

		len += strlen((char*)param);
		param[len++] = ',';
	}

	param[len++] = gpio->awake_level;

	if(ESP_SendAtCmd(WAKEUPGPIO, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	  return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets the maximum value of the RF TX Power.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupRfPower(uint8_t power, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint8(param, power);

	len += strlen((char*)param);

	if(ESP_SendAtCmd(RFPOWER, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets current system messages.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupSystemMessageCur(uint8_t msg, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint8(param, msg);

	len += strlen((char*)param);

	if(ESP_SendAtCmd(SYSMSG_CUR, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}

/**
* This function sets current system messages.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupSystemMessageDef(uint8_t msg, uint32_t timeout)
{
	size_t len = 0;
	char* param = ESP_AllocParamBuffer();
	char* answer = ESP_AllocAnswerBuffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	Conver_DigToStringUint8(param, msg);

	len += strlen((char*)param);

	if(ESP_SendAtCmd(SYSMSG_DEF, param, len) == false) {
		return false;
	}

	if(ESP_HardWareReceiveUartBlock((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return ESP_PatternCheck(answer, pattern_OK);
}
