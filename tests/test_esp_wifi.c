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
#include "esp_wifi.h"
#include <string.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
static void Test_CWMODECmd(void **state)
{
	bool result = false;
	ESP_WifiMode_t mode;

	result = ESP_Init();
	assert_true(result);

	result = ESP_SetupWifiModeCur(WIFI_CLIENT, 5000u);
	assert_true(result);
	result = ESP_RequestWifiModeCur(&mode, 5000u);
	assert_true(result);
	assert_int_equal(mode, WIFI_CLIENT);

	result = ESP_SetupWifiModeCur(WIFI_AP, 5000u);
	assert_true(result);
	result = ESP_RequestWifiModeCur(&mode, 5000u);
	assert_true(result);
	assert_int_equal(mode, WIFI_AP);

	result = ESP_SetupWifiModeCur(WIFI_DUAL, 5000u);
	assert_true(result);
	result = ESP_RequestWifiModeCur(&mode, 5000u);
	assert_true(result);
	assert_int_equal(mode, WIFI_DUAL);
}

static void Test_CWJAPCmd(void **state)
{
	ESPStatus_t result = ESP_PASS;
	bool res = false;
	char ssid_cur[35] = "ESPTestAtCommand\0";
	char pass_cur[65] = "123456789ABCDEF\0";
	char ssid_pattern[35] = "ESPTestAtCommand\0";

	res = ESP_Init();
	assert_true(res);

	result = ESP_JoinToWifiApCur(ssid_cur, pass_cur, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = ESP_JoinToWifiApCur(ssid_cur, pass_cur, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = ESP_JoinToWifiApCur(ssid_cur, pass_cur, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = ESP_JoinToWifiApCur(ssid_cur, pass_cur, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = ESP_JoinToWifiApCur(ssid_cur, pass_cur, 5000u);
	assert_int_equal(result, ESP_PASS);





	memset(ssid_cur, '\0', sizeof(ssid_cur));

	result = ESP_RequestNameConnectedApCur(ssid_cur, 5000u);
	assert_int_equal(result, ESP_PASS);
	assert_string_equal(ssid_cur, ssid_pattern);


	res = ESP_UnJoinFromWifiAp(5000u);
	assert_true(res);
}

static void Test_SoftAP(void **state)
{
	bool res = false;
	ESP_WifiMode_t mode;
	ESPStatus_t result = ESP_PASS;
	char ssid[35] = "ESPTestAtCommand\0";
	char pass[65] = "123456789ABCDEF\0";
	char ssidReq[35] = "";
	char passReq[65] = "";
	char channel = '1';
	char enc = OPEN;
	ESP_ApParam_t param;

	res = ESP_Init();
	assert_true(res);

	result = ESP_CreateWifiApCur(ssid, pass, channel, enc, 5000u);
	assert_int_equal(result, ESP_PASS);

	//TODO: Add function for name of SoftAP request
	res =  ESP_RequestSoftApParamCur(ssidReq, passReq, &param, 5000u);
	assert_true(res);
	assert_string_equal(ssidReq, ssid);
	assert_string_equal(passReq, pass);
	assert_int_equal(param.channel, '1');
	assert_int_equal(param.enc, OPEN);
}

static void Test_ConfigDhcpCmd(void **state)
{
	bool res = false;

	res = ESP_Init();
	assert_true(res);

	res = ESP_ConfigDhcpModeCur(WIFI_CLIENT, DHCP_OFF, 5000u);
	assert_true(res);

	res = ESP_ConfigDhcpModeCur(WIFI_CLIENT, DHCP_ON, 5000u);
	assert_true(res);
}

static void Test_AutoconnectCmd(void **state)
{
	bool res = false;

	res = ESP_Init();
	assert_true(res);

	res = ESP_AutoconnectEnable(5000u);
	assert_true(res);

	res = ESP_AutoconnectDisable(5000u);
	assert_true(res);
}

static void Test_ConfigIpStationCmd(void **state)
{
	bool res = false;
	ESPStatus_t result = ESP_INNER_ERR;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	Ipv4Addr_t ipv4 = 0;
	Ipv4Addr_t gw = 0;
	Ipv4Addr_t mask = 0;
	Ipv4Addr_t ipv4Test = 0;
	Ipv4Addr_t gwTest = 0;
	Ipv4Addr_t maskTest = 0;

	res = ESP_Init();
	assert_true(res);

//	result = ipv4StringToAddr(ipStr, &ipv4);
//	assert_int_equal(result, ESP_PASS);
//	result = ipv4StringToAddr(gwStr, &gw);
//	assert_int_equal(result, ESP_PASS);
//	result = ipv4StringToAddr(maskStr, &mask);
//	assert_int_equal(result, ESP_PASS);
	res = Convert_StringToIpv4addr(&ipv4, ipStr);
	assert_true(res);
	res = Convert_StringToIpv4addr(&gw, gwStr);
	assert_true(res);
	res = Convert_StringToIpv4addr(&mask, maskStr);
	assert_true(res);

	res = ESP_SetupWifiStationIpAddrCur(ipv4, gw, mask, 5000u);
	assert_true(res);

	result = ESP_RequestWifiStationIpAddrCur(&ipv4Test, &gwTest, &maskTest, 5000u);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}

static void Test_ConfigIpApCmd(void **state)
{
	bool res = false;
	ESPStatus_t result = ESP_INNER_ERR;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	Ipv4Addr_t ipv4 = 0;
	Ipv4Addr_t gw = 0;
	Ipv4Addr_t mask = 0;
	Ipv4Addr_t ipv4Test = 0;
	Ipv4Addr_t gwTest = 0;
	Ipv4Addr_t maskTest = 0;

	res = ESP_Init();
	assert_true(res);

//	result = ipv4StringToAddr(ipStr, &ipv4);
//	assert_int_equal(result, ESP_PASS);
//	result = ipv4StringToAddr(gwStr, &gw);
//	assert_int_equal(result, ESP_PASS);
//	result = ipv4StringToAddr(maskStr, &mask);
//	assert_int_equal(result, ESP_PASS);
	res = Convert_StringToIpv4addr(&ipv4, ipStr);
	assert_true(res);
	res = Convert_StringToIpv4addr(&gw, gwStr);
	assert_true(res);
	res = Convert_StringToIpv4addr(&mask, maskStr);
	assert_true(res);

	res = ESP_SetupWifiApIpAddrCur(ipv4, gw, mask, 5000u);
	assert_true(res);

	res = ESP_RequestWifiApIpAddrCur(&ipv4Test, &gwTest, &maskTest, 5000u);
	assert_true(res);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}


static void Test_ConfigMacStationCmd(void **state)
{
	MacAddr_t mac = {0};
	MacAddr_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = ESP_Init();
	assert_true(res);

	res = SetupStationMacAddrCur(mac, 5000u);
	assert_true(res);

	res = ESP_RequestStationMacAddrCur(&_mac, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}

static void Test_ConfigMacSoftApCmd(void **state)
{
	MacAddr_t mac = {0};
	MacAddr_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = ESP_Init();
	assert_true(res);

	res = ESP_SetupSoftApMacAddrCur(mac, 5000u);
	assert_true(res);

	res = ESP_RequestSoftApMacAddrCur(&_mac, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspWifiStack(void)
{
  const UnitTest tests[] =
  {
	  unit_test(Test_CWMODECmd),
	  unit_test(Test_CWJAPCmd),
	  unit_test(Test_SoftAP),
	  unit_test(Test_ConfigDhcpCmd),
	  unit_test(Test_AutoconnectCmd),
	  unit_test(Test_ConfigIpStationCmd),
	  unit_test(Test_ConfigIpApCmd),
	  unit_test(Test_ConfigMacStationCmd),
	  unit_test(Test_ConfigMacSoftApCmd),
  };

  run_tests(tests);
}
