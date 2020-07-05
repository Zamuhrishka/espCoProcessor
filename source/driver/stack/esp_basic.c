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
#include "esp_basic.h"

#include <string.h>
#include <assert.h>
#include "esp_drv.h"
#include "slre.h"
#include "convert.h"
#include "esp_utils.h"
//_____ C O N F I G S  ________________________________________________________
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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_data_transmit(cmd, sizeof(cmd), 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function used used to restart the module.
*
* Public function defined in esp_basic.h
*/
bool esp_reset(uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_cmd_transmit(RST, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function return the version of AT commands and SDK that you are using.
*
* Public function defined in esp_basic.h
*/
bool esp_get_version(esp_at_version_t *at, esp_sdk_version_t *sdk, uint32_t timeout)
{
	struct slre_cap caps[4] = {0};
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = NULL;

	assert(NULL != at);
	assert(NULL != sdk);

	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_cmd_transmit(GMR, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	if (slre_match((const char *)"AT version:(\\d+)\\.(\\d+)\\.(\\d+)\\.(\\d+)\\S+", pAnswer, strlen(pAnswer), caps, 4, 0) <= 0) {
	   return false;
	}

	at->major = convert_string_to_uint8(caps[0].ptr);
	at->minor = convert_string_to_uint8(caps[1].ptr);
	at->patch[0] = convert_string_to_uint8(caps[2].ptr);
	at->patch[1] = convert_string_to_uint8(caps[3].ptr);

	if (slre_match((const char *)"\\S+\r\nSDK version:(\\d+)\\.(\\d+)\\.(\\d+)\\S+", pAnswer, strlen(pAnswer), caps, 3, 0) <= 0) {
	   return false;
	}

	sdk->major = convert_string_to_uint8(caps[0].ptr);
	sdk->minor = convert_string_to_uint8(caps[1].ptr);
	sdk->patch = convert_string_to_uint8(caps[2].ptr);

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
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	convert_uint32_to_string(pParam, time);
	param_size = strlen((char*)pParam);

	if(esp_cmd_transmit(GSLP, pParam, param_size) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function enable echo with module pAnswers.
*
* Public function defined in esp_basic.h
*/
bool esp_enable_echo(uint32_t timeout)
{
	char esp_enable = '1';
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_cmd_transmit(ATE, &esp_enable, 1) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return true;
}

/**
* This function disable echo with module pAnswers.
*
* Public function defined in esp_basic.h
*/
bool esp_disable_echo(uint32_t timeout)
{
	char* pAnswer = esp_alloc_answer_buffer();
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char esp_disable = '0';

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_cmd_transmit(ATE, &esp_disable, 1) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
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
	char* pAnswer = esp_alloc_answer_buffer();
	size_t len = ESP_ANSWER_BUFF_SIZE;

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

	if(esp_cmd_transmit(RESTORE, NULL, 0) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
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
	size_t len = ESP_ANSWER_BUFF_SIZE;

	assert(NULL != cfg);

	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	Conver_DigToStringUint32(pParam, cfg->baudRate);
	len = strlen((char*)pParam);

	if(strcat ((char*)pParam, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);
	pParam[len++] = cfg->dataBits;
	pParam[len++] = ',';
	pParam[len++] = cfg->stopBits;
	pParam[len++] = ',';
	pParam[len++] = cfg->parity;
	pParam[len++] = ',';
	pParam[len++] = cfg->flowControl;
	pParam[len] = 0;
	len = strlen((char*)pParam);

	if(esp_cmd_transmit(UART, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#else

/**
* This function sets the UART configuration.
*
* Public function defined in esp_basic.h
*/
bool esp_uart_cfg(const esp_uart_t *cfg, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;

	assert(NULL != cfg);

	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	convert_uint32_to_string(pParam, cfg->baud_rate);
	len = strlen((char*)pParam);

	if(strcat ((char*)pParam, (char*)",\0") == NULL) {
		return false;
	}

	len = strlen((char*)pParam);
	pParam[len++] = cfg->data_bits;
	pParam[len++] = ',';
	pParam[len++] = cfg->stop_bits;
	pParam[len++] = ',';
	pParam[len++] = cfg->parity;
	pParam[len++] = ',';
	pParam[len++] = cfg->flow_control;
	pParam[len] = 0;
	len = strlen((char*)pParam);

	if(esp_cmd_transmit((save ? UART_DEF : UART_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}
#endif

/**
* This function sets ESP8266 sleep mode.
*
* Public function defined in esp_basic.h
*/
bool esp_sleep(esp_sleep_mode_t mode, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer)
	{
		assert(false);
		return false;
	}

    if(esp_cmd_transmit(SLEEP, (char*)&mode, 1ul) == false) {
    	return false;
    }

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function configures a GPIO to wake up from Light-sleep mode.
*
* Public function defined in esp_basic.h
*/
bool esp_wgpio_cfg(const esp_wgpio_t *gpio, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pParam = NULL;
	char* pAnswer = NULL;

	assert(NULL != gpio);

	pParam = esp_alloc_param_buffer();
	pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	pParam[len++] = gpio->enable;
	pParam[len++] = ',';

	convert_uint8_to_string(&pParam[len], gpio->trigger_gpio);

	len += strlen((char*)pParam);
	pParam[len++] = ',';

	pParam[len++] = gpio->trigger_level;
	pParam[len++] = ',';

	if(gpio->awake_gpio != 0)
	{
		convert_uint8_to_string(&pParam[len], gpio->awake_gpio);

		len += strlen((char*)pParam);
		pParam[len++] = ',';
	}

	pParam[len++] = gpio->awake_level;

	if(esp_cmd_transmit(WAKEUPGPIO, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function sets the maximum value of the RF TX Power.
*
* Public function defined in esp_basic.h
*/
bool esp_rf_power(uint8_t power, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	convert_uint8_to_string(pParam, power);

	len += strlen((char*)pParam);

	if(esp_cmd_transmit(RFPOWER, pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

/**
* This function sets current system messages.
*
* Public function defined in esp_basic.h
*/
bool esp_setup_sys_message(uint8_t msg, bool save, uint32_t timeout)
{
	size_t len = ESP_ANSWER_BUFF_SIZE;
	char* pParam = esp_alloc_param_buffer();
	char* pAnswer = esp_alloc_answer_buffer();

	if(NULL == pAnswer || NULL == pParam)
	{
		assert(false);
		return false;
	}

	convert_uint8_to_string(pParam, msg);

	len += strlen((char*)pParam);

	if(esp_cmd_transmit((save ? SYSMSG_DEF : SYSMSG_CUR), pParam, len) == false) {
		return false;
	}

	if(esp_data_receive(pAnswer, len, timeout) <= 0) {
		return false;
	}

	return esp_pattern_check(pAnswer, PATTERN_OK);
}

bool esp_basic_receive_handle(char *msg, size_t size)
{
	return false;
}


