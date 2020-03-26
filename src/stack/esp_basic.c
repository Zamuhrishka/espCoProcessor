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
#include <assert.h>
#include "slre.h"
#include "convert.h"
#include "esp_drv.h"
#include "esp_utils.h"
//_____ C O N F I G S  ________________________________________________________________________
// If unit testing is enabled override assert with mock_assert().
#if defined(UNIT_TESTING)
extern void mock_assert(const int result, const char* const expression,
                        const char * const file, const int line);
#undef assert
#define assert(expression) \
    mock_assert((int)(expression), #expression, __FILE__, __LINE__);
#endif // UNIT_TESTING
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function used to test the setup function.
*
* Public function defined in esp_basic.h
*/
bool esp_test(uint32_t timeout)
{
	char cmd[] = "AT\r\n";
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL) {
		return false;
	}

	if(esp_data_send(cmd, sizeof(cmd)) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function used used to restart the module.
*
* Public function defined in esp_basic.h
*/
bool esp_reset(uint32_t timeout)
{
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL) {
		return false;
	}

	if(esp_at_cmd_send(RST, NULL, 0) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function return the version of AT commands and SDK that you are using.
*
* Public function defined in esp_basic.h
*/
bool esp_get_version(esp_at_version_t *at, esp_sdk_version_t *sdk, uint32_t timeout)
{
	struct slre_cap capsAt[4];
	struct slre_cap capsSDK[3];
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || at == NULL || sdk == NULL) {
		return false;
	}

	if(esp_at_cmd_send(GMR, NULL, 0) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	if (slre_match((const char *)"AT version:(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\\S+", answer, strlen(answer), capsAt, 4, 0) <= 0) {
	   return false;
	}

	if (slre_match((const char *)"\\S+\r\nSDK version:(\\d+)\\.(\\d+)\\.(\\d+)\\S+", answer, strlen(answer), capsSDK, 3, 0) <= 0) {
	   return false;
	}

	at->major = convert_string_to_uint8(capsAt[0].ptr);
	at->minor = convert_string_to_uint8(capsAt[1].ptr);
	at->patch[0] = convert_string_to_uint8(capsAt[2].ptr);
	at->patch[1] = convert_string_to_uint8(capsAt[3].ptr);

	sdk->major = convert_string_to_uint8(capsSDK[0].ptr);
	sdk->minor = convert_string_to_uint8(capsSDK[1].ptr);
	sdk->patch = convert_string_to_uint8(capsSDK[2].ptr);

	return true;
}

/**
* This function is used to invoke the deep-sleep mode of the module.
*
* Public function defined in esp_basic.h
*/
bool esp_deep_sleep(uint32_t time, uint32_t timeout)
{
	size_t param_size = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint32_to_string(param, time);
	param_size = strlen((char*)param);

	if(esp_at_cmd_send(GSLP, param, param_size) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function enable echo with module answers.
*
* Public function defined in esp_basic.h
*/
bool esp_enable_echo(uint32_t timeout)
{
	char* answer = esp_alloc_answer_buffer();
	char esp_enable = '1';

	if(answer == NULL) {
		return false;
	}

	if(esp_at_cmd_send(ATE, &esp_enable, 1) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return true;
}

/**
* This function disable echo with module answers.
*
* Public function defined in esp_basic.h
*/
bool esp_disable_echo(uint32_t timeout)
{
	char* answer = esp_alloc_answer_buffer();
	char esp_disable = '0';

	if(answer == NULL) {
		return false;
	}

	if(esp_at_cmd_send(ATE, &esp_disable, 1) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return true;
}

/**
* This function is used to reset all parameters saved in flash.
*
* Public function defined in esp_basic.h
*/
bool esp_restore(uint32_t timeout)
{
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL) {
		return false;
	}

	if(esp_at_cmd_send(RESTORE, NULL, 0) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

#ifdef ESP_DEPRECATED_API_SUPPORT
/**
* This function sets the UART configuration.
*
* @warning	This API is deprecated!
*
* Public function defined in esp_basic.h
*/
bool esp_uart_cfg(const ESP_UartParam_t *cfg, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

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

	if(esp_at_cmd_send(UART, param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}
#else

/**
* This function sets the UART configuration.
*
* Public function defined in esp_basic.h
*/
bool esp_uart_cfg(const esp_uart_t *cfg, bool save, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL || cfg == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint32_to_string(param, cfg->baud_rate);
	len = strlen((char*)param);

	if(strcat ((char*)param, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)param);
	param[len++] = cfg->data_bits;
	param[len++] = ',';
	param[len++] = cfg->stop_bits;
	param[len++] = ',';
	param[len++] = cfg->parity;
	param[len++] = ',';
	param[len++] = cfg->flow_control;
	param[len] = 0;
	len = strlen((char*)param);

	if(esp_at_cmd_send((save ? UART_DEF : UART_CUR), param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}
#endif

/**
* This function sets ESP8266 sleep mode.
*
* Public function defined in esp_basic.h
*/
bool esp_sleep(esp_sleep_mode_t mode, uint32_t timeout)
{
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL) {
		return false;
	}

    if(esp_at_cmd_send(SLEEP, (char*)&mode, 1ul) == false) {
    	return false;
    }

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) != ESP_PASS) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function configures a GPIO to wake up from Light-sleep mode.
*
* Public function defined in esp_basic.h
*/
bool esp_wgpio_cfg(const esp_wgpio_t *gpio, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL || gpio == NULL) {
		return ESP_INNER_ERR;
	}

	param[len++] = gpio->enable;
	param[len++] = ',';

	convert_uint8_to_string(&param[len], gpio->trigger_gpio);

	len += strlen((char*)param);
	param[len++] = ',';

	param[len++] = gpio->trigger_level;
	param[len++] = ',';

	if(gpio->awake_gpio != 0)
	{
		convert_uint8_to_string(&param[len], gpio->awake_gpio);

		len += strlen((char*)param);
		param[len++] = ',';
	}

	param[len++] = gpio->awake_level;

	if(esp_at_cmd_send(WAKEUPGPIO, param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
	  return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function sets the maximum value of the RF TX Power.
*
* Public function defined in esp_basic.h
*/
bool esp_rf_power(uint8_t power, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint8_to_string(param, power);

	len += strlen((char*)param);

	if(esp_at_cmd_send(RFPOWER, param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function sets current system messages.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupSystemMessageCur(uint8_t msg, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint8_to_string(param, msg);

	len += strlen((char*)param);

	if(esp_at_cmd_send(SYSMSG_CUR, param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}

/**
* This function sets current system messages.
*
* Public function defined in esp_basic.h
*/
bool ESP_SetupSystemMessageDef(uint8_t msg, uint32_t timeout)
{
	size_t len = 0;
	char* param = esp_alloc_param_buffer();
	char* answer = esp_alloc_answer_buffer();

	if(answer == NULL || param == NULL) {
		return ESP_INNER_ERR;
	}

	convert_uint8_to_string(param, msg);

	len += strlen((char*)param);

	if(esp_at_cmd_send(SYSMSG_DEF, param, len) == false) {
		return false;
	}

	if(esp_hardware_receive_block((uint8_t*)answer, ESP_ANSWER_BUFF_SIZE, timeout) < 0) {
		return false;
	}

	return esp_pattern_check(answer, PATTERN_OK);
}
