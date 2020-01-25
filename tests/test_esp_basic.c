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
#include "esp_drv.h"
#include "esp_basic.h"
#include <string.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
static void Test_TestCmd(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_Test(2000);
	assert_true(result);
}

static void Test_ResetCmd(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_Reset(2000);
	assert_true(result);
}

static void Test_ReqVersionCmd(void **state)
{
	bool result = false;
	char at[50] = {0};
	char sdk[50] = {0};

	result = ESP_Init();
	assert_true(result);

	result = ESP_GetVersion(at, sdk, 2000);
	assert_true(result);
}

static void Test_EnterDeepSleepCmd(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_EnterToDeepSleep(1000, 2000);
	assert_true(result);
}

static void Test_EchoCmd(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_EnableEcho(2000);
	assert_true(result);

	result = ESP_DisableEcho(2000);
	assert_true(result);
}

static void Test_RestoreCmd(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_Restore(2000);
	assert_true(result);
}

static void Test_UartCurCmd(void **state)
{
	bool result = false;
	ESP_UartParam_t param;

	param.baudRate = 115200;
	param.dataBits = ESP_DataBits_8;
	param.flowControl = ESP_FlowControlOff;
	param.parity = ESP_ParityNone;
	param.stopBits = ESP_DataBits_1;

	result = ESP_Init();
	assert_true(result);

	result = ESP_SetupUartParamCur(&param, 2000);
	assert_true(result);
}

static void Test_SleepCmd(void **state)
{
	bool result = false;
	ESP_SleepModes_t mode = ESP_SleepModeLight;

	result = ESP_Init();
	assert_true(result);

	result = ESP_Sleep(mode, 2000);
	assert_true(result);
}


static void Test_ConfigWakeupPinCmd(void **state)
{
	bool result = false;
	ESP_WakeupGpioParam_t gpio;

	gpio.awake_GPIO = 3;
	gpio.awake_level = ESP_AwakeupPinHighLevel;
	gpio.enable = ESP8266_Enable;
	gpio.trigger_GPIO = 5;
	gpio.trigger_level = ESP_WakeupPinHighLevel;

	result = ESP_Init();
	assert_true(result);

	result = ESP_ConfigWakeupGpio(&gpio, 2000);
	assert_true(result);
}

static void Test_SetupRfPower(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_SetupRfPower(55, 2000);
	assert_true(result);
}

static void Test_SetupSystemMessageCur(void **state)
{
	bool result = false;

	result = ESP_Init();
	assert_true(result);

	result = ESP_SetupSystemMessageCur(3, 2000);
	assert_true(result);
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspBasicStack(void)
{
  const UnitTest tests[] =
  {
//	  unit_test(Test_TestCmd),
	  unit_test(Test_ResetCmd),
//	  unit_test(Test_ReqVersionCmd),
	  unit_test(Test_EnterDeepSleepCmd),
	  unit_test(Test_EchoCmd),
	  unit_test(Test_RestoreCmd),
	  unit_test(Test_UartCurCmd),
	  unit_test(Test_SleepCmd),
	  unit_test(Test_ConfigWakeupPinCmd),
	  unit_test(Test_SetupRfPower),
	  unit_test(Test_SetupSystemMessageCur),
  };

  run_tests(tests);
}
