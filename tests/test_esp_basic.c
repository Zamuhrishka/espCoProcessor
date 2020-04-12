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
static void Test_TestCmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_test(2000);
	assert_true(result);
}

static void Test_ResetCmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_reset(2000);
	assert_true(result);
}

static void Test_ReqVersionCmd(void **state)
{
	bool result = false;
	esp_at_version_t at = {0};
	esp_sdk_version_t sdk = {0};

	result = esp_init();
	assert_true(result);

	result = esp_get_version(&at, &sdk, 2000);
	assert_true(result);
}

static void Test_EnterDeepSleepCmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_deep_sleep(1000, 2000);
	assert_true(result);
}

static void Test_EchoCmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_enable_echo(2000);
	assert_true(result);

	result = esp_disable_echo(2000);
	assert_true(result);
}

static void Test_RestoreCmd(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_restore(2000);
	assert_true(result);
}

static void Test_UartCurCmd(void **state)
{
	bool result = false;
	esp_uart_t param;

	param.baud_rate = 115200;
	param.data_bits = ESP_DATABITS_8;
	param.flow_control = ESP_FLOW_CNTRL_OFF;
	param.parity = ESP_PARITY_NONE;
	param.stop_bits = ESP_STOPBITS_1;

	result = esp_init();
	assert_true(result);

	result = esp_uart_cfg(&param, false, 2000);
	assert_true(result);
}

static void Test_SleepCmd(void **state)
{
	bool result = false;
	esp_sleep_mode_t mode = ESP_SLEEP_MODE_LIGHT;

	result = esp_init();
	assert_true(result);

	result = esp_sleep(mode, 2000);
	assert_true(result);
}


static void Test_ConfigWakeupPinCmd(void **state)
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

static void Test_SetupRfPower(void **state)
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
	  unit_test(Test_TestCmd),
	  unit_test(Test_ResetCmd),
	  unit_test(Test_ReqVersionCmd),
	  unit_test(Test_EnterDeepSleepCmd),
	  unit_test(Test_EchoCmd),
	  unit_test(Test_RestoreCmd),
	  unit_test(Test_UartCurCmd),
	  unit_test(Test_SleepCmd),
	  unit_test(Test_ConfigWakeupPinCmd),
	  unit_test(Test_SetupRfPower),
#if 0
	  unit_test(Test_SetupSystemMessageCur),
#endif
  };

  run_tests(tests);
}
