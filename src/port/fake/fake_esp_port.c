
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_port.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief
*
*
********************************************************************************/
#if defined(UNIT_TESTING)
//_____ I N C L U D E S _______________________________________________________
#include "fake_esp_port.h"

#include <string.h>
#include <stdio.h>
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
char rxBuffer[BUFFER_SIZE] = {0};
char txBuffer[BUFFER_SIZE] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* This function init hardware unit.
*
* Public function defined in esp_port.h
*/
void esp_harware_init(void)
{

}

/**
* This function is low level function for enable power on module.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_power_on(void)
{
	return false;
}

/**
* This function is low level function for disable power on module.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_power_off(void)
{
	return false;
}



/**
* This function is low level function for transmit data throw UART.
*
* Public function defined in esp_port.h
*/
bool esp_hardware_transmit_block(const char data[], uint16_t size)
{
	esp_basic_handle(data);
	esp_wifi_handle(data);
	esp_tcp_handle(data);
	return true;
}

/**
* This function is low level function for receive data
* throw UART in block mode.
*
* Public function defined in esp_port.h
*/
esp_status_t esp_hardware_receive_block(char **msg, size_t *len, uint32_t timeout)
{
	*msg = txBuffer;
	*len = strlen(txBuffer);

	return ESP_PASS;
}

bool esp_hardware_receive(char **msg, size_t *len)
{
	*msg = txBuffer;

	return true;
}


/**
* This function is interrupt handler for received
* data throw UART
*
* Public function defined in esp_port.h
*/
void esp_hardware_receive_irq(void)
{

}
#endif
