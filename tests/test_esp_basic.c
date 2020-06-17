// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/**********************************************************************************************
* @file    		QAdfifo.c
* @author  		Kovalchuk Alexander aka mz4
* @email 		roux@yandex.ru
* @version 		0.1
* @date    		06.06.2018
* @brief		Functions for testing dfifo library.
***********************************************************************************************/
//_____ I N C L U D E S _______________________________________________________________________
#include "stdint.h"
#include "stdbool.h"
#include "stdlib.h"
#include "cmockery.h"
#include "esp.h"
#include "esp_basic.h"
#include <string.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
static void test_esp_test_cmd(void **state)
{
	bool result = false;

	result = esp_test(2000);
	assert_true(result);
}

static void test_esp_reset_cmd(void **state)
{
	bool result = false;

	result = esp_reset(2000);
	assert_true(result);
}

static void test_esp_version_request_cmd(void **state)
{
	bool result = false;
	esp_at_version_t at_pattern= {0};
	esp_sdk_version_t sdk_pattern = {0};
	esp_at_version_t at = {0};
	esp_sdk_version_t sdk = {0};

	at_pattern.major = 1;
	at_pattern.minor = 1;
	at_pattern.patch[0] = 0;
	at_pattern.patch[1] = 0;

	sdk_pattern.major = 1;
	sdk_pattern.minor = 5;
	sdk_pattern.patch = 4;

	result = esp_get_version(&at, &sdk, 2000);

	assert_memory_equal(&at, &at_pattern, sizeof(esp_at_version_t));
	assert_memory_equal(&sdk, &sdk_pattern, sizeof(esp_sdk_version_t));

	assert_true(result);
}

static void test_esp_enter_deep_sleep_cmd(void **state)
{
	bool result = false;

	result = esp_deep_sleep(1000, 2000);
	assert_true(result);
}

static void test_esp_echo_cmd(void **state)
{
	bool result = false;

	result = esp_enable_echo(2000);
	assert_true(result);

	result = esp_disable_echo(2000);
	assert_true(result);
}

static void test_esp_restore_cmd(void **state)
{
	bool result = false;

	result = esp_restore(2000);
	assert_true(result);
}

static void test_esp_uart_cfg_cur_cmd(void **state)
{
	bool result = false;
	esp_uart_t param;

	param.baud_rate = 115200;
	param.data_bits = ESP_DATABITS_8;
	param.flow_control = ESP_FLOW_CNTRL_OFF;
	param.parity = ESP_PARITY_NONE;
	param.stop_bits = ESP_STOPBITS_1;

	result = esp_uart_cfg(&param, false, 2000);
	assert_true(result);
}

static void test_esp_uart_cfg_def_cmd(void **state)
{
	bool result = false;
	esp_uart_t param;

	param.baud_rate = 115200;
	param.data_bits = ESP_DATABITS_8;
	param.flow_control = ESP_FLOW_CNTRL_OFF;
	param.parity = ESP_PARITY_NONE;
	param.stop_bits = ESP_STOPBITS_1;

	result = esp_uart_cfg(&param, true, 2000);
	assert_true(result);
}

static void test_esp_sleep_cmd(void **state)
{
	bool result = false;
	esp_sleep_mode_t mode = ESP_SLEEP_MODE_LIGHT;

	result = esp_sleep(mode, 2000);
	assert_true(result);
}


static void test_esp_config_wakeup_pins_cmd(void **state)
{
	bool result = false;
	esp_wgpio_t gpio;

	gpio.awake_gpio = 3;
	gpio.awake_level = ESP_AWPIN_LOW;
	gpio.enable = ESP_WPIN_ENABLE;
	gpio.trigger_gpio = 5;
	gpio.trigger_level = ESP_AWPIN_LOW;

	result = esp_init();
	assert_true(result);

	result = esp_wgpio_cfg(&gpio, 2000);
	assert_true(result);
}

static void test_esp_setup_rf_power_cmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_rf_power(55, 2000);
	assert_true(result);
}

#if 0
static void Test_SetupSystemMessageCur(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = ESP_SetupSystemMessageCur(3, 2000);
	assert_true(result);
}
#endif
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspBasicStack(void)
{
  const UnitTest tests[] =
  {
	  unit_test(test_esp_test_cmd),
	  unit_test(test_esp_reset_cmd),
	  unit_test(test_esp_version_request_cmd),
	  unit_test(test_esp_enter_deep_sleep_cmd),
	  unit_test(test_esp_echo_cmd),
	  unit_test(test_esp_restore_cmd),
	  unit_test(test_esp_uart_cfg_cur_cmd),
	  unit_test(test_esp_uart_cfg_def_cmd),
	  unit_test(test_esp_sleep_cmd),
	  unit_test(test_esp_config_wakeup_pins_cmd),
	  unit_test(test_esp_setup_rf_power_cmd),
#if 0
	  unit_test(Test_SetupSystemMessageCur),
#endif
  };

  run_tests(tests);
}
