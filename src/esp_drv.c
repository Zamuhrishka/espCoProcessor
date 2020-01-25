// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/*******************************************************************************
* @file    		esp_drv.c
* @author  		alexander kovalchuk
* @email        roux@yandex.ru
* @brief
*******************************************************************************/
//_____ I N C L U D E S ________________________________________________________
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stack/esp_basic.h>
#include <stack/esp_wifi.h>
#include <stack/esp_tcpip.h>
#include "esp_drv.h"
#include "esp_queue.h"
#include <esp_port.h>
//_____ D E F I N I T I O N ____________________________________________________
//_____ M A C R O S ____________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  _________________________________
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   __________________
//_____ S T A T I Ñ  F U N C T I O N   D E F I N I T I O N   ___________________
//_____ F U N C T I O N   D E F I N I T I O N   ________________________________
/**
* This function initializate.
*
* Public function defined in esp_drv.h
*/
bool ESP_Init(void)
{
  ESP_HardWareInit();
  ESP_HardWarePowerOn();
  ESP_DisableEcho(1000);
  return true;    
}

/**
* This function handle of receive data.
*
* Public function defined in esp_drv.h
*/
bool ESP_MsgHandlCallBack(void)
{
	size_t size = 0;
	char* buffer = NULL;

	if(ESP_RxQueueDenqueue(&buffer, &size))
	{
		ESP_TcpMsgHandleCallBack(buffer, size);
	}

	return true;
}
