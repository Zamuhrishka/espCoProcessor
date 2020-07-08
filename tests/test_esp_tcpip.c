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
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "cmockery.h"
#include "esp_tcpip.h"
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________________________
static void test_esp_conn_status_request(void **state)
{
	esp_status_t result = ESP_INNER_ERR;
	esp_conn_status_t status;

	assert_true(esp_drv_init());

	result = esp_conn_status_request(&status, 5000u);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(status.id, ESP_ID0);
	assert_int_equal(status.localPort, 9988);
	assert_int_equal(status.remoteIp, 0x764A8C0);
	assert_int_equal(status.remotePort, 9786);
	assert_int_equal(status.stat, ESP_GOT_IP);
	assert_int_equal(status.type, ESP_TCP);
#if 0
	assert_int_equal(status.tetype, ESP_CLIENT);
#endif
}

static void test_esp_tcp_connect(void **state)
{
	esp_status_t result = ESP_INNER_ERR;
	esp_tcp_cfg_t cfg;

	assert_true(esp_drv_init());

	cfg.id = ESP_ID0;
	cfg.keepAlive = 0;
	cfg.localPort = 9988;
	cfg.remoteIp = 0x764A8C0;
	cfg.remotePort = 9786;
	result = esp_tcp_connect(&cfg, 1000);
	assert_int_equal(result, ESP_PASS);
}

#if 0
static void test_esp_udp_transmission(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(false);
}
#endif

static void test_esp_close_connection_m(void **state)
{
	esp_status_t result = false;

	assert_true(esp_drv_init());
	result = esp_close_connection_m(ESP_ID0, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_close_connection(void **state)
{
	esp_status_t result = false;

	assert_true(esp_drv_init());
	result = esp_close_connection(1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_multiple_connection(void **state)
{
	esp_status_t result = false;

	assert_true(esp_drv_init());
	result = esp_multiple_connection_enable(1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_single_connection(void **state)
{
	esp_status_t result = false;

	assert_true(esp_drv_init());
	result = esp_single_connection_enable(1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_mux_cfg_request(void **state)
{
	esp_status_t result = ESP_INNER_ERR;
	esp_conn_mode_t mode;

	assert_true(esp_drv_init());

	result = esp_mux_cfg_request(&mode, 500);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(mode, ESP_MULTIPLE_CONNECT);
}

static void test_esp_transmit_mode_setup(void **state)
{
	esp_status_t result = false;
	esp_tx_mode_t mode = ESP_NORMAL_MODE;

	assert_true(esp_drv_init());
	result = esp_transmit_mode_setup(mode, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_transmit_mode_request(void **state)
{
	esp_status_t result = ESP_INNER_ERR;
	esp_tx_mode_t mode;

	assert_true(esp_drv_init());

	result = esp_transmit_mode_request(&mode, 500);
	assert_int_equal(result, ESP_PASS);

	assert_int_equal(mode, ESP_NORMAL_MODE);
}

static void test_esp_tcp_server_create(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(esp_drv_init());
	result = esp_tcp_server_create(9988, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_tcp_server_delete(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(esp_drv_init());
	result = esp_tcp_server_delete(9988, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_tcp_server_timeout_setup(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(esp_drv_init());
	result = esp_tcp_server_timeout_setup(5000, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_tcp_server_maxconn_setup(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(esp_drv_init());
	result = esp_tcp_server_maxconn_setup(3, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_sntp_timezone_setup(void **state)
{
	esp_status_t result = ESP_INNER_ERR;

	assert_true(esp_drv_init());
	result = esp_sntp_timezone_setup(3, 1000);
	assert_int_equal(result, ESP_PASS);

	result = esp_sntp_timezone_setup(-10, 1000);
	assert_int_equal(result, ESP_PASS);

	result = esp_sntp_timezone_setup(0, 1000);
	assert_int_equal(result, ESP_PASS);
}

static void test_esp_get_time(void **state)
{
	esp_status_t result = ESP_INNER_ERR;
	esp_datetime_t dt = {0};
	esp_datetime_t dt_pattern = {0};

	assert_true(esp_drv_init());

	dt_pattern.day = 4;
	dt_pattern.day_of_week = 5;
	dt_pattern.hour = 14;
	dt_pattern.min = 48;
	dt_pattern.month = 8;
	dt_pattern.sec = 5;
	dt_pattern.year = 2016;

	result = esp_get_time(&dt, 1000);
	assert_int_equal(result, ESP_PASS);
	assert_memory_equal(&dt, &dt_pattern, sizeof(esp_datetime_t));
}

//_____ F U N C T I O N   D E F I N I T I O N   _______________________________________________
void Test_EspTcpStack(void)
{
  const UnitTest tests[] =
  {
	  unit_test(test_esp_conn_status_request),
	  unit_test(test_esp_tcp_connect),
#if 0
	  unit_test(test_esp_udp_transmission),
#endif
	  unit_test(test_esp_close_connection_m),
	  unit_test(test_esp_close_connection),
	  unit_test(test_esp_multiple_connection),
	  unit_test(test_esp_single_connection),
	  unit_test(test_esp_mux_cfg_request),
	  unit_test(test_esp_transmit_mode_setup),
	  unit_test(test_transmit_mode_request),
	  unit_test(test_esp_tcp_server_create),
	  unit_test(test_esp_tcp_server_delete),
	  unit_test(test_esp_tcp_server_timeout_setup),
	  unit_test(test_esp_tcp_server_maxconn_setup),
	  unit_test(test_esp_sntp_timezone_setup),
	  unit_test(test_esp_get_time),
  };

  run_tests(tests);
}
