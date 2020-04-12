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
#include "esp_wifi.h"
#include <string.h>
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   __________________________________
static void test_esp_wifi_mode_setup_cur(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_wifi_mode_setup(ESP_WIFI_STATION, false, 5000u);
	assert_true(result);
}

static void test_esp_wifi_mode_request_cur(void **state)
{
	bool result = false;
	esp_wifi_mode_t mode;

	result = esp_init();
	assert_true(result);

	result = esp_wifi_mode_request(&mode, false, 5000u);
	assert_true(result);

	assert_int_equal(mode, ESP_WIFI_STATION);
}

static void test_esp_wifi_mode_setup_def(void **state)
{
	bool result = false;

	result = esp_init();
	assert_true(result);

	result = esp_wifi_mode_setup(ESP_WIFI_SOFTAP, true, 5000u);
	assert_true(result);
}

static void test_esp_wifi_mode_request_def(void **state)
{
	bool result = false;
	esp_wifi_mode_t mode;

	result = esp_init();
	assert_true(result);

	result = esp_wifi_mode_request(&mode, true, 5000u);
	assert_true(result);

	assert_int_equal(mode, ESP_WIFI_STATION);
}

static void test_esp_wifi_ap_join_cur(void **state)
{
	esp_status_t result = ESP_PASS;
	bool res = false;
	char ssid_cur[35] = "ESPTestAtCommand\0";
	char pass_cur[65] = "123456789ABCDEF\0";

	res = esp_init();
	assert_true(res);

	result = esp_wifi_ap_join(ssid_cur, pass_cur, false, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = esp_wifi_ap_join(ssid_cur, pass_cur, false, 5000u);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_wifi_ap_ssid_request_cur(void **state)
{
	esp_status_t result = ESP_PASS;
	bool res = false;
	char ssid_cur[35] = "ESPTestAtCommand\0";
	char ssid_pattern[35] = "ESPTestAtCommand\0";

	res = esp_init();
	assert_true(res);

	memset(ssid_cur, '\0', sizeof(ssid_cur));

	result = esp_wifi_ap_ssid_request(ssid_cur, false, 5000u);
	assert_int_equal(result, ESP_PASS);
	assert_string_equal(ssid_cur, ssid_pattern);
}

static void test_esp_wifi_ap_join_def(void **state)
{
	esp_status_t result = ESP_PASS;
	bool res = false;
	char ssid_cur[35] = "ESPTestAtCommand\0";
	char pass_cur[65] = "123456789ABCDEF\0";

	res = esp_init();
	assert_true(res);

	result = esp_wifi_ap_join(ssid_cur, pass_cur, true, 5000u);
	assert_int_equal(result, ESP_INNER_ERR);

	result = esp_wifi_ap_join(ssid_cur, pass_cur, true, 5000u);
	assert_int_equal(result, ESP_PASS);
}


static void test_esp_wifi_ap_ssid_request_def(void **state)
{
	esp_status_t result = ESP_PASS;
	bool res = false;
	char ssid_cur[35] = "ESPTestAtCommand\0";
	char ssid_pattern[35] = "ESPTestAtCommand\0";

	res = esp_init();
	assert_true(res);

	memset(ssid_cur, '\0', sizeof(ssid_cur));

	result = esp_wifi_ap_ssid_request(ssid_cur, true, 5000u);
	assert_int_equal(result, ESP_PASS);
	assert_string_equal(ssid_cur, ssid_pattern);
}


static void test_esp_wifi_ap_unjoin(void **state)
{
	bool res = false;

	res = esp_wifi_ap_unjoin(5000u);
	assert_true(res);
}

static void test_esp_wifi_softap_setup_cur(void **state)
{
	bool res = false;
	esp_status_t result = ESP_PASS;
	char ssid[35] = "ESPTestAtCommand\0";
	char pass[65] = "123456789ABCDEF\0";
	char channel = '1';
	esp_encription_t enc = ESP_OPEN;

	res = esp_init();
	assert_true(res);

	result = esp_wifi_softap_setup(ssid, pass, channel, enc, false, 5000u);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_wifi_softap_request_cur(void **state)
{
	bool res = false;
	char ssid[35] = "ESPTestAtCommand\0";
	char pass[65] = "123456789ABCDEF\0";
	char ssidReq[35] = "";
	char passReq[65] = "";
	esp_ap_config_t param;

	res = esp_init();
	assert_true(res);

	res =  esp_wifi_softap_request(ssidReq, passReq, &param, 5000u);
	assert_true(res);
	assert_string_equal(ssidReq, ssid);
	assert_string_equal(passReq, pass);
	assert_int_equal(param.channel, '1');
	assert_int_equal(param.enc, ESP_OPEN);
}


static void test_esp_wifi_softap_setup_def(void **state)
{
	bool res = false;
	esp_status_t result = ESP_PASS;
	char ssid[35] = "ESPTestAtCommand\0";
	char pass[65] = "123456789ABCDEF\0";
	char channel = '1';
	esp_encription_t enc = ESP_OPEN;

	res = esp_init();
	assert_true(res);

	result = esp_wifi_softap_setup(ssid, pass, channel, enc, true, 5000u);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_wifi_softap_request_def(void **state)
{
	bool res = false;
	char ssid[35] = "ESPTestAtCommand\0";
	char pass[65] = "123456789ABCDEF\0";
	char ssidReq[35] = "";
	char passReq[65] = "";
	esp_ap_config_t param;

	res = esp_init();
	assert_true(res);

	res =  esp_wifi_softap_request(ssidReq, passReq, &param, 5000u);
	assert_true(res);
	assert_string_equal(ssidReq, ssid);
	assert_string_equal(passReq, pass);
	assert_int_equal(param.channel, '1');
	assert_int_equal(param.enc, ESP_OPEN);

	assert_true(false);
}

static void test_esp_dhcp_mode_setup_cur(void **state)
{
	bool res = false;

	res = esp_init();
	assert_true(res);

	res = esp_dhcp_mode_setup(ESP_WIFI_STATION, ESP_DHCP_OFF, false, 5000u);
	assert_true(res);

	res = esp_dhcp_mode_setup(ESP_WIFI_STATION, ESP_DHCP_ON, false, 5000u);
	assert_true(res);
}

static void test_esp_dhcp_mode_setup_def(void **state)
{
	bool res = false;

	res = esp_init();
	assert_true(res);

	res = esp_dhcp_mode_setup(ESP_WIFI_STATION, ESP_DHCP_OFF, true, 5000u);
	assert_true(res);

	res = esp_dhcp_mode_setup(ESP_WIFI_STATION, ESP_DHCP_ON, true, 5000u);
	assert_true(res);
}

static void test_esp_wifi_autoconnect(void **state)
{
	bool res = false;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_autoconnect_enable(5000u);
	assert_true(res);

	res = esp_wifi_autoconnect_disable(5000u);
	assert_true(res);
}

static void test_esp_wifi_station_ip_setup_cur(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_station_ip_setup(ipv4, gw, mask, false, 5000u);
	assert_true(res);
}

static void test_esp_wifi_station_ip_request_cur(void **state)
{
	bool res = false;
	esp_status_t result = ESP_INNER_ERR;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	ip4addr_t ipv4Test = 0;
	ip4addr_t gwTest = 0;
	ip4addr_t maskTest = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	result = esp_wifi_station_ip_request(&ipv4Test, &gwTest, &maskTest, false, 5000u);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}

static void test_esp_wifi_station_ip_setup_def(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_station_ip_setup(ipv4, gw, mask, true, 5000u);
	assert_true(res);
}

static void test_esp_wifi_station_ip_request_def(void **state)
{
	bool res = false;
	esp_status_t result = ESP_INNER_ERR;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	ip4addr_t ipv4Test = 0;
	ip4addr_t gwTest = 0;
	ip4addr_t maskTest = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	result = esp_wifi_station_ip_request(&ipv4Test, &gwTest, &maskTest, true, 5000u);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}

static void test_esp_wifi_softap_ip_setup_cur(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_softap_ip_setup(ipv4, gw, mask, false, 5000u);
	assert_true(res);
}

static void test_esp_wifi_softap_ip_request_cur(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	ip4addr_t ipv4Test = 0;
	ip4addr_t gwTest = 0;
	ip4addr_t maskTest = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_softap_ip_request(&ipv4Test, &gwTest, &maskTest, false, 5000u);
	assert_true(res);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}


static void test_esp_wifi_softap_ip_setup_def(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_softap_ip_setup(ipv4, gw, mask, true, 5000u);
	assert_true(res);
}

static void test_esp_wifi_softap_ip_request_def(void **state)
{
	bool res = false;
	char ipStr[20] = "192.168.6.100\0";
	char gwStr[20] = "192.168.6.1\0";
	char maskStr[20] = "255.255.255.0\0";
	ip4addr_t ipv4 = 0;
	ip4addr_t gw = 0;
	ip4addr_t mask = 0;
	ip4addr_t ipv4Test = 0;
	ip4addr_t gwTest = 0;
	ip4addr_t maskTest = 0;

	res = esp_init();
	assert_true(res);

	res = convert_string_to_ip4addr(&ipv4, ipStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&gw, gwStr);
	assert_true(res);
	res = convert_string_to_ip4addr(&mask, maskStr);
	assert_true(res);

	res = esp_wifi_softap_ip_request(&ipv4Test, &gwTest, &maskTest, true, 5000u);
	assert_true(res);

	assert_int_equal(ipv4, ipv4Test);
	assert_int_equal(gw, gwTest);
	assert_int_equal(mask, maskTest);
}

static void test_esp_wifi_station_mac_setup_cur(void **state)
{
	mac_t mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_station_mac_setup(mac, false, 5000u);
	assert_true(res);
}

static void test_esp_wifi_station_mac_request_cur(void **state)
{
	mac_t mac = {0};
	mac_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_station_mac_request(&_mac, false, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}

static void test_esp_wifi_station_mac_setup_def(void **state)
{
	mac_t mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_station_mac_setup(mac, true, 5000u);
	assert_true(res);
}



static void test_esp_wifi_station_mac_request_def(void **state)
{
	mac_t mac = {0};
	mac_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_station_mac_request(&_mac, true, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}

static void test_esp_wifi_softap_mac_setup_cur(void **state)
{
	mac_t mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_softap_mac_setup(mac, false, 5000u);
	assert_true(res);
}

static void test_esp_wifi_softap_mac_request_cur(void **state)
{
	mac_t mac = {0};
	mac_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_softap_mac_request(&_mac, false, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}

static void test_esp_wifi_softap_mac_setup_def(void **state)
{
	mac_t mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_softap_mac_setup(mac, true, 5000u);
	assert_true(res);
}

static void test_esp_wifi_softap_mac_request_def(void **state)
{
	mac_t mac = {0};
	mac_t _mac = {0};
	bool res = false;

	mac.value[0] = 0x13;
	mac.value[1] = 0xfa;
	mac.value[2] = 0x30;
	mac.value[3] = 0x87;
	mac.value[4] = 0xd3;
	mac.value[5] = 0x2b;

	res = esp_init();
	assert_true(res);

	res = esp_wifi_softap_mac_request(&_mac, true, 5000u);
	assert_true(res);

	assert_memory_equal(&mac.value, &_mac.value, sizeof(mac.value));
}
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspWifiStack(void)
{
  const UnitTest tests[] =
  {
	  unit_test(test_esp_wifi_mode_setup_cur),
	  unit_test(test_esp_wifi_mode_request_cur),
	  unit_test(test_esp_wifi_mode_setup_def),
	  unit_test(test_esp_wifi_mode_request_def),
	  unit_test(test_esp_wifi_ap_join_cur),
	  unit_test(test_esp_wifi_ap_ssid_request_cur),
	  unit_test(test_esp_wifi_ap_join_cur),
	  unit_test(test_esp_wifi_ap_ssid_request_cur),
	  unit_test(test_esp_wifi_ap_unjoin),
	  unit_test(test_esp_wifi_softap_setup_cur),
	  unit_test(test_esp_wifi_softap_request_cur),
	  unit_test(test_esp_wifi_softap_setup_def),
	  unit_test(test_esp_wifi_softap_request_def),
	  unit_test(test_esp_dhcp_mode_setup_cur),
	  unit_test(test_esp_dhcp_mode_setup_def),
	  unit_test(test_esp_wifi_autoconnect),
	  unit_test(test_esp_wifi_station_ip_setup_cur),
	  unit_test(test_esp_wifi_station_ip_request_cur),
	  unit_test(test_esp_wifi_station_ip_setup_def),
	  unit_test(test_esp_wifi_station_ip_request_def),
	  unit_test(test_esp_wifi_station_mac_setup_cur),
	  unit_test(test_esp_wifi_station_mac_request_cur),
	  unit_test(test_esp_wifi_station_mac_setup_def),
	  unit_test(test_esp_wifi_station_mac_request_def),
	  unit_test(test_esp_wifi_softap_mac_setup_cur),
	  unit_test(test_esp_wifi_softap_mac_request_cur),
	  unit_test(test_esp_wifi_softap_mac_setup_def),
	  unit_test(test_esp_wifi_softap_mac_request_def),
  };

  run_tests(tests);
}
