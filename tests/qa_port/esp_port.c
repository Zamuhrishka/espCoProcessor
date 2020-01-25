
// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
/********************************************************************************
*
* @file    		esp_port.c
* @author  		Kovalchuk Alexander
* @email  		roux@yandex.ru
* @brief
*
********************************************************************************/
//_____ I N C L U D E S _______________________________________________________
#include <stack/esp_wifi.h>
#include <qa_port.h>
#include "esp_drv.h"
#include "usart.h"
#include "stm32f4xx_hal_dma.h"
#include "debug.h"
#include <string.h>
#include <stdio.h>
#include "esp_queue.h"
//_____ D E F I N I T I O N ___________________________________________________
//_____ M A C R O S ___________________________________________________________
//_____ V A R I A B L E   D E F I N I T I O N  ________________________________
char rxBuffer[BUFFER_SIZE] = {0};
char txBuffer[BUFFER_SIZE] = {0};
//_____ I N L I N E   F U N C T I O N   D E F I N I T I O N   _________________
/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_UartIrqDisable(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_UartIrqEnable(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_UartClrFlag(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static bool ESP_IsUartIrq(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_DmaIrqDisable(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_DmaIrqEnable(void)
{
}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static bool ESP_IsDmaIrq(void)
{

}


inline static void ESP_ClrDmaIrq(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_DmaDisable(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_DmaEnable(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static size_t ESP_DmaGetNbm(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
inline static void ESP_ReceiveDmaCfg(uint8_t *pData, uint16_t Size)
{

}
//_____ S T A T I C  F U N C T I O N   D E F I N I T I O N   __________________
//_____ F U N C T I O N   D E F I N I T I O N   _______________________________
/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
void ESP_HardWareInit(void)
{

}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
void ESP_HardWareSwitchMode(Blocking_t mode)
{

}

/**
* @brief 	Low level function for enable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_HardWarePowerOn(void)
{
	return false;
}

/**
* @brief 	Low level function for disable power on module.
*
* @param	none.
*
* @return 	true/false.
*/
bool ESP_HardWarePowerOff(void)
{
	return false;
}

/**
* @brief 	Low level function for transmit data throw UART.
*
* @param	data[in] - array of data.
* @param	size[in] - size of data.
*
* @return 	none.
*/
bool ESP_HardWareTransmitUartBlock(const uint8_t* data, uint16_t size)
{
	esp_basic_handle(data);
	esp_wifi_handle(data);

	return true;
}

/**
* @brief 	Low level function for transmit data throw UART.
*
* @param	data[in] - array of data.
* @param	size[in] - size of data.
*
* @return 	none.
*/
ESPStatus_t ESP_HardWareReceiveUartBlock(uint8_t* data, uint16_t size, uint32_t timeout)
{
	for(uint16_t i = 0; i < size; i++)
	{
		data[i] = txBuffer[i];
	}

	return ESP_PASS;
}

/**
* @brief 	none.
*
* @param	none.
*
* @return 	none.
*/
void ESP_HardWareReceiveCallBack(void)
{

}
