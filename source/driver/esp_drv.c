// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include "esp_drv.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "stack/esp_basic.h"
#include "stack/esp_wifi.h"
#include "stack/esp_tcpip.h"
#include "esp_port.h"
//_____ C O N F I G S  _________________________________________________________
//_____ D E F I N I T I O N ____________________________________________________
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   ___________________
static char esp_msg[RX_BUFFER_SIZE] = {0};
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This initializations function of esp driver.
*
* Public function defined in esp_drv.h
*/
bool esp_drv_init(void)
{
	esp_harware_init();
	esp_hardware_power_on();
	esp_disable_echo(100);

	return true;
}

/**
* This function is handle of received data from esp chip.
*
* Public function defined in esp_drv.h
*/
void esp_drv_receive_handle(void)
{
	int32_t size = esp_data_receive(esp_msg, sizeof(esp_msg), 0);

	if(size > 0)
	{
		esp_basic_receive_handle(esp_msg, size);
		esp_wifi_receive_handle(esp_msg, size);
		esp_tcpip_receive_handle(esp_msg, size);
	}
}

/**
* This function is handle of transmit data to esp chip.
*
* Public function defined in esp_drv.h
*/
void esp_drv_transmit_handle(void)
{
	esp_tcp_transmit_handle();
}
